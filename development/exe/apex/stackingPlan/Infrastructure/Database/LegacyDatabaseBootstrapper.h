#pragma once

#include "../../Application/Services/LegacyRuntimeContext.h"

#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include <memory>

class QSqlDatabase;

namespace StackingPlan
{
	namespace Infrastructure
	{
		namespace Logging
		{
			class FileLogger;
		}

		namespace Database
		{
			class LegacyDatabaseBootstrapper
			{
			public:
				explicit LegacyDatabaseBootstrapper(
					std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> logger);

				bool Initialize(
					StackingPlan::Application::Services::LegacyRuntimeContext& runtimeContext);

			private:
				bool OpenMainDatabase(
					const StackingPlan::Application::Services::LegacyRuntimeContext& runtimeContext);
				bool OpenBasketCraneDatabase(
					const StackingPlan::Application::Services::LegacyRuntimeContext& runtimeContext);

				bool EnsureTables();
				bool EnsureSeedData();
				bool LoadRuntimeContext(
					StackingPlan::Application::Services::LegacyRuntimeContext& runtimeContext);

				bool ExecuteOnMain(const QString& sql);
				bool ExecuteOnBasketCrane(const QString& sql);
				QString ExecuteScalarOnMain(const QString& sql);

			private:
				std::shared_ptr<StackingPlan::Infrastructure::Logging::FileLogger> _logger;
			};
		}
	}
}