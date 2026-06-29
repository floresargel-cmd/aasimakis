#pragma once
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"

namespace StackingPlan::Application::Services
{
	class UserContextService
	{
	public:
		// 🔹 User identity
		void SetUser(const QString& userName, int privilegeType);
		QString GetUserName() const;
		int GetPrivilegeType() const;

		// 🔹 Runtime state (NEW)
		void SetGlobalPressNumber(int value);
		int GetGlobalPressNumber() const;

		void SetHavePlcComms(bool value);
		bool GetHavePlcComms() const;

	private:
		QString _userName;
		int _privilegeType = 0;

		// 🔥 NEW STATE (replaces globals)
		int _globalPressNumber = 1;
		bool _havePlcComms = false;
	};
}