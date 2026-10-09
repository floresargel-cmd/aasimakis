/**
 * @file ProductionApplicationService.cpp
 * @brief Implements the ProductionApplicationService class, providing application-level operations for production profiles.
 *
 * This service acts as an intermediary between the application layer and the production repository,
 * handling business logic and logging for production-related operations.
 */

#include "ProductionApplicationService.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../Domain/Entities/ProductionProfile.h"
#include "../../Infrastructure/Logging/FileLogger.h"
#include "../Interfaces/IProductionRepository.h"
#include <memory>

namespace StackingPlan::Application::Services
{
	/**
	 * @brief Constructs a ProductionApplicationService.
	 * @param productionRepository Shared pointer to the production repository interface.
	 * @param logger Shared pointer to the file logger for logging operations.
	 */
	ProductionApplicationService::ProductionApplicationService(
		std::shared_ptr<StackingPlan::Application::Interfaces::IProductionRepository> productionRepository,
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger)
		: _productionRepository(productionRepository),
		_logger(logger)
	{
	}

	/**
	 * @brief Loads all production profiles for a given press number.
	 * @param pressNumber The press number to filter production profiles.
	 * @return QVector of ProductionProfile entities matching the press number.
	 */
	QVector<StackingPlan::Domain::Entities::ProductionProfile>
		ProductionApplicationService::LoadByPress(int pressNumber)
	{
		if (_logger)
		{
			_logger->Info(QString("Loading production for press %1").arg(pressNumber));
		}

		return _productionRepository->GetByPress(pressNumber);
	}

	/**
	 * @brief Retrieves a production profile by its unique identifier.
	 * @param id The unique identifier of the production profile.
	 * @return The ProductionProfile entity with the specified id.
	 */
	StackingPlan::Domain::Entities::ProductionProfile
		ProductionApplicationService::GetById(int id)
	{
		return _productionRepository->GetById(id);
	}

	/**
	 * @brief Changes the status of a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param status The new status value.
	 * @return True if the status was updated successfully, false otherwise.
	 */
	bool ProductionApplicationService::ChangeStatus(int id, int status)
	{
		if (_logger)
		{
			_logger->Info(QString("ChangeStatus Id=%1 Status=%2").arg(id).arg(status));
		}

		return _productionRepository->UpdateStatus(id, status);
	}

	/**
	 * @brief Updates the total and basket quantities for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param totalQuantity The new total quantity.
	 * @param quantityInBaskets The new quantity in baskets.
	 * @return True if the quantities were updated successfully, false otherwise.
	 */
	bool ProductionApplicationService::ChangeQuantities(int id, int totalQuantity, int quantityInBaskets)
	{
		if (_logger)
		{
			_logger->Info(QString("ChangeQuantities Id=%1 Total=%2 Basket=%3")
				.arg(id)
				.arg(totalQuantity)
				.arg(quantityInBaskets));
		}

		return _productionRepository->UpdateQuantities(id, totalQuantity, quantityInBaskets);
	}

	/**
	 * @brief Updates the temper value for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param temper The new temper value as a QString.
	 * @return True if the temper was updated successfully, false otherwise.
	 */
	bool ProductionApplicationService::ChangeTemper(int id, const QString& temper)
	{
		if (_logger)
		{
			_logger->Info(QString("ChangeTemper Id=%1 Temper=%2")
				.arg(id)
				.arg(temper));
		}

		return _productionRepository->UpdateTemper(id, temper);
	}

	/**
	 * @brief Updates the length value for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param length The new length value.
	 * @return True if the length was updated successfully, false otherwise.
	 */
	bool ProductionApplicationService::ChangeLength(int id, int length)
	{
		if (_logger)
		{
			_logger->Info(QString("ChangeLength Id=%1 Length=%2")
				.arg(id)
				.arg(length));
		}

		return _productionRepository->UpdateLength(id, length);
	}

	/**
	 * @brief Updates the linear density value for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param linearDensity The new linear density value.
	 * @return True if the linear density was updated successfully, false otherwise.
	 */
	bool ProductionApplicationService::ChangeLinearDensity(int id, double linearDensity)
	{
		if (_logger)
		{
			_logger->Info(QString("ChangeLinearDensity Id=%1 Value=%2")
				.arg(id)
				.arg(linearDensity));
		}

		return _productionRepository->UpdateLinearDensity(id, linearDensity);
	}

	/**
	 * @brief Updates the double length value for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param doubleLength The new double length value.
	 * @return True if the double length was updated successfully, false otherwise.
	 */
	bool ProductionApplicationService::ChangeDoubleLength(int id, int doubleLength)
	{
		if (_logger)
		{
			_logger->Info(QString("ChangeDoubleLength Id=%1 Value=%2")
				.arg(id)
				.arg(doubleLength));
		}

		return _productionRepository->UpdateDoubleLength(id, doubleLength);
	}

	/**
	 * @brief Updates the "on next basket" value for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param onNextBasket The new value for "on next basket".
	 * @return True if the value was updated successfully, false otherwise.
	 */
	bool ProductionApplicationService::ChangeOnNextBasket(int id, int onNextBasket)
	{
		if (_logger)
		{
			_logger->Info(QString("ChangeOnNextBasket Id=%1 Value=%2")
				.arg(id)
				.arg(onNextBasket));
		}

		return _productionRepository->UpdateOnNextBasket(id, onNextBasket);
	}

	/**
	 * @brief Updates the "on next layer" value for a production profile.
	 * @param id The unique identifier of the production profile.
	 * @param onNextLayer The new value for "on next layer".
	 * @return True if the value was updated successfully, false otherwise.
	 */
	bool ProductionApplicationService::ChangeOnNextLayer(int id, int onNextLayer)
	{
		if (_logger)
		{
			_logger->Info(QString("ChangeOnNextLayer Id=%1 Value=%2")
				.arg(id)
				.arg(onNextLayer));
		}

		return _productionRepository->UpdateOnNextLayer(id, onNextLayer);
	}
}
