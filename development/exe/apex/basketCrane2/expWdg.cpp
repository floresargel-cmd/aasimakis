#include "pch.h"
#include "expWdg.h"
exportStationWidgetClass::exportStationWidgetClass(QString dbTable_,QString title_,QWidget *p):QWidget(p)
{
	dbTable=dbTable_;
	title=title_;
	setWindowTitle(title);
	selectedBasket=0;
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);mainLayout->setMargin(1);
	QHBoxLayout *topLayout=new QHBoxLayout();mainLayout->addLayout(topLayout);
	QHBoxLayout *listLayout=new QHBoxLayout();mainLayout->addLayout(listLayout);
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);

	topLayout->addWidget(new QLabel(title));

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
}
exportStationWidgetClass::~exportStationWidgetClass()
{
}
void exportStationWidgetClass::setToolBoxClass(QToolBox *parrentToolBox_,int parrentToolBoxIndex_)
{
	parrentToolBox=parrentToolBox_;
	parrentToolBoxIndex=parrentToolBoxIndex_;
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
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select basket,priority from %1 order by priority asc").arg(dbTable),bDb);
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
		QString str=tr("%1").arg(basket);
		if (clearList)
			exportsList->addItem(str);
		else
			exportsList->item(i)->setText(str);
		bool canChangePriority=true;
		exportsList->item(i)->setData(Qt::UserRole,dataVV[i][0]);
		exportsList->item(i)->setData(Qt::UserRole+1,canChangePriority);
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
		if (exportsList->item(selectedIndex)->data(Qt::UserRole+1).toBool())
		{
			if ((selectedIndex>0)&&(exportsList->item(selectedIndex-1)->data(Qt::UserRole+1).toBool()))
				basketUpBtn->setEnabled(true);
			if ((selectedIndex!=(exportsList->count()-1))&&(exportsList->item(selectedIndex+1)->data(Qt::UserRole+1).toBool()))
				basketDownBtn->setEnabled(true);
			basketRemoveBtn->setEnabled(true);
		}
	}
	QString displayStr=QString("%1%2").arg(title).arg(dataVV.count()>0?tr(" (%1 Baskets)").arg(dataVV.count()):"");
	if (parrentToolBox)
		parrentToolBox->setItemText(parrentToolBoxIndex,displayStr);
	if (exportsList->count()==0)
		exportsList->addItem(tr("No Baskets to export"));
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
			int priorityOfSelected=getOne(QString("select priority from %1 where basket=%2").arg(dbTable).arg(selectedBasket),bDb).toInt();
			int priorityOfOther=getOne(QString("select priority from %1 where basket=%2").arg(dbTable).arg(exportsList->item(i-1)->data(Qt::UserRole).toInt()),bDb).toInt();
			execQuery(QString("update %1 set priority=-1 where basket=%3").arg(dbTable).arg(selectedBasket),bDb);
			execQuery(QString("update %1 set priority=%2 where basket=%3").arg(dbTable).arg(priorityOfSelected).arg(exportsList->item(i-1)->data(Qt::UserRole).toInt()),bDb);
			execQuery(QString("update %1 set priority=%2 where basket=%3").arg(dbTable).arg(priorityOfOther).arg(selectedBasket),bDb);
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
			int priorityOfSelected=getOne(QString("select priority from %1 where basket=%2").arg(dbTable).arg(selectedBasket),bDb).toInt();
			int priorityOfOther=getOne(QString("select priority from %1 where basket=%2").arg(dbTable).arg(exportsList->item(i+1)->data(Qt::UserRole).toInt()),bDb).toInt();
			execQuery(QString("update %1 set priority=-1 where basket=%3").arg(dbTable).arg(selectedBasket),bDb);
			execQuery(QString("update %1 set priority=%2 where basket=%3").arg(dbTable).arg(priorityOfSelected).arg(exportsList->item(i+1)->data(Qt::UserRole).toInt()),bDb);
			execQuery(QString("update %1 set priority=%2 where basket=%3").arg(dbTable).arg(priorityOfOther).arg(selectedBasket),bDb);
			break;
		}
	}
	updateFromDatabase();
}
void exportStationWidgetClass::basketRemoveSlot()
{
	execQuery(QString("delete from %1 where basket=%2").arg(dbTable).arg(selectedBasket),bDb);
	updateFromDatabase();
	if (exportsList->count()>0)
		selectedBasket=exportsList->item(0)->data(Qt::UserRole).toInt();
	emit listChangedSignal();
}
