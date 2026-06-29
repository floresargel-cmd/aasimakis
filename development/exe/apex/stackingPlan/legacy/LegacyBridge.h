#pragma once

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"

namespace LegacyBridge
{
	void LogInfo(const QString& message);
	void LogWarning(const QString& message);
	void LogError(const QString& message);
	void NotifyOthers();
}