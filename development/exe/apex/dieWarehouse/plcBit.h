#ifndef PLCBIT_H
#define PLCBIT_H
#include "Gd.h"
class plcBitClass
{
public:
	QString									ip;
	QString									table;
	int											index;
	int											bit;
public:
	plcBitClass()
	{
		ip="";table="";index=-1;bit=-1;
	}
	plcBitClass(QString ip_,QString table_,int index_,int bit_)
	{
		ip=ip_;table=table_;index=index_;bit=bit_;
	}
	~plcBitClass()
	{
	}
};
#endif
