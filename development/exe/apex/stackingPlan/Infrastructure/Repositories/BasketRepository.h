#pragma once

#include "../../Application/Interfaces/IBasketRepository.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
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
	 * @class BasketRepository
	 * @brief Implements the IBasketRepository interface to manage basket-related data operations.
	 *
	 * The BasketRepository class provides methods for accessing and manipulating basket data
	 * within the stacking plan application. It interacts with the database through a
	 * SqlCommandExecutor and logs operations using a FileLogger. This repository supports
	 * operations such as retrieving basket information, reordering production sequences,
	 * managing basket spacers, and updating basket quantities.
	 *
	 * @note This class is marked as final and cannot be inherited.
	 */
	class BasketRepository final
		: public StackingPlan::Application::Interfaces::IBasketRepository
	{
	public:
		/**
		 * @brief Constructs a BasketRepository with the required dependencies.
		 * @param sqlCommandExecutor Shared pointer to the SQL command executor for database operations.
		 * @param logger Shared pointer to the file logger for logging activities.
		 */
		BasketRepository(
			std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> sqlCommandExecutor,
			std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger);

		/**
		 * @brief Retrieves the sales order number associated with a given TID.
		 * @param tid The unique identifier for the transaction.
		 * @return The sales order number as a QString.
		 */
		QString GetSoNumberByTid(int tid) override;

		/**
		 * @brief Retrieves the die information associated with a given TID.
		 * @param tid The unique identifier for the transaction.
		 * @return The die information as a QString.
		 */
		QString GetDieByTid(int tid) override;

		/**
		 * @brief Reorders the production sequence by moving a basket before another.
		 * @param beforeTid The TID before which the dropped basket should be placed.
		 * @param droppedTid The TID of the basket to move.
		 * @return True if the operation was successful, false otherwise.
		 */
		bool ReorderProductionSequence(int beforeTid, int droppedTid) override;

		/**
		 * @brief Retrieves the basket table ID associated with a given position ID.
		 * @param positionId The position identifier.
		 * @return The basket table ID.
		 */
		int GetBasketTableIdByPositionId(int positionId) override;

		/**
		 * @brief Marks that users need an update, typically after a significant change.
		 * @return True if the operation was successful, false otherwise.
		 */
		bool MarkUsersNeedsUpdate() override;

		/**
		 * @brief Gets the quantity of pending baskets for a specific TID and basket.
		 * @param tid The transaction identifier.
		 * @param basketId The basket identifier.
		 * @return The quantity of pending baskets.
		 */
		int GetPendingBasketQuantityByTidAndBasket(int tid, int basketId) override;

		/**
		 * @brief Deletes all spacers associated with a specific basket.
		 * @param basketId The basket identifier.
		 * @return True if the operation was successful, false otherwise.
		 */
		bool DeleteBasketSpacers(int basketId) override;

		/**
		 * @brief Inserts a spacer record for a basket with specified parameters.
		 * @param basketId The basket identifier.
		 * @param layer The layer number.
		 * @param inComb The combination index.
		 * @param sp1-sp8 Spacer values for the basket.
		 * @return True if the operation was successful, false otherwise.
		 */
		bool InsertBasketSpacer(
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
			int sp8) override;

		/**
		 * @brief Increments the quantity in baskets for a given TID.
		 * @param tid The transaction identifier.
		 * @param quantityToAdd The quantity to add to the basket.
		 * @return True if the operation was successful, false otherwise.
		 */
		bool IncrementQuantityInBaskets(int tid, int quantityToAdd) override;

	private:
		std::shared_ptr<StackingPlan::Infrastructure::Database::SqlCommandExecutor> _sqlCommandExecutor; ///< Executes SQL commands for database access.
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger; ///< Logs repository operations and errors.
	};
}