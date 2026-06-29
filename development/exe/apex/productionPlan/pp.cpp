#include "pp.h"
profileProductionClass::profileProductionClass(int tableId_,int isFirstInList,int productionOrder_,int profileCode_,int totalProfileQuantity_,int inBasketsProfileQuantity_,double linearDencity_,int profileLength_,int doubleLength_,int productionStaus_,int onNextBasket_,int onNextLayer_,QString temper_,QString soNum_,QString scheduleDate,int scheduleShift,bool isVirtual
	,int numberOfBilletsProduced,double billetLength,double billetButtLength,double scrapFrontLength,double scrapRearLength,double runOutLength,double dieMultiplicity,QVector<QVariant> temperDescriptionVector,QVector<QVariant> temperTypeVector):QWidget()
{
	setAutoFillBackground(true);
	isSelected=false;
	offsetToTouchTheNextProfileInTheLayer=0.;
	offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated=true;
	soNum=soNum_;
	//
	tableId=tableId_;
	productionOrder=productionOrder_;
	profileCode=profileCode_;
	int productionStaus=productionStaus_;
	int onNextBasket=onNextBasket_;
	int onNextLayer=onNextLayer_;
	QString temper=temper_;
	//
	inBasketsProfileQuantity=inBasketsProfileQuantity_;
	int profileLength=profileLength_;
	int doubleLength=doubleLength_;
	double linearDencity=linearDencity_;
	isVirtualEntryInDatabase=isVirtual;
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
	int totalProfileQuantity=totalProfileQuantity_;
	if ((numberOfBilletsProduced>0)&&(billetLength>0.))
	{
		double totalWeight=numberOfBilletsProduced*(billetLength-billetButtLength)*0.0871;
		double weightOfRunOut=runOutLength*0.001*dieMultiplicity*linearDencity;
		int numberOfRunOuts=totalWeight/weightOfRunOut;
		int numberOfProfilesPerRunOut=dieMultiplicity*(runOutLength-scrapFrontLength-scrapRearLength)/profileLength;
		totalProfileQuantity=numberOfRunOuts*numberOfProfilesPerRunOut;
		// commented out by Gene
		apexExecQuery(QString("update epicsProduction2 set totalQuantity=%1 where tid=%2").arg(totalProfileQuantity).arg(tableId));
	}
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	QString shiftStr;
	switch (scheduleShift)
	{
	case 1:
			shiftStr="S_1";break;
	case 2:
			shiftStr="S_2";break;
	case 3:
			shiftStr="S_3";break;
	default:
		break;
	} 

	//mainLayout->addWidget(new QLabel(QString("%1 (<font color='red'><b>%2</b></font>,%3,%4)").arg(soNum).arg(profileCode).arg(scheduleDate.left(5)).arg(shiftStr)),0,Qt::AlignHCenter);
	mainLayout->addWidget(new QLabel(QString("SO: %1 Die: %2,%3,%4").arg(soNum).arg(profileCode).arg(scheduleDate.left(10)).arg(shiftStr)),0,Qt::AlignHCenter);
	myScene=new QGraphicsScene(this);//list of objects
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
	///status
	QGroupBox *statusGroupBox=new QGroupBox(tr("status"));mainLayout->addWidget(statusGroupBox);
	QGridLayout *statusGroupBoxLayout=new QGridLayout();statusGroupBox->setLayout(statusGroupBoxLayout);
	statusGroupBoxLayout->addWidget(productionStatusScheduledRadioBtn=new QRadioButton(tr("Scheduled")),0,0        );if (productionStaus==PRODUCTION_STATUS_SCHEDULED)productionStatusScheduledRadioBtn->setChecked(true);        connect(productionStatusScheduledRadioBtn,SIGNAL(clicked(bool)),this,SLOT(productionStatusScheduledRadioBtnSlot(bool)));
	statusGroupBoxLayout->addWidget(productionStatusOnPressRadioBtn=new QRadioButton(tr("Press")),0,1       );if (productionStaus==PRODUCTION_STATUS_ON_PRESS)productionStatusOnPressRadioBtn->setChecked(true);				connect(productionStatusOnPressRadioBtn,SIGNAL(clicked(bool)),this,SLOT(productionStatusOnPressRadioBtnSlot(bool)));
	statusGroupBoxLayout->addWidget(productionStatusFinishedRadioBtn=new QRadioButton(tr("Finished")),0,2      );																																																					connect(productionStatusFinishedRadioBtn,SIGNAL(clicked(bool)),this,SLOT(productionStatusFinishedRadioBtnSlot(bool)));
	statusGroupBoxLayout->setMargin(1);
	//quantitySpinBox
	QGridLayout *quantityLayout=new QGridLayout();mainLayout->addLayout(quantityLayout);
	quantityLayout->addWidget(new QLabel(tr("Total quantity")),0,0);
	totalQuantitySpinBox=new QSpinBox();
	quantityLayout->addWidget(totalQuantitySpinBox,0,1);
	totalQuantitySpinBox->setMinimum(1);
	totalQuantitySpinBox->setMaximum(INT_MAX);
	totalQuantitySpinBox->blockSignals(true);
	totalQuantitySpinBox->setValue(totalProfileQuantity);
	totalQuantitySpinBox->blockSignals(false);
	connect(totalQuantitySpinBox,SIGNAL(valueChanged(int)),this,SLOT(totalQuantitySpinBoxSlot(int)));
	quantityInBasketsSpinBox=NULL;
	if (isFirstInList)
	{
		quantityLayout->addWidget(new QLabel(tr("Quantity in baskets")),1,0);
		quantityInBasketsSpinBox=new QSpinBox();
		quantityLayout->addWidget(quantityInBasketsSpinBox,1,1);
		quantityInBasketsSpinBox->setMinimum(0);
		quantityInBasketsSpinBox->setMaximum(INT_MAX);
		quantityInBasketsSpinBox->blockSignals(true);
		quantityInBasketsSpinBox->setValue(inBasketsProfileQuantity);
		quantityInBasketsSpinBox->blockSignals(false);
		connect(quantityInBasketsSpinBox,SIGNAL(valueChanged(int)),this,SLOT(quantityInBasketsSpinBoxSlot(int)));
	}
	//more
	moreToolButton=new QPushButton();moreToolButton->setIcon(QIcon(":/plus.png"));mainLayout->addWidget(moreToolButton,Qt::AlignRight);
	moreToolButton->setFlat(true);
	moreToolButton->setMaximumWidth(10);moreToolButton->setMaximumHeight(10);
	connect(moreToolButton,SIGNAL(clicked(bool)),this,SLOT(morePlusToolButtonSlot(bool)));
	moreWidget=new QWidget();mainLayout->addWidget(moreWidget);
	QVBoxLayout *moreWidgetLayout=new QVBoxLayout();moreWidget->setLayout(moreWidgetLayout);
	moreWidgetLayout->setMargin(1);
	/////////////////////////////////////////////////////////////////////////////////////
	QGridLayout *dataLayout=new QGridLayout();moreWidgetLayout->addLayout(dataLayout);
	//
	//next BasketCheckBtn
	dataLayout->addWidget(onNextBasketCheckBtn=new QCheckBox(tr("On next basket")),0,0);if (onNextBasket==1)onNextBasketCheckBtn->setChecked(true);
	connect(onNextBasketCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(onNextBasketCheckBtnSlot(int)));
	//onNextlayer
	dataLayout->addWidget(onNextLayerCheckBtn=new QCheckBox(tr("On next layer")),0,1);if (onNextLayer==1)onNextLayerCheckBtn->setChecked(true);
	connect(onNextLayerCheckBtn,SIGNAL(stateChanged(int)),this,SLOT(onNextLayerCheckBtnSlot(int)));
	//temperComboBox
	dataLayout->addWidget(new QLabel(tr("Temper")),1,0);
	dataLayout->addWidget(temperComboBox=new QComboBox(),1,1);
	for (int i=0;i<temperDescriptionVector.count();i++)
		temperComboBox->addItem(temperDescriptionVector[i].toString().trimmed());
	int currentIndex=temperDescriptionVector.indexOf(temper);
	if (currentIndex>=0)
		temperComboBox->setCurrentIndex(currentIndex);
	connect(temperComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(temperComboBoxSlot(int)));
	if (!isVirtualEntryInDatabase)
		temperComboBox->setEnabled(false);
	//lengthSpinBox
	dataLayout->addWidget(new QLabel(tr("Length")),2,0);
	lengthSpinBox=new QSpinBox();
	dataLayout->addWidget(lengthSpinBox,2,1);
	lengthSpinBox->setMinimum(1);
	lengthSpinBox->setMaximum(INT_MAX);
	lengthSpinBox->blockSignals(true);
	lengthSpinBox->setValue(profileLength);
	lengthSpinBox->blockSignals(false);
	connect(lengthSpinBox,SIGNAL(valueChanged(int)),this,SLOT(lengthSpinBoxSlot(int)));
	if (!isVirtualEntryInDatabase)
		lengthSpinBox->setEnabled(false);
	//linearDencitySpinBox
	dataLayout->addWidget(new QLabel(tr("Linear dencity")),3,0);
	linearDencitySpinBox=new QDoubleSpinBox();
	dataLayout->addWidget(linearDencitySpinBox,3,1);
	linearDencitySpinBox->setMinimum(0.1);
	linearDencitySpinBox->setMaximum(DBL_MAX);
	linearDencitySpinBox->blockSignals(true);
	linearDencitySpinBox->setValue(linearDencity);
	linearDencitySpinBox->blockSignals(false);
	connect(linearDencitySpinBox,SIGNAL(valueChanged(double)),this,SLOT(linearDencitySpinBoxSlot(double)));
	if (!isVirtualEntryInDatabase)
		linearDencitySpinBox->setEnabled(false);
	//doubleLengthBox
	dataLayout->addWidget(new QLabel(tr("Double")),4,0);
	doubleLengthBox=new QCheckBox(tr(""));
	dataLayout->addWidget(doubleLengthBox,4,1);
	if ((doubleLength==PROFILE_SINGLE_LENGTH)||(doubleLength==PROFILE_DOUBLE_LENGTH))
		doubleLengthBox->setChecked(doubleLength==PROFILE_DOUBLE_LENGTH);
	else//not set before
	{
		if (profileLength<3352.8)//11 feet
			doubleLengthBox->setChecked(true);
	}
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

	//interLayerSpaceSpinBox
	//dataLayout->addWidget(new QLabel(tr("Layer spacing")),6,0);
	//interLayerSpaceSpinBox=new QSpinBox();
	//dataLayout->addWidget(interLayerSpaceSpinBox,6,1);
	//interLayerSpaceSpinBox->setMinimum(0);
	//interLayerSpaceSpinBox->setMaximum(INT_MAX);
	//interLayerSpaceSpinBox->blockSignals(true);
	//interLayerSpaceSpinBox->setValue(interLayerSpace);
	//interLayerSpaceSpinBox->blockSignals(false);
	//connect(interLayerSpaceSpinBox,SIGNAL(valueChanged(int)),this,SLOT(interLayerSpaceSpinBoxSlot(int)));
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
	QGroupBox *spacersGroupBox=new QGroupBox(tr("Spacers"));moreWidgetLayout->addWidget(spacersGroupBox);
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
	moreWidget->hide();
	//profile
	color=getGlobalColor();

	QString dxfDir=dxfDirPath;
	if (!QDir().exists(dxfDir))
		dxfDir=QDir::currentPath()+"./../../productionPlanDxfs/";
	if (!QDir().exists(dxfDir))
		EXIT("Error",QString("The dxf files dir of the profiles '%1' could not be located!").arg(dxfDir));
	profileGraphics=new dxfGroupOfGraphicsItemClass(dxfDir+QString("%1//%2.dxf").arg((int)(floor(profileCode/100)*100)).arg(profileCode),angle,true,color);
	myScene->addItem(profileGraphics);
	profileGraphics->setFlag(QGraphicsItem::ItemIsMovable,false);

	// new code below added by Gene

	// Get the actual width and height
	double height = getProfileHeight();
	double width = getProfileWidth();


	if ((height > 300.0) || (width > 500.0)) {
		// Calculate scale factor to fit within 300 height and 500 width
		double scaleFactorH = 300.0 / height;
		double scaleFactorW = 500.0 / width;
		double scaleFactor = qMin(scaleFactorH, scaleFactorW); // Keep aspect ratio

		profileGraphics->setScale(scaleFactor);
		// Calculate new dimensions
		double newWidth = width * scaleFactor;
		double newHeight = height * scaleFactor;

		// Push to the item
		//profileGraphics->>setScaledSize(newWidth, newHeight);
		// apexExecQuery(QString("update epicsProduction2 set quantityInBaskets = totalQuantity, status = 2 where tid=%2").arg(tableId));
	//	QMessageBox::critical(this, "Error", tr("dxf:%1 needs to be cleaned.                      ").arg(profileCode));
	//	QMessageBox::critical(this, "Error", tr("dxf:%1 has been logged ask Eng to clean DXF (line removed) please RELOAD APP.").arg(profileCode));
		QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

		apexExecQuery(QString("INSERT INTO errorLog (tid, die, emessage, ctime) "
			"VALUES (%1, %2, '%3', '%4')")
			.arg(tableId)
			.arg(profileCode)
			.arg(QString("Die is oversized (%1mm H x %2mm W).")
				.arg(QString::number(height, 'f', 2))
				.arg(QString::number(width, 'f', 2)))
			.arg(currentTime));
		QString profileCodeStr = QString::number(profileCode);
		 int newProfile = 9901;
		// int newProfile = 9957;
		myScene->removeItem(profileGraphics);
		profileGraphics = new dxfGroupOfGraphicsItemClass(dxfDir + QString("%1//%2.dxf").arg((int)(floor(newProfile / 100) * 100)).arg(newProfile), angle, true, QColor(Qt::black));
		
		 QRectF scaledRect = profileGraphics->mapToScene(profileGraphics->boundingRect()).boundingRect();


		 myScene->setSceneRect(scaledRect);
		
		//myScene->addItem(profileGraphics);
	
		profileGraphics->setFlag(QGraphicsItem::ItemIsMovable, false);
		// Create and add a text item to the scene
		// Convert int to QString for display
	//	QString profileCodeStr = QString::number(profileCode);

		// Add the text to the scene
		QGraphicsTextItem* textItem = myScene->addText(profileCodeStr);
		// Set font (optional but recommended)
		QFont font("Arial", 8, QFont::Bold);
		textItem->setFont(font);

		// Center the text within the scaledRect
		QRectF textBounds = textItem->boundingRect();
		qreal x = scaledRect.center().x() - textBounds.width() / 2;
		qreal y = scaledRect.center().y() - textBounds.height() / 2;
		textItem->setPos(x, y);
		QTransform t = textItem->transform();
		t.scale(1, -1);
		textItem->setTransform(t);

		// Make the text non-movable as well (optional)
		textItem->setFlag(QGraphicsItem::ItemIsMovable, false);
		myScene->addItem(profileGraphics);
	}


		//

		calculateOffsetToTouchTheNextProfileInTheLayer();

		if ((sp1 == 0) && (sp2 == 0) && (sp3 == 0) && (sp4 == 0) && (sp5 == 0) && (sp6 == 0) && (sp7 == 0) && (sp8 == 0))
			calculateSpacers();
		setPrivileges(PRIVILEGETYPE);
	
}
profileProductionClass::~profileProductionClass()
{
	delete profileGraphics;
	//delete myScene;
	//delete myQGraphicsView;
	//delete myScene;
	//delete productionStatusScheduledRadioBtn;
	//delete productionStatusOnPressRadioBtn;
	//delete productionStatusFinishedRadioBtn;
	//delete onNextBasketCheckBtn;
	//delete onNextLayerCheckBtn;
	//delete temperComboBox;
	//delete moreWidget;
	//delete moreToolButton;
	//delete totalQuantitySpinBox;
	//if (quantityInBasketsSpinBox)
	//	delete quantityInBasketsSpinBox;
}
QString profileProductionClass::getSoNum()
{
	return soNum;
}
//
dxfGroupOfGraphicsItemClass *profileProductionClass::getProfileGraphics()
{
	return profileGraphics;
}
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
void profileProductionClass::setNeedsToBeLoadedFromDatabase(int needsToBeLoadedFromDatabase_)
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
void profileProductionClass::setPrivileges(int privilegeType)
{
	if (!(privilegeType&PRIVILEGE_TYPE_PRESS))
		productionStatusOnPressRadioBtn->setEnabled(false);
	else
		productionStatusOnPressRadioBtn->setEnabled(true);
	if (!(privilegeType&PRIVILEGE_TYPE_STACKER))
		productionStatusFinishedRadioBtn->setEnabled(false);
	else
		productionStatusFinishedRadioBtn->setEnabled(true);
	if ((!(privilegeType&PRIVILEGE_TYPE_OFFICE))||(!(privilegeType&PRIVILEGE_TYPE_STACKER)))
		totalQuantitySpinBox->setEnabled(false);
	else
		totalQuantitySpinBox->setEnabled(true);
	if (quantityInBasketsSpinBox)
	{
		if (!(privilegeType&PRIVILEGE_TYPE_STACKER))
			quantityInBasketsSpinBox->setEnabled(false);
		else
			quantityInBasketsSpinBox->setEnabled(true);
		if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
			onNextBasketCheckBtn->setEnabled(false);
	}
	else
		onNextBasketCheckBtn->setEnabled(true);
	if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
		onNextLayerCheckBtn->setEnabled(false);
	else
		onNextLayerCheckBtn->setEnabled(true);
	if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
		temperComboBox->setEnabled(false);
	else
	{
		if (isVirtualEntryInDatabase)
			temperComboBox->setEnabled(true);
	}
	if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
		lengthSpinBox->setEnabled(false);
	else
	{
		if (isVirtualEntryInDatabase)
			lengthSpinBox->setEnabled(true);
	}
	if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
		linearDencitySpinBox->setEnabled(false);
	else
	{
		if (isVirtualEntryInDatabase)
			linearDencitySpinBox->setEnabled(true);
	}
	if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
		doubleLengthBox->setEnabled(false);
	else
		doubleLengthBox->setEnabled(true);
	if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
		angleSpinBox->setEnabled(false);
	else
		angleSpinBox->setEnabled(true);
	if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
		multiplicitySpinBox->setEnabled(false);
	else
		multiplicitySpinBox->setEnabled(true);
	//if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
	//	interLayerSpaceSpinBox->setEnabled(false);
	//else
	//	interLayerSpaceSpinBox->setEnabled(true);
	if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
		interProfileSpaceSpinBox->setEnabled(false);
	else
		interProfileSpaceSpinBox->setEnabled(true);
	if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
		inCombCheckBox->setEnabled(false);
	else
		inCombCheckBox->setEnabled(true);
	if (!(privilegeType&PRIVILEGE_TYPE_OFFICE))
	{
		sp1Box->setEnabled(false);
		sp2Box->setEnabled(false);
		sp3Box->setEnabled(false);
		sp4Box->setEnabled(false);
		sp5Box->setEnabled(false);
		sp6Box->setEnabled(false);
		sp7Box->setEnabled(false);
		sp8Box->setEnabled(false);
	}
	else
	{
		sp1Box->setEnabled(true);
		sp2Box->setEnabled(true);
		sp3Box->setEnabled(true);
		sp4Box->setEnabled(true);
		sp5Box->setEnabled(true);
		sp6Box->setEnabled(true);
		sp7Box->setEnabled(true);
		sp8Box->setEnabled(true);
	}
}
void profileProductionClass::enterEvent(QEvent *ev)
{
	setBackgroundRole(QPalette::Midlight);
}
void profileProductionClass::leaveEvent(QEvent *ev)
{
	if (isSelected)
		setBackgroundRole(QPalette::Highlight);
	else
		setBackgroundRole(QPalette::Dark);
}
void profileProductionClass::mousePressEvent(QMouseEvent *ev)
{
	if (isSelected)
		emit selectedSignal(this,false);
	else
		emit selectedSignal(this,true);
}
void profileProductionClass::setIsSelected(bool s)
{
	isSelected=s;
	if (isSelected)
		setBackgroundRole(QPalette::Highlight);
	else
		setBackgroundRole(QPalette::Dark);
}
bool profileProductionClass::getIsSelected()
{
	return isSelected;
}
int profileProductionClass::getProfileCode()
{
	return profileCode;
}
int profileProductionClass::getProfilesEpicsTableId()
{
	return tableId;
}
int profileProductionClass::getTotalProfileMultiplicity()
{
	return multiplicitySpinBox->value()*(doubleLengthBox->isChecked()?2.:1.);
}
void profileProductionClass::setProductionOrder(int po)
{
	productionOrder=po;
	apexExecQuery(QString("update epicsProduction2 set productionOrder=%1 where tid=%2").arg(productionOrder).arg(tableId));
}
int profileProductionClass::getProductionOrder()
{
	return productionOrder;
}
QPainterPath profileProductionClass::getProfilePath()
{

	
	return profileGraphics->getPath();
}
int profileProductionClass::getTemperType()
{
	return GETONE(QString("select temperType from temperTypes where id=%1").arg(temperComboBox->currentIndex()+1),BASKET_CRANE_DB_CONNECTION).toInt();
}
double profileProductionClass::getProfileLength()
{
	return lengthSpinBox->value();
}
int profileProductionClass::getProfileMultiplicity()
{
	return multiplicitySpinBox->value();
}
double profileProductionClass::getProfileLinearDencity()
{
	return linearDencitySpinBox->value();
}
double profileProductionClass::getWeightOfOneProfile()
{
	return (0.001*lengthSpinBox->value())*multiplicitySpinBox->value()*linearDencitySpinBox->value()*(doubleLengthBox->isChecked()?2.:1.);
}
int profileProductionClass::getProductionStatus()
{
	if (productionStatusScheduledRadioBtn->isChecked())
		return PRODUCTION_STATUS_SCHEDULED;
	else if (productionStatusOnPressRadioBtn->isChecked())
		return PRODUCTION_STATUS_ON_PRESS;
	else if (productionStatusFinishedRadioBtn->isChecked())
		return PRODUCTION_STATUS_FINISHED;
	return 0;
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
int profileProductionClass::getNumberOfProfilesNotInABasket()
{
	return totalQuantitySpinBox->value()-inBasketsProfileQuantity;
}
bool profileProductionClass::getIsDoubleLength()
{
	return doubleLengthBox->isChecked();
}
int profileProductionClass::getMultiplicityOfProfiles()
{
	return multiplicitySpinBox->value();
}
bool profileProductionClass::getIsToBePutOnNextBasket()
{
	return onNextBasketCheckBtn->checkState()==Qt::Checked;
}
bool profileProductionClass::getIsToBePutOnNextLayer()
{
	return onNextLayerCheckBtn->checkState()==Qt::Checked;
}
double profileProductionClass::getProfileHeight()
{
	if (!profileGraphics)
		return 0.0;  // Or some sensible default/error
	return profileGraphics->boundingRect().height();
}
//double profileProductionClass::getProfileInterLayerSpaceing()
//{
//	return interLayerSpaceSpinBox->value();
//}
double profileProductionClass::getProfileSpaceingInLayer()
{
	return interProfileSpaceSpinBox->value();
}
double profileProductionClass::getProfileWidth()
{
	if (!profileGraphics)
		return 0.0;  // Or some sensible default/error
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
void profileProductionClass::calculateSpacersBtnSlot(bool)
{
	calculateSpacers();
}
void profileProductionClass::morePlusToolButtonSlot(bool)
{
	moreWidget->setMaximumWidth(QWidget::width());
	if (moreWidget->isVisible())
	{
		moreWidget->hide();
		moreToolButton->setIcon(QIcon(":/plus.png"));
	}
	else
	{
		moreWidget->show();
		moreToolButton->setIcon(QIcon(":/minus.png"));
	}
}
void profileProductionClass::productionStatusScheduledRadioBtnSlot(bool checked)
{
	if (checked)
	{
		apexExecQuery(QString("update epicsProduction2 set status=%1 where tid=%2").arg(PRODUCTION_STATUS_SCHEDULED).arg(tableId));
		emit productionChangedSignal(true);
	}
}
void profileProductionClass::productionStatusOnPressRadioBtnSlot(bool checked)
{
	if (checked)
	{
		apexExecQuery(QString("update epicsProduction2 set status=%1 where tid=%2").arg(PRODUCTION_STATUS_ON_PRESS).arg(tableId));
		emit productionChangedSignal(true);
	}
}
void profileProductionClass::productionStatusFinishedRadioBtnSlot(bool checked)
{
	if (checked)
	{
		apexExecQuery(QString("update epicsProduction2 set status=%1 where tid=%2").arg(PRODUCTION_STATUS_FINISHED).arg(tableId));
		emit productionChangedSignal(true);
	}
}
void profileProductionClass::lengthSpinBoxSlot(int v)
{
	apexExecQuery(QString("update epicsProduction2 set length=%1 where tid=%2").arg(v).arg(tableId));
	if (v<3352.8)//11 feet
		doubleLengthBox->setChecked(true);
	else
		doubleLengthBox->setChecked(false);
	emit productionChangedSignal(false);
}
void profileProductionClass::linearDencitySpinBoxSlot(double v)
{
	apexExecQuery(QString("update epicsProduction2 set linearDencity=%1 where tid=%2").arg(v).arg(tableId));
	emit productionChangedSignal(false);
}
void profileProductionClass::doubleLengthBoxSlot(int v)
{
	int in=v==Qt::Checked?PROFILE_DOUBLE_LENGTH:PROFILE_SINGLE_LENGTH;
	apexExecQuery(QString("update epicsProduction2 set doubleLength=%1 where tid=%2").arg(in).arg(tableId));
	calculateSpacersBtnSlot(true);
	emit productionChangedSignal(false);
}
void profileProductionClass::onNextBasketCheckBtnSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	apexExecQuery(QString("update epicsProduction2 set onNextBasket=%1 where tid=%2").arg(in).arg(tableId));
	if (v==Qt::Checked)
		onNextLayerCheckBtn->setChecked(true);
	emit productionChangedSignal(false);
}
void profileProductionClass::onNextLayerCheckBtnSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	apexExecQuery(QString("update epicsProduction2 set onNextLayer=%1 where tid=%2").arg(in).arg(tableId));
	emit productionChangedSignal(false);
}
void profileProductionClass::temperComboBoxSlot(int index)
{
	QVector<QVariant> productionTemperDescription=execQuery(QString("select temper from temperTypes "),BASKET_CRANE_DB_CONNECTION);
	apexExecQuery(QString("update epicsProduction2 set temper='%1' where tid=%2").arg(productionTemperDescription[index].toString()).arg(tableId));
	emit productionChangedSignal(false);
}

void profileProductionClass::totalQuantitySpinBoxSlot(int v)
{
	apexExecQuery(QString("update epicsProduction2 set totalQuantity=%1 where tid=%2").arg(v).arg(tableId));
//	if (quantityInBasketsSpinBox)
	//	quantityInBasketsSpinBox->setMaximum(v);
	emit productionChangedSignal(false);
}
void profileProductionClass::quantityInBasketsSpinBoxSlot(int v)
{
	inBasketsProfileQuantity=v;
	apexExecQuery(QString("update epicsProduction2 set quantityInBaskets=%1 where tid=%2").arg(v).arg(tableId));
	emit productionChangedSignal(false);
}
void profileProductionClass::angleSpinBoxSlot(double v)
{
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set angle=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,%2,1,0,0,0,0,0,0,0,0,0)").arg(profileCode).arg(v));
	profileGraphics->loadFromDxfFile(v,true);
	emit productionChangedSignal(false);
	offsetToTouchTheNextProfileInTheLayerNeedsToBeCalculated=true;
}
//void profileProductionClass::interLayerSpaceSpinBoxSlot(int v)
//{
//	if (v>0)
//		inCombCheckBox->setChecked(true);
//	execQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set interLayerSpace=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,%2,0,1,0,0,0,0,0,0,0,0,0)").arg(profileCode).arg(v));
//	emit productionChangedSignal();
//}
void profileProductionClass::interProfileSpaceSpinBoxSlot(int v)
{
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set interProfileSpace=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,%2,0,0,1,0,0,0,0,0,0,0,0)").arg(profileCode).arg(v));
	emit productionChangedSignal(false);
}
void profileProductionClass::multiplicitySpinBoxSlot(int v)
{
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set multiplicity=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,%2,0,0,0,0,0,0,0,0,0)").arg(profileCode).arg(v));
	emit productionChangedSignal(false);
}
void profileProductionClass::inCombCheckBoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	//if (in==0)
	//	interLayerSpaceSpinBox->setValue(0);
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set inComb=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,%2,0,0,0,0,0,0,0,0)").arg(profileCode).arg(in));
	emit productionChangedSignal(false);
}
void profileProductionClass::sp1BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp1=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,%2,0,0,0,0,0,0,0)").arg(profileCode).arg(in));
	emit productionChangedSignal(false);
}
void profileProductionClass::sp2BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp2=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,%2,0,0,0,0,0,0)").arg(profileCode).arg(in));
	emit productionChangedSignal(false);
}

void profileProductionClass::sp3BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;

	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp3=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,%2,0,0,0,0,0)").arg(profileCode).arg(in));
	emit productionChangedSignal(false);
}
void profileProductionClass::sp4BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp4=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,0,%2,0,0,0,0)").arg(profileCode).arg(in));
	emit productionChangedSignal(false);
}
void profileProductionClass::sp5BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp5=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,0,0,%2,0,0,0)").arg(profileCode).arg(in));
	emit productionChangedSignal(false);
}
void profileProductionClass::sp6BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp6=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,0,0,0,%2,0,0)").arg(profileCode).arg(in));
	emit productionChangedSignal(false);
}
void profileProductionClass::sp7BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp7=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,0,0,0,0,%2,0)").arg(profileCode).arg(in));
	emit productionChangedSignal(false);
}
void profileProductionClass::sp8BoxSlot(int v)
{
	int in=v==Qt::Checked?1:0;
	apexExecQuery(QString("if exists (select die from profileCode where die=%1) update profileCode set sp8=%2 where die=%1 else insert into profileCode (die,interProfileSpace,angle,multiplicity,inComb,sp1,sp2,sp3,sp4,sp5,sp6,sp7,sp8) values (%1,0,0,1,0,0,0,0,0,0,0,0,%2)").arg(profileCode).arg(in));
	emit productionChangedSignal(false);
}