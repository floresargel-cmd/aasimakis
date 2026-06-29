#include "exp.h"
extern bool globalInMyLaptop;
exportStationClass::exportStationClass(int stationTypeEnum_,tcpServerClass *tcpServer_,QWidget *w):QWidget(w)
{
	alarmIsActive=false;
	stationTypeEnum=stationTypeEnum_;
	tcpServer=tcpServer_;
	//
	QVBoxLayout *mainLayout=new QVBoxLayout(this);
	setLayout(mainLayout);
	QSplitter *mainSplitter=new QSplitter(Qt::Vertical);
	mainLayout->addWidget(mainSplitter);
	//
	QWidget *upWidget=new QWidget();QVBoxLayout *upLayout=new QVBoxLayout();upWidget->setLayout(upLayout);
	QWidget *downWidget=new QWidget();QVBoxLayout *downLayout=new QVBoxLayout();downWidget->setLayout(downLayout);
	mainSplitter->addWidget(upWidget);
	mainSplitter->addWidget(downWidget);
	//upLayout
	QHBoxLayout *butnsLayout=new QHBoxLayout();upLayout->addLayout(butnsLayout);
	QHBoxLayout *contentsLayout=new QHBoxLayout();upLayout->addLayout(contentsLayout);

	stationNameLabel=new QLabel(stationTypeEnum==TYPE_OMAV?tr("   Left station"):tr("   Right station"));
	butnsLayout->addWidget(stationNameLabel);
	basketLabel=new QLabel(tr("No basket"));
	butnsLayout->addWidget(basketLabel);
	nextBasketLabel=new QLabel(tr("No next basket"));
	butnsLayout->addWidget(nextBasketLabel);
	butnsLayout->addStretch(1);
	alarmLabel=new QLabel(tr("Alarm"));alarmLabel->setAlignment(Qt::AlignHCenter);alarmLabel->setFont(QFont("Helvetica",20));alarmLabel->setStyleSheet("QLabel {color:gray;font-size:100px;}");
	alarmLabel->hide();
	butnsLayout->addWidget(alarmLabel);
	connect(&flashTimer, SIGNAL(timeout()), this, SLOT(onFlashTimer()));
#ifdef PASS_ACTIVE
	userNameLabel=new QLabel(tr("No user loged in"));//userNameLabel->setStyleSheet("font-size:25px;");
	butnsLayout->addWidget(userNameLabel);

	loginPushButton=new QPushButton(QIcon(":unLock.png"),tr("Login"),this);
	loginPushButton->setStatusTip(tr("User login"));
	loginPushButton->setToolTip(tr("User login"));
	loginPushButton->setIconSize(QSize(30,30));
	connect(loginPushButton,SIGNAL(released()),this,SLOT(loginSlot()));
	butnsLayout->addWidget(loginPushButton);
	
	logoutPushButton=new QPushButton(QIcon(":lock.png"),tr("Logout"),this);
	logoutPushButton->setStatusTip(tr("User logout"));
	logoutPushButton->setToolTip(tr("User logout"));
	logoutPushButton->setEnabled(false);
	logoutPushButton->setIconSize(QSize(30,30));
	connect(logoutPushButton,SIGNAL(released()),this,SLOT(logoutSlot()));
	butnsLayout->addWidget(logoutPushButton);
#endif
	basketContents=new basketContentsClass(stationTypeEnum,tcpServer,this);
	connect(basketContents,SIGNAL(basketChangedSignal()),this,SLOT(basketChangedSlot()));
	upLayout->addWidget(basketContents,1);
	//downLayout
	basketList=new basketListClass(stationTypeEnum,this);
	downLayout->addWidget(basketList,0);

	QList<int> s;s<<800<<300;
	mainSplitter->setSizes(s);

	//
	//lockGui();
#ifdef PASS_ACTIVE
	curentUser="debug user";
	unLockGui();
#endif
}
void exportStationClass::onFlashTimer()
{
}
#ifdef PASS_ACTIVE
void exportStationClass::lockGui()
{
	basketContents->lockGui();
	basketList->lockGui();
}
void exportStationClass::unLockGui()
{
	basketContents->unLockGui(curentUser);
	basketList->unLockGui();
}
void exportStationClass::plcValueChanged(int index,int value)
{
	if (index==0)
		basketContents->plcStatusChanged(0,value);
	if (((stationTypeEnum==TYPE_OMAV)&&(index==14))||((stationTypeEnum==TYPE_ASSIM)&&(index==11)))
	{
		basketContents->updateData(value);
		basketLabel->setText(QString("%1").arg(value));
		if (!globalInMyLaptop)
			apexExecQuery(QString("update DestackerStations set basketnum=%1 where stationid=%2").arg(value).arg(stationTypeEnum==TYPE_OMAV?"1":"2"),APEX_DB_CONNECTION);
	}
	if (((stationTypeEnum==TYPE_OMAV)&&(index==13))||((stationTypeEnum==TYPE_ASSIM)&&(index==10)))
	{
		if (value!=0)
			nextBasketLabel->setText(QString("( next is %1 )").arg(value));
		else
			nextBasketLabel->setText(QString("( no next basket )"));
	}
}
void exportStationClass::basketChangedSlot()
{
	basketList->updateList(true);
}
//////////////////////////////////////////////////////////////////////////////////////////
void exportStationClass::loginSlot()
{
	passwordClass pass(this);
	if (pass.hasPrivileges())
	{
		curentUser=pass.getUser();
		userNameLabel->setText(tr("User: ")+curentUser);
		loginPushButton->setDisabled(true);
		logoutPushButton->setDisabled(false);
		unLockGui();
	}
}
void exportStationClass::logoutSlot()
{
	curentUser="";
	userNameLabel->setText(tr("No user loged in"));
	loginPushButton->setDisabled(false);
	logoutPushButton->setDisabled(true);
	lockGui();
}
#endif
