#pragma once

#include <QVector>
#include <QVariant>
#include <QString>

#include "..\G.h" // for MAIN_DB_CONNECTION

// Forward-declare the existing function (wherever it's actually defined)
QVector<QVariant> execQuery(QString stmnt, QString connection);

QVector<QVariant> execDBQuery(QString stmnt, QString connection = MAIN_DB_CONNECTION);