#include "pp.h"
profileProductionClass::profileProductionClass(int profileCode_):QWidget()
{
	profileCode=profileCode_;
	setAutoFillBackground(true);
	offsetToTouchTheNextProfileInTheLayer=0.;
	offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated=true;
	//
	int interProfileSpace=GETFIRST(QString("select interProfileSpace from profileCode where die=%1").arg(profileCode)).toInt();
	int multiplicity=GETFIRST(QString("select multiplicity from profileCode where die=%1").arg(profileCode)).toInt();
	int inComb=GETFIRST(QString("select inComb from profileCode where die=%1").arg(profileCode)).toInt();
	int sp1=GETFIRST(QString("select sp1 from profileCode where die=%1").arg(profileCode)).toInt();
	int sp2=GETFIRST(QString("select sp2 from profileCode where die=%1").arg(profileCode)).toInt();
	int sp3=GETFIRST(QString("select sp3 from profileCode where die=%1").arg(profileCode)).toInt();
	int sp4=GETFIRST(QString("select sp4 from profileCode where die=%1").arg(profileCode)).toInt();
	int sp5=GETFIRST(QString("select sp5 from profileCode where die=%1").arg(profileCode)).toInt();
	int sp6=GETFIRST(QString("select sp6 from profileCode where die=%1").arg(profileCode)).toInt();
	int sp7=GETFIRST(QString("select sp7 from profileCode where die=%1").arg(profileCode)).toInt();
	int sp8=GETFIRST(QString("select sp8 from profileCode where die=%1").arg(profileCode)).toInt();
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);

	mainLayout->addWidget(new QLabel(QString("%1").arg(profileCode)),0,Qt::AlignHCenter);
	myScene=new QGraphicsScene();//list of objects
	myQGraphicsView=new dxfQGraphicsViewClass(this,50.0,1.0);//viewer 
	myQGraphicsView->setMinimumSize(200,200);
	myQGraphicsView->scale(1.0,1.0);
	myQGraphicsView->setMaximumWidth(200);
	QBrush b=QBrush(Qt::SolidPattern);
	b.setColor(QColor(230,230,230));
	myQGraphicsView->setBackgroundBrush(b);
	myQGraphicsView->setScene(myScene);//setScene
	mainLayout->addWidget(myQGraphicsView,0,Qt::AlignHCenter);
	//zero y line
	QPen pen=QPen(Qt::DashLine);
	pen.setColor(QColor(150,150,150));
	pen.setWidthF(0.0);
	QGraphicsLineItem *zeroXLine=new QGraphicsLineItem(QLine(-30.,0.,30.,0.));
	zeroXLine->setPen(pen);
	myScene->addItem(zeroXLine);
	/////////////////////////////////////////////////////////////////////////////////////
	QGridLayout *dataLayout=new QGridLayout();mainLayout->addLayout(dataLayout);
	//
	//lengthSpinBox
	dataLayout->addWidget(new QLabel(tr("Length")),2,0);
	lengthSpinBox=new QSpinBox();
	dataLayout->addWidget(lengthSpinBox,2,1);
	lengthSpinBox->setMinimum(1);
	lengthSpinBox->setMaximum(INT_MAX);
	lengthSpinBox->blockSignals(true);
	lengthSpinBox->setValue(6000);
	lengthSpinBox->blockSignals(false);
	connect(lengthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(lengthSpinBoxSlot(int)));
	//doubleLengthBox
	dataLayout->addWidget(new QLabel(tr("Double")),4,0);
	doubleLengthBox=new QCheckBox(tr(""));
	dataLayout->addWidget(doubleLengthBox,4,1);
	connect(doubleLengthBox,SIGNAL(stateChanged(int)),this,SLOT(doubleLengthBoxSlot(int)));
	//angleSpinBox
	dataLayout->addWidget(new QLabel(tr("Angle")),5,0);
	angleSpinBox=new QDoubleSpinBox();
	dataLayout->addWidget(angleSpinBox,5,1);
	angleSpinBox->setMinimum(-180.);
	angleSpinBox->setMaximum(180.);
	angleSpinBox->blockSignals(true);
	double angle=GETFIRST(QString("select angle from profileCode where die=%1").arg(profileCode)).toDouble();
	angleSpinBox->setValue(angle);
	angleSpinBox->blockSignals(false);
	connect(angleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(angleSpinBoxSlot(double)));
	//interProfileSpaceSpinBox
	dataLayout->addWidget(new QLabel(tr("Profile spacing")),7,0);
	interProfileSpaceSpinBox=new QSpinBox();
	dataLayout->addWidget(interProfileSpaceSpinBox,7,1);
	interProfileSpaceSpinBox->setMinimum(0);
	interProfileSpaceSpinBox->setMaximum(INT_MAX);
	interProfileSpaceSpinBox->blockSignals(true);
	interProfileSpaceSpinBox->setValue(interProfileSpace);
	interProfileSpaceSpinBox->blockSignals(false);
	connect(interProfileSpaceSpinBox,SIGNAL(valueChanged(int)),this,SLOT(interProfileSpaceSpinBoxSlot(int)));
	//multiplicitySpinBox
	dataLayout->addWidget(new QLabel(tr("Profile multiplicity")),8,0);
	multiplicitySpinBox=new QSpinBox();
	dataLayout->addWidget(multiplicitySpinBox,8,1);
	multiplicitySpinBox->setMinimum(1);
	multiplicitySpinBox->setMaximum(INT_MAX);
	multiplicitySpinBox->blockSignals(true);
	multiplicitySpinBox->setValue(multiplicity);
	multiplicitySpinBox->blockSignals(false);
	connect(multiplicitySpinBox,SIGNAL(valueChanged(int)),this,SLOT(multiplicitySpinBoxSlot(int)));
	//spacers
	QGroupBox *spacersGroupBox=new QGroupBox(tr("Spacers"));mainLayout->addWidget(spacersGroupBox);
	QVBoxLayout *spacersGroupBoxLayout=new QVBoxLayout();spacersGroupBox->setLayout(spacersGroupBoxLayout);
	spacersGroupBoxLayout->setMargin(1);
	//inCombCheckBox
	inCombCheckBox=new QCheckBox(tr("In comb"));
	spacersGroupBoxLayout->addWidget(inCombCheckBox);
	inCombCheckBox->setChecked(inComb>0?true:false);
	connect(inCombCheckBox,SIGNAL(stateChanged(int)),this,SLOT(inCombCheckBoxSlot(int)));
	//spacers
	QGridLayout *spacersGroupBoxDownLayout=new QGridLayout();spacersGroupBoxLayout->addLayout(spacersGroupBoxDownLayout);
	QPushButton *calculateSpacersBtn=new QPushButton();calculateSpacersBtn->setIcon(QIcon(":/calculator.png"));spacersGroupBoxDownLayout->addWidget(calculateSpacersBtn,0,0,2,1,Qt::AlignCenter);
	//calculateSpacersBtn->setFlat(true);
	//calculateSpacersBtn->setMaximumWidth(16);calculateSpacersBtn->setMaximumHeight(22);
	connect(calculateSpacersBtn,SIGNAL(clicked(bool)),this,SLOT(calculateSpacersBtnSlot(bool)));
	spacersGroupBoxDownLayout->addWidget(new QLabel(tr("1")),0,1,Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(new QLabel(tr("2")),0,2,Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(new QLabel(tr("3")),0,3,Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(new QLabel(tr("4")),0,4,Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(new QLabel(tr("5")),0,5,Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(new QLabel(tr("6")),0,6,Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(new QLabel(tr("7")),0,7,Qt::AlignHCenter);
	spacersGroupBoxDownLayout->addWidget(new QLabel(tr("8")),0,8,Qt::AlignHCenter);

	spacersGroupBoxDownLayout->addWidget(sp1Box=new QCheckBox(),1,1,Qt::AlignHCenter);sp1Box->setChecked(sp1==1?true:false);connect(sp1Box,SIGNAL(stateChanged(int)),this,SLOT(sp1BoxSlot(int)));
	spacersGroupBoxDownLayout->addWidget(sp2Box=new QCheckBox(),1,2,Qt::AlignHCenter);sp2Box->setChecked(sp2==1?true:false);connect(sp2Box,SIGNAL(stateChanged(int)),this,SLOT(sp2BoxSlot(int)));
	spacersGroupBoxDownLayout->addWidget(sp3Box=new QCheckBox(),1,3,Qt::AlignHCenter);sp3Box->setChecked(sp3==1?true:false);connect(sp3Box,SIGNAL(stateChanged(int)),this,SLOT(sp3BoxSlot(int)));
	spacersGroupBoxDownLayout->addWidget(sp4Box=new QCheckBox(),1,4,Qt::AlignHCenter);sp4Box->setChecked(sp4==1?true:false);connect(sp4Box,SIGNAL(stateChanged(int)),this,SLOT(sp4BoxSlot(int)));
	spacersGroupBoxDownLayout->addWidget(sp5Box=new QCheckBox(),1,5,Qt::AlignHCenter);sp5Box->setChecked(sp5==1?true:false);connect(sp5Box,SIGNAL(stateChanged(int)),this,SLOT(sp5BoxSlot(int)));
	spacersGroupBoxDownLayout->addWidget(sp6Box=new QCheckBox(),1,6,Qt::AlignHCenter);sp6Box->setChecked(sp6==1?true:false);connect(sp6Box,SIGNAL(stateChanged(int)),this,SLOT(sp6BoxSlot(int)));
	spacersGroupBoxDownLayout->addWidget(sp7Box=new QCheckBox(),1,7,Qt::AlignHCenter);sp7Box->setChecked(sp7==1?true:false);connect(sp7Box,SIGNAL(stateChanged(int)),this,SLOT(sp7BoxSlot(int)));
	spacersGroupBoxDownLayout->addWidget(sp8Box=new QCheckBox(),1,8,Qt::AlignHCenter);sp8Box->setChecked(sp8==1?true:false);connect(sp8Box,SIGNAL(stateChanged(int)),this,SLOT(sp8BoxSlot(int)));
	spacersGroupBoxDownLayout->setMargin(0);
	mainLayout->addStretch(1);
	//profile
	color=getGlobalColor();
#ifdef LOCAL_DB
	QString dxfDir="#ProductionPlanDxfs//";
#else
	QString dxfDir="I://productionPlanDxfs//";
#endif
	if (!QDir().exists(dxfDir))
		dxfDir=QDir::currentPath()+"//productionPlanDxfs//";
	if (!QDir().exists(dxfDir))
		EXIT("Error","The dxf files of the profiles could not be located!");
	if (profileCode<1100)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("1000//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<1200)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("1100//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<1300)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("1200//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<1400)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("1300//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<1500)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("1400//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<1600)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("1500//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<1700)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("1600//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<1800)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("1700//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<1900)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("1800//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<2000)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("1900//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<2100)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("2000//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<2200)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("2100//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<2300)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("2200//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<2400)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("2300//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<2500)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("2400//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<2600)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("2500//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<2700)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("2600//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<2800)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("2700//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<2900)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("2800//%1.dxf").arg(profileCode),angle,true,color);
	else if (profileCode<3000)profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("2900//%1.dxf").arg(profileCode),angle,true,color);
	else EXIT("error",QString("Could not locate dxf file for profile:%1").arg(profileCode));

	myScene->addItem(profileGraphics);
	profileGraphics->setFlag(QGraphicsItem::ItemIsMovable,false);
	//
	calculateOffsetToTouchTheNextProfileInTheLayer();
	if ( (sp1==0)&&(sp2==0)&&(sp3==0)&&(sp4==0)&&(sp5==0)&&(sp6==0)&&(sp7==0)&&(sp8==0) )
		calculateSpacers();
}
profileProductionClass::~profileProductionClass()
{
}
//
void profileProductionClass::setSpacers(bool sp1,bool sp2,bool sp3,bool sp4,bool sp5,bool sp6,bool sp7,bool sp8)
{
	sp1Box->setChecked(sp1);
	sp2Box->setChecked(sp2);
	sp3Box->setChecked(sp3);
	sp4Box->setChecked(sp4);
	sp5Box->setChecked(sp5);
	sp6Box->setChecked(sp6);
	sp7Box->setChecked(sp7);
	sp8Box->setChecked(sp8);
}
void profileProductionClass::calculateSpacers()
{
	double totalLength=getProfileLength()*(doubleLengthBox->isChecked()?2.:1.);
	if (doubleLengthBox->isChecked())
	{
		if (getProfileLength()<3000)
			setSpacers(0,1,1,0,0,1,1,0);
		else 
			setSpacers(1,1,0,1,1,0,0,1);
	}
	else
	{
		if (getProfileLength()<4110)
			setSpacers(0,0,1,1,0,1,0,0);
		else if (getProfileLength()<5750)
			setSpacers(0,1,0,1,1,0,1,0);
		else 
			setSpacers(1,0,1,0,0,1,0,1);
	}
}
QVector<bool> profileProductionClass::getSpacers()
{
	QVector<bool> ans;
	ans<<(sp1Box->isChecked()?true:false);
	ans<<(sp2Box->isChecked()?true:false);
	ans<<(sp3Box->isChecked()?true:false);
	ans<<(sp4Box->isChecked()?true:false);
	ans<<(sp5Box->isChecked()?true:false);
	ans<<(sp6Box->isChecked()?true:false);
	ans<<(sp7Box->isChecked()?true:false);
	ans<<(sp8Box->isChecked()?true:false);
	return ans;
}
void profileProductionClass::setNeedsToBeLoadedFromDatabase(bool needsToBeLoadedFromDatabase_)
{
	if (needsToBeLoadedFromDatabase!=needsToBeLoadedFromDatabase_)
	{
		needsToBeLoadedFromDatabase=needsToBeLoadedFromDatabase_;
		if (needsToBeLoadedFromDatabase)
		{
			QBrush b=QBrush(Qt::BDiagPattern);
			b.setColor(QColor(150,150,150));
			myQGraphicsView->setBackgroundBrush(b);
		}
		else
		{
			QBrush b=QBrush(Qt::SolidPattern);
			b.setColor(QColor(230,230,230));
			myQGraphicsView->setBackgroundBrush(b);
		}
	}
}
QPainterPath profileProductionClass::getProfilePath()
{
	return profileGraphics->getPath();
}
double profileProductionClass::getProfileLength()
{
	return lengthSpinBox->value();
}
int profileProductionClass::getProfileMultiplicity()
{
	return multiplicitySpinBox->value();
}
double profileProductionClass::getWeightOfOneProfile()
{
	return (0.001*lengthSpinBox->value())*multiplicitySpinBox->value()*0.1*(doubleLengthBox->isChecked()?2.:1.);
}

void profileProductionClass::calculateOffsetToTouchTheNextProfileInTheLayer()
{
	offsetToTouchTheNextProfileInTheLayer=getMinXDistanceOfPaths(profileGraphics->getPath(),profileGraphics->getPath().translated(profileGraphics->getPath().boundingRect().width(),0.0));
	offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated=false;
}
double profileProductionClass::getOffsetToTouchTheNextProfileInTheLayer()
{
	if (offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated)
		calculateOffsetToTouchTheNextProfileInTheLayer();
	return offsetToTouchTheNextProfileInTheLayer;
}
int profileProductionClass::getMultiplicityOfProfiles()
{
	return multiplicitySpinBox->value();
}
double profileProductionClass::getProfileHeight()
{
	return profileGraphics->boundingRect().height();
}
double profileProductionClass::getProfileSpaceingInLayer()
{
	return interProfileSpaceSpinBox->value();
}
double profileProductionClass::getProfileWidth()
{
	return profileGraphics->boundingRect().width();
}
bool profileProductionClass::getIsOnComb()
{
	return 	(inCombCheckBox->checkState()==Qt::Checked?true:false);
}
QColor profileProductionClass::getColor()
{
	return color;
}
///////////////////////////////////////////////
void profileProductionClass::lengthSpinBoxSlot(int v)
{
	if (v<3352.8)//11 feet
		doubleLengthBox->setChecked(true);
	else
		doubleLengthBox->setChecked(false);
	emit productionChangedSignal();
}
void profileProductionClass::doubleLengthBoxSlot(int v)
{
	int in=v==Qt::Checked?PROFILE_DOUBLE_LENGTH:PROFILE_SINGLE_LENGTH;
	emit productionChangedSignal();
}
void profileProductionClass::calculateSpacersBtnSlot(bool)
{
	calculateSpacers();
}
bool profileProductionClass::getIsToBePutOnNextLayer()
{
	return false;
}
int profileProductionClass::getProfileCode()
{
	return profileCode;
}
void profileProductionClass::angleSpinBoxSlot(double v)
{
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set angle=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,%2,1,0,0,0,0,0,0,0,0,0)").arg(profileCode).arg(v),MAIN_DB_CONNECTION);
	profileGraphics->loadFromDxfFile(v,true);
	emit productionChangedSignal();
	offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated=true;
}
void profileProductionClass::interProfileSpaceSpinBoxSlot(int v)
{
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set interProfileSpace=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,%2,0,0,1,0,0,0,0,0,0,0,0)").arg(profileCode).arg(v),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}
void profileProductionClass::multiplicitySpinBoxSlot(int v)
{
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set multiplicity=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,%2,0,0,0,0,0,0,0,0,0)").arg(profileCode).arg(v),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}
void profileProductionClass::inCombCheckBoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set inComb=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,%2,0,0,0,0,0,0,0,0)").arg(profileCode).arg(in),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}
void profileProductionClass::sp1BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp1=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,%2,0,0,0,0,0,0,0)").arg(profileCode).arg(in),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}
void profileProductionClass::sp2BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp2=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,%2,0,0,0,0,0,0)").arg(profileCode).arg(in),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}

void profileProductionClass::sp3BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;

	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp3=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,%2,0,0,0,0,0)").arg(profileCode).arg(in),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}
void profileProductionClass::sp4BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp4=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,0,%2,0,0,0,0)").arg(profileCode).arg(in),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}
void profileProductionClass::sp5BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp5=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,0,0,%2,0,0,0)").arg(profileCode).arg(in),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}
void profileProductionClass::sp6BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp6=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,0,0,0,%2,0,0)").arg(profileCode).arg(in),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}
void profileProductionClass::sp7BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp7=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,0,0,0,0,%2,0)").arg(profileCode).arg(in),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}
void profileProductionClass::sp8BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp8=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,0,0,0,0,0,%2)").arg(profileCode).arg(in),MAIN_DB_CONNECTION);
	emit productionChangedSignal();
}