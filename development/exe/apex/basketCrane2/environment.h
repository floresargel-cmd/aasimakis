#ifndef CRANE2_ENVIRONMENT_H
#define CRANE2_ENVIRONMENT_H

#include <QString>
#include <QStringList>
#include <QSqlDatabase>

namespace Crane2 {
enum class Mode { Test, Live, LiveObserver };

struct DatabaseProfile {
    QString dsn;
    QString catalog;
    QString user;
    QString password;
};

// Loaded once, before constructing any database-writing or PLC-connected object.
class Environment {
public:
    static Environment& instance();
    bool load(const QString& file, const QString& host, QString* error);
    bool openDatabases(QString* error) const;
    bool loaded() const { return loaded_; }
    Mode mode() const { return mode_; }
    bool isObserver() const { return mode_ == Mode::LiveObserver; }
    bool livePlcEnabled() const { return loaded_ && mode_ == Mode::Live; }
    QString name() const;
    QString description() const;
    QString plcAddress(int index) const;
    const DatabaseProfile& mainDatabase() const { return main_; }
    const DatabaseProfile& epicsDatabase() const { return epics_; }
    static bool validateTestProfile(const DatabaseProfile& test, const DatabaseProfile& live);
    static bool verifyReadOnly(const QSqlDatabase& db, QString* error);
private:
    bool loaded_ = false;
    Mode mode_ = Mode::Test;
    DatabaseProfile main_, epics_;
    QStringList plcAddresses_;
};
}
#endif
