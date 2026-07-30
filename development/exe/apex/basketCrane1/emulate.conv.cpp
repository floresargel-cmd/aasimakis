#include "emulate.conv.h"

#include "configuration/ConfigurationManager.h"

conveyorsEmulatorClass::conveyorsEmulatorClass(tuxipClass *conveyorTuxip_):QWidget()
{
	const BasketCrane::Configuration::ConfigurationManager& configuration =
		BasketCrane::Configuration::ConfigurationManager::Instance();
	conveyorTuxip=conveyorTuxip_;
	///
	connect(&mainTimer,SIGNAL(timeout()),this,SLOT(mainTimerSlot()));
	mainTimer.start(configuration.Timers().EmulationConveyorMainMilliseconds());
	QVBoxLayout *mainLayout=new QVBoxLayout();QWidget::setLayout(mainLayout);
	//stayInOven
	ovenEntranseReady=false;
	QHBoxLayout *stayInOvenLayout=new QHBoxLayout();mainLayout->addLayout(stayInOvenLayout);
	stayInOvenLayout->addWidget(new QLabel(tr("stay in oven for:")));
	stayInOvenTimeSpinBox=new QSpinBox();stayInOvenLayout->addWidget(stayInOvenTimeSpinBox);
	stayInOvenTimeSpinBox->setMinimum(configuration.Emulation().OvenStayMinimumSeconds());
	stayInOvenTimeSpinBox->setMaximum(configuration.Emulation().OvenStayMaximumSeconds());
	stayInOvenTimeSpinBox->setValue(configuration.Emulation().OvenStayDefaultSeconds());
	stayInOvenLayout->addWidget(new QLabel(tr(" sec")));
	ovenReadyPushButton=new QPushButton(tr("oven ready"));stayInOvenLayout->addWidget(ovenReadyPushButton);
	stayInOvenLayout->addStretch(1);

	connect(stayInOvenTimeSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onStayInOvenTimerSpinBoxSlot(int)));
	connect(ovenReadyPushButton,SIGNAL(clicked()),this,SLOT(onOvenReadySlot()));
	connect(&stayInOvenTimer,SIGNAL(timeout()),this,SLOT(onOvenReadySlot()));
	if (hasBasketsInOven())
		stayInOvenTimer.start(stayInOvenTimeSpinBox->value()*1000);
	//
	connect(&propagateBasketsTimer,SIGNAL(timeout()),this,SLOT(propagateBasketsTimerSlot()));
	propagateBasketsTimer.start(configuration.Timers().EmulationConveyorPropagationMilliseconds());
	//
	mainLayout->addStretch(1);
	//
}

void conveyorsEmulatorClass::mainTimerSlot()
{
}

void conveyorsEmulatorClass::tuxIsReady()
{
}

bool conveyorsEmulatorClass::hasBasketsInOven()
{
	if (GETONE(QString("select basket from positions where description='oven600'"),EMULATE_DB_CONNECTION).toInt()!=0)return true;
	if (GETONE(QString("select basket from positions where description='oven610'"),EMULATE_DB_CONNECTION).toInt()!=0)return true;
	if (GETONE(QString("select basket from positions where description='oven620'"),EMULATE_DB_CONNECTION).toInt()!=0)return true;
	if (GETONE(QString("select basket from positions where description='oven630'"),EMULATE_DB_CONNECTION).toInt()!=0)return true;
	if (GETONE(QString("select basket from positions where description='oven640'"),EMULATE_DB_CONNECTION).toInt()!=0)return true;
	if (GETONE(QString("select basket from positions where description='oven650'"),EMULATE_DB_CONNECTION).toInt()!=0)return true;
	return false;
}
void conveyorsEmulatorClass::onStayInOvenTimerSpinBoxSlot(int t)
{
	stayInOvenTimer.setInterval(t*1000);

}
void conveyorsEmulatorClass::onOvenReadySlot()
{
	takeBasketsOutFromOven();
}
////

void conveyorsEmulatorClass::takeBasketsInToOven()
{
	if (hasBasketsInOven())
		return;
	moveBasket("beforeOven100","oven600",GETONE(QString("select basket from positions where description='beforeOven100'"),EMULATE_DB_CONNECTION).toInt());
	moveBasket("beforeOven110","oven610",GETONE(QString("select basket from positions where description='beforeOven110'"),EMULATE_DB_CONNECTION).toInt());
	moveBasket("beforeOven120","oven620",GETONE(QString("select basket from positions where description='beforeOven120'"),EMULATE_DB_CONNECTION).toInt());
	moveBasket("beforeOven130","oven630",GETONE(QString("select basket from positions where description='beforeOven130'"),EMULATE_DB_CONNECTION).toInt());
	moveBasket("beforeOven140","oven640",GETONE(QString("select basket from positions where description='beforeOven140'"),EMULATE_DB_CONNECTION).toInt());
	moveBasket("beforeOven150","oven650",GETONE(QString("select basket from positions where description='beforeOven150'"),EMULATE_DB_CONNECTION).toInt());
	conveyorTuxip->writeInteger("N11",12,GETONE("select basket from positions where description='oven600'",EMULATE_DB_CONNECTION).toInt());
	conveyorTuxip->writeInteger("N11",14,GETONE("select basket from positions where description='oven610'",EMULATE_DB_CONNECTION).toInt());
	conveyorTuxip->writeInteger("N11",4, GETONE("select basket from positions where description='oven620'",EMULATE_DB_CONNECTION).toInt());
	conveyorTuxip->writeInteger("N11",6, GETONE("select basket from positions where description='oven630'",EMULATE_DB_CONNECTION).toInt());
	conveyorTuxip->writeInteger("N11",13,GETONE("select basket from positions where description='oven640'",EMULATE_DB_CONNECTION).toInt());
	conveyorTuxip->writeInteger("N11",5, GETONE("select basket from positions where description='oven650'",EMULATE_DB_CONNECTION).toInt());

	stayInOvenTimer.start(stayInOvenTimeSpinBox->value()*1000);
}

void conveyorsEmulatorClass::takeBasketsOutFromOven()
{
	if (GETONE("select basket from positions where description='afterOven660'",EMULATE_DB_CONNECTION).toInt()>0)return;
	if (GETONE("select basket from positions where description='afterOven670'",EMULATE_DB_CONNECTION).toInt()>0)return;
	if (GETONE("select basket from positions where description='afterOven680'",EMULATE_DB_CONNECTION).toInt()>0)return;
	if (GETONE("select basket from positions where description='afterOven690'",EMULATE_DB_CONNECTION).toInt()>0)return;

	moveBasket("oven600","afterOven660",GETONE(QString("select basket from positions where description='oven600'"),EMULATE_DB_CONNECTION).toInt());
	moveBasket("oven610","afterOven670",GETONE(QString("select basket from positions where description='oven610'"),EMULATE_DB_CONNECTION).toInt());
	moveBasket("oven620","afterOven680",GETONE(QString("select basket from positions where description='oven620'"),EMULATE_DB_CONNECTION).toInt());
	moveBasket("oven630","afterOven690",GETONE(QString("select basket from positions where description='oven630'"),EMULATE_DB_CONNECTION).toInt());
	moveBasket("oven640","afterOven670",GETONE(QString("select basket from positions where description='oven640'"),EMULATE_DB_CONNECTION).toInt());
	moveBasket("oven650","afterOven690",GETONE(QString("select basket from positions where description='oven650'"),EMULATE_DB_CONNECTION).toInt());

	conveyorTuxip->writeInteger("N11",15,GETONE("select basket from positions where description='afterOven660'",EMULATE_DB_CONNECTION).toInt());
	conveyorTuxip->writeInteger("N11",16,GETONE("select basket from positions where description='afterOven670'",EMULATE_DB_CONNECTION).toInt());
	conveyorTuxip->writeInteger("N11",7 ,GETONE("select basket from positions where description='afterOven680'",EMULATE_DB_CONNECTION).toInt());
	conveyorTuxip->writeInteger("N11",8 ,GETONE("select basket from positions where description='afterOven690'",EMULATE_DB_CONNECTION).toInt());
	stayInOvenTimer.stop();
}

void conveyorsEmulatorClass::propagateBasketsTimerSlot()
{
	// returnigFromOven160                                                          returnigFromOven160
	int basketOn160=GETONE(QString("select basket from positions where description='returnigFromOven160'"),EMULATE_DB_CONNECTION).toInt();
	int basketOn720=GETONE(QString("select basket from positions where description='returnigFromOven720'"),EMULATE_DB_CONNECTION).toInt();
	int basketOn710=GETONE(QString("select basket from positions where description='returnigFromOven710'"),EMULATE_DB_CONNECTION).toInt();
	int basketOn700=GETONE(QString("select basket from positions where description='returnigFromOven700'"),EMULATE_DB_CONNECTION).toInt();
	int basketOn730=GETONE(QString("select basket from positions where description='returnigFromOven730'"),EMULATE_DB_CONNECTION).toInt();
	
	int basketOn660=GETONE(QString("select basket from positions where description='afterOven660'"),EMULATE_DB_CONNECTION).toInt();
	int basketOn670=GETONE(QString("select basket from positions where description='afterOven670'"),EMULATE_DB_CONNECTION).toInt();
	int basketOn680=GETONE(QString("select basket from positions where description='afterOven680'"),EMULATE_DB_CONNECTION).toInt();
	int basketOn690=GETONE(QString("select basket from positions where description='afterOven690'"),EMULATE_DB_CONNECTION).toInt();
	if ((basketOn160==0)&&(basketOn720!=0))
	{
			moveBasket("returnigFromOven720","returnigFromOven160",basketOn720);
			conveyorTuxip->writeInteger("N11",22,basketOn720);
			return;
	}
	if ((basketOn720==0)&&(basketOn710!=0))
	{
		moveBasket("returnigFromOven710","returnigFromOven720",basketOn710);
		conveyorTuxip->writeInteger("N11",20,basketOn710);
		return;
	}
	if ((basketOn710==0)&&(basketOn700!=0))
	{
		moveBasket("returnigFromOven700","returnigFromOven710",basketOn700);
		conveyorTuxip->writeInteger("N11",19,basketOn700);
		return;
	}
	if ((basketOn160==0)&&(basketOn720==0)&&(basketOn710==0)&&(basketOn730!=0))
	{
		moveBasket("returnigFromOven730","returnigFromOven160",basketOn730);
		conveyorTuxip->writeInteger("N11",22,basketOn730);
		return;
	}
	//
	if ((basketOn700==0)&&(basketOn670!=0))
	{
		moveBasket("afterOven670","returnigFromOven700",basketOn670);
		conveyorTuxip->writeInteger("N11",17,basketOn670);
		return;
	}
	if ((basketOn700==0)&&(basketOn670==0)&&(basketOn690!=0))
	{
		moveBasket("afterOven690","returnigFromOven700",basketOn690);
		conveyorTuxip->writeInteger("N11",17,basketOn690);
		return;
	}
	if ((basketOn670==0)&&(basketOn660!=0))
	{
		moveBasket("afterOven660","afterOven670",basketOn660);
		conveyorTuxip->writeInteger("N11",16,basketOn660);
		return;
	}
	if ((basketOn690==0)&&(basketOn680!=0))
	{
		moveBasket("afterOven680","afterOven690",basketOn680);
		conveyorTuxip->writeInteger("N11",8,basketOn680);
		return;
	}

}
void conveyorsEmulatorClass::plcDataChanged(QString table,int index,QString value)
{
	LOG("conveyorsEmulatorClass", QString("plcDataChanged: table=%1 index=%2 value=%3").arg(table).arg(index).arg(value));
	if (table == "basketsForOvenReady")
		takeBasketsInToOven();
	else if ((table == "411") && (index == 1))
		execQuery(QString("update positions set basket=%1 where description='afterStacker'").arg(value.toInt()), MAIN_DB_CONNECTION);
	else if ((table == "461") && (index == 1))
		execQuery(QString("update positions set basket=%1 where description='afterStacker2'").arg(value.toInt()), MAIN_DB_CONNECTION);
	else if ((table == "N11") && (index == 22) && (value.toInt() >= 0))
		execQuery(QString("update positions set basket=%1 where description='returnigFromOven160'").arg(value.toInt()), EMULATE_DB_CONNECTION);
	else if ((table=="N11") && (index== 9) && (value.toInt() >= 0))
		execQuery(QString("update positions set basket=%1 where description='beforeOven100'").arg(value.toInt()), EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index==11)&&(value.toInt()>=0))
		execQuery(QString("update positions set basket=%1 where description='beforeOven110'").arg(value.toInt()), EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index== 1)&&(value.toInt()>=0))
		execQuery(QString("update positions set basket=%1 where description='beforeOven120'").arg(value.toInt()), EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index== 3)&&(value.toInt()>=0))
		execQuery(QString("update positions set basket=%1 where description='beforeOven130'").arg(value.toInt()), EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index==10)&&(value.toInt()>=0))
		execQuery(QString("update positions set basket=%1 where description='beforeOven140'").arg(value.toInt()), EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index== 2)&&(value.toInt()>=0))
		execQuery(QString("update positions set basket=%1 where description='beforeOven150'").arg(value.toInt()), EMULATE_DB_CONNECTION);
}
