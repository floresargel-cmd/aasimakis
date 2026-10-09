#include "pch.h"
#include "expWdg.h"
exportStationWidgetClass::exportStationWidgetClass(QString dbTable_,QString title_,int plcId_,QWidget *p):QWidget(p)
{
	plcId=plcId_;
	dbTable=dbTable_;
	title=title_;
	setWindowTitle(title);
	selectedBasket=0;
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);mainLayout->setMargin(1);
	QHBoxLayout *listLayout=new QHBoxLayout();mainLayout->addLayout(listLayout);
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);

	exportsList=new QListWidget();listLayout->addWidget(exportsList);
	exportsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	exportsList->setSelectionMode(QAbstractItemView::SingleSelection);
	exportsList->setAlternatingRowColors(true);
	connect(exportsList,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(exportsListSelectionChangedSlot(QListWidgetItem*,QListWidgetItem*)));

	basketUpBtn=new QToolButton(this);btnsLayout->addWidget(basketUpBtn);
	basketUpBtn->setIcon(QIcon(":arrowUp.png"));
	basketUpBtn->setToolTip(tr("Move selected basket one position up."));
	basketUpBtn->setIconSize(QSize(16,16));
	connect(basketUpBtn,SIGNAL(released()),this,SLOT(basketUpSlot()));
	basketUpBtn->setEnabled(false);

	basketDownBtn=new QToolButton(this);btnsLayout->addWidget(basketDownBtn);
	basketDownBtn->setIcon(QIcon(":arrowDown.png"));
	basketDownBtn->setStatusTip(tr("Move selected basket one position down."));
	basketDownBtn->setIconSize(QSize(16,16));
	connect(basketDownBtn,SIGNAL(released()),this,SLOT(basketDownSlot()));
	basketDownBtn->setEnabled(false);

	addDiesFromEpicsBtn=new QToolButton(this);btnsLayout->addWidget(addDiesFromEpicsBtn);
	addDiesFromEpicsBtn->setIcon(QIcon(":clock.png"));
	addDiesFromEpicsBtn->setStatusTip(tr("Add basket with dies for production."));
	addDiesFromEpicsBtn->setIconSize(QSize(16,16));
	connect(addDiesFromEpicsBtn,SIGNAL(released()),this,SLOT(addDiesFromEpicsBtnSlot()));

	basketAddBtn=new QToolButton(this);btnsLayout->addWidget(basketAddBtn);
	basketAddBtn->setIcon(QIcon(":add.png"));
	basketAddBtn->setStatusTip(tr("Add a basket to the list."));
	basketAddBtn->setIconSize(QSize(16,16));
	connect(basketAddBtn,SIGNAL(released()),this,SLOT(basketAddSlot()));

	basketRemoveBtn=new QToolButton(this);btnsLayout->addWidget(basketRemoveBtn);
	basketRemoveBtn->setIcon(QIcon(":delete.png"));
	basketRemoveBtn->setStatusTip(tr("Remove selected basket from list."));
	basketRemoveBtn->setIconSize(QSize(16,16));
	connect(basketRemoveBtn,SIGNAL(released()),this,SLOT(basketRemoveSlot()));
	basketRemoveBtn->setEnabled(false);
	//
	QTimer *dbUpdateTimer=new QTimer(this);
	connect(dbUpdateTimer,SIGNAL(timeout()),this,SLOT(dbUpdateTimerSlot()));
	dbUpdateTimerSlot();
	dbUpdateTimer->start(5000);
	basketSelect=new basketSelectClass(title,dbTable,this);

	basketSelectForSchedule=new basketSelectForScheduleClass(this);
}
exportStationWidgetClass::~exportStationWidgetClass()
{
}
void exportStationWidgetClass::dbUpdateTimerSlot()
{
	updateFromDatabase();
}

void exportStationWidgetClass::resizeEvent(QResizeEvent *e)
{
	updateFromDatabase();
}
void exportStationWidgetClass::updateFromDatabase()
{
	//fill
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select basket,priority from %1 order by priority asc").arg(dbTable),db);
	bool clearList=false;
	if (exportsList->count()!=dataVV.count())
	{
		clearList=true;
		exportsList->blockSignals(true);
		exportsList->clear();
		exportsList->blockSignals(false);
	}
	int restToComeOut=0;
	int selectedIndex=dataVV.count()>0?0:-1;
	for (int i=0;i<dataVV.count();i++)
	{
		int basket=dataVV[i][0].toInt();
		int cell=getFirst(QString("select plcId from pos where basket=%1 and plcId<%2").arg(basket).arg(cranePlcId),db).toInt();
		QString notInCellTxt;
		bool canChangePriority=true;
		bool canBeDeleted=true;
		if (cell!=0)
		{
			if (getFirst(QString("select status from missions where fromPos=%1 or toPos=%1 ").arg(cell),db).toString()!="0")
			{
				canChangePriority=false;
				canBeDeleted=false;
				notInCellTxt=tr("moving with crane");
			}
			else
				restToComeOut++;
		}
		else
		{
			canChangePriority=false;
			notInCellTxt=tr("not in cells");
			if (getFirst(QString("select basket from pos where plcId=%1").arg(plcId),db).toInt()==basket)
				notInCellTxt=tr("On export station");
		}
		QString str=tr("%1 %2").arg(basket).arg(cell!=0?tr("cell:%1").arg(cell):notInCellTxt);
		if (clearList)
			exportsList->addItem(str);
		else
			exportsList->item(i)->setText(str);
		exportsList->item(i)->setData(Qt::UserRole,dataVV[i][0]);
		exportsList->item(i)->setData(Qt::UserRole+1,canChangePriority);
		exportsList->item(i)->setData(Qt::UserRole+2,canBeDeleted);
		QString toolStr;
		toolStr+="<html><body bgcolor=\"#E6E6FA\"><font style='color:#333333'>"+tr("Basket number:")+"</font><font style='color:#000077'>"+QString("%1").arg(basket)+"</font><br>";
		toolStr+=tr("Located in cell:")+"</font><font style='color:#000077'>"+QString("%1").arg(cell)+"</font><br>";
		toolStr+=tr("Priority:")+"</font><font style='color:#000077'>"+QString("%1").arg(dataVV[i][1].toInt())+"</font><br>";
		toolStr+="</body></html>";
		exportsList->item(i)->setToolTip(toolStr);
		if (!canChangePriority)
			exportsList->item(i)->setForeground(Qt::gray);
		else
			exportsList->item(i)->setForeground(Qt::black);
		if (selectedBasket==basket)
			selectedIndex=i;
	}
	if (selectedIndex>=0)
	{
		exportsList->setItemSelected(exportsList->item(selectedIndex),true);
		exportsList->setCurrentItem(exportsList->item(selectedIndex));
	}
	//btns
	basketUpBtn->setEnabled(false);
	basketDownBtn->setEnabled(false);
	basketRemoveBtn->setEnabled(false);
	if (selectedIndex>=0)
	{
		basketRemoveBtn->setEnabled(exportsList->item(selectedIndex)->data(Qt::UserRole+2).toBool());
		if (exportsList->item(selectedIndex)->data(Qt::UserRole+1).toBool())
		{
			if ((selectedIndex>0)&&(exportsList->item(selectedIndex-1)->data(Qt::UserRole+1).toBool()))
				basketUpBtn->setEnabled(true);
			if ((selectedIndex!=(exportsList->count()-1)))
				basketDownBtn->setEnabled(true);
		}
	}
	if (exportsList->count()==0)
		exportsList->addItem(tr("No Baskets to export"));
	emit titleChangedSignal(QString("%1%2").arg(title).arg(restToComeOut>0?QString(" (%1)").arg(restToComeOut):tr(" (empty)")));
}
void exportStationWidgetClass::exportsListSelectionChangedSlot(QListWidgetItem* selected,QListWidgetItem* deselected)
{
	int sel=0;
	if (selected==NULL)
		sel=0;
	else
		sel=selected->data(Qt::UserRole).toInt();
	if (selectedBasket!=sel)
	{
		selectedBasket=sel;
		updateFromDatabase();
	}
}
void exportStationWidgetClass::basketUpSlot()
{
	for (int i=1;i<exportsList->count();i++)
	{
		if (exportsList->item(i)->data(Qt::UserRole).toInt()==selectedBasket)
		{
			//priorityOfcurrent
			int priorityOfSelected=getOne(QString("select priority from %1 where basket=%2").arg(dbTable).arg(selectedBasket),db).toInt();
			int priorityOfOther=getOne(QString("select priority from %1 where basket=%2").arg(dbTable).arg(exportsList->item(i-1)->data(Qt::UserRole).toInt()),db).toInt();
			execQuery(QString("update %1 set priority=-1 where basket=%3").arg(dbTable).arg(selectedBasket),db);
			execQuery(QString("update %1 set priority=%2 where basket=%3").arg(dbTable).arg(priorityOfSelected).arg(exportsList->item(i-1)->data(Qt::UserRole).toInt()),db);
			execQuery(QString("update %1 set priority=%2 where basket=%3").arg(dbTable).arg(priorityOfOther).arg(selectedBasket),db);
			break;
		}
	}
	updateFromDatabase();
}
void exportStationWidgetClass::basketDownSlot()
{
	for (int i=0;i<(exportsList->count()-1);i++)
	{
		if (exportsList->item(i)->data(Qt::UserRole).toInt()==selectedBasket)
		{
			//priorityOfcurrent
			int priorityOfSelected=getOne(QString("select priority from %1 where basket=%2").arg(dbTable).arg(selectedBasket),db).toInt();
			int priorityOfOther=getOne(QString("select priority from %1 where basket=%2").arg(dbTable).arg(exportsList->item(i+1)->data(Qt::UserRole).toInt()),db).toInt();
			execQuery(QString("update %1 set priority=-1 where basket=%3").arg(dbTable).arg(selectedBasket),db);
			execQuery(QString("update %1 set priority=%2 where basket=%3").arg(dbTable).arg(priorityOfSelected).arg(exportsList->item(i+1)->data(Qt::UserRole).toInt()),db);
			execQuery(QString("update %1 set priority=%2 where basket=%3").arg(dbTable).arg(priorityOfOther).arg(selectedBasket),db);
			break;
		}
	}
	updateFromDatabase();
}
void exportStationWidgetClass::basketAddSlot()
{
	basketSelect->exec();
	updateFromDatabase();
	emit listChangedSignal();
}
void exportStationWidgetClass::basketRemoveSlot()
{
	execQuery(QString("delete from %1 where basket=%2").arg(dbTable).arg(selectedBasket),db);
	updateFromDatabase();
	if (exportsList->count()>0)
		selectedBasket=exportsList->item(0)->data(Qt::UserRole).toInt();
	emit listChangedSignal();
}
void exportStationWidgetClass::newBarcodeReadSlot(int barcodeReaderMode,int basket,QString profile,int quandity,float length,int cardNo,QString color,int soNo,int soLine)
{
	if (myBarcodeReaderMode!=barcodeReaderMode)
		return;
	basketSelect->addBasket(basket);
}
void exportStationWidgetClass::addDiesFromEpicsBtnSlot()
{
	basketSelectForSchedule->exec();
}
