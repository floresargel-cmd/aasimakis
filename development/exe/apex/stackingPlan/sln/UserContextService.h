#pragma once

#include <QString>

namespace StackingPlan::Application::Services
{
	class UserContextService
	{
	public:
		void SetUser(const QString& userName, int privilegeType);
		QString GetUserName() const;
		int GetPrivilegeType() const;

	private:
		QString _userName;
		int _privilegeType = 0;
	};
}