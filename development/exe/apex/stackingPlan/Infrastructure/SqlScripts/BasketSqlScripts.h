#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"


// BasketSqlScripts.h
// ---------------------------------------------
// Provides static methods for generating SQL query strings
// related to basket operations in the StackingPlan application.
//
// This class centralizes the construction of SQL statements used
// for querying and manipulating basket-related data in the database.
// All methods return a QString containing the SQL command.
//
// Namespace: StackingPlan::Infrastructure::SqlScripts
//
// Usage:
//   QString sql = BasketSqlScripts::GetSoNumberByTid();
//
// Methods:
//   - GetSoNumberByTid: Returns SQL to retrieve the SO number by TID.
//   - GetDieByTid: Returns SQL to retrieve die information by TID.
//   - ReorderProductionSequence: Returns SQL to reorder the production sequence.
//   - GetBasketTableIdByPositionId: Returns SQL to get basket table ID by position ID.
//   - MarkUsersNeedsUpdate: Returns SQL to mark users as needing an update.
//   - GetPendingBasketQuantityByTidAndBasket: Returns SQL to get pending basket quantity by TID and basket.
//   - DeleteBasketSpacers: Returns SQL to delete basket spacers.
//   - InsertBasketSpacer: Returns SQL to insert a basket spacer.
//   - IncrementQuantityInBaskets: Returns SQL to increment the quantity in baskets.

namespace StackingPlan::Infrastructure::SqlScripts
{
	// Static class for basket-related SQL script generation.
	class BasketSqlScripts
	{
	public:
		// Returns SQL to get SO number by TID.
		static QString GetSoNumberByTid();

		// Returns SQL to get die information by TID.
		static QString GetDieByTid();

		// Returns SQL to reorder the production sequence.
		static QString ReorderProductionSequence();

		// Returns SQL to get basket table ID by position ID.
		static QString GetBasketTableIdByPositionId();

		// Returns SQL to mark users as needing an update.
		static QString MarkUsersNeedsUpdate();

		// Returns SQL to get pending basket quantity by TID and basket.
		static QString GetPendingBasketQuantityByTidAndBasket();

		// Returns SQL to delete basket spacers.
		static QString DeleteBasketSpacers();

		// Returns SQL to insert a basket spacer.
		static QString InsertBasketSpacer();

		// Returns SQL to increment quantity in baskets.
		static QString IncrementQuantityInBaskets();
	};
}