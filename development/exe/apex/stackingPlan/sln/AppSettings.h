#pragma once

#include <QString>

namespace StackingPlan::Application::Services
{
	class AppSettings
	{
	public:
		QString connectionString;
		int maxRetries = 3;
		int baseDelayMilliseconds = 200;
		QString logDirectory = "logs";
	};
}
