#include "LegacyDatabaseBootstrapper.h"

#include "../Logging/FileLogger.h"
#include "../SqlScripts/LegacyDatabaseBootstrapSqlScripts.h"
#include "../SqlScripts/LegacyDatabaseTableDefinitions.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qvariant.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqldatabase.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqldatabase.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlerror.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlquery.h"
#include "../../Application/Services/LegacyRuntimeContext.h"
#include "../../legacy/ppG.h"
#include <memory>
#include <vector>

namespace StackingPlan
{
	namespace Infrastructure
	{
		namespace Database
		{
			using StackingPlan::Infrastructure::SqlScripts::LegacyDatabaseBootstrapSqlScripts;
			using StackingPlan::Infrastructure::SqlScripts::LegacyDatabaseTableDefinition;
			using StackingPlan::Infrastructure::SqlScripts::LegacyDatabaseTableDefinitions;

			LegacyDatabaseBootstrapper::LegacyDatabaseBootstrapper(
				std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger)
				: _logger(logger)
			{
			}

			bool LegacyDatabaseBootstrapper::Initialize(
				StackingPlan::Application::Services::LegacyRuntimeContext& runtimeContext)
			{
				if (!OpenMainDatabase(runtimeContext))
				{
					return false;
				}

				if (!OpenBasketCraneDatabase(runtimeContext))
				{
					return false;
				}

				if (!EnsureTables())
				{
					return false;
				}

				if (!EnsureSeedData())
				{
					return false;
				}

				if (!LoadRuntimeContext(runtimeContext))
				{
					return false;
				}

				return true;
			}

			bool LegacyDatabaseBootstrapper::OpenMainDatabase(
				const StackingPlan::Application::Services::LegacyRuntimeContext& runtimeContext)
			{
				QSqlDatabase mainDb;

				if (QSqlDatabase::contains(runtimeContext.mainDbConnectionName))
				{
					mainDb = QSqlDatabase::database(runtimeContext.mainDbConnectionName);
				}
				else
				{
					mainDb = QSqlDatabase::addDatabase(runtimeContext.mainDbDriver, runtimeContext.mainDbConnectionName);
					mainDb.setDatabaseName(runtimeContext.mainDbConnectionString);
				}

				if (_logger)
				{
					_logger->Info(QString("Requested DB driver: %1").arg(runtimeContext.mainDbDriver));
					_logger->Info(QString("Available Qt SQL drivers: %1").arg(QSqlDatabase::drivers().join(", ")));
					_logger->Info(QString("Main connection string: %1").arg(runtimeContext.mainDbConnectionString));
				}

				if (mainDb.isOpen())
				{
					return true;
				}

				if (!mainDb.open())
				{
					if (_logger)
					{
						_logger->Error(QString("Failed to open main database: %1 | ConnectionName=%2")
							.arg(mainDb.lastError().text())
							.arg(runtimeContext.mainDbConnectionName));
					}
					return false;
				}

				return true;
			}

			bool LegacyDatabaseBootstrapper::OpenBasketCraneDatabase(
				const StackingPlan::Application::Services::LegacyRuntimeContext& runtimeContext)
			{
				QSqlDatabase basketCraneDb;

				if (QSqlDatabase::contains(runtimeContext.basketCraneConnectionName))
				{
					basketCraneDb = QSqlDatabase::database(runtimeContext.basketCraneConnectionName);
				}
				else
				{
					basketCraneDb = QSqlDatabase::addDatabase(
						runtimeContext.basketCraneDriver,
						runtimeContext.basketCraneConnectionName);
					basketCraneDb.setDatabaseName(runtimeContext.basketCraneConnectionString);
				}

				if (!basketCraneDb.isOpen())
				{
					if (!basketCraneDb.open())
					{
						if (_logger)
						{
							_logger->Error(QString("Failed to open basket crane database: %1 | ConnectionName=%2")
								.arg(basketCraneDb.lastError().text())
								.arg(runtimeContext.basketCraneConnectionName));
						}
						return false;
					}
				}

				return ExecuteOnBasketCrane(LegacyDatabaseBootstrapSqlScripts::UseBasketCraneDatabase());
			}

			bool LegacyDatabaseBootstrapper::EnsureTables()
			{
				const std::vector<LegacyDatabaseTableDefinition> tables =
					LegacyDatabaseTableDefinitions::GetMainTableDefinitions();

				for (size_t i = 0; i < tables.size(); ++i)
				{
					const LegacyDatabaseTableDefinition& table = tables[i];

					if (!ExecuteOnMain(
						LegacyDatabaseBootstrapSqlScripts::CreateTableStatement(
							table.tableName,
							table.columnNames,
							table.columnTypes)))
					{
						return false;
					}
				}

				return true;
			}

			bool LegacyDatabaseBootstrapper::EnsureSeedData()
			{
				return ExecuteOnMain(LegacyDatabaseBootstrapSqlScripts::EnsureAdminUser())
					&& ExecuteOnMain(LegacyDatabaseBootstrapSqlScripts::EnsureLayerMinSupportLengthSetting())
					&& ExecuteOnMain(LegacyDatabaseBootstrapSqlScripts::EnsureArcAngleSetting())
					&& ExecuteOnMain(LegacyDatabaseBootstrapSqlScripts::TrimUserLog());
			}

			bool LegacyDatabaseBootstrapper::LoadRuntimeContext(
				StackingPlan::Application::Services::LegacyRuntimeContext& runtimeContext)
			{
				if (runtimeContext.privilegeType == 0)
				{
					runtimeContext.privilegeType =
						PRIVILEGE_TYPE_OFFICE | PRIVILEGE_TYPE_PRESS | PRIVILEGE_TYPE_STACKER;
				}

				if (runtimeContext.loginName.isEmpty())
				{
					runtimeContext.loginName = "admin";
				}

				const QString arcAngleValue =
					ExecuteScalarOnMain(LegacyDatabaseBootstrapSqlScripts::SelectArcAngleForLine());

				if (runtimeContext.arcAngleForLine <= 0 && !arcAngleValue.isEmpty())
				{
					runtimeContext.arcAngleForLine = arcAngleValue.toDouble();
				}

				const QString maxProfilesValue =
					ExecuteScalarOnMain(
						LegacyDatabaseBootstrapSqlScripts::SelectMaxProfilesToLoad(runtimeContext.loginName));

				if (!maxProfilesValue.isEmpty())
				{
					runtimeContext.maxNoOfProfilesToLoad = maxProfilesValue.toInt();
				}

				if (runtimeContext.maxNoOfProfilesToLoad < 1)
				{
					runtimeContext.maxNoOfProfilesToLoad = 1;
				}

				return true;
			}

			bool LegacyDatabaseBootstrapper::ExecuteOnMain(const QString& sql)
			{
				QSqlDatabase db = QSqlDatabase::database(MAIN_DB_CONNECTION);
				QSqlQuery query(db);

				if (!query.exec(sql))
				{
					if (_logger)
					{
						_logger->Error(QString("ExecuteOnMain failed: %1 | SQL: %2")
							.arg(query.lastError().text())
							.arg(sql));
					}
					return false;
				}

				return true;
			}

			bool LegacyDatabaseBootstrapper::ExecuteOnBasketCrane(const QString& sql)
			{
				QSqlDatabase db = QSqlDatabase::database(BASKET_CRANE_DB_CONNECTION);
				QSqlQuery query(db);

				if (!query.exec(sql))
				{
					if (_logger)
					{
						_logger->Error(QString("ExecuteOnBasketCrane failed: %1 | SQL: %2")
							.arg(query.lastError().text())
							.arg(sql));
					}
					return false;
				}

				return true;
			}

			QString LegacyDatabaseBootstrapper::ExecuteScalarOnMain(const QString& sql)
			{
				QSqlDatabase db = QSqlDatabase::database(MAIN_DB_CONNECTION);
				QSqlQuery query(db);

				if (!query.exec(sql))
				{
					if (_logger)
					{
						_logger->Error(QString("ExecuteScalarOnMain failed: %1 | SQL: %2")
							.arg(query.lastError().text())
							.arg(sql));
					}
					return QString();
				}

				if (query.next())
				{
					return query.value(0).toString();
				}

				return QString();
			}
		}
	}
}
