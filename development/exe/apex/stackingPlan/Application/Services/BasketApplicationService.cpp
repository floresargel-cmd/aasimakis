/**
 * @file BasketApplicationService.cpp
 * @brief Implements the BasketApplicationService class, providing application-level operations for basket management.
 *
 * This service acts as an intermediary between the application layer and the data access layer (repository),
 * encapsulating business logic related to basket operations. It delegates data operations to the IBasketRepository
 * interface and logs significant actions using the FileLogger.
 */

#include "BasketApplicationService.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../Infrastructure/Logging/FileLogger.h"
#include "../Interfaces/IBasketRepository.h"
#include <memory>

namespace StackingPlan::Application::Services
{
	/**
	 * @brief Constructs a BasketApplicationService with the given repository and logger.
	 * @param basketRepository Shared pointer to the basket repository interface.
	 * @param logger Shared pointer to the file logger for logging operations.
	 */
	BasketApplicationService::BasketApplicationService(
		std::shared_ptr<StackingPlan::Application::Interfaces::IBasketRepository> basketRepository,
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger)
		: _basketRepository(basketRepository),
		_logger(logger)
	{
	}

	/**
	 * @brief Retrieves the sales order number associated with a given TID.
	 * @param tid The TID for which to retrieve the sales order number.
	 * @return The sales order number as a QString.
	 */
	QString BasketApplicationService::GetSoNumberByTid(int tid)
	{
		_logger->Info(QString("BasketApplicationService::GetSoNumberByTid TID=%1").arg(tid));
		return _basketRepository->GetSoNumberByTid(tid);

	}

	/**
	 * @brief Retrieves the die information associated with a given TID.
	 * @param tid The TID for which to retrieve the die information.
	 * @return The die information as a QString.
	 */
	QString BasketApplicationService::GetDieByTid(int tid)
	{
		_logger->Info(QString("BasketApplicationService::GetDieByTid TID=%1").arg(tid));
		return _basketRepository->GetDieByTid(tid);

	}

	/**
	 * @brief Reorders the production sequence by moving a TID before another.
	 * @param beforeTid The TID before which the dropped TID will be placed.
	 * @param droppedTid The TID to be moved.
	 * @return True if the operation was successful, false otherwise.
	 */
	bool BasketApplicationService::ReorderProductionSequence(int beforeTid, int droppedTid)
	{
		if (_logger)
		{
			_logger->Info(QString("BasketApplicationService::ReorderProductionSequence BeforeTid=%1 DroppedTid=%2")
				.arg(beforeTid)
				.arg(droppedTid));
		}

		return _basketRepository->ReorderProductionSequence(beforeTid, droppedTid);
	}

	/**
	 * @brief Retrieves the basket table ID associated with a given position ID.
	 * @param positionId The position ID for which to retrieve the basket table ID.
	 * @return The basket table ID as an integer.
	 */
	int BasketApplicationService::GetBasketTableIdByPositionId(int positionId)
	{
		_logger->Info(QString("BasketApplicationService::GetBasketTableIdByPositionId PositionId=%1").arg(positionId));
		return _basketRepository->GetBasketTableIdByPositionId(positionId);

	}

	/**
	 * @brief Marks that users need to be updated.
	 * @return True if the operation was successful, false otherwise.
	 */
	bool BasketApplicationService::MarkUsersNeedsUpdate()
	{
		_logger->Info("BasketApplicationService::MarkUsersNeedsUpdate");
		return _basketRepository->MarkUsersNeedsUpdate();

	}

	/**
	 * @brief Retrieves the quantity of pending baskets for a given TID and basket ID.
	 * @param tid The TID to query.
	 * @param basketId The basket ID to query.
	 * @return The quantity of pending baskets as an integer.
	 */
	int BasketApplicationService::GetPendingBasketQuantityByTidAndBasket(int tid, int basketId)
	{
		_logger->Info(QString("BasketApplicationService::GetPendingBasketQuantityByTidAndBasket TID=%1 BasketId=%2")
			.arg(tid)
			.arg(basketId));
		return _basketRepository->GetPendingBasketQuantityByTidAndBasket(tid, basketId);

	}

	/**
	 * @brief Deletes all spacers associated with a given basket.
	 * @param basketId The ID of the basket whose spacers will be deleted.
	 * @return True if the operation was successful, false otherwise.
	 */
	bool BasketApplicationService::DeleteBasketSpacers(int basketId)
	{
		_logger->Info(QString("BasketApplicationService::DeleteBasketSpacers BasketId=%1").arg(basketId));
		return _basketRepository->DeleteBasketSpacers(basketId);
	}

	/**
	 * @brief Inserts a new spacer for a basket with the specified parameters.
	 * @param basketId The ID of the basket.
	 * @param layer The layer number.
	 * @param inComb The combination index.
	 * @param sp1-sp8 Spacer values for the basket.
	 * @return True if the operation was successful, false otherwise.
	 */
	bool BasketApplicationService::InsertBasketSpacer(
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
		_logger->Info(QString("BasketApplicationService::InsertBasketSpacer BasketId=%1 Layer=%2 InComb=%3 Spacers=[%4, %5, %6, %7, %8, %9, %10, %11]")
			.arg(basketId)
			.arg(layer)
			.arg(inComb)
			.arg(sp1)
			.arg(sp2)
			.arg(sp3)
			.arg(sp4)
			.arg(sp5)
			.arg(sp6)
			.arg(sp7)
			.arg(sp8));
		return _basketRepository->InsertBasketSpacer(
			basketId, layer, inComb, sp1, sp2, sp3, sp4, sp5, sp6, sp7, sp8);
	}

	/**
	 * @brief Increments the quantity in baskets for a given TID.
	 * @param tid The TID for which to increment the quantity.
	 * @param quantityToAdd The quantity to add.
	 * @return True if the operation was successful, false otherwise.
	 */
	bool BasketApplicationService::IncrementQuantityInBaskets(int tid, int quantityToAdd)
	{
		_logger->Info(QString("BasketApplicationService::IncrementQuantityInBaskets TID=%1 QuantityToAdd=%2")
			.arg(tid)
			.arg(quantityToAdd));
		return _basketRepository->IncrementQuantityInBaskets(tid, quantityToAdd);
	}
}
