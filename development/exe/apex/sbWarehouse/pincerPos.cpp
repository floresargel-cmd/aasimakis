#include "pch.h"
#include "pincerPos.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////positionClass//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern gItemClass			*externEmptyBasketGraphicsItem								;
extern gItemClass			*externFullBasketGraphicsItem									;
extern gItemClass			*externBasketPlcFromWithBasketGraphicsItem		;
extern gItemClass			*externBasketPlcFromWithOutBasketGraphicsItem	;
extern gItemClass			*externBasketPlcToWithBasketGraphicsItem			;
extern gItemClass			*externBasketPlcToWithOutBasketGraphicsItem		;
extern gItemClass			*externBasketUndefinedGraphicsItem						;
extern gItemClass			*externBasketUserSelectedFromPosGraphicsItem	;
extern gItemClass			*externNoBasketGraphicsItem										;
extern gItemClass			*externPosGraphicsItem												;

pincerPositionClass::pincerPositionClass(posClass *parrentPos_,QGraphicsScene *myScene_,float plcX_,float plcZ_,QWidget *p):QWidget(p)
{
	parrentPos=parrentPos_;
	myScene=myScene_;
	plcX=plcX_;
	plcZ=plcZ_;
	abovePosition=belowPosition=NULL;
	permanentMode=tempMode=0;
	float xPos=-plcX;
	float yPos=plcZ;
	posGraphicsItem													=new gItemClass(externPosGraphicsItem													,xPos,yPos);myScene->addItem(posGraphicsItem													);
	connect(posGraphicsItem														,SIGNAL(mousePressedSignal(QGraphicsSceneMouseEvent*)),this,SLOT(mousePressedSlot(QGraphicsSceneMouseEvent*)));
	
	noBasketGraphicsItem										=new gItemClass(externNoBasketGraphicsItem										,xPos,yPos);myScene->addItem(noBasketGraphicsItem											);
	emptyBasketGraphicsItem									=new gItemClass(externEmptyBasketGraphicsItem									,xPos,yPos);myScene->addItem(emptyBasketGraphicsItem									);
	fullBasketGraphicsItem									=new gItemClass(externFullBasketGraphicsItem									,xPos,yPos);myScene->addItem(fullBasketGraphicsItem										);
	basketPlcFromWithBasketGraphicsItem			=new gItemClass(externBasketPlcFromWithBasketGraphicsItem			,xPos,yPos);myScene->addItem(basketPlcFromWithBasketGraphicsItem			);
	basketPlcFromWithOutBasketGraphicsItem	=new gItemClass(externBasketPlcFromWithOutBasketGraphicsItem	,xPos,yPos);myScene->addItem(basketPlcFromWithOutBasketGraphicsItem		);
	basketPlcToWithBasketGraphicsItem				=new gItemClass(externBasketPlcToWithBasketGraphicsItem				,xPos,yPos);myScene->addItem(basketPlcToWithBasketGraphicsItem				);
	basketPlcToWithOutBasketGraphicsItem		=new gItemClass(externBasketPlcToWithOutBasketGraphicsItem		,xPos,yPos);myScene->addItem(basketPlcToWithOutBasketGraphicsItem			);
	basketUndefinedGraphicsItem							=new gItemClass(externBasketUndefinedGraphicsItem							,xPos,yPos);myScene->addItem(basketUndefinedGraphicsItem							);
	basketUserSelectedFromPosGraphicsItem		=new gItemClass(externBasketUserSelectedFromPosGraphicsItem		,xPos,yPos);myScene->addItem(basketUserSelectedFromPosGraphicsItem		);
	setGraphicsItem(emptyBasketGraphicsItem);
	graphicsItemText=new myGraphicsTextItem("",posGraphicsItem->boundingRect(),QFont("Arial",75),QColor(100,150,100),myScene);
	graphicsItemText->setZValue(-1);

	QTransform txtTransformation;
	txtTransformation.scale(1,-1.);
	txtTransformation.translate(xPos,-(yPos+1.*posGraphicsItem->boundingRect().height()));
	graphicsItemText->setTransform(txtTransformation,false);
	graphicsItemText->setAcceptHoverEvents(true);

	tooltipHeader=tr("Position:%1").arg(parrentPos->getPosId());
	updateVisual();
}
void pincerPositionClass::setPos(float x,float y)
{
	
	posGraphicsItem->setPos(x,y);
	noBasketGraphicsItem->setPos(x,y);
	emptyBasketGraphicsItem->setPos(x,y);
	fullBasketGraphicsItem->setPos(x,y);
	basketPlcFromWithBasketGraphicsItem->setPos(x,y);
	basketPlcFromWithOutBasketGraphicsItem->setPos(x,y);
	basketPlcToWithBasketGraphicsItem->setPos(x,y);
	basketPlcToWithOutBasketGraphicsItem->setPos(x,y);
	basketUndefinedGraphicsItem->setPos(x,y);
	basketUserSelectedFromPosGraphicsItem->setPos(x,y);
	graphicsItemText->setPos(x,y);
}
void pincerPositionClass::setAbovePosition(pincerPositionClass* p)
{
	abovePosition=p;
}
void pincerPositionClass::setBelowPosition(pincerPositionClass* p)
{
	belowPosition=p;
}
int pincerPositionClass::getPosId()
{
	return parrentPos->getPosId();  
}
int pincerPositionClass::getPlcIndex()
{
	return parrentPos->getPlcIndex();  
}
bool pincerPositionClass::getHasSb()
{ 
	return parrentPos->getHasSb();
} 
bool pincerPositionClass::getHasMode(int m)
{
	return HAVECOMMONBIT(permanentMode,m);
}
void pincerPositionClass::addMode(int m)
{
	int n=ADDBIT(permanentMode,m);
	if (n!=permanentMode)
	{
		permanentMode=n;
		emit changedSignal(getPosId());
	}
	updateVisual();
}
void pincerPositionClass::removeMode(int m)
{
	int n=REMOVEBIT(permanentMode,m);
	if (n!=permanentMode)
	{
		permanentMode=n;
		emit changedSignal(getPosId());
	}
	updateVisual();
}
void pincerPositionClass::checkModes()
{
	tempMode=permanentMode;
	if (HAVECOMMONBIT(tempMode,SHOWN_FOR_USER_TO_SELECTE_TO))
		tempMode=REMOVEBIT(tempMode,USER_CAN_DEFINE_BASKET);
	if (HAVECOMMONBIT(tempMode,ENABLED_FOR_USER_TO_SELECTE_FROM))
	{
		if (abovePosition&&(abovePosition->getHasSb())||(!getHasSb()))
			tempMode=REMOVEBIT(tempMode,ENABLED_FOR_USER_TO_SELECTE_FROM);
	}
	if (HAVECOMMONBIT(tempMode,SHOWN_FOR_USER_TO_SELECTE_TO))
	{
		if (
			//(canTakeBasketFromCrane!=1)
			//||
			(abovePosition&&abovePosition->getHasSb())
			||
			(   belowPosition  &&   (!belowPosition->getHasSb())  )
			||
			( belowPosition && belowPosition->getHasMode(USER_SELECTED) )
			||
			getHasSb()
			)
			tempMode=REMOVEBIT(tempMode,SHOWN_FOR_USER_TO_SELECTE_TO);
	}
	if (HAVECOMMONBIT(tempMode,USER_CAN_DEFINE_BASKET))
	{
		if (
			getHasSb()
			||
			(belowPosition&&((!belowPosition->getHasSb())||(belowPosition->getHasMode(PLC_FROM))||(belowPosition->getHasMode(PLC_TO))))
			)
		{
			tempMode=REMOVEBIT(tempMode,USER_CAN_DEFINE_BASKET);
		}
	}
	if (HAVECOMMONBIT(tempMode,LOCKED_FROM_PLC))
	{
		tempMode=REMOVEBIT(tempMode,USER_SELECTED);
	}
#ifdef _DEBUG
	QString toolTip;
	if (HAVECOMMONBIT(permanentMode,PLC_ON_MISSION										))toolTip.append("p plc on mission\n");
	if (HAVECOMMONBIT(permanentMode,USER_SELECTED											))toolTip.append("p user selected\n");
	if (HAVECOMMONBIT(permanentMode,PLC_FROM													))toolTip.append("p plc from \n");
	if (HAVECOMMONBIT(permanentMode,PLC_TO														))toolTip.append("p plc to\n");
	if (HAVECOMMONBIT(permanentMode,ENABLED_FOR_USER_TO_SELECTE_FROM	))toolTip.append("p enabled for user to selecte_from\n");
	if (HAVECOMMONBIT(permanentMode,SHOWN_FOR_USER_TO_SELECTE_TO			))toolTip.append("p shown for user to selecte to\n");
	if (HAVECOMMONBIT(permanentMode,USER_CAN_DEFINE_BASKET						))toolTip.append("p user can define basket\n");
	if (HAVECOMMONBIT(permanentMode,LOCKED_FROM_PLC										))toolTip.append("p locked from oven plc\n");
	if (HAVECOMMONBIT(permanentMode,MAINTENANCE												))toolTip.append("p maintenance\n\n");
	if (HAVECOMMONBIT(tempMode,PLC_ON_MISSION													))toolTip.append("plc on mission\n");
	if (HAVECOMMONBIT(tempMode,USER_SELECTED													))toolTip.append("user selected\n");
	if (HAVECOMMONBIT(tempMode,PLC_FROM																))toolTip.append("plc from\n");
	if (HAVECOMMONBIT(tempMode,PLC_TO																	))toolTip.append("plc to\n");
	if (HAVECOMMONBIT(tempMode,ENABLED_FOR_USER_TO_SELECTE_FROM				))toolTip.append("enabled for user to selecte_from\n");
	if (HAVECOMMONBIT(tempMode,SHOWN_FOR_USER_TO_SELECTE_TO						))toolTip.append("shown for user to selecte to\n");
	if (HAVECOMMONBIT(tempMode,USER_CAN_DEFINE_BASKET									))toolTip.append("user can define basket\n");
	if (HAVECOMMONBIT(tempMode,LOCKED_FROM_PLC												))toolTip.append("locked from oven plc\n");
	if (HAVECOMMONBIT(tempMode,MAINTENANCE														))toolTip.append("maintenance\n");
	posGraphicsItem->setToolTip(toolTip);
#endif
}

void pincerPositionClass::setUserMaintenanseMode(bool m)
{
	if (m)
		addMode(MAINTENANCE);
	else
		removeMode(MAINTENANCE);
}
bool pincerPositionClass::getIsUserSelected()
{
	return HAVECOMMONBIT(permanentMode,ENABLED_FOR_USER_TO_SELECTE_FROM);
}
void pincerPositionClass::setGraphicsItem(gItemClass* g)
{
	currentGraphicsItem=g;
	emptyBasketGraphicsItem									->showGraphics(false);
	fullBasketGraphicsItem									->showGraphics(false);
	basketPlcFromWithBasketGraphicsItem			->showGraphics(false);
	basketPlcFromWithOutBasketGraphicsItem	->showGraphics(false);
	basketPlcToWithBasketGraphicsItem				->showGraphics(false);
	basketPlcToWithOutBasketGraphicsItem		->showGraphics(false);
	basketUndefinedGraphicsItem							->showGraphics(false);
	basketUserSelectedFromPosGraphicsItem		->showGraphics(false);
	posGraphicsItem													->showGraphics(false);
	currentGraphicsItem->showGraphics(true);
}

void pincerPositionClass::updateVisual()
{
	blockSignals(true);
	checkModes();
	QString toolTip=tooltipHeader;
	parrentPos->loadFromDatabase();
	if (getHasSb())//basket
	{
		if (parrentPos->getHasTicket())
		{
			setGraphicsItem(fullBasketGraphicsItem);
			graphicsItemText->setText(tr("%1").arg(parrentPos->getTicket()));
		}
		else
		{
			setGraphicsItem(emptyBasketGraphicsItem);
			graphicsItemText->setText(tr(""));
		}
	}
	else
	{
		setGraphicsItem(noBasketGraphicsItem);
		graphicsItemText->setText(tr("%1").arg(getPlcIndex()));
	}
	if (HAVECOMMONBIT(tempMode,PLC_ON_MISSION))//PLC_ON_MISSION
	{
		setEnabled(false);
		posGraphicsItem->showGraphics(false);
		if (HAVECOMMONBIT(tempMode,MAINTENANCE))//MAINTENANCE
		{
			currentGraphicsItem->showGraphics(true);
			posGraphicsItem->showGraphics(true);
			setEnabled(true);
		}
		if (HAVECOMMONBIT(tempMode,PLC_FROM))
		{
			if (getHasSb())
				setGraphicsItem(basketPlcFromWithBasketGraphicsItem);
			else
				setGraphicsItem(basketPlcFromWithOutBasketGraphicsItem);
		}
		else if (HAVECOMMONBIT(tempMode,PLC_TO))
		{
			if (getHasSb())
				setGraphicsItem(basketPlcToWithBasketGraphicsItem);
			else
				setGraphicsItem(basketPlcToWithOutBasketGraphicsItem);
		}
		else if ((HAVECOMMONBIT(tempMode,USER_CAN_DEFINE_BASKET))&&(getHasSb()))
		{
			setGraphicsItem(basketUndefinedGraphicsItem);
			setEnabled(true);
		}
	} 
	else
	{
		if (HAVECOMMONBIT(tempMode,MAINTENANCE))//MAINTENANCE
		{
			currentGraphicsItem->showGraphics(true);
			posGraphicsItem->showGraphics(true);
			setEnabled(true);
		}
		else
		{
			setEnabled(false);
			if (HAVECOMMONBIT(tempMode,USER_SELECTED))
			{
				setEnabled(!HAVECOMMONBIT(tempMode,LOCKED_FROM_PLC));
				setGraphicsItem(basketUserSelectedFromPosGraphicsItem);
			}
			if (HAVECOMMONBIT(tempMode,ENABLED_FOR_USER_TO_SELECTE_FROM))
			{
				setEnabled(!HAVECOMMONBIT(tempMode,LOCKED_FROM_PLC));
				posGraphicsItem->showGraphics(!HAVECOMMONBIT(tempMode,LOCKED_FROM_PLC));
			}
			if (HAVECOMMONBIT(tempMode,SHOWN_FOR_USER_TO_SELECTE_TO))
			{
				currentGraphicsItem->showGraphics(!HAVECOMMONBIT(tempMode,LOCKED_FROM_PLC));
				setGraphicsItem(posGraphicsItem);
				setEnabled(!HAVECOMMONBIT(tempMode,LOCKED_FROM_PLC));
			}
			else if ((HAVECOMMONBIT(tempMode,USER_CAN_DEFINE_BASKET))&&(getHasSb()))
			{
				setEnabled(true);
				posGraphicsItem->showGraphics(true);
				setGraphicsItem(basketUndefinedGraphicsItem);
			}
		}
	}
	blockSignals(false);
	posGraphicsItem->setToolTip(toolTip);
	graphicsItemText->setToolTip(toolTip);
}
void pincerPositionClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if ((getPlcIndex()==pincer2Id)&&(table=="PC2_PCcomReal"))
	{
		if (index==0)
		{
			plcX=value.toFloat();
			plcPositionChanged();
		}
		if (index==1)
		{
			plcZ=value.toFloat();
			plcPositionChanged();
		}
	}
	if (index==parrentPos->getPlcIndex())
	{
		//if (basketNumber!=value.toInt())
		//{
		//	basketNumber=value.toInt();
			updateVisual();
			emit changedSignal(getPosId());
		//}
	}

}
void pincerPositionClass::plcPositionChanged()
{
	emptyBasketGraphicsItem->setPos(-plcX,-plcZ);
	fullBasketGraphicsItem->setPos(-plcX,-plcZ);
	basketPlcFromWithBasketGraphicsItem->setPos(-plcX,-plcZ);
	basketPlcFromWithOutBasketGraphicsItem->setPos(-plcX,-plcZ);
	basketPlcToWithBasketGraphicsItem->setPos(-plcX,-plcZ);
	basketPlcToWithOutBasketGraphicsItem->setPos(-plcX,-plcZ);
	basketUndefinedGraphicsItem->setPos(-plcX,-plcZ);
	basketUserSelectedFromPosGraphicsItem->setPos(-plcX,-plcZ);
	noBasketGraphicsItem->setPos(-plcX,-plcZ);
	posGraphicsItem->setPos(-plcX,-plcZ);
	graphicsItemText->setPos(-plcX,-plcZ);
}
void pincerPositionClass::mousePressedSlot(QGraphicsSceneMouseEvent *ev)
{
	if (ev->button()!=Qt::LeftButton)
		return;
	if ((HAVECOMMONBIT(permanentMode,MAINTENANCE))||(HAVECOMMONBIT(permanentMode,USER_CAN_DEFINE_BASKET)&&(getHasSb()))&&(!HAVECOMMONBIT(permanentMode,SHOWN_FOR_USER_TO_SELECTE_TO)))
	{
		parrentPos->showPositionDlg(false);
		emit changedSignal(getPosId());
		updateVisual();
	}
	else 
		emit clickedSignal(getPosId());
}     