#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "ProductionSqlScripts.h"

namespace StackingPlan::Infrastructure::SqlScripts
{
	QString ProductionSqlScripts::GetByPress()
	{
		return
			"SELECT "
			"    tid, "
			"    pressNum, "
			"    die, "
			"    productionOrder, "
			"    totalQuantity, "
			"    quantityInBaskets, "
			"    soNum, "
			"    temper, "
			"    length, "
			"    onNextBasket, "
			"    onNextLayer "
			"FROM epicsProduction2 "
			"WHERE pressNum = :pressNum "
			"ORDER BY productionOrder";
	}

	QString ProductionSqlScripts::GetById()
	{
		return
			"SELECT "
			"    tid, "
			"    pressNum, "
			"    die, "
			"    productionOrder, "
			"    totalQuantity, "
			"    quantityInBaskets, "
			"    soNum, "
			"    temper, "
			"    length, "
			"    onNextBasket, "
			"    onNextLayer "
			"FROM epicsProduction2 "
			"WHERE tid = :id";
	}

	QString ProductionSqlScripts::UpdateStatus()
	{
		return "UPDATE epicsProduction2 SET status = :status WHERE tid = :id";
	}

	QString ProductionSqlScripts::UpdateQuantities()
	{
		return
			"UPDATE epicsProduction2 "
			"SET totalQuantity = :totalQuantity, "
			"    quantityInBaskets = :quantityInBaskets "
			"WHERE tid = :id";
	}

	QString ProductionSqlScripts::UpdateTemper()
	{
		return "UPDATE epicsProduction2 SET temper = :temper WHERE tid = :id";
	}

	QString ProductionSqlScripts::UpdateLength()
	{
		return "UPDATE epicsProduction2 SET length = :length WHERE tid = :id";
	}

	QString ProductionSqlScripts::UpdateLinearDensity()
	{
		return "UPDATE epicsProduction2 SET linearDencity = :linearDensity WHERE tid = :id";
	}

	QString ProductionSqlScripts::UpdateDoubleLength()
	{
		return "UPDATE epicsProduction2 SET doubleLength = :doubleLength WHERE tid = :id";
	}

	QString ProductionSqlScripts::UpdateOnNextBasket()
	{
		return "UPDATE epicsProduction2 SET onNextBasket = :onNextBasket WHERE tid = :id";
	}

	QString ProductionSqlScripts::UpdateOnNextLayer()
	{
		return "UPDATE epicsProduction2 SET onNextLayer = :onNextLayer WHERE tid = :id";
	}
}

/**
 * @file ProductionSqlScripts.cpp
 * @brief Implements SQL query generators for the epicsProduction2 table.
 *
 * This file defines the methods of the ProductionSqlScripts class, which provide
 * parameterized SQL statements for querying and updating production records in the
 * epicsProduction2 database table. Each method returns a QString containing a SQL
 * statement with named parameters for use with Qt's database APIs.
 *
 * Namespace: StackingPlan::Infrastructure::SqlScripts
 *
 * Methods:
 *  - GetByPress(): Returns a SELECT statement to retrieve all production records for a given press number, ordered by production order.
 *  - GetById(): Returns a SELECT statement to retrieve a production record by its unique ID.
 *  - UpdateStatus(): Returns an UPDATE statement to set the status of a production record by ID.
 *  - UpdateQuantities(): Returns an UPDATE statement to set the total quantity and quantity in baskets for a production record by ID.
 *  - UpdateTemper(): Returns an UPDATE statement to set the temper value for a production record by ID.
 *  - UpdateLength(): Returns an UPDATE statement to set the length for a production record by ID.
 *  - UpdateLinearDensity(): Returns an UPDATE statement to set the linear density for a production record by ID.
 *  - UpdateDoubleLength(): Returns an UPDATE statement to set the double length for a production record by ID.
 *  - UpdateOnNextBasket(): Returns an UPDATE statement to set the onNextBasket flag for a production record by ID.
 *  - UpdateOnNextLayer(): Returns an UPDATE statement to set the onNextLayer flag for a production record by ID.
 *
 * Usage:
 *  These methods are intended to be used wherever parameterized SQL queries are required
 *  for interacting with the epicsProduction2 table, ensuring consistency and maintainability
 *  of SQL code throughout the application.
 */
