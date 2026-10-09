#include "observerWindow.h"
#include "environment.h"
#include <QLabel>
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

namespace Crane2 {
ObserverWindow::ObserverWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Apex Basket Crane 2 [LiveObserver] - Database comparison");
    resize(1500, 900);
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* layout = new QVBoxLayout(central);
    QLabel* banner = new QLabel(Environment::instance().description(), this);
    banner->setStyleSheet("font-size:18px;font-weight:bold;padding:12px;background:#e3f1ff;color:#123456;");
    layout->addWidget(banner);
    layout->addWidget(new QLabel("Database snapshots only. No PLC telemetry, mission execution, recovery, schema changes, or database logging. Refreshes every 5 seconds.", this));
    status_ = new QLabel(this);
    layout->addWidget(status_);
    QTabWidget* tabs = new QTabWidget(this);
    layout->addWidget(tabs);
    auto add = [&](const QString& title, const QString& sql, const QString& connection) {
        QTableWidget* table = new QTableWidget(tabs);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setAlternatingRowColors(true);
        table->horizontalHeader()->setStretchLastSection(true);
        tabs->addTab(table, title);
        View view = { table, sql, connection };
        views_.append(view);
    };
    add("Positions", "SELECT posNumber,posIndex,description,basketTableId,locked FROM positions ORDER BY posNumber,posIndex", "bDb");
    add("Missions", "SELECT status,fromPosNumber,fromPosIndex,toPosNumber,toPosIndex FROM c2missions ORDER BY status", "bDb");
    add("HCB exports", "SELECT basket,priority FROM c2exportsDestacker ORDER BY priority", "bDb");
    add("Anodizing exports", "SELECT basket,priority FROM c2exportsPacking ORDER BY priority", "bDb");
    add("Packing 1 exports", "SELECT basket,priority FROM c2exportsPackingDestacker1 ORDER BY priority", "bDb");
    add("Packing 2 exports", "SELECT basket,priority FROM c2exportsPackingDestacker2 ORDER BY priority", "bDb");
    add("Recent events", "SELECT TOP 300 mess,details,messenger,type,time FROM c2logGui ORDER BY tId DESC", "bDb");
    add("EPICS contents", "SELECT r.RackId,s.DieNum,r.Pc,s.OrdLen*1000 AS LengthMm,s.Temper,r.SONum,r.SOItemNum,r.CurrentFinCode,s.StdPract,s.CustShip,r.NextDeptNum FROM RackDetail r INNER JOIN SOItem s ON r.SONum=s.SONum AND r.SOItemNum=s.SOItemNum WHERE r.Pc<>0 AND r.RackId BETWEEN 1 AND 130 ORDER BY r.RackId,r.SONum,r.SOItemNum", "epicsDb");
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() { refresh(); });
    refresh();
    timer->start(5000);
}

void ObserverWindow::refresh() {
    QStringList failures;
    for (const View& view : views_) {
        QSqlQuery query(QSqlDatabase::database(view.connection));
        query.setForwardOnly(true);
        // Only fixed SELECT statements above reach this independent observer path.
        if (!query.exec(view.sql)) {
            view.table->setRowCount(0); // Never present stale data as a fresh snapshot.
            failures << query.lastError().text();
            continue;
        }
        view.table->setRowCount(0);
        const QSqlRecord record = query.record();
        view.table->setColumnCount(record.count());
        QStringList headings;
        for (int column = 0; column < record.count(); ++column) headings << record.fieldName(column);
        view.table->setHorizontalHeaderLabels(headings);
        while (query.next()) {
            const int row = view.table->rowCount();
            view.table->insertRow(row);
            for (int column = 0; column < record.count(); ++column)
                view.table->setItem(row, column, new QTableWidgetItem(query.value(column).toString()));
        }
        if (query.lastError().isValid()) {
            view.table->setRowCount(0);
            failures << query.lastError().text();
        }
        view.table->resizeColumnsToContents();
    }
    const QString stamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    status_->setText(failures.isEmpty() ? "Last database refresh: " + stamp : "Refresh incomplete at " + stamp + ": " + failures.join("; "));
    status_->setWordWrap(true);
    status_->setStyleSheet(failures.isEmpty() ? "color:#246030" : "color:#a00000");
}
}
