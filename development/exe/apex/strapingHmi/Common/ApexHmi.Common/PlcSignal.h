#pragma once

#include <QString>

struct PlcSignal
{
	QString table;
	int index = 0;
	QString value;
};