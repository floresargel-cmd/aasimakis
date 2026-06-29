#include "pch.h"
#include "rollersDlg.h"
rollersDlgClass::rollersDlgClass(tuxipClass *tuxip_,QWidget *p):QDialog(p)
{
	resize(1300,1000);
	tuxip=tuxip_;
	setWindowTitle(tr("Rollers HMI"));
	QVBoxLayout *mainL=new QVBoxLayout();mainL->setMargin(0);
	setLayout(mainL);
	QSplitter *centralSplitter=new QSplitter(Qt::Horizontal);mainL->addWidget(centralSplitter);
	QWidget *modeW=new QWidget();QVBoxLayout *modeL=new QVBoxLayout();modeL->setMargin(2);modeW->setLayout(modeL);centralSplitter->addWidget(modeW);
	QWidget *rollersW=new QWidget();QVBoxLayout *rollersWL=new QVBoxLayout();rollersWL->setMargin(2);rollersW->setLayout(rollersWL);centralSplitter->addWidget(rollersW);
	//modeW
	int row;
	QGroupBox *modeGroupBox=new QGroupBox(tr("Modes"));QVBoxLayout *modeGroupBoxL=new QVBoxLayout();modeGroupBoxL->setMargin(2);modeGroupBox->setLayout(modeGroupBoxL);modeL->addWidget(modeGroupBox);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Auto"),"",tuxip,"pcCom",0,1,0,"","pcCom",1,0,"","","","","",buttonStyle,buttonStyleActive,tr("Auto"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Manual"),"",tuxip,"pcCom",0,2,0,"","pcCom",1,1,"","","","","",buttonStyle,buttonStyleActive,tr("Manual"),this);modeGroupBoxL->addWidget(pushButtonsVector.last());
	
	QGroupBox *layer1GroupBox=new QGroupBox(tr("Layer 1"));QGridLayout *layer1GroupBoxL=new QGridLayout();layer1GroupBoxL->setMargin(2);layer1GroupBox->setLayout(layer1GroupBoxL);modeL->addWidget(layer1GroupBox);
	row=0;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 1 On") ,"",tuxip,"pcCom",0,1010,0,"","pcCom",1,8,"","","","","",buttonStyle,buttonStyleActive,tr("Layer 1 On"),this);layer1GroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 1 Off"),"",tuxip,"pcCom",0,1011,0,"","pcCom",1,8,"","","","","",buttonStyleActive,buttonStyle,tr("Layer 1 Off"),this);layer1GroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 1B On") ,"",tuxip,"pcCom",0,1016,0,"","pcCom",1,22,"","","","","",buttonStyle,buttonStyleActive,tr("Layer 1B On"),this);layer1GroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 1B Off"),"",tuxip,"pcCom",0,1017,0,"","pcCom",1,22,"","","","","",buttonStyleActive,buttonStyle,tr("Layer 1B Off"),this);layer1GroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("To Station 1"),"",tuxip,"pcCom",0,1012,0,"","pcCom",1,3,"","","","","",buttonStyleActive,buttonStyle,tr("To Station 1"),this);layer1GroupBoxL->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("To Station 2"),"",tuxip,"pcCom",0,1013,0,"","pcCom",1,3,"","","","","",buttonStyle,buttonStyleActive ,tr("To Station 2"),this);layer1GroupBoxL->addWidget(pushButtonsVector.last(),row,1);

	QGroupBox *layer2GroupBox=new QGroupBox(tr("Layer 2"));QGridLayout *layer2GroupBoxL=new QGridLayout();layer2GroupBoxL->setMargin(2);layer2GroupBox->setLayout(layer2GroupBoxL);modeL->addWidget(layer2GroupBox);
	row=0;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 2 On") ,"",tuxip,"pcCom",0,2010,0,"","pcCom",1,9,"","","","","",buttonStyle,buttonStyleActive,tr("Layer 2 On"),this);layer2GroupBoxL-> addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 2 Off"),"",tuxip,"pcCom",0,2011,0,"","pcCom",1,9,"","","","","",buttonStyleActive,buttonStyle,tr("Layer 2 Off"),this);layer2GroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 2A On") ,"",tuxip,"pcCom",0,2012,0,"","pcCom",1,10,"","","","","",buttonStyle,buttonStyleActive,tr("Layer 2A On"),this);layer2GroupBoxL-> addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 2A Off"),"",tuxip,"pcCom",0,2013,0,"","pcCom",1,10,"","","","","",buttonStyleActive,buttonStyle,tr("Layer 2A Off"),this);layer2GroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 2B On") ,"",tuxip,"pcCom",0,2014,0,"","pcCom",1,11,"","","","","",buttonStyle,buttonStyleActive,tr("Layer 2B On"),this);layer2GroupBoxL-> addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 2B Off"),"",tuxip,"pcCom",0,2015,0,"","pcCom",1,11,"","","","","",buttonStyleActive,buttonStyle,tr("Layer 2B Off"),this);layer2GroupBoxL->addWidget(pushButtonsVector.last(),row,1);

	QGroupBox *layer3GroupBox=new QGroupBox(tr("Layer 3"));QGridLayout *layer3GroupBoxL=new QGridLayout();layer3GroupBoxL->setMargin(2);layer3GroupBox->setLayout(layer3GroupBoxL);modeL->addWidget(layer3GroupBox);
	row=0;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 3 On") ,"",tuxip,"pcCom",0,3010,0,"","pcCom",1,12,"","","","","",buttonStyle,buttonStyleActive,tr("Layer 3 On"),this);layer3GroupBoxL ->addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 3 Off"),"",tuxip,"pcCom",0,3011,0,"","pcCom",1,12,"","","","","",buttonStyleActive,buttonStyle,tr("Layer 3 Off"),this);layer3GroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 3A On") ,"",tuxip,"pcCom",0,3012,0,"","pcCom",1,13,"","","","","",buttonStyle,buttonStyleActive,tr("Layer 3A On"),this);layer3GroupBoxL-> addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 3A Off"),"",tuxip,"pcCom",0,3013,0,"","pcCom",1,13,"","","","","",buttonStyleActive,buttonStyle,tr("Layer 3A Off"),this);layer3GroupBoxL->addWidget(pushButtonsVector.last(),row,1);
	row++;
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 3B On") ,"",tuxip,"pcCom",0,3014,0,"","pcCom",1,14,"","","","","",buttonStyle,buttonStyleActive,tr("Layer 3B On"),this);layer3GroupBoxL-> addWidget(pushButtonsVector.last(),row,0);
	pushButtonsVector<<new hmiPushButtonWidgetClass(tr("Layer 3B Off"),"",tuxip,"pcCom",0,3015,0,"","pcCom",1,14,"","","","","",buttonStyleActive,buttonStyle,tr("Layer 3B Off"),this);layer3GroupBoxL->addWidget(pushButtonsVector.last(),row,1);

	QGroupBox *alarmsGroupBox=new QGroupBox(tr("Alarms"));QVBoxLayout *alarmsGroupBoxL=new QVBoxLayout();alarmsGroupBoxL->setMargin(2);alarmsGroupBox->setLayout(alarmsGroupBoxL);modeL->addWidget(alarmsGroupBox);
	bitLogVectorV<<new bitLogVectorClass("pcCom","rollersAlarms.txt",bitClass::TYPE_SHOW_HIDE,styleLabelR,styleLabelGr,pDb,"","","",false,bitLogHisoryClass::typeMsSql,this);alarmsGroupBoxL->addWidget(bitLogVectorV.last());

	//rollersW
	layoutGraphicsView=new myQGraphicsViewClass(true,this);rollersWL->addWidget(layoutGraphicsView);
	layoutScene=new QGraphicsScene(this);
	QPushButton *zoomExtentsBtn=new QPushButton(QIcon(":/zoomExtents.png"),tr("Zoom extents"),layoutGraphicsView);
	zoomExtentsBtn->move(5,5);
	zoomExtentsBtn->setIconSize(QSize(16,16));
	connect(zoomExtentsBtn,SIGNAL(clicked()),this,SLOT(resetGraphicsViewZoom()));
	gItemClass *layoutGraphicsItem=new gItemClass(":/dxfs/rollersLayout.dxf",QPen(QColor(200,200,200)),false,false,QColor(),this);
	layoutGraphicsView->setScene(layoutScene);
	layoutGraphicsItem->setZValue(-10.);layoutScene->addItem(layoutGraphicsItem);
	layoutGraphicsView->scale(0.025,0.025);

	float btnSize=750;
	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,1002,"pcCom",2,0,layoutScene,tr("Roller 1 FWD"),-90.,0,btnSize); gBtnItemVector.last()->setPos(1000.,-1500-btnSize);
	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,1003,"pcCom",2,1,layoutScene,tr("Roller 1 BWD"),+90.,0,btnSize); gBtnItemVector.last()->setPos(1000.,-1000+btnSize);

	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,2002,"pcCom",3,0,layoutScene,tr("Roller 2 FWD"),-90.,0,btnSize); gBtnItemVector.last()->setPos(1000.,-10500-btnSize);
	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,2003,"pcCom",3,1,layoutScene,tr("Roller 2 BWD"),+90.,0,btnSize); gBtnItemVector.last()->setPos(1000.,-10000+btnSize);

	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,3002,"pcCom",4,0,layoutScene,tr("Roller 3 FWD"),-90.,0,btnSize); gBtnItemVector.last()->setPos(1000.,-19500-btnSize);
	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,3003,"pcCom",4,1,layoutScene,tr("Roller 3 BWD"),+90.,0,btnSize); gBtnItemVector.last()->setPos(1000.,-19000+btnSize);

	gProximitySwitchVector<<new gProximitySwitchClass(crane3Ip,"pcCom",3,13,tr("Roller 2 Is Up. Input CB3.303"),0,layoutScene,gProximitySwitchClass::typeHorizontal,3500.,-9070,100.);
	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,2004,"pcCom",3,10,layoutScene,tr("Roller 2 Up")  ,+90.,0,btnSize); gBtnItemVector.last()->setPos(+3500.,-10000+btnSize);
	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,2005,"pcCom",3,11,layoutScene,tr("Roller 2 Down"),-90.,0,btnSize); gBtnItemVector.last()->setPos(+3500.,-10500-btnSize);
	gProximitySwitchVector<<new gProximitySwitchClass(crane3Ip,"pcCom",3,14,tr("Roller 2 Is Down. Input CB3.304"),0,layoutScene,gProximitySwitchClass::typeHorizontal,3500.,-11400,100.);

	gProximitySwitchVector<<new gProximitySwitchClass(crane3Ip,"pcCom",4,13,tr("Roller 3 Is Up. Input CB3.310"),0,layoutScene,gProximitySwitchClass::typeHorizontal,3500.,-18080,100.);
	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,3004,"pcCom",4,10,layoutScene,tr("Roller 3 Up")  ,+90.,0,btnSize); gBtnItemVector.last()->setPos(+3500.,-19000+btnSize);
	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,3005,"pcCom",4,11,layoutScene,tr("Roller 3 Down"),-90.,0,btnSize); gBtnItemVector.last()->setPos(+3500.,-19500-btnSize);
	gProximitySwitchVector<<new gProximitySwitchClass(crane3Ip,"pcCom",4,14,tr("Roller 3 Is Down. Input CB3.311"),0,layoutScene,gProximitySwitchClass::typeHorizontal,3500.,-20420,100.);
	
	gProximitySwitchVector<<new gProximitySwitchClass(crane3Ip,"pcCom",2,3,tr("Roller 1 Existence. Input CB3.300"),0,layoutScene,gProximitySwitchClass::typeHorizontal,1000.,0,1000.);
	gProximitySwitchVector<<new gProximitySwitchClass(crane3Ip,"pcCom",3,3,tr("Roller 2 Start. Input CB3.305"),0,layoutScene,gProximitySwitchClass::typeHorizontal,1000.,-5000,1000.);
	gProximitySwitchVector<<new gProximitySwitchClass(crane3Ip,"pcCom",4,3,tr("Roller 3 Start. Input CB3.312"),0,layoutScene,gProximitySwitchClass::typeHorizontal,1000.,-15000,1000.);
	gProximitySwitchVector<<new gProximitySwitchClass(crane3Ip,"pcCom",4,4,tr("Roller 3 Stop. Input CB3.302"),0,layoutScene,gProximitySwitchClass::typeHorizontal,1000.,-23000,1000.);

	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,1023,"pcCom",3,-1,layoutScene,tr("Roller 1-2 BWD"),+90.,0,btnSize); gBtnItemVector.last()->setPos(+3500.,-4700+btnSize);
	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,1022,"pcCom",3,-1,layoutScene,tr("Roller 1-2 FWD"),-90.,0,btnSize); gBtnItemVector.last()->setPos(+3500.,-5200-btnSize);

	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,2023,"pcCom",3,-1,layoutScene,tr("Roller 2-3 BWD"),+90.,0,btnSize); gBtnItemVector.last()->setPos(+3500.,-14000+btnSize);
	gBtnItemVector<<new gBtnItemClass(tuxip,"pcCom",0,2022,"pcCom",3,-1,layoutScene,tr("Roller 2-3 FWD"),-90.,0,btnSize); gBtnItemVector.last()->setPos(+3500.,-14500-btnSize);

	centralSplitter->setSizes(QList<int>()<<300<<1000-300);
}
rollersDlgClass::~rollersDlgClass()
{
}
void rollersDlgClass::resetGraphicsViewZoom()
{
	layoutScene->setSceneRect(layoutScene->itemsBoundingRect());
	layoutGraphicsView->fitInView(layoutGraphicsView->sceneRect(),Qt::KeepAspectRatio);
}
int rollersDlgClass::exec()
{
	return QDialog::exec();
}
void rollersDlgClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (crane3Ip==ip)
	{
		if ("pcCom"==table)
		{
			for (int i=0;i<pushButtonsVector.count();i++)
				pushButtonsVector[i]->updateValue(table,index,value.toInt());
			for (int i=0;i<gProximitySwitchVector.count();i++)
				gProximitySwitchVector[i]->dataInPlcChanged(ip,table,index,value.toInt());
			for (int i=0;i<gBtnItemVector.count();i++)
				gBtnItemVector[i]->dataInPlcChanged(ip,table,index,value.toInt());
			for (int i=0;i<bitLogVectorV.count();i++)
				bitLogVectorV[i]->updateValue(table,index,value.toInt());
		}
	}
}