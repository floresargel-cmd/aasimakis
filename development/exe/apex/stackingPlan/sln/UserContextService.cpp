#include "UserContextService.h"

namespace StackingPlan::Application::Services
{
	void UserContextService::SetUser(const QString& userName, int privilegeType)
	{
		_userName = userName;
		_privilegeType = privilegeType;
	}

	QString UserContextService::GetUserName() const
	{
		return _userName;
	}

	int UserContextService::GetPrivilegeType() const
	{
		return _privilegeType;
	}
}