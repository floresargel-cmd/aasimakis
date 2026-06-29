#include "LegacyDatabaseTableDefinitions.h"

namespace StackingPlan
{
	namespace Infrastructure
	{
		namespace SqlScripts
		{
			std::vector<LegacyDatabaseTableDefinition>
				LegacyDatabaseTableDefinitions::GetMainTableDefinitions()
			{
				std::vector<LegacyDatabaseTableDefinition> tables;

				{
					LegacyDatabaseTableDefinition table;
					table.tableName = "epicsProduction2";
					table.columnNames
						<< "tid" << "pressNum" << "die" << "productionOrder" << "status" << "totalQuantity"
						<< "quantityInBaskets" << "temper" << "length" << "linearDencity" << "profileSpacing"
						<< "doubleLength" << "inComb" << "onNextBasket" << "onNextLayer" << "scheduleDate"
						<< "scheduleShift" << "soNum" << "isVirtual" << "billetsNo" << "billetLength"
						<< "billetButtLength" << "scrapFrontLength" << "scrapRearLength" << "runOutLength"
						<< "dieMultiplicity" << "baskets";
					table.columnTypes
						<< "int PRIMARY KEY IDENTITY" << "int" << "int" << "int" << "int" << "int"
						<< "int" << "char(16)" << "int" << "real" << "real" << "int" << "int" << "int"
						<< "int" << "varchar(12)" << "char(1)" << "varchar(20)" << "int" << "int" << "int"
						<< "int" << "int" << "int" << "int" << "int" << "int";
					tables.push_back(table);
				}

				{
					LegacyDatabaseTableDefinition table;
					table.tableName = "basketSpacers";
					table.columnNames
						<< "tid" << "basket" << "layer" << "inComb" << "sp1" << "sp2" << "sp3" << "sp4"
						<< "sp5" << "sp6" << "sp7" << "sp8";
					table.columnTypes
						<< "int PRIMARY KEY IDENTITY" << "int" << "int" << "int" << "int" << "int" << "int"
						<< "int" << "int" << "int" << "int" << "int";
					tables.push_back(table);
				}

				{
					LegacyDatabaseTableDefinition table;
					table.tableName = "profileCode";
					table.columnNames
						<< "tid" << "die" << "interProfileSpace" << "angle" << "multiplicity" << "inComb"
						<< "sp1" << "sp2" << "sp3" << "sp4" << "sp5" << "sp6" << "sp7" << "sp8";
					table.columnTypes
						<< "int PRIMARY KEY IDENTITY" << "int" << "int" << "float" << "int" << "int"
						<< "int" << "int" << "int" << "int" << "int" << "int" << "int" << "int";
					tables.push_back(table);
				}

				{
					LegacyDatabaseTableDefinition table;
					table.tableName = "users";
					table.columnNames
						<< "tid" << "login" << "password" << "office" << "press" << "stacker"
						<< "needsUpdate" << "maxNoOfProfilesToLoad";
					table.columnTypes
						<< "int PRIMARY KEY IDENTITY" << "char(30)" << "char(30)" << "int" << "int"
						<< "int" << "int" << "int";
					tables.push_back(table);
				}

				{
					LegacyDatabaseTableDefinition table;
					table.tableName = "settings";
					table.columnNames << "tid" << "name" << "value";
					table.columnTypes << "int PRIMARY KEY IDENTITY" << "char(30)" << "char(30)";
					tables.push_back(table);
				}

				{
					LegacyDatabaseTableDefinition table;
					table.tableName = "userLog";
					table.columnNames << "tid" << "userName" << "description" << "date" << "sec";
					table.columnTypes << "int PRIMARY KEY IDENTITY" << "char(15)" << "char(100)" << "char(20)" << "int";
					tables.push_back(table);
				}

				{
					LegacyDatabaseTableDefinition table;
					table.tableName = "errorLog";
					table.columnNames << "rid" << "tid" << "die" << "emessage" << "ctime";
					table.columnTypes << "int PRIMARY KEY IDENTITY(1,1)" << "int" << "int" << "char(250)" << "char(25)";
					tables.push_back(table);
				}

				return tables;
			}
		}
	}
}