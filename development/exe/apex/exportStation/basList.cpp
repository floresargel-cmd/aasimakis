#include "basList.h"
basketListClass::basketListClass(int stationTypeEnum_,QWidget *w):QWidget(w)
{
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Minimum);
	stationTypeEnum=stationTypeEnum_;
	selectedBasketNumber=0;
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	QSplitter *mainSplitter=new QSplitter(Qt::Horizontal);
	QVBoxLayout *leftLayout=new QVBoxLayout();QWidget *leftW=new QWidget(this);mainSplitter->addWidget(leftW);leftW->setLayout(leftLayout);
	QVBoxLayout *rightLayout=new QVBoxLayout();QWidget *rightW=new QWidget(this);mainSplitter->addWidget(rightW);rightW->setLayout(rightLayout);

	QList<int> s;s<<50<<300;
	mainSplitter->setSizes(s);

	mainLayout->addWidget(new QLabel(tr("List of baskets:")));
	mainLayout->addWidget(mainSplitter);
	//leftLayout
	listView=new QListWidget(this);leftLayout->addWidget(listView);
	listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	listView->setAlternatingRowColors(true);
	connect(listView,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(listSelectionChanged(QListWidgetItem*,QListWidgetItem*)));
	QVBoxLayout *btnsLayout=new QVBoxLayout();leftLayout->addLayout(btnsLayout);
	QHBoxLayout *btnsUpLayout=new QHBoxLayout();btnsLayout->addLayout(btnsUpLayout);
	QHBoxLayout *btnsDownLayout=new QHBoxLayout();btnsLayout->addLayout(btnsDownLayout);

	basketUpButton=new QToolButton(this);btnsUpLayout->addWidget(basketUpButton);
	basketUpButton->setIcon(QIcon(":arrowUp.png"));
	basketUpButton->setStatusTip(tr("Move selected basket one position up."));
	basketUpButton->setToolTip(tr("Move selected basket one position up."));
	basketUpButton->setIconSize(QSize(32,32));
	connect(basketUpButton,SIGNAL(released()),this,SLOT(basketUpSlot()));
	basketUpButton->setEnabled(false);

	basketDownButton=new QToolButton(this);btnsUpLayout->addWidget(basketDownButton);
	basketDownButton->setIcon(QIcon(":arrowDown.png"));
	basketDownButton->setStatusTip(tr("Move selected basket one position down."));
	basketDownButton->setToolTip(tr("Move selected basket one position down."));
	basketDownButton->setIconSize(QSize(32,32));
	connect(basketDownButton,SIGNAL(released()),this,SLOT(basketDownSlot()));
	basketDownButton->setEnabled(false);

	basketRemoveButton=new QToolButton(this);btnsDownLayout->addWidget(basketRemoveButton);
	basketRemoveButton->setIcon(QIcon(":delete.png"));
	basketRemoveButton->setStatusTip(tr("Remove selected basket from list."));
	basketRemoveButton->setToolTip(tr("Remove selected basket from list."));
	basketRemoveButton->setIconSize(QSize(32,32));
	connect(basketRemoveButton,SIGNAL(released()),this,SLOT(basketRemoveSlot()));
	basketRemoveButton->setEnabled(false);

	basketAddButton=new QToolButton(this);btnsDownLayout->addWidget(basketAddButton);
	basketAddButton->setIcon(QIcon(":add.png"));
	basketAddButton->setStatusTip(tr("Add a basket in the list."));
	basketAddButton->setToolTip(tr("Add a basket in the list."));
	basketAddButton->setIconSize(QSize(32,32));
	connect(basketAddButton,SIGNAL(released()),this,SLOT(basketAddSlot()));

	//rightLayout
	tableView=new QTableWidget();
	tableView->setAlternatingRowColors(true);
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->setColumnCount(6);
	QStringList h;h<<tr("Profile")<<tr("Length")<<tr("Quantity")<<tr("SONum")<<tr("SOItemNum")<<tr("Lot");
	tableView->setHorizontalHeaderLabels(h);
	rightLayout->addWidget(tableView);
	//
	updateGui();
}
void basketListClass::updateGui()
{
	updateList();
	updateTable();
}
void basketListClass::updateList(bool forceUpdate)
{
	QVector<QVariant> basketsTableIdsArray;
	if (stationTypeEnum==TYPE_OMAV)
		basketsTableIdsArray=apexExecQuery(QString("select basketTableId from toOmavExports order by priority"));
	else
		basketsTableIdsArray=apexExecQuery(QString("select basketTableId from toAssimakisExports order by priority"));
	if (
		(listView->count()!=basketsTableIdsArray.count())
		||
		((listView->count()==1)&&(listView->item(0)->text()=="Empty"))
		||
		(forceUpdate)
		)
	{
		int selectedBasketNumberTemp=selectedBasketNumber;
		listView->clear();//selectedBasketNumber becomes zero;
		selectedBasketNumber=selectedBasketNumberTemp;
		for (int i=0;i<basketsTableIdsArray.count();i++)
		{
			QVariant bNumber=GETONE(QString("select number from baskets where pid=%1").arg(basketsTableIdsArray[i].toString()));
			listView->addItem(bNumber.toString());
			listView->item(i)->setData(Qt::UserRole,bNumber.toInt());
			if (selectedBasketNumber==bNumber.toInt())
			{
				listView->item(i)->setSelected(true);
				listView->setCurrentRow(i);
			}
		}
	}
/////////////////
	//
	basketUpButton->setEnabled(false);
	basketDownButton->setEnabled(false);
	if (selectedBasketNumber!=0)
	{
		if (listView->currentRow()>0)
			basketUpButton->setEnabled(true);
		if (listView->currentRow()!=(listView->count()-1))
			basketDownButton->setEnabled(true);
	}
	if (basketsTableIdsArray.count()==0)
		listView->addItem(tr("Empty"));
}
void basketListClass::updateTable()
{
	if (selectedBasketNumber!=0)
	{
		int selectedBasketTableId=GETONE(QString("select pid from baskets where number=%1").arg(selectedBasketNumber)).toInt();
		QVector<QVariant> profilesArray=apexExecQuery(QString("select dienum from basket_details where basketnum=%1 order by sequence desc").arg(selectedBasketTableId),APEX_DB_CONNECTION);
		QVector<QVariant> quantityArray=apexExecQuery(QString("select Pc from basket_details where basketnum=%1 order by sequence desc").arg(selectedBasketTableId),APEX_DB_CONNECTION);
		QVector<QVariant> lengthArray=apexExecQuery(QString("select PcLen from basket_details where basketnum=%1 order by sequence desc").arg(selectedBasketTableId),APEX_DB_CONNECTION);
		QVector<QVariant> SONumArray=apexExecQuery(QString("select SONum from basket_details where basketnum=%1 order by sequence desc").arg(selectedBasketTableId),APEX_DB_CONNECTION);
		QVector<QVariant> SOItemArray=apexExecQuery(QString("select SOItemNum from basket_details where basketnum=%1 order by sequence desc").arg(selectedBasketTableId),APEX_DB_CONNECTION);
		QVector<QVariant> LotArray=apexExecQuery(QString("select Lot from basket_details where basketnum=%1 order by sequence desc").arg(selectedBasketTableId),APEX_DB_CONNECTION);
		tableView->clearContents();
		tableView->setRowCount(profilesArray.count());
		for (int i=0;i<profilesArray.count();i++)
		{
			QWidget *profileWidget=new QWidget(this);
			QHBoxLayout *profileWidgetLayout=new QHBoxLayout();
			profileWidget->setLayout(profileWidgetLayout);
			QToolButton *profileButton=new QToolButton(this);profileButton->setStyleSheet("QToolButton{font-size:16px;text-align:center;}");

			profileButton->setIcon(QIcon(getPicFileNameFromProfile(profilesArray[i].toInt())));
			profileButton->setIconSize(QSize(30,30));

			profileWidgetLayout->addWidget(profileButton,1,Qt::AlignHCenter);
			profileWidgetLayout->addWidget(new QLabel(profilesArray[i].toString()),1,Qt::AlignHCenter);
			tableView->setCellWidget(i,0,profileWidget);

			tableView->setItem(i,1,new QTableWidgetItem(lengthArray[i].toString()));
			tableView->item(i,1)->setTextAlignment(Qt::AlignCenter);
			tableView->setItem(i,2,new QTableWidgetItem(quantityArray[i].toString()));
			tableView->item(i,2)->setTextAlignment(Qt::AlignCenter);
			tableView->setItem(i,3,new QTableWidgetItem(SONumArray[i].toString()));
			tableView->item(i,3)->setTextAlignment(Qt::AlignCenter);
			tableView->setItem(i,4,new QTableWidgetItem(QString().number(SOItemArray[i].toInt())));
			tableView->item(i,4)->setTextAlignment(Qt::AlignCenter);
			tableView->setItem(i,5,new QTableWidgetItem(QString().number(LotArray[i].toInt())));
			tableView->item(i,5)->setTextAlignment(Qt::AlignCenter);

			tableView->setRowHeight(i,125);
		}
		tableView->resizeColumnsToContents();
		tableView->horizontalHeader()->setStretchLastSection(true);
	}
	else
	{
		tableView->clearContents();
		tableView->setRowCount(0);
	}
}
void basketListClass::lockGui()
{
	tableView->setDisabled(true);
	listView->setDisabled(true);
}
void basketListClass::unLockGui()
{
	tableView->setDisabled(false);
	listView->setDisabled(false);
}
//////////////////////////////////////////////////////slots
void basketListClass::listSelectionChanged(QListWidgetItem* selected,QListWidgetItem* deselected)
{
	selectedBasketNumber=selected?selected->data(Qt::UserRole).toInt():0;
	updateTable();
	//
	basketUpButton->setEnabled(false);
	basketDownButton->setEnabled(false);
	basketRemoveButton->setEnabled(false);
	if (selectedBasketNumber!=0)
	{
		basketRemoveButton->setEnabled(true);
		if (listView->currentRow()>0)
			basketUpButton->setEnabled(true);
		if (listView->currentRow()!=(listView->count()-1))
			basketDownButton->setEnabled(true);
	}
}
void basketListClass::basketUpSlot()
{
	QVector<QVariant> basketsArray;
	if (stationTypeEnum==TYPE_OMAV)
		basketsArray=apexExecQuery(QString("select basketTableId from toOmavExports order by priority"));
	else
		basketsArray=apexExecQuery(QString("select basketTableId from toAssimakisExports order by priority"));
	if (listView->count()!=basketsArray.count())
		return;
	for (int i=0;i<listView->count();i++)
	{
		if (listView->item(i)->data(Qt::UserRole)==selectedBasketNumber)
		{
			int selectedBasketTableId=GETONE(QString("select pid from baskets where number=%1").arg(selectedBasketNumber)).toInt();
			//priorityOfcurrent
			int priorityOfcurrent;
			if (stationTypeEnum==1)
				priorityOfcurrent=GETONE(QString("select priority from toOmavExports where basketTableId=%1").arg(selectedBasketTableId)).toInt();
			else
				priorityOfcurrent=GETONE(QString("select priority from toAssimakisExports where basketTableId=%1").arg(selectedBasketTableId)).toInt();
			//otherCassette
			int otherBasketNumber=listView->item(i-1)->text().toInt();
			int otherBasketTableId=GETONE(QString("select pid from baskets where number=%1").arg(otherBasketNumber)).toInt();
			//priorityOfOther
			int priorityOfOther;
			if (stationTypeEnum==1)
				priorityOfOther=GETONE(QString("select priority from toOmavExports where basketTableId=%1").arg(otherBasketTableId)).toInt();
			else
				priorityOfOther=GETONE(QString("select priority from toAssimakisExports where basketTableId=%1").arg(otherBasketTableId)).toInt();
			//
			if (stationTypeEnum==TYPE_OMAV)
			{
				apexExecQuery(QString("update toOmavExports set priority=%1 where basketTableId=%2").arg(priorityOfcurrent).arg(otherBasketTableId));
				apexExecQuery(QString("update toOmavExports set priority=%1 where basketTableId=%2").arg(priorityOfOther).arg(selectedBasketTableId));
			}
			else
			{
				apexExecQuery(QString("update toAssimakisExports set priority=%1 where basketTableId=%2").arg(priorityOfcurrent).arg(otherBasketTableId));
				apexExecQuery(QString("update toAssimakisExports set priority=%1 where basketTableId=%2").arg(priorityOfOther).arg(selectedBasketTableId));
			}
			updateList(true);
			break;
		}
	}
}
void basketListClass::basketDownSlot()
{
	QVector<QVariant> basketsArray;
	if (stationTypeEnum==TYPE_OMAV)
		basketsArray=apexExecQuery(QString("select basketTableId from toOmavExports order by priority"));
	else
		basketsArray=apexExecQuery(QString("select basketTableId from toAssimakisExports order by priority"));
	if (listView->count()!=basketsArray.count())
		return;
	for (int i=0;i<listView->count();i++)
	{
		if (listView->item(i)->data(Qt::UserRole)==selectedBasketNumber)
		{
			int selectedBasketTableId=GETONE(QString("select pid from baskets where number=%1").arg(selectedBasketNumber)).toInt();
			//priorityOfcurrent
			int priorityOfcurrent;
			if (stationTypeEnum==1)
				priorityOfcurrent=GETONE(QString("select priority from toOmavExports where basketTableId=%1").arg(selectedBasketTableId)).toInt();
			else
				priorityOfcurrent=GETONE(QString("select priority from toAssimakisExports where basketTableId=%1").arg(selectedBasketTableId)).toInt();
			//otherCassette
			int otherBasketNumber=listView->item(i+1)->text().toInt();
			int otherBasketTableId=GETONE(QString("select pid from baskets where number=%1").arg(otherBasketNumber)).toInt();
		//priorityOfOther
			int priorityOfOther;
			if (stationTypeEnum==1)
				priorityOfOther=GETONE(QString("select priority from toOmavExports where basketTableId=%1").arg(otherBasketTableId)).toInt();
			else
				priorityOfOther=GETONE(QString("select priority from toAssimakisExports where basketTableId=%1").arg(otherBasketTableId)).toInt();
			//
			if (stationTypeEnum==TYPE_OMAV)
			{
				apexExecQuery(QString("update toOmavExports set priority=%1 where basketTableId=%2").arg(priorityOfcurrent).arg(otherBasketTableId));
				apexExecQuery(QString("update toOmavExports set priority=%1 where basketTableId=%2").arg(priorityOfOther).arg(selectedBasketTableId));
			}
			else
			{
				apexExecQuery(QString("update toAssimakisExports set priority=%1 where basketTableId=%2").arg(priorityOfcurrent).arg(otherBasketTableId));
				apexExecQuery(QString("update toAssimakisExports set priority=%1 where basketTableId=%2").arg(priorityOfOther).arg(selectedBasketTableId));
			}
			updateList(true);
			break;
		}
	}
}
void basketListClass::basketRemoveSlot()
{
	if (stationTypeEnum==TYPE_OMAV)
		apexExecQuery(QString("delete from toOmavExports where basketTableId in (select pid from baskets where number=%1)").arg(selectedBasketNumber));
	else
		apexExecQuery(QString("delete from toAssimakisExports where basketTableId in (select pid from baskets where number=%1)").arg(selectedBasketNumber));
	updateList();
}
void basketListClass::basketAddSlot()
{
	selectBasketClass *selBas=new selectBasketClass(stationTypeEnum,this,this);
	selBas->exec();
	delete selBas;
}
