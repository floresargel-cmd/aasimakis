#include "pch.h"
#include "allPos.h"
QGraphicsScene *posClass::scene=NULL;
QTransform posClass::txtTransformation=QTransform();
allPossClass::allPossClass(tuxipClass *sbTuxip_,tuxipClass *autoTuxip_,int showType_,QGraphicsScene *layoutScene,QWidget *p):QWidget(p)
{
	manualSelectedFromPos=NULL;
	sbTuxip=sbTuxip_;
	autoTuxip=autoTuxip_;
	noOfRows=9;
	setVisible(false);
	showType=showType_;
	gItemClass *mouseOverItem=new gItemClass(":dxfs/posMouseOver.dxf",QPen(QColor(255,100,100)),this,QBrush(QColor(225,225,255,150)),Qt::NoBrush);
	gItemClass *cellItem=new gItemClass(":dxfs/posCell.dxf",QPen(QColor(200,200,250)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *cassetteItem=new gItemClass(":dxfs/posCassette.dxf",QPen(QColor(100,200,100)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *sbBlueItem=new gItemClass(":dxfs/posSbBlue.dxf",QPen(QColor(100,100,200)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *profilesItem=new gItemClass(":dxfs/posProfiles.dxf",QPen(QColor(100,100,200)),this,Qt::NoBrush,QBrush(QColor(200,200,255)));
	gItemClass *lockedGraphicsItem=new gItemClass(":dxfs/posLocked.dxf",QPen(QColor(255,100,100)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *activeFromItem=new gItemClass(":dxfs/posFrom.dxf",QPen(QColor(200,100,100)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *activeToItem=new gItemClass(":dxfs/posTo.dxf",QPen(QColor(200,100,100)),this,Qt::NoBrush,Qt::NoBrush);

	gItemClass *sbMouseOverItem=new gItemClass(":dxfs/posSbMouseOver.dxf",QPen(QColor(255,100,100)),this,QBrush(QColor(225,225,255,150)),Qt::NoBrush);
	gItemClass *convItem=new gItemClass(":dxfs/posConveyor.dxf",QPen(QColor(250,200,200)),this,Qt::NoBrush,Qt::NoBrush);
	gItemClass *convSbItem=new gItemClass(":dxfs/posSbConveyor.dxf",QPen(QColor(250,200,200)),this,Qt::NoBrush,Qt::NoBrush);
	crane=NULL;
	posClass::scene=layoutScene;
	posClass::txtTransformation.scale(posClass::textScale,-posClass::textScale);

	splash->showMessage(tr("Loading cell data..."),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select posId,description,cassette,sbNumber,pos.ticket,colNo,rowNo,dx,dy,dz,locked,ttt.weight from pos left join (select max(tId) as tId,ticket,weight from tickets group by ticket) as ttt on pos.ticket=ttt.ticket"),mDb);//
	bool calcCorrections=false;
	float sbYStep=482.f;
	for (int i=0;i<dataVV.count();i++)
	{
		if ((i%10)==0)
			splash->showMessage(tr("Loading cell data (%1%)").arg(100*i/dataVV.count()),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
		int posId=dataVV[i][0].toInt();
		QString description=dataVV[i][1].toString().trimmed();
		int cassetteNum=dataVV[i][2].toInt();
		int sbNumber=dataVV[i][3].toInt();
		int ticket=dataVV[i][4].toInt();
		int col=dataVV[i][5].toInt();
		int row=dataVV[i][6].toInt();
		float dx=dataVV[i][7].toFloat();
		float dy=dataVV[i][8].toFloat();
		float dz=dataVV[i][9].toFloat();
		bool locked=dataVV[i][10].toBool();
		float ticketWeight=dataVV[i][11].toFloat();
		//if ( (posId==craneId) )
		//	continue;
		if (!calcCorrections)
		{
			if ((posId==-379)||(posId==-388)||(posId==-397)||(posId==-478)||(posId==-487))
				continue;
		}
		float plcXPos=1035.f+(col-1)*cellXStep;
		float plcYPos=0.f;
		float plcZPos=cellZStep*(row-1);		
		if (showType&showCranePos)
		{
			if (posId<=craneId)
			{
				allPosV<<new posClass(posId,false,cassetteNum,description,col,row,plcXPos,plcYPos,plcZPos,dx,dy,dz,locked,sbNumber,ticket,ticketWeight,mouseOverItem,cellItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this);
				connect(allPosV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
			}
		}
		if ((showType&showCranePos)||(showType&showPincer1Pos))
		{
			if (posId==pincer1Id)
			{
				allPosV<<(pincer1=new posClass(pincer1Id,true,cassetteNum,description,col,row,-500.f,0.f,12000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				connect(allPosV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			//exit to dstacker
			if (posId==conveyor__1_id)
			{
				allPosV<<(craneFrontConveyor1=new posClass(conveyor__1_id,false,cassetteNum,description,col,row,0.f,0.f,layoutYPlusOffset+2000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				connect(allPosV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==conveyor__2_id)
			{
				allPosV<<(craneFrontConveyor2=new posClass(conveyor__2_id,false,cassetteNum,description,col,row,-1350.f,0.f,layoutYPlusOffset+2000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==conveyor__4_id)
			{
				allPosV<<(craneFrontConveyor3=new posClass(conveyor__4_id,false,cassetteNum,description,col,row,-1350.f,0.f,0.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==conveyor__5_id)
			{
				allPosV<<(craneFrontConveyor3=new posClass(conveyor__5_id,false,cassetteNum,description,col,row,-1350.f,0.f,layoutYMinusOffset+2000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==conveyor__6_id)
			{
				allPosV<<(craneFrontConveyor3=new posClass(conveyor__6_id,false,cassetteNum,description,col,row,0.f,0.f,layoutYMinusOffset+2000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				connect(allPosV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}

			//pincer1 sb positions
			if (posId==pincer1_41_id)
			{
				allPosV<<(oldPincerBlueBuffer_1_1=new posClass(posId,true,cassetteNum,description,col,row,-3000.f,0.f,layoutYPlusOffset+2000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			//if (posId==pincer1_42_id)
			//{
			//	allPosV<<(oldPincerBlueBuffer_1_2=new posClass(posId,true,cassetteNum,description,col,row,-2150.f,0.f,layoutYPlusOffset+2000.f+sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
			//	allPosV.last()->setPlcAdress(autoTuxip);
			//	plcPosV<<allPosV.last();
			//}
			//if (posId==pincer1_43_id)
			//{
			//	allPosV<<(oldPincerBlueBuffer_1_3=new posClass(posId,true,cassetteNum,description,col,row,-2150.f,0.f,layoutYPlusOffset+2000.f+2.f*sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
			//	allPosV.last()->setPlcAdress(autoTuxip);
			//	plcPosV<<allPosV.last();
			//}
			//if (posId==pincer1_44_id)
			//{
			//	allPosV<<(oldPincerBlueBuffer_1_4=new posClass(posId,true,cassetteNum,description,col,row,-2150.f,0.f,layoutYPlusOffset+2000.f+3.f*sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
			//	allPosV.last()->setPlcAdress(autoTuxip);
			//	plcPosV<<allPosV.last();
			//}

			if (posId==pincer1_45_id)
			{
				allPosV<<(oldPincerBlueBuffer_2_1=new posClass(posId,true,cassetteNum,description,col,row,-4950.f,0.f,layoutYPlusOffset+2000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			//if (posId==pincer1_46_id)
			//{
			//	allPosV<<(oldPincerBlueBuffer_2_2=new posClass(posId,true,cassetteNum,description,col,row,-3000.f,0.f,layoutYPlusOffset+2000.f+sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
			//	allPosV.last()->setPlcAdress(autoTuxip);
			//	plcPosV<<allPosV.last();
			//}
			//if (posId==pincer1_47_id)
			//{
			//	allPosV<<(oldPincerBlueBuffer_2_3=new posClass(posId,true,cassetteNum,description,col,row,-3000.f,0.f,layoutYPlusOffset+2000.f+2.f*sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
			//	allPosV.last()->setPlcAdress(autoTuxip);
			//	plcPosV<<allPosV.last();
			//}
			//if (posId==pincer1_48_id)
			//{
			//	allPosV<<(oldPincerBlueBuffer_2_4=new posClass(posId,true,cassetteNum,description,col,row,-3000.f,0.f,layoutYPlusOffset+2000.f+3.f*sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
			//	allPosV.last()->setPlcAdress(autoTuxip);
			//	plcPosV<<allPosV.last();
			//}

			if (posId==pincer1_49_id)
			{
				allPosV<<(oldPincerBeforeDestucker=new posClass(posId,true,cassetteNum,description,col,row,-3950.f,0.f,layoutYPlusOffset+2000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer1_50_id)
			{
				allPosV<<(oldPincerDestucker=new posClass(posId,true,cassetteNum,description,col,row,-3850.f,0.f,layoutYPlusOffset-1500.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer1_51_id)
			{
				allPosV<<(oldPincerScrap=new posClass(posId,true,cassetteNum,description,col,row,-3000.f,0.f,layoutYPlusOffset-1500.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer1_52_id)
			{
				allPosV<<(oldPincerInOut=new posClass(posId,true,cassetteNum,description,col,row,-3850.f,0.f,layoutYMinusOffset+3000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer1_53_id)
			{
				allPosV<<(oldPincerSideMoving=new posClass(posId,true,cassetteNum,description,col,row,-3850.f,0.f,layoutYMinusOffset-3000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer1_54_id)
			{
				allPosV<<(oldPincerIn=new posClass(posId,true,cassetteNum,description,col,row,-3850.f,0.f,layoutYMinusOffset-9000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(autoTuxip);
				plcPosV<<allPosV.last();
			}
		}
		if ((showType&showCranePos)||(showType&showPincer2Pos))
		{
			if (posId==pincer2Id)
			{
				allPosV<<(pincer2=new posClass(pincer2Id,true,cassetteNum,description,col,row,48000.f,0.f,layoutYMinusOffset-3000.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				connect(allPosV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			//triple
			if (posId==cassetteTripleExit__8_id)
			{
				allPosV<<(tripleExit1=new posClass(cassetteTripleExit__8_id,false,cassetteNum,description,col,row,plcXPos,0.f,layoutYMinusOffset,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				connect(allPosV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==cassetteTripleExit__9_id)
			{
				allPosV<<(tripleExit1Out=new posClass(cassetteTripleExit__9_id,false,cassetteNum,description,col,row,plcXPos,0.f,layoutYMinusOffset-5500.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==cassetteTripleExit_10_id)
			{
				allPosV<<(tripleImportOut=new posClass(cassetteTripleExit_10_id,false,cassetteNum,description,col,row,plcXPos,0.f,layoutYMinusOffset-5500.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==cassetteTripleExit_11_id)
			{
				allPosV<<(tripleImport=new posClass(cassetteTripleExit_11_id,false,cassetteNum,description,col,row,plcXPos,0.f,layoutYMinusOffset,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				connect(allPosV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==cassetteTripleExit_12_id)
			{
				allPosV<<(tripleExit2=new posClass(cassetteTripleExit_12_id,false,cassetteNum,description,col,row,plcXPos,0.f,layoutYMinusOffset,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				connect(allPosV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==cassetteTripleExit_13_id)
			{
				allPosV<<(tripleExit2Out=new posClass(cassetteTripleExit_13_id,false,cassetteNum,description,col,row,plcXPos,0.f,layoutYMinusOffset-5500.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			//double
			if (posId==cassetteDoubleExit_15_id)
			{
				allPosV<<(doubleExit=new posClass(cassetteDoubleExit_15_id,false,cassetteNum,description,col,row,plcXPos,0.f,layoutYMinusOffset,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				connect(allPosV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==cassetteDoubleExit_16_id)
			{
				allPosV<<(doubleImport=new posClass(cassetteDoubleExit_16_id,false,cassetteNum,description,col,row,plcXPos,0.f,layoutYMinusOffset,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				connect(allPosV.last(),SIGNAL(posCtrlMouseReleasedSignal(posClass*,QGraphicsSceneMouseEvent*)),this,SLOT(posCtrlMouseReleasedSlot(posClass*,QGraphicsSceneMouseEvent*)));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==cassetteDoubleExit_18_id)
			{
				allPosV<<(doubleExitOut=new posClass(cassetteDoubleExit_18_id,false,cassetteNum,description,col,row,plcXPos,0.f,layoutYMinusOffset-5500.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==cassetteDoubleExit_17_id)
			{
				allPosV<<(doubleImportOut=new posClass(cassetteDoubleExit_17_id,false,cassetteNum,description,col,row,plcXPos,0.f,layoutYMinusOffset-5500.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,mouseOverItem,convItem,cassetteItem,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			//new crane
			if (posId==pincer2_22_id)
			{
				allPosV<<(newPincerImportBuffer1=new posClass(posId,true,cassetteNum,description,col,row,47000.f,0.f,layoutYMinusOffset-5500.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer2_27_id)
			{
				allPosV<<(newPincerImportBuffer2=new posClass(posId,true,cassetteNum,description,col,row,47000.f,0.f,layoutYMinusOffset-5500.f+sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer2_28_id)
			{
				allPosV<<(newPincerImportBuffer3=new posClass(posId,true,cassetteNum,description,col,row,47000.f,0.f,layoutYMinusOffset-5500.f+2.*sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer2_29_id)
			{
				allPosV<<(newPincerImportBuffer4=new posClass(posId,true,cassetteNum,description,col,row,47000.f,0.f,layoutYMinusOffset-5500.f+3.*sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}

			if (posId==pincer2_21_id)
			{
				allPosV<<(newPincerUserImportBuffer1=new posClass(posId,true,cassetteNum,description,col,row,47000.f,0.f,-24400.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer2_24_id)
			{
				allPosV<<(newPincerUserImportBuffer2=new posClass(posId,true,cassetteNum,description,col,row,47000.f,0.f,-24400.f+sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer2_25_id)
			{
				allPosV<<(newPincerUserImportBuffer3=new posClass(posId,true,cassetteNum,description,col,row,47000.f,0.f,-24400.f+2.*sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer2_26_id)
			{
				allPosV<<(newPincerUserImportBuffer4=new posClass(posId,true,cassetteNum,description,col,row,47000.f,0.f,-24400.f+3.*sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}

			if (posId==pincer2_20_id)
			{
				allPosV<<(newPincerUnloadBuffer1=new posClass(posId,true,cassetteNum,description,col,row,43350.f,0.f,layoutYMinusOffset-5500.f,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,convSbItem,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer2_31_id)
			{
				allPosV<<(newPincerUnloadBuffer2=new posClass(posId,true,cassetteNum,description,col,row,43350.f,0.f,layoutYMinusOffset-5500.f+sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer2_32_id)
			{
				allPosV<<(newPincerUnloadBuffer3=new posClass(posId,true,cassetteNum,description,col,row,43350.f,0.f,layoutYMinusOffset-5500.f+2.*sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
			if (posId==pincer2_33_id)
			{
				allPosV<<(newPincerUnloadBuffer4=new posClass(posId,true,cassetteNum,description,col,row,43350.f,0.f,layoutYMinusOffset-5500.f+3.*sbYStep,0.f,0.f,0.f,locked,sbNumber,ticket,ticketWeight,sbMouseOverItem,NULL,NULL,sbBlueItem,profilesItem,lockedGraphicsItem,activeFromItem,activeToItem,true,this));
				allPosV.last()->setPlcAdress(sbTuxip);
				plcPosV<<allPosV.last();
			}
		}
	}
	if (calcCorrections)
		calculateCorrections();
#ifdef eastStation
	QTimer *readFromDbTimer=new QTimer(this);
	connect(readFromDbTimer,SIGNAL(timeout()),this,SLOT(readFromDbTimerTimeoutSlot()));
	readFromDbTimer->start(2000);
#endif
}
allPossClass::~allPossClass()
{
}
void allPossClass::calculateCorrections()
{
	QVector<posCorrectionClass*> corV;
	/////////////////////////////////////////////////////down
	//-
	corV<<new posCorrectionClass(-55,	 1,			  75.f,    +0.f,  +0.0f);
	corV<<new posCorrectionClass( -1,	 1,				0.f,    +0.f,  +0.0f);
	corV<<new posCorrectionClass(-55,	 9,			  65.f,    +0.f,  +0.0f);
	corV<<new posCorrectionClass( -1,	 9,				13.f,    +0.f,  +0.0f);

	corV<<new posCorrectionClass( 1,	 1,			   0.f,    +0.f,  +0.0f);
	corV<<new posCorrectionClass(10,	 1,			 12.5f,    +0.f,  +0.0f);
	corV<<new posCorrectionClass(15,	 1,			 19.5f,    +0.f,  +5.0f);
	corV<<new posCorrectionClass(30,	 1,			 40.3f,    +0.f,  +5.0f);
	corV<<new posCorrectionClass(40,	 1,			 54.1f,    +0.f,  +3.0f);
	corV<<new posCorrectionClass(44,	 1,			 59.7f,    +0.f,  +8.0f);
	corV<<new posCorrectionClass(48,	 1,			 65.3f,    +0.f, +12.0f);
	corV<<new posCorrectionClass(55,	 1,				75.f,    +0.f,  +0.0f);

	corV<<new posCorrectionClass( 1,	 9,			-10.0f,    +0.f, +0.0f);
	corV<<new posCorrectionClass(10,	 9,			  4.1f,    +0.f, +0.0f);
	corV<<new posCorrectionClass(15,	 9,			 12.0f,    +0.f, +5.0f);
	corV<<new posCorrectionClass(30,	 9,			 35.6f,    +0.f, +5.0f);
	corV<<new posCorrectionClass(40,	 9,			 51.4f,    +0.f, +3.0f);
	corV<<new posCorrectionClass(44,	 9,			 57.7f,    +0.f, +8.0f);
	corV<<new posCorrectionClass(48,	 9,			 64.0f,    +0.f,+12.0f);
	corV<<new posCorrectionClass(55,	 9,				75.f,    +0.f, +0.0f);

	//fill bottom and top rows
	for (int i=0;i<allPosV.count();i++)
	{
		splash->showMessage(tr("Corrections pass 1 (%1)").arg(100*i/allPosV.count()),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
		if (allPosV[i]->getPosId()>=craneId)
			continue;
		if (allPosV[i]->getRow()==1)
		{
			posCorrectionClass *c=NULL;
			posCorrectionClass *c0=NULL;
			posCorrectionClass *c1=NULL;
			for (int j=0;j<corV.count();j++)
			{
				if (corV[j]->row==1)
				{
					if (corV[j]->col<(allPosV[i]->getColumnWithSign()))
						c0=corV[j];
					if (corV[j]->col>allPosV[i]->getColumnWithSign())
					{
						c1=corV[j];
						if (c0)
							break;
					}
					if (corV[j]->col==allPosV[i]->getColumnWithSign())
						c=corV[j];
				}
			}
			if (c)
			{
				allPosV[i]->setPlcXCorrection(c->x);
				allPosV[i]->setPlcYCorrection(c->y);
				allPosV[i]->setPlcZCorrection(c->z);
				allPosV[i]->updateGui();
			}
			else if ((c0!=NULL)&&(c1!=NULL))
			{
				allPosV[i]->setPlcXCorrection(c0->x+(allPosV[i]->getColumnWithSign()-c0->col)*(c1->x-c0->x)/(c1->col-c0->col));
				allPosV[i]->setPlcYCorrection(c0->y+(allPosV[i]->getColumnWithSign()-c0->col)*(c1->y-c0->y)/(c1->col-c0->col));
				allPosV[i]->setPlcZCorrection(c0->z+(allPosV[i]->getColumnWithSign()-c0->col)*(c1->z-c0->z)/(c1->col-c0->col));
				allPosV[i]->updateGui();
			}
		}
		if (allPosV[i]->getRow()==noOfRows)
		{
			posCorrectionClass *c=NULL;
			posCorrectionClass *c0=NULL;
			posCorrectionClass *c1=NULL;
			for (int j=0;j<corV.count();j++)
			{
				if (corV[j]->row==noOfRows)
				{
					if (corV[j]->col<allPosV[i]->getColumnWithSign())
						c0=corV[j];
					if (corV[j]->col>allPosV[i]->getColumnWithSign())
					{
						c1=corV[j];
						if (c0)
							break;
					}
					if (corV[j]->col==allPosV[i]->getColumnWithSign())
						c=corV[j];
				}
			}
			if ((c0!=NULL)&&(c1!=NULL))
			{
				allPosV[i]->setPlcXCorrection(c0->x+(allPosV[i]->getColumnWithSign()-c0->col)*(c1->x-c0->x)/(c1->col-c0->col));
				allPosV[i]->setPlcYCorrection(c0->y+(allPosV[i]->getColumnWithSign()-c0->col)*(c1->y-c0->y)/(c1->col-c0->col));
				allPosV[i]->setPlcZCorrection(c0->z+(allPosV[i]->getColumnWithSign()-c0->col)*(c1->z-c0->z)/(c1->col-c0->col));
				allPosV[i]->updateGui();
			}
			if (c)
			{
				allPosV[i]->setPlcXCorrection(c->x);
				allPosV[i]->setPlcYCorrection(c->y);
				allPosV[i]->setPlcZCorrection(c->z);
				allPosV[i]->updateGui();
			}
		}
	}
	//fill rest
	for (int i=0;i<allPosV.count();i++)
	{
		splash->showMessage(tr("Corrections pass 2 (%1)").arg(100*i/allPosV.count()),Qt::AlignRight|Qt::AlignBottom,QColor(150,150,200));
		if ((allPosV[i]->getRow()==1)||(allPosV[i]->getRow()==noOfRows))
			continue;
		posClass *r0=getPosWithColumnRow(allPosV[i]->getColumnWithSign(),1);
		posClass *r1=getPosWithColumnRow(allPosV[i]->getColumnWithSign(),noOfRows);
		if ((r0!=NULL)&&(r1!=NULL))
		{
			allPosV[i]->setPlcXCorrection(r0->getPlcXCorrection()+(allPosV[i]->getRow()-r0->getRow())*(r1->getPlcXCorrection()-r0->getPlcXCorrection())/(r1->getRow()-r0->getRow()));
			allPosV[i]->setPlcYCorrection(r0->getPlcYCorrection()+(allPosV[i]->getRow()-r0->getRow())*(r1->getPlcYCorrection()-r0->getPlcYCorrection())/(r1->getRow()-r0->getRow()));
			allPosV[i]->setPlcZCorrection(r0->getPlcZCorrection()+(allPosV[i]->getRow()-r0->getRow())*(r1->getPlcZCorrection()-r0->getPlcZCorrection())/(r1->getRow()-r0->getRow()));
			allPosV[i]->updateGui();
		}
	}
	for (int i=0;i<allPosV.count();i++)
	{
		if (allPosV[i]->getCorrectedPlcZ()>7920)
			allPosV[i]->setPlcZCorrection(0);
	}
}
posClass* allPossClass::getPosWithColumnRow(int col,int row)
{
	for (int i=0;i<allPosV.count();i++)
	{
		if ((allPosV[i]->getColumnWithSign()==col)&&(allPosV[i]->getRow()==row))
			return allPosV[i];
	}
	return NULL;
}
void allPossClass::setCraneClass(craneClass *c)
{
	crane=c;
}
void allPossClass::readFromDbTimerTimeoutSlot()
{
	for (int i=0;i<allPosV.count();i++)
		allPosV[i]->loadFromDatabase();
}
posClass* allPossClass::getPosWithId(int id,bool unLockedOnly,bool exitIfNotFound)
{
	posClass* ans=NULL;
	for (int i=0;i<allPosV.count();i++)
	{
		if (allPosV[i]->getPosId()==id)
		{
			if (!unLockedOnly)
				ans=allPosV[i];
			else if (!allPosV[i]->getIsLocked())
				ans=allPosV[i];
			break;
		}
	}
	if ((exitIfNotFound)&&(!ans))
	{
		log(QString("getPosWithId id:%1 returning null").arg(id),errorStr);
		uExit("Internal error",QString("getPosWithId id:%1 returning null").arg(id));
	}
	return ans;
}
posClass* allPossClass::getEmptyCellForCassette(int b)
{
	int p=getFirst(QString("select posId from pos where posId=%1 and cassette=0 and locked=0 and posId not in (select fromPos from missions) and posId not in (select toPos from missions)").arg(b),mDb).toInt();
	if (p!=0)
		return getPosWithId(p);
	return NULL;
}
int allPossClass::getEmptyIdleCassette()
{
	int p=getFirst(QString("select posId from pos %1 and cassette not in (select cassette from cassetteContents) and posId not in (select fromPos from missions) and posId not in (select toPos from missions) order by abs (col-67) asc,row asc").arg(whereIdleCassette),mDb).toInt();
	return p;
}
posClass* allPossClass::getCellWithCassette(int b,bool unLockedOnly)
{
	posClass* ans=NULL;
	for (int i=0;i<allPosV.count();i++)
	{
		if ((allPosV[i]->getPosId()<craneId)&&(allPosV[i]->getCassetteNumber()==b))
		{
			if (!unLockedOnly)
				ans=allPosV[i];
			else if (!allPosV[i]->getIsLocked())
				ans=allPosV[i];
			break;
		}
	}
	return ans;
}
void allPossClass::drawColForReport(int col,int dW,int dH,int noOfColumnsInPage,int firstcol,bool rightSideCells,QPainter *painter)
{
	for (int i=0;i<allPosV.count();i++)
	{
		if (   ((!rightSideCells)&&(allPosV[i]->getPosId()>0))    ||   ((rightSideCells)&&(allPosV[i]->getPosId()<0))   )
		{
			if (allPosV[i]->getColumn()==col)
			{
				painter->setPen(QPen(QBrush(QColor(150,150,150)),0));
				int drawCol=0;
				if (!rightSideCells)
					drawCol=allPosV[i]->getColumn()-firstcol-1;
				else
					drawCol=noOfColumnsInPage+firstcol-allPosV[i]->getColumn();
				painter->drawText(drawCol*dW,dH+noOfRows*dH-allPosV[i]->getRow()*dH,dW,dH,Qt::AlignBottom|Qt::AlignHCenter,QString("%1").arg(allPosV[i]->getPosId()));
				painter->setPen(QPen(QBrush(QColor(0,0,0)),0));
				if (allPosV[i]->getCassetteNumber()>0)
					painter->drawText(drawCol*dW,dH+noOfRows*dH-allPosV[i]->getRow()*dH,dW,dH,Qt::AlignTop|Qt::AlignHCenter,QString("%1").arg(allPosV[i]->getCassetteNumber()));
			}
		}
	}
}
void allPossClass::drawPageLayout(int maxW,int maxH,int dW,int dH,int noOfColumnsInPage,int firstcol,QString headerText,QString footerText,bool rightSideCells,QPainter *painter)
{
	painter->setPen(QPen(QBrush(QColor(0,0,0)),0));
	int y=dH;
	for (int i=0;i<=noOfRows;i++)
	{
		painter->drawLine(0,y,maxW,y);
		y+=dH;
	}
	y-=dH;
	for (int i=0;i<=noOfColumnsInPage;i++)
	{
		painter->drawLine(i*dW,dH,i*dW,y);
		if (rightSideCells)
			painter->drawText(i*dW,y,dW,0.25*dH,Qt::AlignCenter,QString("%1").arg(noOfColumnsInPage+firstcol-i));
		else
			painter->drawText(i*dW,y,dW,0.25*dH,Qt::AlignCenter,QString("%1").arg(firstcol+i+1));
	}
	painter->drawText(0,0,maxW,dH,Qt::AlignHCenter|Qt::AlignBottom,headerText);
	painter->drawText(0,maxH-0.25*dH,maxW,0.25*dH,Qt::AlignRight|Qt::AlignVCenter,footerText);
}
void allPossClass::drawForReport(QString pdfFile)
{
	int noOfColumnsInPage=22;
	QPrinter printer(QPrinter::HighResolution);
	float scale=46;
	int W=297*scale;
	int H=208*scale;
	int dW=W/noOfColumnsInPage;
	int dH=0.9*H/noOfRows;
	printer.setOutputFileName(pdfFile);
	printer.setOrientation(QPrinter::Landscape);
	printer.setPageSize(QPageSize(QPageSize::A4));
	QString footerText=tr("warehouse report, created on:%1").arg(timeVerbose);
	QPainter painter;
	painter.begin(&printer);
	//painter.setTransform(QTransform(scale,0.,0.,scale,0.,0.),false);
	
	int columnWidthInReport=(280./noOfColumnsInPage)*scale;//280mm of 297
	int colsReported=0;
	//left cells
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,0,tr("Left cells"),footerText,false,&painter);
	for (int i=0;i<=21;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,0,false,&painter);
	printer.newPage();
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,22,tr("Left cells"),footerText,false,&painter);
	for (int i=22;i<=43;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,22,false,&painter);
	printer.newPage();
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,44,tr("Left cells"),footerText,false,&painter);
	for (int i=44;i<=65;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,44,false,&painter);
	printer.newPage();
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,66,tr("Left cells"),footerText,false,&painter);
	for (int i=66;i<=83;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,66,false,&painter);
	printer.newPage();
	//right cells
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,0,tr("Right cells"),footerText,true,&painter);
	for (int i=0;i<22;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,0,true,&painter);
	printer.newPage();
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,22,tr("Right cells"),footerText,true,&painter);
	for (int i=22;i<44;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,22,true,&painter);
	printer.newPage();
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,44,tr("Right cells"),footerText,true,&painter);
	for (int i=44;i<66;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,44,true,&painter);
	printer.newPage();
	drawPageLayout(W,H,dW,dH,noOfColumnsInPage,66,tr("Right cells"),footerText,true,&painter);
	for (int i=66;i<82;i++)
		drawColForReport(i+1,dW,dH,noOfColumnsInPage,66,true,&painter);
	painter.end();
}
void allPossClass::posCtrlMouseReleasedSlot(posClass *p,QGraphicsSceneMouseEvent *ev)
{
	if (!crane)
		return;
	if (p->getIsLocked())
		return;
	if (!manualSelectedFromPos)
	{
		if (p->getCassetteNumber()!=0)
		{
			manualSelectedFromPos=p;
			manualSelectedFromPos->setIsSelected(true);
		}
	}
#ifndef eastStation
	else
	{
		if ((ev->modifiers()&Qt::ControlModifier))
		{
			if (manualSelectedFromPos!=p)
				crane->sendManualMission(manualSelectedFromPos,p);
		}
		manualSelectedFromPos->setIsSelected(false);
		manualSelectedFromPos=NULL;
	}
#endif
}
bool allPossClass::isCassettesActive(int cassettes)
{
	//check db positions
	QString activConv=getFirst(QString("select conveyorPos from missions where (fromPos in (select posId from pos where cassettes=%1) or toPos in (select posId from pos where cassettes=%1))").arg(cassettes),mDb).toString().trimmed();
	if (activConv!="0")
		return true;
	return false;
}
void allPossClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	for (int i=0;i<plcPosV.count();i++)
		plcPosV[i]->dataInPlcChanged(ip,table,index,value);
}
QVector<posClass*> allPossClass::getAllPos()
{
	return allPosV;
}
QVector<QPointF> allPossClass::getMinusCellsPositions()
{
	QVector<QPointF> ans;
	ans.resize(495+1);
	for (int i=0;i<=(-minCassetteId);i++)
	{
		posClass *p=getPosWithId(-i,false,false);
		if (p&&(p->getPosId()<craneId)&&(p->getPosId()<0))
			ans[i]=QPointF(p->getCorrectedPlcX(),p->getCorrectedPlcZ());
		else
			ans[i]=QPointF(0.f,0.f);
	}
	return ans;
}
QVector<QPointF> allPossClass::getPlusCellsPositions()
{
	QVector<QPointF> ans;
	ans.resize(495+1);
	for (int i=0;i<=maxCassetteId;i++)
	{
		posClass *p=getPosWithId(i,false,false);
		if (p&&(p->getPosId()<craneId)&&(p->getPosId()>0))
			ans[i]=QPointF(p->getCorrectedPlcX(),p->getCorrectedPlcZ());
		else
			ans[i]=QPointF(0.f,0.f);
	}
	return ans;
}
int allPossClass::getNumberOfCells()
{
	int ans=0;
	for (int i=0;i<allPosV.count();i++)
	{
		if (allPosV[i]->getPosId()<craneId)
			ans++;
	}
	return ans;
}
