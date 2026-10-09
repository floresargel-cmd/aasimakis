#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"

#include <memory>

namespace StackingPlan::Application::Interfaces
{
	class IBasketRepository;
}

namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::Application::Services
{
	/**
	 * @brief Service class providing business logic operations for basket management in the stacking plan application.
	 *
	 * The BasketApplicationService acts as an application layer service, orchestrating basket-related operations
	 * by interacting with the basket repository and logging infrastructure. It exposes methods for querying,
	 * updating, and managing basket data, including production sequence reordering, basket spacers, and quantity management.
	 */
	class BasketApplicationService
	{
	public:
		/**
		 * @brief Constructs a BasketApplicationService with the required repository and logger dependencies.
		 * @param basketRepository Shared pointer to the basket repository interface for data access.
		 * @param logger Shared pointer to the file logger for logging operations.
		 */
		BasketApplicationService(
			std::shared_ptr<StackingPlan::Application::Interfaces::IBasketRepository> basketRepository,
			std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger);

		/**
		 * @brief Retrieves the sales order number associated with a given TID.
		 * @param tid The unique identifier for the transaction/item.
		 * @return The sales order number as a QString.
		 */
		QString GetSoNumberByTid(int tid);

		/**
		 * @brief Retrieves the die information associated with a given TID.
		 * @param tid The unique identifier for the transaction/item.
		 * @return The die information as a QString.
		 */
		QString GetDieByTid(int tid);

		/**
		 * @brief Reorders the production sequence by moving an item before another.
		 * @param beforeTid The TID before which the dropped item should be placed.
		 * @param droppedTid The TID of the item to move.
		 * @return True if the operation succeeds, false otherwise.
		 */
		bool ReorderProductionSequence(int beforeTid, int droppedTid);

		/**
		 * @brief Gets the basket table ID corresponding to a given position ID.
		 * @param positionId The position identifier.
		 * @return The basket table ID.
		 */
		int GetBasketTableIdByPositionId(int positionId);

		/**
		 * @brief Marks that users need to update their data, possibly triggering a refresh.
		 * @return True if the operation succeeds, false otherwise.
		 */
		bool MarkUsersNeedsUpdate();

		/**
		 * @brief Retrieves the quantity of pending baskets for a given TID and basket ID.
		 * @param tid The transaction/item identifier.
		 * @param basketId The basket identifier.
		 * @return The quantity of pending baskets.
		 */
		int GetPendingBasketQuantityByTidAndBasket(int tid, int basketId);

		/**
		 * @brief Deletes all spacers associated with a specific basket.
		 * @param basketId The basket identifier.
		 * @return True if the operation succeeds, false otherwise.
		 */
		bool DeleteBasketSpacers(int basketId);

		/**
		 * @brief Inserts a spacer configuration for a basket.
		 * @param basketId The basket identifier.
		 * @param layer The layer number.
		 * @param inComb The combination index.
		 * @param sp1-sp8 Spacer values for the configuration.
		 * @return True if the operation succeeds, false otherwise.
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
			int sp8);

		/**
		 * @brief Increments the quantity in baskets for a given TID.
		 * @param tid The transaction/item identifier.
		 * @param quantityToAdd The quantity to add.
		 * @return True if the operation succeeds, false otherwise.
		 */
		bool IncrementQuantityInBaskets(int tid, int quantityToAdd);

	private:
		std::shared_ptr<StackingPlan::Application::Interfaces::IBasketRepository> _basketRepository; ///< Repository for basket data access.
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger; ///< Logger for recording operations.
	};
}
