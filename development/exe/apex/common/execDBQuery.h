#pragma once

#include <QVector>
#include <QVariant>
#include <QString>
#include <QStringList>

#include "..\G.h" // for MAIN_DB_CONNECTION

// Forward-declare the existing function (wherever it's actually defined)
QVector<QVariant> execQuery(QString stmnt, QString connection);

QVector<QVariant> execDBQuery(QString stmnt, QString connection = MAIN_DB_CONNECTION);
QVector<QVector<QVariant> > execDBTableQuery(QString stmnt, QString connection = MAIN_DB_CONNECTION);

// Live Observer mode uses the production database profiles but permits only
// read queries (plus USE to select the configured catalog).
void setDatabaseReadOnlyMode(bool enabled);
bool isDatabaseReadOnlyMode();

// Executes all statements as one atomic unit. On a SQL Server deadlock or lock
// timeout, the complete transaction is rolled back and retried (never an
// individual statement in the middle of the transaction).
bool execDBTransaction(const QStringList& statements,
    QString connection = MAIN_DB_CONNECTION,
    int maxRetries = 3, QString* errorMessage = NULL);
