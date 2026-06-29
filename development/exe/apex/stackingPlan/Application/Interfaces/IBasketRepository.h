#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#pragma once


namespace StackingPlan::Application::Interfaces
{
	/**
	 * @brief Interface for basket repository operations.
	 *
	 * Provides methods for accessing and manipulating basket data, production sequences,
	 * spacers, and related user update flags within the stacking plan domain.
	 */
	class IBasketRepository
	{
	public:
		/// Virtual destructor for safe polymorphic use.
		virtual ~IBasketRepository() = default;

		/**
		 * @brief Retrieves the sales order number associated with a given TID.
		 * @param tid The unique identifier for the transaction/item.
		 * @return The sales order number as a QString.
		 */
		virtual QString GetSoNumberByTid(int tid) = 0;

		/**
		 * @brief Retrieves the die information associated with a given TID.
		 * @param tid The unique identifier for the transaction/item.
		 * @return The die information as a QString.
		 */
		virtual QString GetDieByTid(int tid) = 0;

		/**
		 * @brief Reorders the production sequence by moving an item before another.
		 * @param beforeTid The TID before which the dropped item will be placed.
		 * @param droppedTid The TID of the item to move.
		 * @return True if the operation was successful, false otherwise.
		 */
		virtual bool ReorderProductionSequence(int beforeTid, int droppedTid) = 0;

		/**
		 * @brief Gets the basket table ID associated with a given position ID.
		 * @param positionId The position identifier.
		 * @return The basket table ID.
		 */
		virtual int GetBasketTableIdByPositionId(int positionId) = 0;

		/**
		 * @brief Marks that users need to be updated, typically after a data change.
		 * @return True if the operation was successful, false otherwise.
		 */
		virtual bool MarkUsersNeedsUpdate() = 0;

		/**
		 * @brief Retrieves the pending basket quantity for a given TID and basket.
		 * @param tid The transaction/item identifier.
		 * @param basketId The basket identifier.
		 * @return The quantity of pending items in the basket.
		 */
		virtual int GetPendingBasketQuantityByTidAndBasket(int tid, int basketId) = 0;

		/**
		 * @brief Deletes all spacers associated with a specific basket.
		 * @param basketId The basket identifier.
		 * @return True if the spacers were deleted successfully, false otherwise.
		 */
		virtual bool DeleteBasketSpacers(int basketId) = 0;

		/**
		 * @brief Inserts a spacer into a basket at a specified layer and combination.
		 * @param basketId The basket identifier.
		 * @param layer The layer number.
		 * @param inComb The combination index.
		 * @param sp1-sp8 Spacer values for the layer.
		 * @return True if the spacer was inserted successfully, false otherwise.
		 */
		virtual bool InsertBasketSpacer(
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
			int sp8) = 0;

		/**
		 * @brief Increments the quantity in baskets for a given TID.
		 * @param tid The transaction/item identifier.
		 * @param quantityToAdd The quantity to add.
		 * @return True if the quantity was incremented successfully, false otherwise.
		 */
		virtual bool IncrementQuantityInBaskets(int tid, int quantityToAdd) = 0;
	};
}