#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../Domain/Entities/ProductionProfile.h"


#include <memory>

namespace StackingPlan::Domain::Entities
{
	struct ProductionProfile;
}

namespace StackingPlan::Application::Interfaces
{
	class IProductionRepository;
}

namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger;
}

namespace StackingPlan::Application::Services
{
	class ProductionApplicationService
	{
	public:
		ProductionApplicationService(
			std::shared_ptr<StackingPlan::Application::Interfaces::IProductionRepository> productionRepository,
			std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger);

		// READ
		QVector<StackingPlan::Domain::Entities::ProductionProfile> LoadByPress(int pressNumber);
		StackingPlan::Domain::Entities::ProductionProfile GetById(int id);

		// UPDATE
		bool ChangeStatus(int id, int status);
		bool ChangeQuantities(int id, int totalQuantity, int quantityInBaskets);
		bool ChangeTemper(int id, const QString& temper);
		bool ChangeLength(int id, int length);
		bool ChangeLinearDensity(int id, double linearDensity);
		bool ChangeDoubleLength(int id, int doubleLength);
		bool ChangeOnNextBasket(int id, int onNextBasket);
		bool ChangeOnNextLayer(int id, int onNextLayer);

	private:
		std::shared_ptr<StackingPlan::Application::Interfaces::IProductionRepository> _productionRepository;
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger;
	};
}