#include "DatabaseInitializer.h"

#include "../Logging/FileLogger.h"
#include "SqlCommandExecutor.h"

namespace StackingPlan::Infrastructure::Database
{
	DatabaseInitializer::DatabaseInitializer(
		std::shared_ptr<SqlCommandExecutor> sqlCommandExecutor,
		std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger)
		: _sqlCommandExecutor(sqlCommandExecutor),
		_logger(logger)
	{
	}

	bool DatabaseInitializer::Initialize()
	{
		_logger->Info("Database initialization started.");

		if (!_sqlCommandExecutor->Open())
		{
			_logger->Error("Database open failed: " + _sqlCommandExecutor->GetLastError());
			return false;
		}

		if (!EnsureErrorLogTable())
		{
			_logger->Error("Failed to ensure ErrorLog table exists.");
			return false;
		}

		_logger->Info("Database initialization completed.");
		return true;
	}

	bool DatabaseInitializer::EnsureErrorLogTable()
	{
		const QString sql =
			"IF NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[ErrorLog]') AND type in (N'U')) "
			"BEGIN "
			"    CREATE TABLE [dbo].[ErrorLog] ("
			"        [Id] INT IDENTITY(1,1) NOT NULL PRIMARY KEY,"
			"        [Message] NVARCHAR(1000) NOT NULL,"
			"        [CreatedDate] DATETIME NOT NULL DEFAULT(GETDATE())"
			"    )"
			"END";

		return _sqlCommandExecutor->ExecuteNonQuery(sql);
	}
}