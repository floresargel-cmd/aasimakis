#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"



namespace StackingPlan::Infrastructure::SqlScripts
{
	/**
	 * @class ProductionSqlScripts
	 * @brief Provides static methods to retrieve SQL query strings for production-related database operations.
	 *
	 * This utility class contains a set of static methods, each returning a QString containing
	 * a specific SQL statement used for querying or updating production data in the database.
	 * The methods are organized to support common production operations such as retrieving records
	 * by press or ID, and updating various production attributes.
	 */
	class ProductionSqlScripts
	{
	public:
		/**
		 * @brief Returns the SQL query string to retrieve production records by press.
		 * @return QString containing the SQL statement.
		 */
		static QString GetByPress();

		/**
		 * @brief Returns the SQL query string to retrieve a production record by its unique identifier.
		 * @return QString containing the SQL statement.
		 */
		static QString GetById();

		/**
		 * @brief Returns the SQL query string to update the status of a production record.
		 * @return QString containing the SQL statement.
		 */
		static QString UpdateStatus();

		/**
		 * @brief Returns the SQL query string to update the quantities of a production record.
		 * @return QString containing the SQL statement.
		 */
		static QString UpdateQuantities();

		/**
		 * @brief Returns the SQL query string to update the temper attribute of a production record.
		 * @return QString containing the SQL statement.
		 */
		static QString UpdateTemper();

		/**
		 * @brief Returns the SQL query string to update the length attribute of a production record.
		 * @return QString containing the SQL statement.
		 */
		static QString UpdateLength();

		/**
		 * @brief Returns the SQL query string to update the linear density of a production record.
		 * @return QString containing the SQL statement.
		 */
		static QString UpdateLinearDensity();

		/**
		 * @brief Returns the SQL query string to update the double length attribute of a production record.
		 * @return QString containing the SQL statement.
		 */
		static QString UpdateDoubleLength();

		/**
		 * @brief Returns the SQL query string to update the "on next basket" status of a production record.
		 * @return QString containing the SQL statement.
		 */
		static QString UpdateOnNextBasket();

		/**
		 * @brief Returns the SQL query string to update the "on next layer" status of a production record.
		 * @return QString containing the SQL statement.
		 */
		static QString UpdateOnNextLayer();
	};
}