#include "pch.h"
#include "Gd.h"

specialDiesClass *specialDiesG;
specialDiesClass::specialDiesClass()
{
	specialDiesG=this;
	specialCassettes1<<1<<2<<3<<4<<5<<6<<7<<8<<9;
	specialCassettes2<<27<<28<<29<<30<<31<<32;
	specialDies1<<"1705"<<"1062"<<"4180"<<"1701"<<"1003"<<"1642";
	specialDies2<<"3573"<<"1034"<<"3590"<<"4181"<<"4854"<<"1015"<<"3826"<<"1019"<<"4710";
}
specialDiesClass::~specialDiesClass()
{
}
bool specialDiesClass::isCombinationValid(int cassette,QString die)
{
	if (die.split('-').count()!=2)
		uExit(errorStr,QString("Non Valid Die Detected [%1]. Die's Format Length Should Be 6 Or 7 Characters, Including \"-\"").arg(die));
	die=die.split('-')[0];
	//type1
	if(specialCassettes1.contains(cassette)&&specialDies1.contains(die))
	{
		currentErrorString.clear();
		return true;
	}
	if(specialCassettes1.contains(cassette)||specialDies1.contains(die))
	{
		if(specialCassettes1.contains(cassette))
			currentErrorString=QString("Die will be deleted\nThis Cassette Accepts Special Types Of Die Only\n%1\n").arg(specialDies1.join(','));
		else
		{
			currentErrorString="Die will be deleted\nThis Type Of Die Can Be Placed On Special Cassettes Only\n";
			for (int i=0;i<specialCassettes1.count();i++)
				currentErrorString.append(QString("%1,").arg(specialCassettes1[i]));
		}
		return false;
	}
	//type2
	if(specialCassettes2.contains(cassette)&&specialDies2.contains(die))
	{
		currentErrorString.clear();
		return true;
	}
	if(specialCassettes2.contains(cassette)||specialDies2.contains(die))
	{
		if(specialCassettes2.contains(cassette))
			currentErrorString=QString("Die will be deleted\nThis Cassette Accepts Special Types Of Die Only\n%1\n").arg(specialDies2.join(','));
		else
		{
			currentErrorString="Die will be deleted\nThis Type Of Die Can Be Placed On Special Cassettes Only\n";
			for (int i=0;i<specialCassettes2.count();i++)
				currentErrorString.append(QString("%1,").arg(specialCassettes2[i]));
		}
		return false;
	}
	currentErrorString.clear();
	return true;
}
QString specialDiesClass::getErrorStr()
{
	return currentErrorString;
}
