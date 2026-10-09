#pragma once

#include <QString>

namespace StackingPlan::Domain::Entities
{
    class ProductionProfile
    {
    public:
        int id = 0;
        int pressNumber = 0;
        int dieNumber = 0;
        int productionOrder = 0;
        int totalQuantity = 0;
        int quantityInBaskets = 0;
        QString soNumber;
        QString temper;
        int length = 0;
        bool onNextBasket = false;
        bool onNextLayer = false;
    };
}
