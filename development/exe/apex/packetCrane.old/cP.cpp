#include "cP.h"
cellPositionClass::cellPositionClass(int posId,int packetId_,int cassette_,int acceptableContents_,int storagingTypeOfCassette_,int isLocked_,int col_,int row_,QWidget *p):packetPositionClass(posId,1.111f+(col_-1)*.820f,posId>0?7.576f:-7.576f,.677f+(row_-1)*.740f,p)
{
	packetId=packetId_;
	cassette=cassette_;
	acceptableContents=acceptableContents_;
	storagingTypeOfCassette=storagingTypeOfCassette_;
	isLocked=isLocked_==1?true:false;
	row=row_;
	col=col_;
	//
	isCasseteOnCell=cassette!=0?true:false;
	loadContentsFromDatabase();
}
cellPositionClass::~cellPositionClass()
{
}
int cellPositionClass::getRow()
{
	return row;
}
int cellPositionClass::getColumn()
{
	return col;
}

void cellPositionClass::saveToDatabase()
{
	packetPositionClass::saveToDatabase();
}
void cellPositionClass::loadFromDatabase()
{
	packetPositionClass::loadFromDatabase();
	cassette=GETONE(QString("select cassette from positions where posId=%1").arg(posId)).toInt();
	isCasseteOnCell=cassette!=0?true:false;
	isLocked=GETONE(QString("select isLocked from positions where posId=%1").arg(posId)).toInt()==IS_LOCKED?true:false;
}
void cellPositionClass::loadContentsFromDatabase()
{
	packetPositionClass::loadContentsFromDatabase();
}
void cellPositionClass::setCassette(int cas)
{
	apexExecQuery(QString("insert into log (type,posId,newValue,oldValue,date,sec) values('define cassette',%1,%2,%3,'%4',%5)").arg(posId).arg(cas).arg(cassette).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm:ss")).arg(QDateTime::currentDateTime().toTime_t()));
	if ((cassette!=0)&&(cas!=0))
	{
		vLog("error","cellPositionClass::setCassette()",QString("Attempt to set cassette %1 on position %2. Position %2 allready has cassette %3").arg(cas).arg(posId).arg(cassette));
		EXIT("internal error",QString("Attempt to set cassette %1 on position %2. Position %2 allready has cassette %3").arg(cas).arg(posId).arg(cassette));
	}
	apexExecQuery(QString("update positions set cassette=%1 where posId=%2").arg(cas).arg(posId));
	vLog("info","cellPositionClass::setCassette()",QString("id:%1,cass:%2").arg(posId).arg(cas));
	if (cas!=0)
		packetPositionClass::setPacketId(GETONE(QString("select packetId from positions where cassette=%1").arg(cas)).toInt());
	else
		packetPositionClass::setPacketId(0);
	loadFromDatabase();
}
int cellPositionClass::getCassette()
{
	return cassette;
}
int cellPositionClass::getStoragingTypeOfCassette()
{
	return storagingTypeOfCassette;
}

/////////////////////////////////////////////////////////////////////////////////////////////
#include "uglyfont.h"
void cellPositionClass::draw()
{
	packetPositionClass::draw((isLocked?QColor(255,0,0):QColor(0,0,0)),isCasseteOnCell);
}
void cellPositionClass::mouseClicked(QColor pickedColor)
{
	if (pickedColor==pickingColor)
	{
		showDlg();
	}
}
void cellPositionClass::showDlg()
{
		cellContentsClass *tCellContents=new cellContentsClass(posId,this);
		connect(tCellContents,SIGNAL(cellDataChangedSignal(int)),this,SLOT(onCellDataChangedSlot(int)));
		tCellContents->exec();
}
bool cellPositionClass::hasCassette()
{
	if (cassette!=0)
		return true;
	return false;
}

void cellPositionClass::onCellDataChangedSlot(int cell)
{
	emit cellDataChangedSignal(cell);
}
