#include "pch.h"
#include "expWdg.h"
myListWidget::myListWidget():QListWidget()
{
	this->setViewMode(QListView::ListMode);
	this->setIconSize(QSize(32,32));
	this->setDragEnabled(true);
	this->setDefaultDropAction(Qt::MoveAction);
	//this->setAcceptDrops(true);
	this->setDropIndicatorShown(true);
	this->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->setSelectionMode(QAbstractItemView::ExtendedSelection);
	this->setAlternatingRowColors(true);
}
Qt::DropAction myListWidget::supportedDropActions()
{
	return Qt::MoveAction;
}
void myListWidget::startDrag(Qt::DropActions supportedActions)
{
	QMimeData* mimeData=new QMimeData;
	QList<QListWidgetItem*> selectedItems=this->selectedItems();
	QString selectedStr;
	QVector<int> itemRemovedTIds;
	for (int i=0;i<selectedItems.count();i++)
	{
		itemRemovedTIds<<selectedItems[i]->data(myListWidget::roleId).toInt();
		selectedStr+=selectedItems[i]->data(myListWidget::roleCassette).toString()+"|";
	}
	mimeData->setData("application/x-item",selectedStr.toLatin1());
	QDrag* drag=new QDrag(this);
	drag->setMimeData(mimeData);
	if (drag->exec(Qt::MoveAction)==Qt::MoveAction)
	{
		emit itemsRemovedSignal(itemRemovedTIds);
	}
}
void myListWidget::dropEvent(QDropEvent *ev)
{
	QListWidgetItem* item=this->itemAt(ev->pos());
	int itemId=0;
	if (item)
		itemId=item->data(myListWidget::roleId).toInt();
	ev->setDropAction(Qt::MoveAction);
	ev->accept();
	emit itemDropedOnSignal(itemId,QString(ev->mimeData()->data("application/x-item")));
}
void myListWidget::dragEnterEvent(QDragEnterEvent* ev)
{
		if (ev->mimeData()->hasFormat("application/x-item"))
			ev->accept();
		else
				ev->ignore();
}
void myListWidget::dragMoveEvent(QDragMoveEvent *ev)
{
	QListWidgetItem* item=this->itemAt(ev->pos());
	if (item)
	{
		bool acceptsDrops=true;
		if (acceptsDrops)
		{
			ev->setDropAction(Qt::MoveAction);
			ev->accept();
		}
		else
		{
			ev->ignore();
		}
	}
	else
	{
		ev->setDropAction(Qt::MoveAction);
		ev->accept();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
exportStationWidgetClass::exportStationWidgetClass(QString dbTable_,QString title_,QToolBox *parrentToolBox_,int parrentToolBoxIndex_,QLabel *displayLabel_,bool canUserChangeList_,QWidget *p):QWidget(p)
{
	parrentToolBox=parrentToolBox_;
	parrentToolBoxIndex=parrentToolBoxIndex_;
	displayLabel=displayLabel_;
	dbTable=dbTable_;
	title=title_;
	canUserChangeList=canUserChangeList_;
	setWindowTitle(title);
	//selectedCassette=0;
	//hasChecks=false;
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);mainLayout->setMargin(1);
	QHBoxLayout *listLayout=new QHBoxLayout();mainLayout->addLayout(listLayout);
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);

	exportsList=new myListWidget();listLayout->addWidget(exportsList);
	exportsList->setFont(QFont("Courier "));
	if (canUserChangeList)
	{
		connect(exportsList,SIGNAL(itemSelectionChanged()),this,SLOT(exportsListSelectionChangedSlot()));
		connect(exportsList,SIGNAL(itemsRemovedSignal(QVector<int>)),this,SLOT(itemRemovedSlot(QVector<int>)));
		connect(exportsList,SIGNAL(itemDropedOnSignal(int,QString)),this,SLOT(itemDropedOnSlot(int,QString)));

		cassetteUpBtn=new QToolButton(this);btnsLayout->addWidget(cassetteUpBtn);
		cassetteUpBtn->setIcon(QIcon(":arrowUp.png"));
		cassetteUpBtn->setToolTip(tr("Move selected cassette one position up."));
		cassetteUpBtn->setIconSize(QSize(16,16));
		connect(cassetteUpBtn,SIGNAL(released()),this,SLOT(cassetteUpSlot()));
	//	cassetteUpBtn->setEnabled(false);

		cassetteDownBtn=new QToolButton(this);btnsLayout->addWidget(cassetteDownBtn);
		cassetteDownBtn->setIcon(QIcon(":arrowDown.png"));
		cassetteDownBtn->setStatusTip(tr("Move selected cassette one position down."));
		cassetteDownBtn->setIconSize(QSize(16,16));
		connect(cassetteDownBtn,SIGNAL(released()),this,SLOT(cassetteDownSlot()));
	//	cassetteDownBtn->setEnabled(false);

		cassetteAddBtn=new QToolButton(this);btnsLayout->addWidget(cassetteAddBtn);
		cassetteAddBtn->setIcon(QIcon(":add.png"));
		cassetteAddBtn->setStatusTip(tr("Add a cassette to the list."));
		cassetteAddBtn->setIconSize(QSize(16,16));
		connect(cassetteAddBtn,SIGNAL(released()),this,SLOT(cassetteAddSlot()));

		cassetteRemoveBtn=new QToolButton(this);btnsLayout->addWidget(cassetteRemoveBtn);
		cassetteRemoveBtn->setIcon(QIcon(":delete.png"));
		cassetteRemoveBtn->setStatusTip(tr("Remove selected cassette from list."));
		cassetteRemoveBtn->setIconSize(QSize(16,16));
		connect(cassetteRemoveBtn,SIGNAL(released()),this,SLOT(cassetteRemoveSlot()));
		cassetteRemoveBtn->setEnabled(false);
	}
	//
	QTimer *dbUpdateTimer=new QTimer(this);
	connect(dbUpdateTimer,SIGNAL(timeout()),this,SLOT(dbUpdateTimerSlot()));
	dbUpdateTimerSlot();
	dbUpdateTimer->start(1000);
	cassetteSelect=new cassetteSelectClass(title,dbTable,this);
	//
}
exportStationWidgetClass::~exportStationWidgetClass()
{
	delete cassetteSelect;
}
//void exportStationWidgetClass::setHasChecks(bool h)
//{
//	hasChecks=h;
//}
void exportStationWidgetClass::dbUpdateTimerSlot()
{
	updateFromDatabase();
}

void exportStationWidgetClass::resizeEvent(QResizeEvent *e)
{
	updateFromDatabase();
}
void exportStationWidgetClass::itemRemovedSlot(QVector<int> itemRemovedTIds)
{
	for (int i=0;i<itemRemovedTIds.count();i++)
		execQuery(QString("delete from %1 where tId=%2").arg(dbTable).arg(itemRemovedTIds[i]),mDb);
	updateFromDatabase();
}
void exportStationWidgetClass::itemDropedOnSlot(int itemDroppedOnTId,QString cassettes)
{
	QStringList cassetteList=cassettes.split("|");
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select %1.tId,pos.cassette,pos.posId,priority from %1 join pos on pos.cassette=%1.cassette order by priority asc").arg(dbTable),mDb);
	bool isInserted=false;
	int priority=1;
	for (int i=0;i<dataVV.count();i++)
	{
		if (dataVV[i][0].toInt()==itemDroppedOnTId)
		{
			for (int j=0;j<cassetteList.count();j++)
				execQuery(QString("insert into %1 (cassette,priority) values (%2,%3)").arg(dbTable).arg(cassetteList[i].toInt()).arg(priority++),mDb);
			isInserted=true;
		}
		execQuery(QString("update %1 set priority=%2 where tId=%3").arg(dbTable).arg(priority++).arg(dataVV[i][0].toInt()),mDb);
	}
	if (!isInserted)
	{
		for (int i=0;i<cassetteList.count();i++)
			execQuery(QString("insert into %1 (cassette,priority) values (%2,%3)").arg(dbTable).arg(cassetteList[i]).arg(priority++),mDb);
	}
	updateFromDatabase();
}
QString addSpaces(int num,int n)
{
	QString ans;
	if (num!=0)
		ans=QString("%1").arg(num);
	ans=QString("%1").arg(ans,n,' ');
	return ans;
}
void exportStationWidgetClass::updateFromDatabase()
{
	QVector<QVector<QVariant>> dataVV;
	dataVV<<execTableQuery(QString("select %1.tId,%1.cassette,pos.posId,priority,pos.ticket from %1 left join pos on pos.cassette=%1.cassette order by priority asc").arg(dbTable),mDb);
	QVector<int> selectedV;
	QList<QListWidgetItem*> selectedItems=exportsList->selectedItems();
	for (int i=0;i<selectedItems.count();i++)
		selectedV<<selectedItems[i]->data(myListWidget::roleCassette).toInt();
	exportsList->blockSignals(true);
	exportsList->clear();
	for (int i=0;i<dataVV.count();i++)
	{
		int tId=dataVV[i][0].toInt();
		int cassette=dataVV[i][1].toInt();
		int cell=dataVV[i][2].toInt();
		int priority=dataVV[i][3].toInt();
		int ticket=dataVV[i][4].toInt();
		exportsList->addItem("");
		exportsList->item(i)->setData(myListWidget::roleId,tId);
		exportsList->item(i)->setData(myListWidget::roleCassette,cassette);
		QString toolStr;
		toolStr+="<html><body bgcolor=\"#E6E6FA\"><font style='color:#333333'>"+tr("Cassette number:")+"</font><font style='color:#000077'>"+QString("%1").arg(cassette)+"</font><br>";
		toolStr+=tr("Located in cell:")+"</font><font style='color:#000077'>"+QString("%1").arg(cell)+"</font><br>";
		toolStr+=tr("Priority:")+"</font><font style='color:#000077'>"+QString("%1").arg(priority)+"</font>";
			toolStr+="</body></html>";
		exportsList->item(i)->setText(tr("%1\t%2\t(in %3)").arg(cassette).arg(ticket).arg(cell));
		exportsList->item(i)->setToolTip(toolStr);
		if (selectedV.contains(cassette))
			exportsList->setItemSelected(exportsList->item(i),true);
	}
	if (canUserChangeList)
	{
		//btns
		cassetteUpBtn->setEnabled(false);
		cassetteDownBtn->setEnabled(false);
		cassetteRemoveBtn->setEnabled(false);
	}
	if (exportsList->count()==0)
		exportsList->addItem(tr("No Cassettes\nto export"));
	QString displayStr=QString("%1%2").arg(title).arg(dataVV.count()>0?tr(" (%1 cassettes)").arg(dataVV.count()):"");
	if (parrentToolBox)
		parrentToolBox->setItemText(parrentToolBoxIndex,displayStr);
	if (displayLabel)
		displayLabel->setText(displayStr);
	exportsList->blockSignals(false);
	updateGui();
}
void exportStationWidgetClass::updateGui()
{
	QList<QListWidgetItem*> selectedItems=exportsList->selectedItems();
	cassetteUpBtn->setEnabled((selectedItems.count()>0));
	cassetteDownBtn->setEnabled((selectedItems.count()>0));
	cassetteRemoveBtn->setEnabled((selectedItems.count()>0));
}
void exportStationWidgetClass::exportsListSelectionChangedSlot()
{
	updateGui();
}
QVector<int> insertToVector(QVector<int> all,QVector<int> ins,int index)
{
	QVector<int> ans;
	for (int i=0;i<ins.count();i++)
		all.removeAll(ins[i]);
	if (index<=0)
	{
		ans<<ins;
		ans<<all;
		return ans;
	}
	if (index>=(all.count()))
	{
		ans<<all;
		ans<<ins;
		return ans;
	}
	for (int i=0;i<all.count();i++)
	{
		if (i==index)
			ans<<ins;
		ans<<all[i];
	}
	return ans;
}
void exportStationWidgetClass::cassetteUpSlot()
{
	QList<QListWidgetItem*> selectedItems=exportsList->selectedItems();
	if (selectedItems.count()==0)
		return;
	QVector<int> selectedV;
	for (int i=0;i<selectedItems.count();i++)
		selectedV<<selectedItems[i]->data(myListWidget::roleCassette).toInt();
	QVector<int> allV;
	for (int i=0;i<exportsList->count();i++)
		allV<<exportsList->item(i)->data(myListWidget::roleCassette).toInt();
	QVector<int> newV=insertToVector(allV,selectedV,allV.indexOf(selectedV[0])-1);
	execQuery(QString("delete from %1 ").arg(dbTable),mDb);
	for (int i=0;i<newV.count();i++)
		execQuery(QString("insert into %1 (cassette,priority) values(%2,%3)").arg(dbTable).arg(newV[i]).arg(i),mDb);
	updateFromDatabase();
}
void exportStationWidgetClass::cassetteDownSlot()
{
	QList<QListWidgetItem*> selectedItems=exportsList->selectedItems();
	if (selectedItems.count()==0)
		return;
	QVector<int> selectedV;
	for (int i=0;i<selectedItems.count();i++)
		selectedV<<selectedItems[i]->data(myListWidget::roleCassette).toInt();
	QVector<int> allV;
	for (int i=0;i<exportsList->count();i++)
		allV<<exportsList->item(i)->data(myListWidget::roleCassette).toInt();
	QVector<int> newV=insertToVector(allV,selectedV,allV.indexOf(selectedV[0])+1);
	execQuery(QString("delete from %1 ").arg(dbTable),mDb);
	for (int i=0;i<newV.count();i++)
		execQuery(QString("insert into %1 (cassette,priority) values(%2,%3)").arg(dbTable).arg(newV[i]).arg(i),mDb);
	updateFromDatabase();

}
void exportStationWidgetClass::cassetteAddSlot()
{
	cassetteSelect->exec();
	updateFromDatabase();
}
void exportStationWidgetClass::cassetteRemoveSlot()
{
	QVector<int> selectedV;
	QList<QListWidgetItem*> selectedItems=exportsList->selectedItems();
	for (int i=0;i<selectedItems.count();i++)
		selectedV<<selectedItems[i]->data(myListWidget::roleCassette).toInt();
	for (int i=0;i<selectedV.count();i++)
	{
		int selectedCassette=selectedV[i];
		execQuery(QString("delete from %1 where cassette=%2").arg(dbTable).arg(selectedCassette),mDb);
	}
	updateFromDatabase();
}
