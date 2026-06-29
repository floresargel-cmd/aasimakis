#ifndef DBTABLESTATUS_H
#define DBTABLESTATUS_H

#include <QString>

// Call before execQuery: isRead true = green (read), false = blue (write)
void setDbTableActive(const QString& tableName, bool isRead);
// Call after execQuery to restore grey
void setDbTableIdle(const QString& tableName);

#endif
