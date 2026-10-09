#include "emulate.cr.h"
#include "configuration/ConfigurationManager.h"

craneEmulatorClass::craneEmulatorClass(tuxipClass *craneTuxip_):QWidget()
{
	craneTuxip=craneTuxip_;
	float z1=-2.650f;
	float z2=-1.850f;
	float z3=-1.100f;
	float z4=-0.350f;

	beforeStackerBasket=0;
	onStackerBasket=0;
	afterStackerBasket=0;
	basketToAssimakisW=0;
	basketToAssimakis=0;
	basketToOmavW=0;
	basketToOmav=0;
	inOutPosBasket=0;

	positions.append(positionClass(301,0.000f,10.225f,z1));
	positions.append(positionClass(302,0.000f,10.225f,z2));
	positions.append(positionClass(303,0.000f,10.225f,z3));
	positions.append(positionClass(304,0.000f,10.225f,z4));

	positions.append(positionClass(321,0.000f, 8.700f,z1));
	positions.append(positionClass(322,0.000f, 8.700f,z2));
	positions.append(positionClass(323,0.000f, 8.700f,z3));
	positions.append(positionClass(324,0.000f, 8.700f,z4));

	positions.append(positionClass(341,0.000f, 7.175f,z1));
	positions.append(positionClass(342,0.000f, 7.175f,z2));
	positions.append(positionClass(343,0.000f, 7.175f,z3));
	positions.append(positionClass(344,0.000f, 7.175f,z4));

	positions.append(positionClass(361,0.000f, 5.650f,z1));
	positions.append(positionClass(362,0.000f, 5.650f,z2));
	positions.append(positionClass(363,0.000f, 5.650f,z3));
	positions.append(positionClass(364,0.000f, 5.650f,z4));

	positions.append(positionClass(381,0.000f, 4.125f,z1));
	positions.append(positionClass(382,0.000f, 4.125f,z2));
	positions.append(positionClass(383,0.000f, 4.125f,z3));
	positions.append(positionClass(384,0.000f, 4.125f,z4));

	positions.append(positionClass(231,0.000f, 1.650f,z1));
	positions.append(positionClass(232,0.000f, 1.650f,z2));
	positions.append(positionClass(233,0.000f, 1.650f,z3));

	positions.append(positionClass(201,0.000f, 0.000f,z1));
	positions.append(positionClass(202,0.000f, 0.000f,z2));
	positions.append(positionClass(203,0.000f, 0.000f,z3));
	positions.append(positionClass(204,0.000f, 0.000f,z4));
///
	positions.append(positionClass(311,6.350f,10.225f,z1));
	positions.append(positionClass(312,6.350f,10.225f,z2));
	positions.append(positionClass(313,6.350f,10.225f,z3));
	positions.append(positionClass(314,6.350f,10.225f,z4));

	positions.append(positionClass(331,6.350f, 8.700f,z1));
	positions.append(positionClass(332,6.350f, 8.700f,z2));
	positions.append(positionClass(333,6.350f, 8.700f,z3));
	positions.append(positionClass(334,6.350f, 8.700f,z4));

	positions.append(positionClass(351,6.350f, 7.175f,z1));
	positions.append(positionClass(352,6.350f, 7.175f,z2));
	positions.append(positionClass(353,6.350f, 7.175f,z3));
	positions.append(positionClass(354,6.350f, 7.175f,z4));

	positions.append(positionClass(371,6.350f, 5.650f,z1));
	positions.append(positionClass(372,6.350f, 5.650f,z2));
	positions.append(positionClass(373,6.350f, 5.650f,z3));
	positions.append(positionClass(374,6.350f, 5.650f,z4));

	positions.append(positionClass(391,6.350f, 4.125,z1));
	positions.append(positionClass(392,6.350f, 4.125,z2));
	positions.append(positionClass(393,6.350f, 4.125,z3));
	positions.append(positionClass(394,6.350f, 4.125,z4));

	positions.append(positionClass(241,7.900f, 1.650f,z1));
	positions.append(positionClass(242,7.900f, 1.650f,z2));
	positions.append(positionClass(243,7.900f, 1.650f,z3));

	positions.append(positionClass(501,7.900f, 0.000f,z1));

	positions.append(positionClass(401,14.400f, 5.540f,z1));

	positions.append(positionClass(251,18.100f, 1.650f,z1));
	positions.append(positionClass(252,18.100f, 1.650f,z2));
	positions.append(positionClass(253,18.100f, 1.650f,z3));

	positions.append(positionClass(511,18.100f, 0.000f,z1));

	positions.append(positionClass(261,28.960f, 1.650f,z1));
	positions.append(positionClass(262,28.960f, 1.650f,z2));
	positions.append(positionClass(263,28.960f, 1.650f,z3));

	positions.append(positionClass(521,28.960f, 0.000f,z1));

	positions.append(positionClass(271,37.310f, 1.650f,z1));
	positions.append(positionClass(272,37.310f, 1.650f,z2));
	positions.append(positionClass(273,37.310f, 1.650f,z3));

	positions.append(positionClass(211,37.310f, 0.000f,z1));
	positions.append(positionClass(212,37.310f, 0.000f,z2));
	positions.append(positionClass(213,37.310f, 0.000f,z3));
	positions.append(positionClass(214,37.310f, 0.000f,z4));

	positions.append(positionClass(281,46.610f, 1.650f,z1));
	positions.append(positionClass(282,46.610f, 1.650f,z2));
	positions.append(positionClass(283,46.610f, 1.650f,z3));

	positions.append(positionClass(221,46.610f, 0.000f,z1));
	positions.append(positionClass(222,46.610f, 0.000f,z2));
	positions.append(positionClass(223,46.610f, 0.000f,z3));
	positions.append(positionClass(224,46.610f, 0.000f,z4));

	positions.append(positionClass(421,30.950f, 7.185f,z1));

	positions.append(positionClass(101,36.800f, 7.885f,z1));
	positions.append(positionClass(102,36.800f, 7.885f,z2));
	positions.append(positionClass(103,36.800f, 7.885f,z3));
	positions.append(positionClass(104,36.800f, 7.885f,z4));

	positions.append(positionClass(111,44.310f, 7.885f,z1));
	positions.append(positionClass(112,44.310f, 7.885f,z2));
	positions.append(positionClass(113,44.310f, 7.885f,z3));
	positions.append(positionClass(114,44.310f, 7.885f,z4));

	positions.append(positionClass(121,36.800f, 9.545f,z1));
	positions.append(positionClass(122,36.800f, 9.545f,z2));
	positions.append(positionClass(123,36.800f, 9.545f,z3));
	positions.append(positionClass(124,36.800f, 9.545f,z4));

	positions.append(positionClass(131,44.310f, 9.545f,z1));
	positions.append(positionClass(132,44.310f, 9.545f,z2));
	positions.append(positionClass(133,44.310f, 9.545f,z3));
	positions.append(positionClass(134,44.310f, 9.545f,z4));

	positions.append(positionClass(141,41.840f, 7.885f,z1));
	positions.append(positionClass(142,41.840f, 7.885f,z2));
	positions.append(positionClass(143,41.840f, 7.885f,z3));
	positions.append(positionClass(144,41.840f, 7.885f,z4));

	positions.append(positionClass(151,41.840f, 9.545f,z1));
	positions.append(positionClass(152,41.840f, 9.545f,z2));
	positions.append(positionClass(153,41.840f, 9.545f,z3));
	positions.append(positionClass(154,41.840f, 9.545f,z4));

	positions.append(positionClass(411,36.800f, 5.450f,z1));

	positions.append(positionClass(161, 46.625f, 5.450f,z1));
	positions.append(positionClass(162, 46.625f, 5.450f,z2));
	positions.append(positionClass(163, 46.625f, 5.450f,z3));
	positions.append(positionClass(164, 46.625f, 5.450f,z4));

	positions.append(positionClass(801, 50.625f, 5.450f, z1));
	positions.append(positionClass(802, 50.625f, 5.450f, z2));
	positions.append(positionClass(803, 50.625f, 5.450f, z3));
	positions.append(positionClass(804, 50.625f, 5.450f, z4));

	//
	xTarget=0.0f;
	yTarget=0.0f;
	zTarget=0.0f;
	x=0.0f;
	y=0.0f;
	z=0.0f;
	step=0;
	fromPos=toPos=NULL;
	///
	mainTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().EmulationCraneMainMilliseconds());
	QVBoxLayout *mainLayout=new QVBoxLayout();QWidget::setLayout(mainLayout);
	//velocitySpinBox
	QHBoxLayout *velocitySpinBoxLayout=new QHBoxLayout();mainLayout->addLayout(velocitySpinBoxLayout);
	velocitySpinBoxLayout->addWidget(new QLabel(tr("velocity multiplier")));
	velocitySpinBox=new QDoubleSpinBox();
	velocitySpinBox->setMinimum(1.0);
	velocitySpinBox->setMaximum(100.0);
	velocitySpinBox->setValue(100);
	velocityMultiplier=float(velocitySpinBox->value());
	connect(velocitySpinBox,SIGNAL(valueChanged(double)),this,SLOT(onVelocitySpinBoxSlot(double)));
	velocitySpinBoxLayout->addWidget(velocitySpinBox);
	velocitySpinBoxLayout->addStretch(1);
	//fullBasket
	QHBoxLayout *fullBasketLayout=new QHBoxLayout();mainLayout->addLayout(fullBasketLayout);
	fullBasketLayout->addWidget(new QLabel(tr("full basket every:")));
	stackerSpinBox=new QSpinBox();fullBasketLayout->addWidget(stackerSpinBox);
	stackerSpinBox->setMinimum(1);
	stackerSpinBox->setMaximum(1000);
	stackerSpinBox->setValue(5);
	fullBasketLayout->addWidget(new QLabel(tr(" sec")));
	fullBasketLayout->addStretch(1);
	//destacker
	QHBoxLayout *destackerLayout=new QHBoxLayout();mainLayout->addLayout(destackerLayout);
	destackerLayout->addWidget(new QLabel(tr("destacker requests basket every:")));
	destackerSpinBox=new QSpinBox();destackerLayout->addWidget(destackerSpinBox);
	destackerSpinBox->setMinimum(1);
	destackerSpinBox->setMaximum(1000);
	destackerSpinBox->setValue(1000);
	destackerLayout->addWidget(new QLabel(tr(" sec")));
	destackerLayout->addStretch(1);

	connect(stackerSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onStackerSpinBoxSlot(int)));
	stackerTimer.start(stackerSpinBox->value()*1000);

	connect(destackerSpinBox,SIGNAL(valueChanged(int)),this,SLOT(onDestackerSpinBoxSlot(int)));
	destackerTimer.start(destackerSpinBox->value()*1000);
	//
	mainLayout->addStretch(1);
	//
	connect(&animateTimer, SIGNAL(timeout()), this, SLOT(onAnimateTimerSlot()));
	connect(&stackerTimer, SIGNAL(timeout()), this, SLOT(stackerTimerSlot()));
	connect(&destackerTimer, SIGNAL(timeout()), this, SLOT(destackerTimerSlot()));

	connect(&mainTimer, SIGNAL(timeout()), this, SLOT(mainTimerSlot()));
	connect(&stepTimer, SIGNAL(timeout()), this, SLOT(onStepTimerSlot()));
	//////


}
void craneEmulatorClass::tuxIsReady()
{
	beforeStackerBasket = GETONE(QString("select basket from positions where description='beforeStacker'"	),EMULATE_DB_CONNECTION).toInt();
	onStackerBasket = GETONE(QString("select basket from positions where description='onStacker'"	),EMULATE_DB_CONNECTION).toInt();
	afterStackerBasket = GETONE(QString("select basket from positions where description='afterStacker'"	),EMULATE_DB_CONNECTION).toInt();
	// basketToAssimakisW=GETONE(QString("select basket from positions where description='toAsimakisStackerW'"	),EMULATE_DB_CONNECTION).toInt();
	// basketToAssimakis=GETONE(QString("select basket from positions where description='toAsimakisStacker'"		),EMULATE_DB_CONNECTION).toInt();
	// basketToOmavW=GETONE(QString("select basket from positions where description='toOmavStackerW'"				),EMULATE_DB_CONNECTION).toInt();
	// basketToOmav=GETONE(QString("select basket from positions where description='toOmavStacker'"				),EMULATE_DB_CONNECTION).toInt();
	// inOutPosBasket=GETONE(QString("select basket from positions where description='inOut'"			),EMULATE_DB_CONNECTION).toInt();
	 basketToAssimakisW = 0;
	 basketToAssimakis = 0;
	 basketToOmavW = 0;
	 basketToOmav = 0;
	 inOutPosBasket = 0;

	QVector<bool> bit_20_13_Array;  bit_20_13_Array.resize(16);
	bit_20_13_Array[4] = beforeStackerBasket==0?true:false;
	bit_20_13_Array[6] = afterStackerBasket!=0?true:false;
	// bit_20_13_Array[10]=basketToAssimakisW==0?true:false;
	// bit_20_13_Array[12]=basketToOmavW==0?true:false;
	// bit_20_13_Array[14]=inOutPosBasket==0?true:false;
	// bit_20_13_Array[15]=inOutPosBasket!=0?true:false;
	bit_20_13_Array[10] = false;
	bit_20_13_Array[12] = false;
	bit_20_13_Array[14] = false;
	bit_20_13_Array[15] = false;
	bit_20_13_Array[7] = true;//positions before oven unlocked
	bit_20_13_Array[8] = true;//returning from oven position unlocked
	int word_20_13 = deprecatedCalculateInt(bit_20_13_Array);
	craneTuxip->writeInteger("N20", 13, word_20_13);

	QVector<bool> bit_20_15_Array;bit_20_15_Array.resize(16);//auto mode
	bit_20_15_Array.resize(16);bit_20_15_Array[11]=true;
	int word_20_15=deprecatedCalculateInt(bit_20_15_Array);
	craneTuxip->writeInteger("N20",15,word_20_15);
	craneTuxip->writeInteger("N20",12,0);//plc status
}
void craneEmulatorClass::mainTimerSlot()
{
}

void craneEmulatorClass::destackerTimerSlot()
{
	QVector<bool> bitArray;bitArray.resize(16);

	beforeStackerBasket=GETONE(QString("select basket from positions where description='beforeStacker'"	),EMULATE_DB_CONNECTION).toInt();
	afterStackerBasket=GETONE(QString("select basket from positions where description='afterStacker'"	),EMULATE_DB_CONNECTION).toInt();
	bitArray[4] = beforeStackerBasket == 0 ? true : false;
	bitArray[6] = afterStackerBasket != 0 ? true : false;

	//basketToAssimakisW=GETONE(QString("select basket from positions where description='toAsimakisStackerW'	"),EMULATE_DB_CONNECTION).toInt();
	//basketToAssimakis=GETONE(QString("select basket from positions where description='toAsimakisStacker'	"),EMULATE_DB_CONNECTION).toInt();
	//basketToOmavW=GETONE(QString("select basket from positions where description='toOmavStackerW'			"),EMULATE_DB_CONNECTION).toInt();
	//basketToOmav=GETONE(QString("select basket from positions where description='toOmavStacker'				"),EMULATE_DB_CONNECTION).toInt();
	//inOutPosBasket=GETONE(QString("select basket from positions where description='inOut'			"),EMULATE_DB_CONNECTION).toInt();
	basketToAssimakisW = 0;
	basketToAssimakis = 0;
	basketToOmavW = 0;
	basketToOmav = 0;
	inOutPosBasket = 0;

	//bitArray[10] = basketToAssimakisW == 0 ? true : false;
	//bitArray[12] = basketToOmavW == 0 ? true : false;
	//bitArray[14] = inOutPosBasket == 0 ? true : false;
	//bitArray[15] = inOutPosBasket != 0 ? true : false;
	bitArray[10] = false;
	bitArray[12] = false;
	bitArray[14] = false;
	bitArray[15] = false;
	bool bitChanged = false;

	if ((basketToAssimakisW>0)&&(basketToAssimakis==0))
	{
		bitArray[10]=true;bitChanged=true;
		moveBasket("toAsimakisStackerW","toAsimakisStacker",basketToAssimakisW);
		basketToAssimakis=basketToAssimakisW;
	}

	if ((basketToOmavW>0)&&(basketToOmav==0))
	{
		bitArray[12]=true;bitChanged=true;
		moveBasket("toOmavStackerW","toOmavStacker",basketToOmavW);
		basketToOmav=basketToOmavW;
	}

	if (inOutPosBasket==0)
	{
		double random=static_cast<double>(rand())/static_cast<double>(RAND_MAX);
		if (random<0.5)
		{
			if (basketToAssimakis>0) //first basketToAssimakis
			{
				moveBasket("toAsimakisStacker","inOut",basketToAssimakis);
				craneTuxip->writeInteger("N20",17,basketToAssimakis);
				bitArray[15]=true;bitChanged=true;
			}
			else if (basketToOmav>0)
			{
				moveBasket("toOmavStacker","inOut",basketToOmav);
				craneTuxip->writeInteger("N20",17,basketToOmav);
				bitArray[15]=true;bitChanged=true;
			}
		}
		else
		{
			if (basketToOmav>0) //first basketToOmav
			{
				moveBasket("toOmavStacker","inOut",basketToOmav);
				craneTuxip->writeInteger("N20",17,basketToOmav);
				bitArray[15]=true;bitChanged=true;
			}
			else if (basketToAssimakis>0)
			{
				moveBasket("toAsimakisStacker","inOut",basketToAssimakis);
				bitArray[15]=true;bitChanged=true;
				craneTuxip->writeInteger("N20",17,basketToAssimakis);
			}
		}
	}
	inOutPosBasket=GETONE(QString("select basket from positions where description='inOut'"),EMULATE_DB_CONNECTION).toInt();
	bitArray[14]=inOutPosBasket==0?true:false;
	if (bitChanged)
	{
		bitArray[7]=true;//positions before oven unlocked
		bitArray[8]=true;//returning from oven position unlocked
		int word=deprecatedCalculateInt(bitArray);
		craneTuxip->writeInteger("N20",13,word);
	}
}
void craneEmulatorClass::onVelocitySpinBoxSlot(double v)
{
 velocityMultiplier=float(v);
}
void craneEmulatorClass::onStackerSpinBoxSlot(int t)
{
	stackerTimer.setInterval(t*1000);
}
void craneEmulatorClass::onDestackerSpinBoxSlot(int t)
{
	destackerTimer.setInterval(t*1000);
}

void craneEmulatorClass::stackerTimerSlot()
{
	beforeStackerBasket=GETONE(QString("select basket from positions where description='beforeStacker'"	),EMULATE_DB_CONNECTION).toInt();
	onStackerBasket=GETONE(QString("select basket from positions where description='onStacker'"	),EMULATE_DB_CONNECTION).toInt();
	afterStackerBasket=GETONE(QString("select basket from positions where description='afterStacker'"	),EMULATE_DB_CONNECTION).toInt();
	//basketToAssimakisW=GETONE(QString("select basket from positions where description='toAsimakisStackerW'"	),EMULATE_DB_CONNECTION).toInt();
	//basketToAssimakis=GETONE(QString("select basket from positions where description='toAsimakisStacker'"		),EMULATE_DB_CONNECTION).toInt();
	//basketToOmavW=GETONE(QString("select basket from positions where description='toOmavStackerW'"				),EMULATE_DB_CONNECTION).toInt();
	//basketToOmav=GETONE(QString("select basket from positions where description='toOmavStacker'"				),EMULATE_DB_CONNECTION).toInt();
	//inOutPosBasket=GETONE(QString("select basket from positions where description='inOut'"			),EMULATE_DB_CONNECTION).toInt();
	basketToAssimakisW = 0;
	basketToAssimakis = 0;
	basketToOmavW = 0;
	basketToOmav = 0;
	inOutPosBasket = 0;

	bool smthingChanged=false;

	if ((afterStackerBasket == 0) && (onStackerBasket > 0))
	{
		moveBasket("onStacker","afterStacker",onStackerBasket);
		smthingChanged=true;
		afterStackerBasket=onStackerBasket;
		onStackerBasket=0;
	}
	else if ((onStackerBasket == 0) && (beforeStackerBasket > 0))
	{
		moveBasket("beforeStacker","onStacker",beforeStackerBasket);
		smthingChanged=true;
		onStackerBasket=beforeStackerBasket;
		beforeStackerBasket=0;
	}

	if (smthingChanged)
	{
		QVector<bool> bitArray;bitArray.resize(16);
		bitArray[4] = beforeStackerBasket == 0 ? true : false;
		bitArray[6] = afterStackerBasket != 0 ? true : false;
		bitArray[10] = basketToAssimakisW == 0 ? true : false;
		bitArray[12] = basketToOmavW == 0 ? true : false;
		bitArray[14] = inOutPosBasket == 0 ? true : false;
		bitArray[15] = inOutPosBasket != 0 ? true : false;
		bitArray[7] = true;//positions before oven unlocked
		bitArray[8] = true;//returning from oven position unlocked
		int word = deprecatedCalculateInt(bitArray);
		craneTuxip->writeInteger("N20", 13, word);
	}

	//if ((afterStackerBasket>0)&&(onStackerBasket==0)&&(beforeStackerBasket==0))
	//	stackerTimer.stop();
	//if ((afterStackerBasket>0)&&(onStackerBasket>0)&&(beforeStackerBasket==0))
	//	stackerTimer.stop();
	//if ((afterStackerBasket>0)&&(onStackerBasket>0)&&(beforeStackerBasket>0))
	//	stackerTimer.stop();
}
float craneEmulatorClass::stepToTarget(float pos,float target,float step)
{
	if (fabs(pos-target)>FLT_EPSILON)
	{
		if (pos<target)
			return ((pos+step)>target)?target:(pos+step);
		else
			return ((pos-step)<target)?target:(pos-step);
	}
	else
		return target;
}

void craneEmulatorClass::plcDataChanged(QString table,int index,QString value)
{
	if (value.toInt()==-1)
		return;
	if ((table=="N20")&&(index==0))
	{
		setFromPos(value.toInt());
	}
	else if ((table=="N20")&&(index==1))
	{
		setToPos(value.toInt());
	}
	else if ((table=="N20")&&(index==2))
	{
		setPcStatus(value.toInt());
	}
	else if ((table=="401")&&(index==0))
		execQuery(QString("update positions set basket=%1 where description='beforeStacker'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	else if ((table=="411")&&(index==0))
		execQuery(QString("update positions set basket=%1 where description='afterStacker'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	else if ((table=="N20")&&(index==20))
		execQuery(QString("update positions set basket=%1 where description='inOut'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	else if ((table=="N20")&&(index==18))
		execQuery(QString("update positions set basket=%1 where description='toAsimakisStackerW'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	else if ((table=="N20")&&(index==19))
		execQuery(QString("update positions set basket=%1 where description='toOmavStackerW'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index== 9))execQuery(QString("update positions set basket=%1 where description='beforeOven100'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index==11))execQuery(QString("update positions set basket=%1 where description='beforeOven110'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index== 1))execQuery(QString("update positions set basket=%1 where description='beforeOven120'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index== 3))execQuery(QString("update positions set basket=%1 where description='beforeOven130'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index==10))execQuery(QString("update positions set basket=%1 where description='beforeOven140'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	else if ((table=="N11")&&(index== 2))execQuery(QString("update positions set basket=%1 where description='beforeOven150'").arg(value.toInt()),EMULATE_DB_CONNECTION);
	//stackerTimer.start();
}
void craneEmulatorClass::onAnimateTimerSlot()
{
	if (step==START)
	{
		step=FROM_GO_UP;
	}
	if (step==FROM_GO_UP)
	{
		xTarget=x;
		yTarget=y;
		zTarget=0.0;
	}
	if (step==FROM_GO_TO_Y_CLEAR_TO_MOVE)
	{
		step++;//is empty
		//if (fabs(x-fromPos->x)>FLT_EPSILON)
		//{
		//	xTarget=x;
		//	yTarget=1.650;
		//	zTarget=z;
		//}
	}
	if (step==FROM_GO_FROM_POS_X)
	{
		xTarget=fromPos->x;
		yTarget=y;
		zTarget=z;
	}
	if (step==FROM_GO_FROM_POS_Y)
	{
		xTarget=x;
		yTarget=fromPos->y;
		zTarget=z;
	}
	if (step==FROM_GO_FROM_POS_Z)
	{
		xTarget=x;
		yTarget=y;
		zTarget=fromPos->z;
	}
	if (step==FROM_GO_FROM_POS_Z_UP)
	{
		//hasCassette
		sendBit(craneTuxip,"N20",15,14,true);
		xTarget=x;
		yTarget=y;
		zTarget=0.0;
	}
	if (step==TO_GO_TO_X_CLEAR_TO_MOVE)
	{
		if (fromOvenEntry)
		{
			xTarget=36.800f;
			yTarget=y;
			zTarget=z;
		}
		else
			step++;
	}
	if (step==TO_GO_TO_Y_CLEAR_TO_MOVE)
	{
		if ( (fromOvenEntry||fromStackerEntry)&&(toOvenEntry||toStackerEntry) )
		{
			step=TO_GO_TO_POS_Y;
		}
		else if (fabs(x-toPos->x)>FLT_EPSILON)
		{
			xTarget=x;
			yTarget=1.650f;
			zTarget=z;
		}
	}
	if (step==TO_GO_TO_POS_X)
	{
		if (toOvenEntry)
		{
			xTarget=36.800f;
			yTarget=y;
			zTarget=z;
		}
		else
		{
			xTarget=toPos->x;
			yTarget=y;
			zTarget=z;
		}
	}
	if (step==TO_GO_TO_POS_Y)
	{
		xTarget=x;
		yTarget=toPos->y;
		zTarget=z;
	}
	if (step==TO_GO_TO_FINAL_X)
	{
			xTarget=toPos->x;
			yTarget=y;
			zTarget=z;
	}
	if (step==TO_GO_TO_POS_Z)
	{
		xTarget=x;
		yTarget=y;
		zTarget=toPos->z;
	}
	if (step==TO_GO_TO_POS_Z_UP)
	{
		//no Cassette
		sendBit(craneTuxip,"N20",15,14,false);
		craneTuxip->writeInteger("N20",12,2);
		//no Cassette
		xTarget=x;
		yTarget=y;
		zTarget=0.0;
	}
	float moveStep=velocityMultiplier*0.0005*float(time.elapsed());//2m/sec=0.002m/msec
	time.restart();
	float newX=stepToTarget(x,xTarget,moveStep);
	float newY=stepToTarget(y,yTarget,moveStep);
	float newZ=stepToTarget(z,zTarget,0.75*moveStep);
	bool movingZ=false;
	if (fabs(newX-x)>FLT_EPSILON)
	{
		x=newX;
		craneTuxip->writeFloat("F21",10,46698-x*1000);
	}
	if (fabs(newY-y)>FLT_EPSILON)
	{
		y=newY;
		craneTuxip->writeFloat("F21",11,y*1000);
	}
	if (fabs(newZ-z)>FLT_EPSILON)
	{
		z=newZ;
		craneTuxip->writeFloat("F21",9,-z*1000);
		movingZ=true;
	}
	//
	if ( (fabs(xTarget-x)<1.0e-6f)&&(fabs(yTarget-y)<1.0e-6f)&&(fabs(zTarget-z)<1.0e-6f) )
	{
		if (!stepTimer.isActive())
		{
			if (movingZ)
				stepTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().EmulationMovingZStepMilliseconds());
			else
				stepTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().EmulationStepMilliseconds());
		}
	}
	if (step==END)
	{
		step=0;
		myStatus=0;
		craneTuxip->writeInteger("N20",12,0);
		animateTimer.stop();
	}
}
void craneEmulatorClass::onStepTimerSlot()
{
	step++;
	stepTimer.stop();
}
void craneEmulatorClass::setPcStatus(int st)
{
	pcStatus=st;
	if ((pcStatus==1)&&(fromPos)&&(toPos))
	{
		myStatus=1;
		craneTuxip->writeInteger("N20",12,1);
		step=START;
		onAnimateTimerSlot();
		animateTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().EmulationAnimationMilliseconds());
	}
	if ((pcStatus==2)&&(fromPos)&&(toPos))
	{
		fromPos=toPos=NULL;
		myStatus=0;
		pcStatus=0;
	}
}
void craneEmulatorClass::setFromPos(int p)
{
	fromPos=positions.findPosWithId(p);
	fromOvenEntry=((p>=100)&&(p<=154))?true:false;
	fromStackerEntry=(p==411)?true:false;
	if (!fromPos)
		double lala=0.;
}
void craneEmulatorClass::setToPos(int p)
{
	toPos=positions.findPosWithId(p);
	toOvenEntry=((p>=100)&&(p<=154))?true:false;
	toStackerEntry=(p==411)?true:false;
	if (!toPos)
		double lala=0.;
}
