/**
 * @file ProductionRepository.cpp
 * @brief Implements the ProductionRepository class for managing production profile data access.
 *
 * This file contains the implementation of the ProductionRepository class, which provides
 * methods for retrieving and updating production profile records in the database. It uses
 * a SqlCommandExecutor for database operations and a FileLogger for error logging.
 */

#include "ProductionRepository.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqldatabase.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/sql/kernel/qsqlquery.h"
#include "../../Domain/Entities/ProductionProfile.h"
#include "../Database/SqlCommandExecutor.h"
#include "../Logging/FileLogger.h"
#include "../SqlScripts/ProductionSqlScripts.h"
#include <memory>
#include <QtSql/QSqlError>

namespace StackingPlan::Infrastructure::Repositories
{
	using StackingPlan::Domain::Entities::ProductionProfile;
	using StackingPlan::Infrastructure::SqlScripts::ProductionSqlScripts;

	/**
	 * @brief Constructs a ProductionRepository with the given SQL command executor and logger.
	 * @param sqlCommandExecutor Shared pointer to the SQL command executor.
	 * @param logger Shared pointer to the file logger.
	 */
	ProductionRepository::ProductionRepository(
		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> sqlCommandExecutor,
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger)
		: _sqlCommandExecutor(sqlCommandExecutor),
		_logger(logger)
	{
	}

	/**
	 * @brief Retrieves all production profiles for a given press number.
	 * @param pressNumber The press number to filter by.
	 * @return QVector of ProductionProfile objects matching the press number.
	 *         Returns an empty vector if the database cannot be opened or the query fails.
	 */
	QVector<ProductionProfile> ProductionRepository::GetByPress(int pressNumber)
	{
		QVector<ProductionProfile> results;

		if (!_sqlCommandExecutor->Open())
		{
			if (_logger)
			{
				_logger->Error(QString("ProductionRepository::GetByPress failed to open database: %1")
					.arg(_sqlCommandExecutor->GetLastError()));
			}

			return results;
		}

		QSqlDatabase database = QSqlDatabase::database(_sqlCommandExecutor->GetConnectionName());
		if (!database.isValid() || !database.isOpen())
		{
			if (_logger)
			{
				_logger->Error(QString("ProductionRepository::GetByPress invalid or closed database connection."));
			}

			return results;
		}

		QSqlQuery query(database);
		query.prepare(ProductionSqlScripts::GetByPress());
		query.bindValue(":pressNum", pressNumber);

		if (!query.exec())
		{
			if (_logger)
			{
				_logger->Error(QString("ProductionRepository::GetByPress query failed: %1")
					.arg(query.lastError().text()));
			}

			return results;
		}

		while (query.next())
		{
			ProductionProfile item;
			item.id = query.value("tid").toInt();
			item.pressNumber = query.value("pressNum").toInt();
			item.dieNumber = query.value("die").toInt();
			item.productionOrder = query.value("productionOrder").toInt();
			item.totalQuantity = query.value("totalQuantity").toInt();
			item.quantityInBaskets = query.value("quantityInBaskets").toInt();
			item.soNumber = query.value("soNum").toString();
			item.temper = query.value("temper").toString().trimmed();
			item.length = query.value("length").toInt();
			item.onNextBasket = query.value("onNextBasket").toInt() != 0;
			item.onNextLayer = query.value("onNextLayer").toInt() != 0;

			results.push_back(item);
		}

		return results;
	}

	/**
	 * @brief Retrieves a production profile by its unique ID.
	 * @param id The unique identifier of the production profile.
	 * @return The ProductionProfile object if found; otherwise, a default-constructed object.
	 */
	ProductionProfile ProductionRepository::GetById(int id)
	{
		ProductionProfile item;

		if (!_sqlCommandExecutor->Open())
		{
			if (_logger)
			{
				_logger->Error(QString("ProductionRepository::GetById failed to open database: %1")
					.arg(_sqlCommandExecutor->GetLastError()));
			}

			return item;
		}

		QSqlDatabase database = QSqlDatabase::database(_sqlCommandExecutor->GetConnectionName());
		if (!database.isValid() || !database.isOpen())
		{
			if (_logger)
			{
				_logger->Error(QString("ProductionRepository::GetById invalid or closed database connection."));
			}

			return item;
		}

		QSqlQuery query(database);
		query.prepare(ProductionSqlScripts::GetById());
		query.bindValue(":id", id);

		if (!query.exec())
		{
			if (_logger)
			{
				_logger->Error(QString("ProductionRepository::GetById query failed: %1")
					.arg(query.lastError().text()));
			}

			return item;
		}

		if (query.next())
		{
			item.id = query.value("tid").toInt();
			item.pressNumber = query.value("pressNum").toInt();
			item.dieNumber = query.value("die").toInt();
			item.productionOrder = query.value("productionOrder").toInt();
			item.totalQuantity = query.value("totalQuantity").toInt();
			item.quantityInBaskets = query.value("quantityInBaskets").toInt();
			item.soNumber = query.value("soNum").toString();
			item.temper = query.value("temper").toString().trimmed();
			item.length = query.value("length").toInt();
			item.onNextBasket = query.value("onNextBasket").toInt() != 0;
			item.onNextLayer = query.value("onNextLayer").toInt() != 0;
		}

		return item;
	}

	/**
	 * @brief Updates the status of a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param status The new status value.
	 * @return True if the update succeeds; false otherwise.
	 */
	bool ProductionRepository::UpdateStatus(int id, int status)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionSqlScripts::UpdateStatus());
				query.bindValue(":status", status);
				query.bindValue(":id", id);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionRepository::UpdateStatus failed: %1")
							.arg(query.lastError().text()));
					}

					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Updates the total quantity and quantity in baskets for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param totalQuantity The new total quantity.
	 * @param quantityInBaskets The new quantity in baskets.
	 * @return True if the update succeeds; false otherwise.
	 */
	bool ProductionRepository::UpdateQuantities(int id, int totalQuantity, int quantityInBaskets)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionSqlScripts::UpdateQuantities());
				query.bindValue(":totalQuantity", totalQuantity);
				query.bindValue(":quantityInBaskets", quantityInBaskets);
				query.bindValue(":id", id);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionRepository::UpdateQuantities failed: %1")
							.arg(query.lastError().text()));
					}

					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Updates the temper value for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param temper The new temper value.
	 * @return True if the update succeeds; false otherwise.
	 */
	bool ProductionRepository::UpdateTemper(int id, const QString& temper)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionSqlScripts::UpdateTemper());
				query.bindValue(":temper", temper);
				query.bindValue(":id", id);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionRepository::UpdateTemper failed: %1")
							.arg(query.lastError().text()));
					}

					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Updates the length value for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param length The new length value.
	 * @return True if the update succeeds; false otherwise.
	 */
	bool ProductionRepository::UpdateLength(int id, int length)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionSqlScripts::UpdateLength());
				query.bindValue(":length", length);
				query.bindValue(":id", id);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionRepository::UpdateLength failed: %1")
							.arg(query.lastError().text()));
					}

					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Updates the linear density for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param linearDensity The new linear density value.
	 * @return True if the update succeeds; false otherwise.
	 */
	bool ProductionRepository::UpdateLinearDensity(int id, double linearDensity)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionSqlScripts::UpdateLinearDensity());
				query.bindValue(":linearDensity", linearDensity);
				query.bindValue(":id", id);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionRepository::UpdateLinearDensity failed: %1")
							.arg(query.lastError().text()));
					}

					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Updates the double length value for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param doubleLength The new double length value.
	 * @return True if the update succeeds; false otherwise.
	 */
	bool ProductionRepository::UpdateDoubleLength(int id, int doubleLength)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionSqlScripts::UpdateDoubleLength());
				query.bindValue(":doubleLength", doubleLength);
				query.bindValue(":id", id);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionRepository::UpdateDoubleLength failed: %1")
							.arg(query.lastError().text()));
					}

					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Updates the onNextBasket flag for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param onNextBasket The new value for the onNextBasket flag.
	 * @return True if the update succeeds; false otherwise.
	 */
	bool ProductionRepository::UpdateOnNextBasket(int id, int onNextBasket)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionSqlScripts::UpdateOnNextBasket());
				query.bindValue(":onNextBasket", onNextBasket);
				query.bindValue(":id", id);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionRepository::UpdateOnNextBasket failed: %1")
							.arg(query.lastError().text()));
					}

					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Updates the onNextLayer flag for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param onNextLayer The new value for the onNextLayer flag.
	 * @return True if the update succeeds; false otherwise.
	 */
	bool ProductionRepository::UpdateOnNextLayer(int id, int onNextLayer)
	{
		return _sqlCommandExecutor->ExecuteInTransaction([&](QSqlDatabase& database) -> bool
			{
				QSqlQuery query(database);
				query.prepare(ProductionSqlScripts::UpdateOnNextLayer());
				query.bindValue(":onNextLayer", onNextLayer);
				query.bindValue(":id", id);

				if (!query.exec())
				{
					if (_logger)
					{
						_logger->Error(QString("ProductionRepository::UpdateOnNextLayer failed: %1")
							.arg(query.lastError().text()));
					}

					return false;
				}

				return true;
			});
	}

	/**
	 * @brief Executes a raw SQL update statement.
	 * @param sql The SQL statement to execute.
	 * @return True if the execution succeeds; false otherwise.
	 */
	bool ProductionRepository::ExecuteUpdate(const QString& sql)
	{
		return _sqlCommandExecutor->ExecuteNonQuery(sql);
	}
}
