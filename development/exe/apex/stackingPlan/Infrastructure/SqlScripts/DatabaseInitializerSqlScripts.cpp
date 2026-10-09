#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "DatabaseInitializerSqlScripts.h"

// DatabaseInitializerSqlScripts.cpp
//
// This file defines SQL script generation methods for initializing essential database tables
// used by the StackingPlan application. The scripts are returned as QString objects and are
// intended to be executed against a SQL Server database to ensure the existence of required tables.
//
// Namespace: StackingPlan::Infrastructure::SqlScripts
//
// Methods:
//  - EnsureErrorLogTable()
//      Returns a SQL script that checks for the existence of the [dbo].[ErrorLog] table.
//      If the table does not exist, it creates the table with the following columns:
//        - [Id]: INT, identity, primary key
//        - [Message]: NVARCHAR(1000), not null
//        - [CreatedDate]: DATETIME, not null, defaults to current date/time
//
//  - EnsureSyncConfigTable()
//      Returns a SQL script that checks for the existence of the [dbo].[sync_config] table.
//      If the table does not exist, it creates the table with the following columns:
//        - [sync_key]: NVARCHAR(200), primary key
//        - [last_run]: DATETIME, nullable
//
// Usage:
//  These methods are typically called during application startup or database initialization
//  routines to ensure that logging and synchronization configuration tables are present//
namespace StackingPlan::Infrastructure::SqlScripts
{
	QString DatabaseInitializerSqlScripts::EnsureErrorLogTable()
	{
		return
			"IF NOT EXISTS ("
			"    SELECT * FROM sys.objects "
			"    WHERE object_id = OBJECT_ID(N'[dbo].[ErrorLog]') AND type IN (N'U')"
			") "
			"BEGIN "
			"    CREATE TABLE [dbo].[ErrorLog] ("
			"        [Id] INT IDENTITY(1,1) NOT NULL PRIMARY KEY,"
			"        [Message] NVARCHAR(1000) NOT NULL,"
			"        [CreatedDate] DATETIME NOT NULL DEFAULT(GETDATE())"
			"    )"
			"END";
	}

	QString DatabaseInitializerSqlScripts::EnsureSyncConfigTable()
	{
		return
			"IF NOT EXISTS ("
			"    SELECT * FROM sys.objects "
			"    WHERE object_id = OBJECT_ID(N'[dbo].[sync_config]') AND type IN (N'U')"
			") "
			"BEGIN "
			"    CREATE TABLE [dbo].[sync_config] ("
			"        [sync_key] NVARCHAR(200) NOT NULL PRIMARY KEY,"
			"        [last_run] DATETIME NULL"
			"    )"
			"END";
	}
}
