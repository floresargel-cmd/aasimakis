#ifndef GB_H
#define GB_H
#include"..\G.h"

extern QString dieIp;
extern bool adminMode;
#define exportsTable QString("exports")
#define cellTypeNormal 1
#define cellTypeBig 2

#define logMissionsTimes
#define db "mc"
//#define epicsDb "epics"

#define cranePlcId 10000
#define exitPlcId 10010

#define activeStr QString("active")
#define nextStr   QString("next")

#define missionStatusIdle

#define basketAvailableWidth 2800.
#define basketAvailableWeight 1000.

//
#define STYLE_R					 QString("QLabel{font-size:14px;background-color:#ff2a2a;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define STYLE_R_BLINKING QString("QLabel{font-size:14px;background-color:#f0f0f0;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:#ff0000;}")
#define STYLE_O					 QString("QLabel{font-size:14px;background-color:#f27a40;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define STYLE_G					 QString("QLabel{font-size:14px;background-color:#09c500;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:blue;}")
#define STYLE_B					 QString("QLabel{font-size:14px;background-color:#99ccff;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:white;}")
#define STYLE_GRAY			 QString("QLabel{font-size:14px;background-color:#999999;border-radius:7px;border-color:beige;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:#000077;}")

#define STYLE_G_SMALL    QString("QLabel{font-size:10px;background-color:#09c500;border-radius:5px;border-color:beige;padding-left:20px;padding-right:20px;padding-top:5px;padding-bottom:5px;color:blue;}")
#define STYLE_B_SMALL    QString("QLabel{font-size:10px;background-color:#99ccff;border-radius:5px;border-color:beige;padding-left:20px;padding-right:20px;padding-top:5px;padding-bottom:5px;color:white;}")
#define STYLE_GRAY_SMALL QString("QLabel{font-size:10px;background-color:#999999;border-radius:5px;border-color:beige;padding-left:20px;padding-right:20px;padding-top:5px;padding-bottom:5px;color:#000077;}")

#define BUTTON_STYLE QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #8888aa,stop:0.4#DDDDDD,stop:0.5#D8D8D8,stop:1.0#88aa88);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #aa8888,stop:0.4 #DDDDDD,stop:0.5 #D8D8D8,stop:1.0 #8888aa);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define BUTTON_STYLE_ACTIVE QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define BUTTON_STYLE_RED QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #aa5555,stop:0.4#ff5555,stop:0.5#ff2222,stop:1.0#aa5555);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #aa5555,stop:0.4#ff5555,stop:0.5#ff2222,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define BUTTON_STYLE_GREEN QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #55aa55,stop:0.4#55ff55,stop:0.5#22ff22,stop:1.0#55aa55);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")
#define BUTTON_STYLE_O QString("QPushButton{padding:6px;border-radius:4px; border: 1px solid #888888;font-size:12px;color:#111111;background: qlineargradient(x1:0.1,y1:0.1,x2:1,y2:1,stop:0 #f2aa40,stop:0.4#f27740,stop:0.5#f27740,stop:1.0#f2aa40);}QPushButton:hover{color:#000099;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #f27740,stop:0.4#f2aa40,stop:0.5#f2aa40,stop:1.0#f27740);}QPushButton:pressed{color:#aaaaaa;background:qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #555555,stop:0.4 #777777,stop:0.5 #999999,stop:1.0 #444444);}")

#define groupBoxStyle1 "QGroupBox{border:2px solid #000099;border-radius:5px;margin-top:10px;font-size: 15px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-20px;color:#000099;}"
#define groupBoxStyle2 "QGroupBox{border:2px solid #007700;border-radius:5px;margin-top:10px;font-size: 12px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-20px;color:#007700;}"
#define groupBoxStyle3 "QGroupBox{border:1px solid #227722;border-radius:5px;margin-top:10px;font-size: 11px;font-weight:normal;}QGroupBox::title{subcontrol-position: top center;margin-top:-20px;color:#227722;}"

#define groupBoxUpperStyle1 "QGroupBox{border:2px solid #000099;background-color:#b8c3e6;border-radius:9px;margin-top:15px;margin-left:20px;margin-right:20px;font-size: 15px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-30px;color:#000099;}"
#define groupBoxUpperStyle2 "QGroupBox{border:2px solid #f0f0f0;background-color:#dee4f8;border-radius:5px;margin-top:10px;margin-left: 5px;margin-right: 5px;font-size: 12px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-22px;color:#000077;}"
#define groupBoxUpperStyle3 "QGroupBox{border:1px solid #227722;background-color:#dee4f8;border-radius:5px;margin-top:10px;margin-left: 5px;margin-right: 5px;font-size: 11px;font-weight:normal;}QGroupBox::title{subcontrol-position: top center;margin-top:-20px;color:#227722;}"
#define groupBoxLowerStyle1 "QGroupBox{border:2px solid #009900;background-color:#9bc59e;border-radius:9px;margin-top:15px;margin-left:20px;margin-right:20px;font-size: 15px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-30px;color:#009900;}"
#define groupBoxLowerStyle2 "QGroupBox{border:2px solid #f0f0f0;background-color:#bfe5c3;border-radius:5px;margin-top:10px;margin-left: 5px;margin-right: 5px;font-size: 12px;font-weight:bold;  }QGroupBox::title{subcontrol-position: top center;margin-top:-22px;color:#007700;}"
#define groupBoxLowerStyle3 "QGroupBox{border:1px solid #227722;background-color:#bfe5c3;border-radius:5px;margin-top:10px;margin-left: 5px;margin-right: 5px;font-size: 11px;font-weight:normal;}QGroupBox::title{subcontrol-position: top center;margin-top:-20px;color:#227722;}"


#define labelStyleNormalBlack	QString("QLabel{font-size:15px;padding:2px;color:#000000;}")
#define labelStyleBigBlack	QString("QLabel{font-size:20px;padding:1px;color:#000000;}")
#define labelStyleBigRed		QString("QLabel{font-size:20px;padding:1px;color:#aa0000;}")

#define spinStyleBigBlack	QString("QSpinBox{border-width:3px;height:35px;} QSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QSpinBox{font:20px;color:#000000;}")
#define spinStyleBigRed		QString("QSpinBox{border-width:3px;height:35px;} QSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QSpinBox{font:20px;color:#aa0000;}")
#define spinStyleBigGreen	QString("QSpinBox{border-width:3px;height:35px;} QSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QSpinBox{font:20px;color:#00aa00;}")
#define doubleSpinStyleBigBlack	QString("QDoubleSpinBox{border-width:3px;height:35px;}QDoubleSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QDoubleSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QDoubleSpinBox{font:20px;color:#000000;}")
#define doubleSpinStyleBigRed		QString("QDoubleSpinBox{border-width:3px;height:35px;}QDoubleSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QDoubleSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QDoubleSpinBox{font:20px;color:#aa0000;}")
#define doubleSpinStyleBigGreen	QString("QDoubleSpinBox{border-width:3px;height:35px;}QDoubleSpinBox::up-arrow{width:15px;height:15px;image:url(:/spinUpArrow.png);}QDoubleSpinBox::down-arrow{width:15px;height:15px;image:url(:/spinDownArrow.png);}QDoubleSpinBox{font:20px;color:#00aa00;}")

#define splitterHorizontalStyle	QString("QSplitter::handle:horizontal{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:horizontal:hover{height:3px;background-color:#339966;}")
#define splitterVerticalStyle	QString("QSplitter::handle:vertical{height:3px;background-color:#777777;} QSplitterHandle:hover{} QSplitter::handle:vertical:hover{height:3px;background-color:#339966;}")

static float gYPlussOffset=3202.5f;
static float gYMinusOffset=-9383.f;
static float gXSideOffset=30000.f;
///

static QVector<QVariant> getAllIdleBaskets()
{
	QString q=
					QString("select basket from pos where plcId<%1 and basket<>0").arg(cranePlcId)+
					QString("and basket not in (select basket from %1)") .arg(exportsTable)+
					QString("and basket not in (select basket from pos where plcId in (select fromPos from missions))")+
					QString("and basket not in (select basket from pos where plcId in (select toPos from missions))")+
					QString("order by row,col");
	return execQuery(q,db);
}
static QVector<QVariant> getIdleFullBaskets()
{
	QString q=
					QString("select basket from pos where plcId<%1 and basket<>0").arg(cranePlcId)+
					QString("and basket not in (select basket from %1)") .arg(exportsTable)+
					QString("and basket not in (select basket from pos where plcId in (select fromPos from missions))")+
					QString("and basket not in (select basket from pos where plcId in (select toPos from missions))")+
					QString("and basket in (select distinct basket from basketsContents)")+
					QString("order by row,col");
	return execQuery(q,db);
}
static QVector<QVariant> getIdleEmptyBaskets()
{
	QString q=
					QString("select basket from pos where plcId<%1 and basket<>0").arg(cranePlcId)+
					QString("and basket not in (select basket from %1)") .arg(exportsTable)+
					QString("and basket not in (select basket from pos where plcId in (select fromPos from missions))")+
					QString("and basket not in (select basket from pos where plcId in (select toPos from missions))")+
					QString("and basket not in (select distinct basket from basketsContents)")+
					QString("order by row,col");
	return execQuery(q,db);
}
static QVector<QVariant> getIdleBasketsWithContent(QString content)
{
	QString q=
					QString("select basket from basketsContents where contentName='%1' and basket in (").arg(content)+
					QString("select basket from pos where plcId<%1 and basket<>0").arg(cranePlcId)+
					QString("and basket not in (select basket from %1)") .arg(exportsTable)+
					QString("and basket not in (select basket from pos where plcId in (select fromPos from missions))")+
					QString("and basket not in (select basket from pos where plcId in (select toPos from missions)))");
	return execQuery(q,db);
}
/////////////////////////////////////////////////////////////////////log//////////////////////////////////////////////////////////////////////
static int maxMessageLength=75;
static int maxDescriptionLength=300;
#define doVerboseLog
static void logV(QString description,QString type=infoStr)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
#ifdef doVerboseLog
	execQuery(QString("insert into logV (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),db);
#endif
}
static void log(QString description,QString type=infoStr)
{
#ifdef _DEBUG
	if (type.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into log (type,description,time) values('%1','%2','%3')").arg(type.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(timeForLog),db);
	logV(description,type);
}
static void logGui(QString mess,QString description,QString from,QString type=infoStr)
{
#ifdef _DEBUG
	if (mess.length()>maxMessageLength)
		QMessageBox::information(0,"Info","log text truncated");
	if (description.length()>maxDescriptionLength)
		QMessageBox::information(0,"Info","log text truncated");
#endif
	execQuery(QString("insert into logGui (mess,details,messenger,type,time) values('%1','%2','%3','%4','%5')").arg(mess.left(maxMessageLength)).arg(description.left(maxDescriptionLength)).arg(from.left(10)).arg(type.left(10)).arg(timeForLog),db);
	logV(QString ("%1(%2) sender:%3").arg(mess).arg(description).arg(from),type);
}
/////////////////////////////////////////////////////////////////////log//////////////////////////////////////////////////////////////////////
static int staticMoveBasket(int from,int to)
{
	if ((from==0)||(to==0))
	{
		log(QString("moveBasket from==%1 to==%2").arg(from).arg(to),errorStr);
		uExit(errorStr,QString("moveBasket from==%1 to==%2").arg(from).arg(to));
	}
	int fromBasket=getOne(QString("select basket from pos where plcId=%1").arg(from),db).toInt();
	int toBasket=getOne(QString("select basket from pos where plcId=%1").arg(to),db).toInt();
	if ((fromBasket==0)||(toBasket!=0))
	{
		log(QString("moveBasket from==%1 to==%2, fromBasket:%3 toBasket:%4").arg(from).arg(to).arg(fromBasket).arg(toBasket),errorStr);
		uExit(errorStr,QString("moveBasket from==%1 to==%2, fromBasket:%3 toBasket:%4").arg(from).arg(to).arg(fromBasket).arg(toBasket));
	}
	logV(QString("moveBasket b=%1 f=%2 t=%3 ").arg(fromBasket).arg(from).arg(to));
	execQuery(QString("update pos set basket=%1 where plcId=%2").arg(fromBasket).arg(to),db);
	execQuery(QString("update pos set basket=0 where plcId=%1").arg(from),db);
	execQuery(QString("insert into logPos (pos,basket,info,time) values(%1,%2,'from %3','%4')").arg(to).arg(fromBasket).arg(from).arg(timeForLog),db);
	return fromBasket;
}
static bool isBasketActive(int b)
{
	int pos=getFirst(QString("select plcId from pos where basket=%1").arg(b),db).toInt();
	if (pos==cranePlcId)
		return true;
	QString activConv=getFirst(QString("select status from missions where fromPos=%1 or toPos=%1 or fromPos=%1 or toPos=%1").arg(pos),db).toString().trimmed();
	if (activConv!="0")
		return true;
	return false;
}

//#define passWeak QString("!assim")
//#define passStrong QString("assim")
//extern bool adminMode;
//static bool getPassword(QString title="Password is needed to open this window.",QString text="Please give the password",QString pass=QString(),QWidget *p=NULL)
//{
//	if (adminMode)
//		return true;
//#ifdef _DEBUG
//	return true;
//#endif
//	QString userTxt=QInputDialog::getText(p,title,title,QLineEdit::Password);
//	if (uCheckPassword(userTxt))
//		return true;
//	if (pass.length()>0)
//	{
//		if (userTxt==pass)
//			return true;
//	}
//	else if (userTxt==passWeak)
//		return true;
//	else if (userTxt.length()!=0)
//	{
//		QMessageBox::critical(p,"Wrong password","This is the wrong password.");
//		return false;
//	}
//	return false;
//}
extern QMap<QString,double> contentSmallWeightMap;
extern QMap<QString,double> contentSmallWidthMap;
extern QMap<QString,double> contentAllWeightMap;
extern QMap<QString,double> contentAllWidthMap;
#define contentNone QString("None")
static double toLibs(double w)
{
	return w/0.45359237;
}
static double toInches(double w)
{
	return w/25.4;
}
static int typeOfBasket(int b)
{
	return getOne(QString("select type from pos where plcId=%1").arg(b),db).toInt();
	//if ((b>0)&&(b<=26))
	//	return cellTypeBig;
	//else
	//	return cellTypeNormal;
}
static double calculateWeightOfBasket(int b)
{
	return getFirst(QString("select cast(sum(weight) as real) from basketsContents inner join contentTypes on basketsContents.contentType=contentTypes.content where basket=%1").arg(b),db).toDouble();
}
static double calculateWidthOfBasket(int b)
{
	return getFirst(QString("select cast(sum(width) as real) from basketsContents inner join contentTypes on basketsContents.contentType=contentTypes.content where basket=%1").arg(b),db).toDouble();
}
class specialDiesClass
{	
	QVector<int>					specialCassettes1;
	QVector<int>					specialCassettes2;
	QStringList						specialDies1;
	QStringList						specialDies2;
	QString								currentErrorString;
public:
	specialDiesClass();
	~specialDiesClass();
	bool isCombinationValid(int cassette,QString die);
	QString getErrorStr();
};
#endif