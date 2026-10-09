#include "selBas.h"
selectBasketClass::selectBasketClass(int stationTypeEnum_,basketListClass *parentList_,QWidget *w):QDialog(w)
{
	resize(1200,600);
	stationTypeEnum=stationTypeEnum_;
	parentList=parentList_;
	setWindowTitle(tr("Select a basket for %1.").arg(stationTypeEnum==TYPE_OMAV?tr("Left staion"):tr("Right staion")));
	//
	selectedBasketNumber=0;
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	QHBoxLayout *upLayout=new QHBoxLayout();mainLayout->addLayout(upLayout);
	QHBoxLayout *midLayout=new QHBoxLayout();mainLayout->addLayout(midLayout);mainLayout->setStretch(1,1);
	QHBoxLayout *downLayout=new QHBoxLayout();mainLayout->addLayout(downLayout);
	//up
	upLayout->addWidget(new QLabel(tr("List of baskets:")));
	//mid
		QWidget *leftW=new QWidget(this);
		QVBoxLayout *leftLayout=new QVBoxLayout();leftW->setLayout(leftLayout);
		//list
		listView=new QListWidget(this);
		listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		listView->setSelectionMode(QAbstractItemView::SingleSelection);
		listView->setAlternatingRowColors(true);
		connect(listView,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(listSelectionChanged(QListWidgetItem*,QListWidgetItem*)));
		leftLayout->addWidget(listView);
		QGridLayout *filterLayout=new QGridLayout();leftLayout->addLayout(filterLayout);
		//profileFilter
		{
			profileFilterLineEdit=new QLineEdit("",this);
			profileFilterLineEdit->setToolTip(tr("If the filter text box is empty all basket in  buffer are been seen\nIf a number is writen the baskets with this profile are been seen"));
			connect(profileFilterLineEdit,SIGNAL(textEdited(const QString &)),this,SLOT(profileFilterTextChangedSlot(const QString&)));
			filterLayout->addWidget(new QLabel(tr("Die number")),0,0);
			filterLayout->addWidget(profileFilterLineEdit,0,1);
			//QCompleter
			QSqlQuery profileFilterQuery(QSqlDatabase::database(APEX_DB_CONNECTION));
			if (!profileFilterQuery.exec("select distinct dienum from basket_details"))
				EXIT("Error in database",profileFilterQuery.lastError().text());
			//
			QStringList profileStringList;
			while (profileFilterQuery.next())
				profileStringList.append(profileFilterQuery.value(0).toString().trimmed());
			QCompleter *profileCompleter=new QCompleter(profileStringList,this);
			profileFilterLineEdit->setCompleter(profileCompleter);
			connect(profileCompleter,SIGNAL(activated(const QString&)),this,SLOT(profileFilterTextChangedSlot(const QString&)));
			connect(profileCompleter,SIGNAL(highlighted(const QString&)),this,SLOT(profileFilterTextChangedSlot(const QString&)));
		}
		//orderFilter
		{
			orderFilterLineEdit=new QLineEdit("",this);
			orderFilterLineEdit->setToolTip(tr("If the filter text box is empty all basket in  buffer are been seen\nIf a number is writen the baskets with this Order are been seen"));
			connect(orderFilterLineEdit,SIGNAL(textEdited(const QString &)),this,SLOT(orderFilterTextChangedSlot(const QString&)));
			filterLayout->addWidget(new QLabel(tr("Order")),1,0);
			filterLayout->addWidget(orderFilterLineEdit,1,1);
			//QCompleter
			QSqlQuery orderFilterQuery(QSqlDatabase::database(APEX_DB_CONNECTION));
			if (!orderFilterQuery.exec("select distinct SONum from basket_details"))
				EXIT("Error in database",orderFilterQuery.lastError().text());
			//
			QStringList orderStringList;
			while (orderFilterQuery.next())
				orderStringList.append(orderFilterQuery.value(0).toString().trimmed());
			QCompleter *orderCompleter=new QCompleter(orderStringList,this);
			orderFilterLineEdit->setCompleter(orderCompleter);
			connect(orderCompleter,SIGNAL(activated(const QString&)),this,SLOT(orderFilterTextChangedSlot(const QString&)));
			connect(orderCompleter,SIGNAL(highlighted(const QString&)),this,SLOT(orderFilterTextChangedSlot(const QString&)));
		}
		//table
		tableView=new QTableWidget();
		tableView->setAlternatingRowColors(true);
		tableView->horizontalHeader()->setStretchLastSection(true);
		tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tableView->setColumnCount(6);
		QStringList h;h<<tr("Profile")<<tr("Length")<<tr("Quantity")<<tr("SONum")<<tr("SOItemNum")<<tr("Lot");
		tableView->setHorizontalHeaderLabels(h);
		//splitter
		QSplitter *mainSplitter=new QSplitter(Qt::Horizontal);
		mainSplitter->addWidget(leftW);
		mainSplitter->addWidget(tableView);
		mainSplitter->setStretchFactor(1,1);
		QList<int> sizes;sizes<<450<<700;
		mainSplitter->setSizes(sizes);
		midLayout->addWidget(mainSplitter,1);
	//down
	downLayout->addStretch(1);
	cancelButton=new QToolButton(this);
	//cancelButton->setText(tr("Cancel"));
	cancelButton->setIcon(QIcon(":delete.png"));
	cancelButton->setIconSize(QSize(75,75));
	cancelButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	cancelButton->setStatusTip(tr("Cancel"));
	cancelButton->setToolTip(tr("Cancel"));
	connect(cancelButton,SIGNAL(released()),this,SLOT(reject()));
	downLayout->addWidget(cancelButton);

	addSelectedButton=new QToolButton(this);
	//addSelectedButton->setText(tr("Add selected basket"));
	addSelectedButton->setIcon(QIcon(":add.png"));
	addSelectedButton->setIconSize(QSize(75,75));
	addSelectedButton->setStatusTip(tr("Adds the selected basket in the list."));
	addSelectedButton->setToolTip(tr("Adds the selected basket in the list."));
	connect(addSelectedButton,SIGNAL(released()),this,SLOT(addSelectedSlot()));
	downLayout->addWidget(addSelectedButton);
	//
	updateList("","");
}
void selectBasketClass::updateList(QString profileFilter,QString orderFilter)
{
	QString filter=QString("");
	if ((orderFilter.length()>0)  && (profileFilter.isEmpty()))
		filter+=QString(" where SONum like '%1%' ").arg(orderFilter);
	if ((orderFilter.isEmpty()) && (profileFilter.length()>0))
		filter+=QString(" where dienum like '%1%' ").arg(profileFilter);
	if ((orderFilter.length()>0) && (profileFilter.length()>0))
		filter+=QString(" where dienum like '%1%' and SONum like '%2%' ").arg(profileFilter).arg(orderFilter);

	QVector<QVariant> allIdleBaskets;
	//QVector<int> basketsOnTopArray;
	QVector<QVariant> allBasketsWithFilteredContents=apexExecQuery(QString("select DISTINCT  basketnum from basket_details %1").arg(filter),APEX_DB_CONNECTION);
	for (int i=0;i<allBasketsWithFilteredContents.count();i++)
	{
		QVariant time=GETFIRST(QString("select audittime from basket_location where basketnum=%1 and basketlocation=%2").arg(allBasketsWithFilteredContents[i].toString()).arg(3),APEX_DB_CONNECTION);
		if (time.isNull())
			continue;
		if (stationTypeEnum==TYPE_ASSIM)
		{
			int maxLength=GETONE(QString("select max(PcLen) from basket_details where basketnum=%1").arg(allBasketsWithFilteredContents[i].toString()),APEX_DB_CONNECTION).toInt();
			if (maxLength>7365.2)
				continue;
		}
		allIdleBaskets<<apexExecQuery(QString("select basketTableId from positions where (posNumber=160 or posNumber=200 or description='24 feet' or description='30 feet' or description like 'empty%') and targetNumber=0 and basketTableId <>0 and basketTableId not in(select basketTableId from toAssimakisExports) and basketTableId not in(select basketTableId from toOmavExports) and basketTableId=%1").arg(allBasketsWithFilteredContents[i].toString()));
	}
	//short last first in first out
	QVector<QVariant> allBasketsArray=apexExecQuery(QString("select pid from baskets order by secFilled"));
	QVector<QVariant> filledSecArray=apexExecQuery(QString("select secFilled from baskets order by secFilled"));
	listView->clear();
	for (int i=0;i<allBasketsArray.count();i++)
	{
		int index=allIdleBaskets.indexOf(allBasketsArray[i]);
		if (index!=-1)
		{
				int maxLength=GETONE(QString("select max(PcLen) from basket_details where basketnum=%1").arg(allBasketsArray[i].toString()),APEX_DB_CONNECTION).toInt();
				QVariant basketNumber=GETONE(QString("select number from baskets where pid=%1").arg(allIdleBaskets[index].toString()));
				QString lenString=maxLength>7365.2?">24'":"<24'";
				if (filledSecArray[i].toLongLong()>0)
					listView->addItem(basketNumber.toString()+QDateTime().fromMSecsSinceEpoch(filledSecArray[i].toLongLong()*1000).toString("  (yyyy-mm-dd hh:mm:ss)")+lenString);
				else
					listView->addItem(basketNumber.toString()+"  (yyyy-mm-dd hh:mm:ss)"+lenString);
				listView->item(listView->count()-1)->setData(Qt::UserRole,basketNumber);
				if (maxLength>7365.2)
				{
					listView->item(listView->count()-1)->setBackground(QBrush(QColor(255,230,230)));
					listView->item(listView->count()-1)->setForeground(QBrush(QColor(255,100,100)));
				}
		}
	}
	//short find the number of baskets on top
	//for (int i=0;i<basketsIdArray.count();i++)
	//	basketsOnTopArray<<GETONE(QString("select count(basketTableId) from positions where basketTableId<>0 and posNumber in (select posNumber from positions where basketTableId=%1) and posIndex>(select posIndex from positions where basketTableId=%1) and basketTableId not in (select basketTableId from toAssimakisExports) and basketTableId not in (select basketTableId from toOmavExports) ").arg(basketsIdArray[i].toString())).toInt();
	//listView->clear();
	//for (int i=0;i<=3;i++)
	//{
	//	for (int j=0;j<basketsIdArray.count();j++)
	//	{
	//		if (basketsOnTopArray[j]==i)
	//		{
	//			QVariant basketNumber=GETONE(QString("select number from baskets where pid=%1").arg(basketsIdArray[j].toString()));
	//			listView->addItem(basketNumber.toString());
	//			listView->item(listView->count()-1)->setData(Qt::UserRole,basketNumber);
	//		}
	//	}
	//}

	if (listView->count()>0)
	{
		listView->item(0)->setSelected(true);
		listView->setCurrentRow(0);
		updateTable();
	}
	else
	{
		tableView->clearContents();
		tableView->setRowCount(0);
	}
}
void selectBasketClass::updateTable()
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

			tableView->setItem(i,1,new QTableWidgetItem(QString().number(lengthArray[i].toInt())));
			tableView->item(i,1)->setTextAlignment(Qt::AlignCenter);
			tableView->setItem(i,2,new QTableWidgetItem(quantityArray[i].toString()));
			tableView->item(i,2)->setTextAlignment(Qt::AlignCenter);
			tableView->setItem(i,3,new QTableWidgetItem(QString().number(SONumArray[i].toInt())));
			tableView->item(i,3)->setTextAlignment(Qt::AlignCenter);
			tableView->setItem(i,4,new QTableWidgetItem(QString().number(SOItemArray[i].toInt())));
			tableView->item(i,4)->setTextAlignment(Qt::AlignCenter);
			tableView->setItem(i,5,new QTableWidgetItem(QString().number(LotArray[i].toInt())));
			tableView->item(i,5)->setTextAlignment(Qt::AlignCenter);
		tableView->setRowHeight(i,100);

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
//////////////////////////////////////////////////////slots
void selectBasketClass::listSelectionChanged(QListWidgetItem* selected,QListWidgetItem* deselected)
{
	selectedBasketNumber=selected?selected->data(Qt::UserRole).toInt():0;
	updateTable();

}

void selectBasketClass::addSelectedSlot()
{
	if (selectedBasketNumber)
	{
		apexExecQuery(QString("insert into %1 (basketTableId,priority) values(%2,%3)")
			.arg(stationTypeEnum==TYPE_ASSIM?"toAssimakisExports":"toOmavExports")
			.arg(GETONE(QString("select pid from baskets where number=%1").arg(selectedBasketNumber)).toInt())
			.arg(GETONEATMOST(QString("SELECT TOP 1 priority FROM %1 order by priority desc").arg(stationTypeEnum==TYPE_ASSIM?"toAssimakisExports":"toOmavExports")).toInt()+1)
			);
	}
	updateList(profileFilterLineEdit->text(),orderFilterLineEdit->text());
	parentList->updateList();
	parentList->update();
}
void selectBasketClass::profileFilterTextChangedSlot(const QString& str)
{
	updateList(str,orderFilterLineEdit->text());
}
void selectBasketClass::orderFilterTextChangedSlot(const QString& str)
{
	updateList(profileFilterLineEdit->text(),str);
}
