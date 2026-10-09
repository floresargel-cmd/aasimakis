#pragma once

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../Application/Interfaces/IProductionRepository.h"
#include "../../Domain/Entities/ProductionProfile.h"

#include <memory>

namespace StackingPlan::Infrastructure::Database
{
	class SqlCommandExecutor;
}

namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::Infrastructure::Repositories
{
	/**
	 * @brief Repository implementation for managing ProductionProfile entities in the database.
	 *
	 * The ProductionRepository class provides methods to retrieve and update production profile data,
	 * interacting with the underlying database using a SqlCommandExecutor and logging actions via a FileLogger.
	 * Implements the IProductionRepository interface.
	 */
	class ProductionRepository final
		: public StackingPlan::Application::Interfaces::IProductionRepository
	{
	public:
		/**
		 * @brief Constructs a ProductionRepository with the required dependencies.
		 * @param sqlCommandExecutor Shared pointer to the SQL command executor for database operations.
		 * @param logger Shared pointer to the file logger for logging repository actions.
		 */
		ProductionRepository(
			std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> sqlCommandExecutor,
			std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger);

		/**
		 * @brief Retrieves all production profiles associated with a specific press number.
		 * @param pressNumber The press number to filter production profiles.
		 * @return QVector of ProductionProfile entities matching the press number.
		 */
		QVector<StackingPlan::Domain::Entities::ProductionProfile> GetByPress(int pressNumber) override;

		/**
		 * @brief Retrieves a production profile by its unique identifier.
		 * @param id The unique identifier of the production profile.
		 * @return The ProductionProfile entity with the specified id.
		 */
		StackingPlan::Domain::Entities::ProductionProfile GetById(int id) override;

		/**
		 * @brief Updates the status of a production profile.
		 * @param id The unique identifier of the production profile.
		 * @param status The new status value.
		 * @return True if the update was successful, false otherwise.
		 */
		bool UpdateStatus(int id, int status) override;

		/**
		 * @brief Updates the total and basket quantities of a production profile.
		 * @param id The unique identifier of the production profile.
		 * @param totalQuantity The new total quantity.
		 * @param quantityInBaskets The new quantity in baskets.
		 * @return True if the update was successful, false otherwise.
		 */
		bool UpdateQuantities(int id, int totalQuantity, int quantityInBaskets) override;

		/**
		 * @brief Updates the temper value of a production profile.
		 * @param id The unique identifier of the production profile.
		 * @param temper The new temper value.
		 * @return True if the update was successful, false otherwise.
		 */
		bool UpdateTemper(int id, const QString& temper) override;

		/**
		 * @brief Updates the length of a production profile.
		 * @param id The unique identifier of the production profile.
		 * @param length The new length value.
		 * @return True if the update was successful, false otherwise.
		 */
		bool UpdateLength(int id, int length) override;

		/**
		 * @brief Updates the linear density of a production profile.
		 * @param id The unique identifier of the production profile.
		 * @param linearDensity The new linear density value.
		 * @return True if the update was successful, false otherwise.
		 */
		bool UpdateLinearDensity(int id, double linearDensity) override;

		/**
		 * @brief Updates the double length value of a production profile.
		 * @param id The unique identifier of the production profile.
		 * @param doubleLength The new double length value.
		 * @return True if the update was successful, false otherwise.
		 */
		bool UpdateDoubleLength(int id, int doubleLength) override;

		/**
		 * @brief Updates the "on next basket" flag of a production profile.
		 * @param id The unique identifier of the production profile.
		 * @param onNextBasket The new value for the on next basket flag.
		 * @return True if the update was successful, false otherwise.
		 */
		bool UpdateOnNextBasket(int id, int onNextBasket) override;

		/**
		 * @brief Updates the "on next layer" flag of a production profile.
		 * @param id The unique identifier of the production profile.
		 * @param onNextLayer The new value for the on next layer flag.
		 * @return True if the update was successful, false otherwise.
		 */
		bool UpdateOnNextLayer(int id, int onNextLayer) override;

	private:
		/**
		 * @brief Executes a generic SQL update statement.
		 * @param sql The SQL update statement to execute.
		 * @return True if the execution was successful, false otherwise.
		 */
		bool ExecuteUpdate(const QString& sql);

	private:
		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> _sqlCommandExecutor; ///< Database command executor.
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger; ///< Logger for repository actions.
	};
}
