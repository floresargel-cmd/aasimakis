#include "DatabaseConfigurator.h"

#include <QSqlDatabase>

namespace BasketCrane
{
namespace Configuration
{

void DatabaseConfigurator::Apply(
	QSqlDatabase& database,
	const DatabaseProfile& profile)
{
	database.setDatabaseName(profile.Dsn());
	database.setUserName(profile.User());
	database.setPassword(profile.Password());
	if (!profile.ConnectOptions().isEmpty())
		database.setConnectOptions(profile.ConnectOptions());
}

}
}
