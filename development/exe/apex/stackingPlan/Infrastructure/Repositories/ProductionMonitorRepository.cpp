#include "ProductionMonitorRepository.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqldatabase.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlquery.h"
#include "../Database/SqlCommandExecutor.h"
#include "../Logging/FileLogger.h"
#include "../SqlScripts/ProductionMonitorSqlScripts.h"
#include <memory>
#include <QSqlError>


namespace StackingPlan::Infrastructure::Repositories
{
	using StackingPlan::Infrastructure::SqlScripts::ProductionMonitorSqlScripts;

	ProductionMonitorRepository::ProductionMonitorRepository(
		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> sqlCommandExecutor,
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger)
		: _sqlCommandExecutor(sqlCommandExecutor),
		_logger(logger)
	{
	}

	bool ProductionMonitorRepository::NormalizeStatusMinus2Production()
	{
		return _sqlCommandExecutor->ExecuteNonQuery(
			ProductionMonitorSqlScripts::NormalizeStatusMinus2Production());
	}

	bool ProductionMonitorRepository::ActivateInitialProduction()
	{
		return _sqlCommandExecutor->ExecuteNonQuery(
			ProductionMonitorSqlScripts::ActivateInitialProduction());
	}

	bool ProductionMonitorRepository::DoesUserNeedRefresh(const QString& loginName)
	{
		if (!_sqlCommandExecutor->Open())
		{
			if (_logger)
			{
				_logger->Error(QString("ProductionMonitorRepository::DoesUserNeedRefresh failed to open database: %1")
					.arg(_sqlCommandExecutor->GetLastError()));
			}
			return false;
		}

		QSqlDatabase database = QSqlDatabase::database(_sqlCommandExecutor->GetConnectionName());
		QSqlQuery query(database);
		query.prepare(ProductionMonitorSqlScripts::SelectUserNeedsUpdate());
		query.bindValue(":loginName", loginName);

		if (!query.exec())
		{
			if (_logger)
			{
				_logger->Error(QString("ProductionMonitorRepository::DoesUserNeedRefresh query failed: %1")
					.arg(query.lastError().text()));
			}
			return false;
		}

		if (query.next())
		{
			return query.value(0).toBool();
		}

		return false;
	}

	bool ProductionMonitorRepository::ResetUserNeedsUpdate(const QString& loginName)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionMonitorSqlScripts::ResetUserNeedsUpdate());
				query.bindValue(":loginName", loginName);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionMonitorRepository::ResetUserNeedsUpdate failed: %1")
							.arg(query.lastError().text()));
					}
					return false;
				}

				return true;
			});
	}

	bool ProductionMonitorRepository::InsertVirtualProduction(int dieNumber)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionMonitorSqlScripts::InsertVirtualProduction());
				query.bindValue(":die", dieNumber);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionMonitorRepository::InsertVirtualProduction failed: %1")
							.arg(query.lastError().text()));
					}
					return false;
				}

				return true;
			});
	}

	bool ProductionMonitorRepository::MarkProductionAsNext(int tid)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionMonitorSqlScripts::MarkProductionAsNext());
				query.bindValue(":tid", tid);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionMonitorRepository::MarkProductionAsNext failed: %1")
							.arg(query.lastError().text()));
					}
					return false;
				}

				return true;
			});
	}
}
