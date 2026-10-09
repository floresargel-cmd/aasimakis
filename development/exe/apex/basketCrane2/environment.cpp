#include "environment.h"
#include <QFileInfo>
#include <QSettings>
#include <QRegExp>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

namespace Crane2 {
namespace {
bool fail(QString* error, const QString& message) {
    if (error) *error = message;
    return false;
}
bool profile(QSettings& settings, const QString& section, DatabaseProfile* p, QString* error) {
    p->dsn = settings.value(section + "/dsn").toString().trimmed();
    p->catalog = settings.value(section + "/catalog").toString().trimmed();
    p->user = settings.value(section + "/user").toString().trimmed();
    const QString variable = settings.value(section + "/passwordEnv").toString().trimmed();
    if (!variable.isEmpty()) {
        p->password = QString::fromLocal8Bit(qgetenv(variable.toLocal8Bit().constData()));
        if (p->password.isEmpty()) return fail(error, section + ": password environment variable is empty.");
    }
    // DSNs only: do not accept connection-string overrides hidden in this field.
    if (p->dsn.isEmpty() || p->dsn.contains(';') || p->dsn.contains('='))
        return fail(error, section + ": provide an ODBC DSN name, not a connection string.");
    if (!QRegExp("[A-Za-z_][A-Za-z0-9_]*").exactMatch(p->catalog))
        return fail(error, section + ": catalog must be an explicit SQL identifier.");
    return true;
}
bool openProfile(const DatabaseProfile& p, const QString& connection, bool observer, QString* error) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", connection);
    db.setDatabaseName(p.dsn);
    db.setUserName(p.user);
    db.setPassword(p.password);
    // The permission audit below is required too; this ODBC hint alone is not enforcement.
    if (observer) db.setConnectOptions("SQL_ATTR_ACCESS_MODE=SQL_MODE_READ_ONLY");
    if (!db.open()) return fail(error, "Cannot open " + connection + ": " + db.lastError().text());
    QSqlQuery query(db);
    // Never USE an alternative catalog here: validate the DSN's initial target before
    // any application writes, including schema initialization and log retention.
    if (!query.exec("SELECT DB_NAME()") || !query.next() ||
        query.value(0).toString().compare(p.catalog, Qt::CaseInsensitive) != 0) {
        db.close();
        return fail(error, connection + ": DSN default database does not match configured catalog " + p.catalog + ".");
    }
    if (observer && !Environment::verifyReadOnly(db, error)) {
        db.close();
        return false;
    }
    return true;
}
}

Environment& Environment::instance() { static Environment value; return value; }

bool Environment::validateTestProfile(const DatabaseProfile& test, const DatabaseProfile& live) {
    return !test.dsn.isEmpty() && test.dsn.compare(live.dsn, Qt::CaseInsensitive) != 0 &&
        test.catalog.compare(live.catalog, Qt::CaseInsensitive) != 0 &&
        test.catalog.contains("test", Qt::CaseInsensitive);
}

bool Environment::load(const QString& file, const QString& host, QString* error) {
    if (loaded_) return fail(error, "Environment cannot be changed while the application is running.");
    if (!QFileInfo(file).isFile()) return fail(error, "Missing configuration file: " + file);
    QSettings settings(file, QSettings::IniFormat);
    const QString mode = settings.value("Environment/mode", "Test").toString().trimmed();
    if (mode.compare("Test", Qt::CaseInsensitive) == 0) mode_ = Mode::Test;
    else if (mode.compare("Live", Qt::CaseInsensitive) == 0) mode_ = Mode::Live;
    else if (mode.compare("LiveObserver", Qt::CaseInsensitive) == 0) mode_ = Mode::LiveObserver;
    else return fail(error, "Environment/mode must be Test, Live, or LiveObserver.");
    if (mode_ == Mode::Live) {
        const QStringList hosts = settings.value("Environment/liveEnabledHosts").toString().split('|', QString::SkipEmptyParts);
        bool allowed = false;
        for (const QString& entry : hosts) if (entry.trimmed().compare(host, Qt::CaseInsensitive) == 0) allowed = true;
        if (!allowed) return fail(error, "Live control is not enabled for this host in Environment/liveEnabledHosts.");
    }
    const QString suffix = mode_ == Mode::Test ? "Test" : (isObserver() ? "Observer" : "Live");
    if (!profile(settings, "MainDatabase" + suffix, &main_, error) ||
        !profile(settings, "EpicsDatabase" + suffix, &epics_, error)) return false;
    if (mode_ == Mode::Test) {
        DatabaseProfile liveMain, liveEpics;
        liveMain.dsn = settings.value("MainDatabaseLive/dsn").toString().trimmed();
        liveMain.catalog = settings.value("MainDatabaseLive/catalog").toString().trimmed();
        liveEpics.dsn = settings.value("EpicsDatabaseLive/dsn").toString().trimmed();
        liveEpics.catalog = settings.value("EpicsDatabaseLive/catalog").toString().trimmed();
        if (liveMain.dsn.isEmpty() || liveMain.catalog.isEmpty() || liveEpics.dsn.isEmpty() || liveEpics.catalog.isEmpty() ||
            !validateTestProfile(main_, liveMain) || !validateTestProfile(epics_, liveEpics))
            return fail(error, "Test needs separate DSNs and catalogs from Live; both test catalog names must contain Test.");
    }
    if (isObserver()) {
        if (main_.catalog.compare(settings.value("MainDatabaseLive/catalog").toString(), Qt::CaseInsensitive) != 0 ||
            epics_.catalog.compare(settings.value("EpicsDatabaseLive/catalog").toString(), Qt::CaseInsensitive) != 0)
            return fail(error, "Observer catalogs must match the configured Live catalogs.");
    }
    plcAddresses_.clear();
    if (mode_ == Mode::Live) {
        for (const QString& key : QStringList() << "craneIp" << "oldOvenIp" << "newOvenIp") {
            const QString address = settings.value("PLC/" + key).toString().trimmed();
            const QStringList parts = address.split('.');
            bool valid = parts.size() == 4;
            for (const QString& part : parts) {
                bool ok = false; const int value = part.toInt(&ok);
                valid = valid && ok && value >= 0 && value <= 255;
            }
            if (!valid) return fail(error, "PLC/" + key + " requires an IPv4 address.");
            plcAddresses_ << address;
        }
    } else {
        // Even if a linked library accidentally attempts network I/O in emulation,
        // no production PLC address is supplied to it.
        plcAddresses_ << "127.0.0.2" << "127.0.0.3" << "127.0.0.4";
    }
    if (settings.status() != QSettings::NoError) return fail(error, "Unable to read configuration file.");
    loaded_ = true;
    return true;
}

bool Environment::verifyReadOnly(const QSqlDatabase& db, QString* error) {
    // Read-only credentials are mandatory. Audit effective permissions, not just
    // role names (custom roles and explicit object grants also grant writes).
    const QStringList checks = QStringList()
        << "SELECT COUNT(*) FROM sys.fn_my_permissions(NULL, 'SERVER') WHERE permission_name NOT IN ('CONNECT SQL','VIEW ANY DATABASE','VIEW SERVER STATE','VIEW SERVER PERFORMANCE STATE','VIEW ANY DEFINITION')"
        << "SELECT COUNT(*) FROM sys.fn_my_permissions(NULL, 'DATABASE') WHERE permission_name NOT IN ('CONNECT','SELECT','VIEW DEFINITION','VIEW DATABASE STATE','VIEW DATABASE PERFORMANCE STATE','VIEW SECURITY DEFINITION','VIEW PERFORMANCE DEFINITION','UNMASK')"
        << "SELECT COUNT(*) FROM sys.schemas s CROSS APPLY sys.fn_my_permissions(QUOTENAME(s.name), 'SCHEMA') p WHERE s.name NOT IN ('sys','INFORMATION_SCHEMA') AND p.permission_name NOT IN ('SELECT','VIEW DEFINITION','VIEW CHANGE TRACKING','UNMASK')"
        << "SELECT COUNT(*) FROM sys.objects o JOIN sys.schemas s ON s.schema_id=o.schema_id CROSS APPLY sys.fn_my_permissions(QUOTENAME(s.name)+'.'+QUOTENAME(o.name), 'OBJECT') p WHERE o.is_ms_shipped=0 AND p.permission_name NOT IN ('SELECT','VIEW DEFINITION','VIEW CHANGE TRACKING','UNMASK')";
    for (const QString& sql : checks) {
        QSqlQuery query(db);
        if (!query.exec(sql) || !query.next() || query.value(0).isNull() || query.value(0).toInt() != 0)
            return fail(error, "LiveObserver requires a dedicated SELECT-only account with no write, execute, ownership, or administrative permissions (" + db.connectionName() + "). Permission verification failed.");
    }
    return true;
}

bool Environment::openDatabases(QString* error) const {
    if (!loaded_) return fail(error, "Environment must be loaded before connecting.");
    if (!openProfile(main_, "bDb", isObserver(), error)) return false;
    if (!openProfile(epics_, "epicsDb", isObserver(), error)) {
        QSqlDatabase::database("bDb", false).close();
        return false;
    }
    return true;
}
QString Environment::name() const {
    return mode_ == Mode::Live ? "Live" : (isObserver() ? "LiveObserver" : "Test");
}
QString Environment::description() const {
    return mode_ == Mode::Live ? "LIVE - live databases and PLC control" :
        (isObserver() ? "LIVE OBSERVER - database read-only; no PLC connection or output" : "TEST - test databases; PLC emulation only");
}
QString Environment::plcAddress(int index) const { return plcAddresses_.value(index); }
}
