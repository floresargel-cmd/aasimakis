#ifndef BASKETCRANE_CONFIGURATION_DATABASECONFIGURATOR_H
#define BASKETCRANE_CONFIGURATION_DATABASECONFIGURATOR_H

#include "Settings.h"

class QSqlDatabase;

namespace BasketCrane
{
namespace Configuration
{

class DatabaseConfigurator
{
public:
	static void Apply(QSqlDatabase& database, const DatabaseProfile& profile);

private:
	DatabaseConfigurator();
};

}
}

#endif
