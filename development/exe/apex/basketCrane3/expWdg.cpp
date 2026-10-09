#include "pch.h"
#include "expWdg.h"
exportStationWidgetClass::exportStationWidgetClass(QString dbTable_,QString title_,QWidget *p):QWidget(p)
{
	dbTable=dbTable_;
	title=title_;
	setWindowTitle(title);
	selectedBasket=0;
	basketSelect=new basketSelectClass(title,dbTable,this);
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
	connect(basketUpBtn,SIGNAL(released()),this,SLOT(basketUpBtnClickedSlot()));
	basketUpBtn->setEnabled(false);

	basketDownBtn=new QToolButton(this);btnsLayout->addWidget(basketDownBtn);
	basketDownBtn->setIcon(QIcon(":arrowDown.png"));
	basketDownBtn->setStatusTip(tr("Move selected basket one position down."));
	basketDownBtn->setIconSize(QSize(16,16));
	connect(basketDownBtn,SIGNAL(released()),this,SLOT(basketDownBtnClickedSlot()));
	basketDownBtn->setEnabled(false);

	basketAddBtn=new QToolButton(this);btnsLayout->addWidget(basketAddBtn);
	basketAddBtn->setIcon(QIcon(":add.png"));
	basketAddBtn->setStatusTip(tr("Add selected basket to list."));
	basketAddBtn->setIconSize(QSize(16,16));
	connect(basketAddBtn,SIGNAL(released()),this,SLOT(basketAddBtnClickedSlot()));

	basketRemoveBtn=new QToolButton(this);btnsLayout->addWidget(basketRemoveBtn);
	basketRemoveBtn->setIcon(QIcon(":delete.png"));
	basketRemoveBtn->setStatusTip(tr("Remove selected basket from list."));
	basketRemoveBtn->setIconSize(QSize(16,16));
	connect(basketRemoveBtn,SIGNAL(released()),this,SLOT(basketRemoveBtnClickedSlot()));
	basketRemoveBtn->setEnabled(false);
	//
	QTimer *dbUpdateTimer=new QTimer(this);
	connect(dbUpdateTimer,SIGNAL(timeout()),this,SLOT(dbUpdateTimerSlot()));
	dbUpdateTimerSlot();
	dbUpdateTimer->start(5000);
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
		//QString toolStr;
		//toolStr+="<html><body bgcolor=\"#E6E6FA\"><font style='color:#333333'>"+tr("Basket number:")+"</font><font style='color:#000077'>"+QString("%1").arg(basket)+"</font><br>";
		//toolStr+=tr("Priority:")+"</font><font style='color:#000077'>"+QString("%1").arg(dataVV[i][1].toInt())+"</font><br>";
		////if (dbTable==exportsPickingTable)
		//{
		//	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select profile,quantity,length from basketsContents where basket=%1").arg(basket),aw1Db);
		//	toolStr+=QString(QString("<table><tr>"));
		//	for (int i=0;i<dataVV.count();i++)
		//	{
		//		toolStr+=QString(QString("<td>"));
		//		QString imgPath=getPicFileNameFromProfile(dataVV[i][0].toString());
		//		toolStr+=
		//			QString("<img src=\"%1\" ></img><br>").arg(imgPath)+
		//			QString("%1<br>").arg(dataVV[i][0].toString().trimmed())+
		//			tr("pieces:%1<br>").arg(dataVV[i][1].toInt())+
		//			tr("length:%1mm<br>").arg(dataVV[i][2].toInt())
		//			;
		//		toolStr+=QString(QString("</td>"));
		//	}
		//	toolStr+=QString(QString("</tr></table><br><br>"));
		//}
		//toolStr+="</body></html>";
		//exportsList->item(i)->setToolTip(toolStr);
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
void exportStationWidgetClass::basketUpBtnClickedSlot()
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
void exportStationWidgetClass::basketDownBtnClickedSlot()
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
void exportStationWidgetClass::basketAddBtnClickedSlot()
{
	basketSelect->exec();
	updateFromDatabase();
}

void exportStationWidgetClass::basketRemoveBtnClickedSlot()
{
	execQuery(QString("delete from %1 where basket=%2").arg(dbTable).arg(selectedBasket),bDb);
	updateFromDatabase();
	if (exportsList->count()>0)
		selectedBasket=exportsList->item(0)->data(Qt::UserRole).toInt();
	emit listChangedSignal();
}
