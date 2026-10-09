/**
 * @file BasketRepository.cpp
 * @brief Implements the BasketRepository class, providing data access methods for basket-related operations.
 *
 * This repository encapsulates all database interactions related to baskets, including queries and updates.
 * It uses a SqlCommandExecutor for executing SQL commands and a FileLogger for error logging.
 */

#include "BasketRepository.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqldatabase.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlquery.h"
#include "../Database/SqlCommandExecutor.h"
#include "../Logging/FileLogger.h"
#include "../SqlScripts/BasketSqlScripts.h"
#include <QSqlError>

#include <memory>


namespace StackingPlan::Infrastructure::Repositories
{
	using StackingPlan::Infrastructure::SqlScripts::BasketSqlScripts;

	/**
	 * @brief Constructs a BasketRepository with the given SQL command executor and logger.
	 * @param sqlCommandExecutor Shared pointer to the SQL command executor.
	 * @param logger Shared pointer to the file logger.
	 */
	BasketRepository::BasketRepository(
		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> sqlCommandExecutor,
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger)
		: _sqlCommandExecutor(sqlCommandExecutor),
		_logger(logger)
	{
	}

	/**
	 * @brief Retrieves the SO (Sales Order) number for a given TID.
	 * @param tid The transaction ID.
	 * @return The SO number as a QString, or an empty string if not found or on error.
	 */
	QString BasketRepository::GetSoNumberByTid(int tid)
	{
		QString result;

		if (!_sqlCommandExecutor->Open())
		{
			if (_logger)
			{
				_logger->Error(QString("BasketRepository::GetSoNumberByTid failed to open database: %1")
					.arg(_sqlCommandExecutor->GetLastError()));
			}
			return result;
		}

		QSqlDatabase database = QSqlDatabase::database(_sqlCommandExecutor->GetConnectionName());
		QSqlQuery query(database);
		query.prepare(BasketSqlScripts::GetSoNumberByTid());
		query.bindValue(":tid", tid);

		if (!query.exec())
		{
			if (_logger)
			{
				_logger->Error(QString("BasketRepository::GetSoNumberByTid query failed: %1")
					.arg(query.lastError().text()));
			}
			return result;
		}

		if (query.next())
		{
			result = query.value(0).toString();
		}

		return result;
	}

	/**
	 * @brief Retrieves the die information for a given TID.
	 * @param tid The transaction ID.
	 * @return The die as a QString, or an empty string if not found or on error.
	 */
	QString BasketRepository::GetDieByTid(int tid)
	{
		QString result;

		if (!_sqlCommandExecutor->Open())
		{
			if (_logger)
			{
				_logger->Error(QString("BasketRepository::GetDieByTid failed to open database: %1")
					.arg(_sqlCommandExecutor->GetLastError()));
			}
			return result;
		}

		QSqlDatabase database = QSqlDatabase::database(_sqlCommandExecutor->GetConnectionName());
		QSqlQuery query(database);
		query.prepare(BasketSqlScripts::GetDieByTid());
		query.bindValue(":tid", tid);

		if (!query.exec())
		{
			if (_logger)
			{
				_logger->Error(QString("BasketRepository::GetDieByTid query failed: %1")
					.arg(query.lastError().text()));
			}
			return result;
		}

		if (query.next())
		{
			result = query.value(0).toString();
		}

		return result;
	}

	/**
	 * @brief Reorders the production sequence by moving a basket before another.
	 * @param beforeTid The TID to move before.
	 * @param droppedTid The TID being moved.
	 * @return True if the operation succeeded, false otherwise.
	 */
	bool BasketRepository::ReorderProductionSequence(int beforeTid, int droppedTid)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(BasketSqlScripts::ReorderProductionSequence());
				query.bindValue(":beforeTid", beforeTid);
				query.bindValue(":droppedTid", droppedTid);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("BasketRepository::ReorderProductionSequence failed: %1")
							.arg(query.lastError().text()));
					}
					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Gets the basket table ID associated with a given position ID.
	 * @param positionId The position ID.
	 * @return The basket table ID, or 0 if not found or on error.
	 */
	int BasketRepository::GetBasketTableIdByPositionId(int positionId)
	{
		int result = 0;

		if (!_sqlCommandExecutor->Open())
		{
			if (_logger)
			{
				_logger->Error(QString("BasketRepository::GetBasketTableIdByPositionId failed to open database: %1")
					.arg(_sqlCommandExecutor->GetLastError()));
			}
			return result;
		}

		QSqlDatabase database = QSqlDatabase::database(_sqlCommandExecutor->GetConnectionName());
		QSqlQuery query(database);
		query.prepare(BasketSqlScripts::GetBasketTableIdByPositionId());
		query.bindValue(":positionId", positionId);

		if (!query.exec())
		{
			if (_logger)
			{
				_logger->Error(QString("BasketRepository::GetBasketTableIdByPositionId query failed: %1")
					.arg(query.lastError().text()));
			}
			return result;
		}

		if (query.next())
		{
			result = query.value(0).toInt();
		}

		return result;
	}

	/**
	 * @brief Marks all users as needing an update.
	 * @return True if the operation succeeded, false otherwise.
	 */
	bool BasketRepository::MarkUsersNeedsUpdate()
	{
		return _sqlCommandExecutor->ExecuteNonQuery(BasketSqlScripts::MarkUsersNeedsUpdate());
	}

	/**
	 * @brief Gets the pending basket quantity for a given TID and basket ID.
	 * @param tid The transaction ID.
	 * @param basketId The basket ID.
	 * @return The pending quantity, or 0 if not found or on error.
	 */
	int BasketRepository::GetPendingBasketQuantityByTidAndBasket(int tid, int basketId)
	{
		int result = 0;

		if (!_sqlCommandExecutor->Open())
		{
			if (_logger)
			{
				_logger->Error(QString("BasketRepository::GetPendingBasketQuantityByTidAndBasket failed to open database: %1")
					.arg(_sqlCommandExecutor->GetLastError()));
			}
			return result;
		}

		QSqlDatabase database = QSqlDatabase::database(_sqlCommandExecutor->GetConnectionName());
		QSqlQuery query(database);
		query.prepare(BasketSqlScripts::GetPendingBasketQuantityByTidAndBasket());
		query.bindValue(":tid", tid);
		query.bindValue(":basketId", basketId);

		if (!query.exec())
		{
			if (_logger)
			{
				_logger->Error(QString("BasketRepository::GetPendingBasketQuantityByTidAndBasket query failed: %1")
					.arg(query.lastError().text()));
			}
			return result;
		}

		if (query.next())
		{
			result = query.value(0).toInt();
		}

		return result;
	}

	/**
	 * @brief Deletes all spacers for a given basket.
	 * @param basketId The basket ID.
	 * @return True if the operation succeeded, false otherwise.
	 */
	bool BasketRepository::DeleteBasketSpacers(int basketId)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(BasketSqlScripts::DeleteBasketSpacers());
				query.bindValue(":basketId", basketId);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("BasketRepository::DeleteBasketSpacers failed: %1")
							.arg(query.lastError().text()));
					}
					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Inserts a new basket spacer record.
	 * @param basketId The basket ID.
	 * @param layer The layer number.
	 * @param inComb The combination index.
	 * @param sp1-sp8 Spacer values.
	 * @return True if the operation succeeded, false otherwise.
	 */
	bool BasketRepository::InsertBasketSpacer(
		int basketId,
		int layer,
		int inComb,
		int sp1,
		int sp2,
		int sp3,
		int sp4,
		int sp5,
		int sp6,
		int sp7,
		int sp8)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(BasketSqlScripts::InsertBasketSpacer());

				query.bindValue(":basketId", basketId);
				query.bindValue(":layer", layer);
				query.bindValue(":inComb", inComb);
				query.bindValue(":sp1", sp1);
				query.bindValue(":sp2", sp2);
				query.bindValue(":sp3", sp3);
				query.bindValue(":sp4", sp4);
				query.bindValue(":sp5", sp5);
				query.bindValue(":sp6", sp6);
				query.bindValue(":sp7", sp7);
				query.bindValue(":sp8", sp8);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("BasketRepository::InsertBasketSpacer failed: %1")
							.arg(query.lastError().text()));
					}
					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Increments the quantity in baskets for a given TID.
	 * @param tid The transaction ID.
	 * @param quantityToAdd The quantity to add.
	 * @return True if the operation succeeded, false otherwise.
	 */
	bool BasketRepository::IncrementQuantityInBaskets(int tid, int quantityToAdd)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(BasketSqlScripts::IncrementQuantityInBaskets());
				query.bindValue(":tid", tid);
				query.bindValue(":quantityToAdd", quantityToAdd);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("BasketRepository::IncrementQuantityInBaskets failed: %1")
							.arg(query.lastError().text()));
					}
					return false;
				}

				return true;
			});
	}
}
