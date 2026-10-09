#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"


namespace StackingPlan::Domain::Entities
{
	struct ProductionProfile;
}

namespace StackingPlan::Application::Interfaces
{
	class IProductionRepository
	{
	public:
		virtual ~IProductionRepository() = default;

		virtual QVector<StackingPlan::Domain::Entities::ProductionProfile> GetByPress(int pressNumber) = 0;
		virtual StackingPlan::Domain::Entities::ProductionProfile GetById(int id) = 0;

		virtual bool UpdateStatus(int id, int status) = 0;
		virtual bool UpdateQuantities(int id, int totalQuantity, int quantityInBaskets) = 0;
		virtual bool UpdateTemper(int id, const QString& temper) = 0;
		virtual bool UpdateLength(int id, int length) = 0;
		virtual bool UpdateLinearDensity(int id, double linearDensity) = 0;
		virtual bool UpdateDoubleLength(int id, int doubleLength) = 0;
		virtual bool UpdateOnNextBasket(int id, int onNextBasket) = 0;
		virtual bool UpdateOnNextLayer(int id, int onNextLayer) = 0;
	};
}
