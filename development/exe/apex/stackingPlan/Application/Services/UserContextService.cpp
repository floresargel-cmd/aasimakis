#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "UserContextService.h"

using namespace StackingPlan::Application::Services;

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

// 🔥 NEW

void UserContextService::SetGlobalPressNumber(int value)
{
	_globalPressNumber = value;
}

int UserContextService::GetGlobalPressNumber() const
{
	return _globalPressNumber;
}

void UserContextService::SetHavePlcComms(bool value)
{
	_havePlcComms = value;
}

bool UserContextService::GetHavePlcComms() const
{
	return _havePlcComms;
}
