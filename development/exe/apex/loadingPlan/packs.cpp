
#include "packs.h"
//! [0]
packetsClass::packetsClass(QWidget *p):QWidget(p)
{
	totalWeightLabelLabel=new QLabel(tr("total weight"));
	totalWeightLabel=new QLabel("--");
	loadingPlan=NULL;
	activeLoading=-1;
	//
	packetsModel=new QStandardItemModel();
	initialize();

	packetsTableView=new QTableView();
	packetsTableView->setModel(packetsModel);
	packetsTableView->horizontalHeader()->setStretchLastSection(true);
	//
	QGridLayout *statusLayout=new QGridLayout;
	statusLayout->addWidget(totalWeightLabelLabel,0,0);
	statusLayout->addWidget(totalWeightLabel,0,1);
	statusLayout->addWidget(new QLabel(tr("Kgr")),0,2);
	statusLayout->setColumnStretch(3,1);
	//
	QVBoxLayout *mainLayout=new QVBoxLayout(this);
	mainLayout->addWidget(packetsTableView);
	mainLayout->addLayout(statusLayout);
	//
	updateVisual();
	connect(packetsModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(tableChanged(QStandardItem*)));
}
void packetsClass::setLoadingPlan(loadingPlanClass *lp)
{
	loadingPlan=lp;
}
void packetsClass::tableChanged(QStandardItem* item)
{
	////file
	//QFile file(fileNameString);
	//file.open(QIODevice::WriteOnly|QIODevice::Text);
	//QTextStream stream(&file);
	//stream<<"code	width	height	length	weight	type	orderid\n";
	//for (int i=0;i<packetsModel->rowCount();i++)
	//{
	//	stream<<packetsModel->data(packetsModel->index(i,0),Qt::DisplayRole).toString();stream<<"\t";
	//	stream<<packetsModel->data(packetsModel->index(i,1),Qt::DisplayRole).toString();stream<<"\t";
	//	stream<<packetsModel->data(packetsModel->index(i,2),Qt::DisplayRole).toString();stream<<"\t";
	//	stream<<packetsModel->data(packetsModel->index(i,3),Qt::DisplayRole).toString();stream<<"\t";
	//	stream<<packetsModel->data(packetsModel->index(i,4),Qt::DisplayRole).toString();stream<<"\t";
	//	stream<<packetsModel->data(packetsModel->index(i,5),Qt::DisplayRole).toString();stream<<"\t";
	//	stream<<packetsModel->data(packetsModel->index(i,6),Qt::DisplayRole).toString();stream<<"\n";
	//}
	//file.close();
	////vector
	//int row=item->row();
	//int col=item->column();
	//if (col==0)
	//	packetsVector[row]->setCode(packetsModel->data(packetsModel->index(row,col),Qt::DisplayRole).toInt());
	//else if (col==1)
	//	packetsVector[row]->setWidth(packetsModel->data(packetsModel->index(row,col),Qt::DisplayRole).toInt());
	//else if (col==2)
	//	packetsVector[row]->setHeight(packetsModel->data(packetsModel->index(row,col),Qt::DisplayRole).toInt());
	//else if (col==3)
	//	packetsVector[row]->setLength(packetsModel->data(packetsModel->index(row,col),Qt::DisplayRole).toInt());
	//else if (col==4)
	//	packetsVector[row]->setWeight(packetsModel->data(packetsModel->index(row,col),Qt::DisplayRole).toInt());
	//else if (col==5)
	//	packetsVector[row]->setTopPlacementPriority(packetsModel->data(packetsModel->index(row,col),Qt::DisplayRole).toInt());
	//else if (col==6)
	//	packetsVector[row]->setOrderId(packetsModel->data(packetsModel->index(row,col),Qt::DisplayRole).toInt());
	////
	//updateVisual();
	//if (loadingPlan)
	//	loadingPlan->calculate();
}
void packetsClass::initialize()
{
	QVector<QColor> orderColors;
	orderColors.append(QColor(199,189,174,100));
	orderColors.append(QColor(178,118,104,100));
	orderColors.append(QColor(158,47,34,100));
	orderColors.append(QColor(148,12,0,100));
	orderColors.append(QColor(188,153,139,100));
	orderColors.append(QColor(168,82,69,100));
	//
	QVector<QVariant> trucksArray=execQuery(QString("select distinct usernumeric1 from PickList where usernumeric1 is not NULL and status='A'"),eDb);
	if (trucksArray.count()==0)
	{
		EXIT(tr("Error in database"),tr("There are no orders in table: PickList\nquery:select distinct usernumeric1 from PickList where usernumeric1 is not NULL and status='A'"));
	}
	QStringList trucksList;
	for (int i=0;i<trucksArray.count();i++)
		trucksList<<trucksArray[i].toString()+" "+GETFIRST(QString("select TruckID from PickList where usernumeric1=%1 and status='A'").arg(trucksArray[i].toInt()),eDb).toString()+" "+GETFIRST(QString("select DtToShip from PickList where usernumeric1=%1").arg(trucksArray[i].toInt()),eDb).toString();
	bool ok;QString selectedTruck=QInputDialog::getItem(this,tr("Loading selection"),tr("select a loading"),trucksList,trucksList.count()-1,false,&ok);
	QList<QStandardItem*> row;
	QStringList noDimentionsCodesList;
	QStringList notInStoreCodesList;
	if (ok && !selectedTruck.isEmpty())
	{
		activeLoading=selectedTruck.split(" ")[0].toInt();
		packetsModel->clear();
		packetsVector.clear();
		QVector<QVariant> ordersArray=execQuery(QString("select PickListNum from PickList where usernumeric1=%1 and status='A'").arg(activeLoading),eDb);
		QVector<QVariant> dropOrderArray=execQuery(QString("select usernumeric2 from PickList where usernumeric1=%1 and status='A'").arg(activeLoading),eDb);
		QSqlQuery widthQuery(QSqlDatabase::database(whDb));widthQuery.prepare("select width from packets where packetId=:code");
		QSqlQuery heightQuery(QSqlDatabase::database(whDb));heightQuery.prepare("select height from packets where packetId=:code");
		QSqlQuery cassetteQuery(QSqlDatabase::database(whDb));cassetteQuery.prepare("select cassette from positions where packetId=:code and posId<4000");
		for (int i=0;i<ordersArray.count();i++)
		{
			QString PickListNumOr;
#ifdef useLocalDb
			PickListNumOr="or PickListNum is null";
#endif
			QVector<QVariant> codesArray=execQuery(QString("select TicketNum from Ticket where SONum in (select SONum from PickListDetail where PickListNum=%1) and (PickListNum=%1 %2)").arg(ordersArray[i].toString()).arg(PickListNumOr),eDb);
			QVector<QVariant> profileArray=execQuery(QString("select DieNum from Ticket where SONum in (select SONum from PickListDetail where PickListNum=%1) and (PickListNum=%1 %2)").arg(ordersArray[i].toString()).arg(PickListNumOr),eDb);
			QVector<QVariant> lengthArray=execQuery(QString("select Len from Ticket where SONum in (select SONum from PickListDetail where PickListNum=%1) and (PickListNum=%1 %2)").arg(ordersArray[i].toString()).arg(PickListNumOr),eDb);
			QVector<QVariant> weightArray=execQuery(QString("select Gross from Ticket where SONum in (select SONum from PickListDetail where PickListNum=%1) and (PickListNum=%1 %2)").arg(ordersArray[i].toString()).arg(PickListNumOr),eDb);
			QVector<QVariant> SONumArray=execQuery(QString("select SONum from Ticket where SONum in (select SONum from PickListDetail where PickListNum=%1) and (PickListNum=%1 %2)").arg(ordersArray[i].toString()).arg(PickListNumOr),eDb);
			QVector<QVariant> SOItemNumArray=execQuery(QString("select SOItemNum from Ticket where SONum in (select SONum from PickListDetail where PickListNum=%1) and (PickListNum=%1 %2)").arg(ordersArray[i].toString()).arg(PickListNumOr),eDb);
			QVector<QVariant> AlloyArray=execQuery(QString("select Alloy from Ticket where SONum in (select SONum from PickListDetail where PickListNum=%1) and (PickListNum=%1 %2)").arg(ordersArray[i].toString()).arg(PickListNumOr),eDb);
			QVector<QVariant> TemperArray=execQuery(QString("select Temper from Ticket where SONum in (select SONum from PickListDetail where PickListNum=%1) and (PickListNum=%1 %2)").arg(ordersArray[i].toString()).arg(PickListNumOr),eDb);
			QVector<QVariant> DtPackArray=execQuery(QString("select DtPack from Ticket where SONum in (select SONum from PickListDetail where PickListNum=%1) and (PickListNum=%1 %2)").arg(ordersArray[i].toString()).arg(PickListNumOr),eDb);
			QVector<QVariant> StatusArray=execQuery(QString("select Status from Ticket where SONum in (select SONum from PickListDetail where PickListNum=%1) and (PickListNum=%1 %2)").arg(ordersArray[i].toString()).arg(PickListNumOr),eDb);
			int maxPackets=codesArray.count();
			//if (maxPackets>20)
			//	maxPackets=20;
			for (int j=0;j<maxPackets;j++)
			{
				QString c=codesArray[j].toString();
				widthQuery.bindValue(":code",codesArray[j].toString());widthQuery.exec();if (!widthQuery.next())noDimentionsCodesList<<QString().number(codesArray[j].toInt());
				heightQuery.bindValue(":code",codesArray[j].toString());heightQuery.exec();if (!heightQuery.next())noDimentionsCodesList<<QString().number(codesArray[j].toInt());
				cassetteQuery.bindValue(":code",codesArray[j].toString());cassetteQuery.exec();if (!cassetteQuery.next())notInStoreCodesList<<QString().number(codesArray[j].toInt());
				//
				row.clear();
				QStandardItem *codeItem		=new QStandardItem();	codeItem	->setData(codesArray[j].toInt(),Qt::DisplayRole);	row<<codeItem		;
				QStandardItem *profileItem	=new QStandardItem();	profileItem	->setData(profileArray[j].toString(),Qt::DisplayRole);	row<<profileItem	;
				QStandardItem *widthItem	=new QStandardItem();	widthItem	->setData(widthQuery.value(0).toInt(),Qt::DisplayRole);	row<<widthItem		;
				QStandardItem *heightItem	=new QStandardItem();	heightItem	->setData(heightQuery.value(0).toInt(),Qt::DisplayRole);	row<<heightItem		;
				QStandardItem *lengthItem	=new QStandardItem();	lengthItem	->setData(lengthArray[j].toInt(),Qt::DisplayRole);	row<<lengthItem		;
				QStandardItem *weightItem	=new QStandardItem();	weightItem	->setData(weightArray[j].toInt(),Qt::DisplayRole);	row<<weightItem		;
				QStandardItem *putOnTopItem	=new QStandardItem();	putOnTopItem	->setData("-",Qt::DisplayRole);	row<<putOnTopItem		;
				QStandardItem *orderidItem	=new QStandardItem();	orderidItem	->setData(dropOrderArray[i].toInt(),Qt::DisplayRole);	row<<orderidItem	;
				packetsModel->appendRow(row);
				//vector
				packetClass *p=new packetClass(codesArray[j].toInt(),profileArray[j].toString(),widthQuery.value(0).toInt(),heightQuery.value(0).toInt(),lengthArray[j].toInt(),weightArray[j].toInt(),0,i);

				packetsVector.append(new packetWidgetClass(p,SONumArray[j].toString(),SOItemNumArray[j].toString(),AlloyArray[j].toString(),TemperArray[j].toString(),DtPackArray[j].toString(),StatusArray[j].toString(),NULL,(i<orderColors.count()?orderColors[i]:QColor(150,150,150)),cassetteQuery.value(0).toInt(),this));
			}
		}
	}
	else
	{
		EXIT(("Error"),("No loading was selected"));
	}
	QString mess;
	int noOfPacketsInRow=0;
	int maxNoOfPacketsInEveryRow=10;
	if (notInStoreCodesList.count()>0)
	{
		mess+=tr("the packets: \n");
		for (int i=0;i<notInStoreCodesList.count();i++)
		{
			mess+=notInStoreCodesList[i]+" ";
			noOfPacketsInRow++;
			if (noOfPacketsInRow==maxNoOfPacketsInEveryRow)
			{
				noOfPacketsInRow=0;
				mess+="\n";
			}
		}
		mess+=tr("\nare not in the warehouse \n\n");

	}
	noOfPacketsInRow=0;
	if (noDimentionsCodesList.count()>0)
	{
		mess+=tr("The dimensions of packets:\n");
		for (int i=0;i<noDimentionsCodesList.count();i++)
		{
			mess+=noDimentionsCodesList[i]+" ";
			noOfPacketsInRow++;
			if (noOfPacketsInRow==maxNoOfPacketsInEveryRow)
			{
				noOfPacketsInRow=0;
				mess+="\n";
			}
		}
		mess+=tr("\nare wrong in table packets of wareHouseData database.");
	}
	if ((notInStoreCodesList.count()>0)||(noDimentionsCodesList.count()>0))
	{
		QClipboard *clipboard = QApplication::clipboard();
		clipboard->setText(mess);
		QMessageBox::critical(0,tr("Error"),mess);
		//exit(0);
	}
	packetsModel->setHeaderData(0, Qt::Horizontal, tr("Packet"));
	packetsModel->setHeaderData(1, Qt::Horizontal, tr("Profile"));
	packetsModel->setHeaderData(2, Qt::Horizontal, tr("Width"));
	packetsModel->setHeaderData(3, Qt::Horizontal, tr("Height"));
	packetsModel->setHeaderData(4, Qt::Horizontal, tr("Length"));
	packetsModel->setHeaderData(5, Qt::Horizontal, tr("Weight"));
	packetsModel->setHeaderData(6, Qt::Horizontal, tr("Put on top priority"));
	packetsModel->setHeaderData(7, Qt::Horizontal, tr("Drop order"));
	//////
	//file.close();
	///////////////////////////////////////////////////////////////////////////////////////////////////
	////////demo///demo///demo///demo///demo///demo///demo///demo///demo///demo///demo///demo//////////
	////////demo///demo///demo///demo///demo///demo///demo///demo///demo///demo///demo///demo//////////
	////////demo///demo///demo///demo///demo///demo///demo///demo///demo///demo///demo///demo//////////
	////////demo///demo///demo///demo///demo///demo///demo///demo///demo///demo///demo///demo//////////
	///////////////////////////////////////////////////////////////////////////////////////////////////
	////////fileNameString="./1.txt";
	////////QStringList items;
	////////items<<"1"<<"2"<<"3"<<"4";
	////////bool ok;
	////////QString item=QInputDialog::getItem(this,G("Επιλογή αρχείου"),G("αριθμός παραγγελειών"),items,2,false,&ok);
	////////if (ok && !item.isEmpty())
	////////	fileNameString="./"+item+".txt";
	//////////
	////////QFile file(fileNameString);
	////////file.open(QIODevice::ReadOnly|QIODevice::Text);
	////////QTextStream stream(&file);
	/////////////
	////////packetsModel->clear();
	////////packetsVector.clear();
	////////QString text;
	////////stream>>text;stream>>text;stream>>text;stream>>text;stream>>text;stream>>text;stream>>text;
	////////int code,width,height,length,weight,priority,orderid;
	////////QList<QStandardItem*> row;
	////////while(!stream.atEnd())
	////////{
	////////	stream>>text;code=QVariant(text).toInt(); if (code==0) break;
	////////	stream>>text;width=QVariant(text).toInt();
	////////	stream>>text;height=QVariant(text).toInt();
	////////	stream>>text;length=QVariant(text).toInt();
	////////	stream>>text;weight=QVariant(text).toInt();
	////////	stream>>text;priority=QVariant(text).toInt();
	////////	stream>>text;orderid=QVariant(text).toInt();
	////////	row.clear();
	////////	QStandardItem *codeItem		=new QStandardItem();	codeItem	->setData(code,Qt::DisplayRole);	row<<codeItem		;
	////////	QStandardItem *widthItem	=new QStandardItem();	widthItem	->setData(width,Qt::DisplayRole);	row<<widthItem		;
	////////	QStandardItem *heightItem	=new QStandardItem();	heightItem	->setData(height,Qt::DisplayRole);	row<<heightItem		;
	////////	QStandardItem *lengthItem	=new QStandardItem();	lengthItem	->setData(length,Qt::DisplayRole);	row<<lengthItem		;
	////////	QStandardItem *weightItem	=new QStandardItem();	weightItem	->setData(weight,Qt::DisplayRole);	row<<weightItem		;
	////////	QStandardItem *priorityItem	=new QStandardItem();	priorityItem->setData(priority,Qt::DisplayRole);row<<priorityItem	;
	////////	QStandardItem *orderidItem	=new QStandardItem();	orderidItem	->setData(orderid,Qt::DisplayRole);	row<<orderidItem	;
	////////	packetsModel->appendRow(row);
	////////	//vector
	////////	packetClass *p=new packetClass(code,width,height,length,weight,priority,orderid);
	////////	packetsVector.append(new packetWidgetClass(p,NULL,QColor(150,150,150)));
	////////}
	////////packetsModel->setHeaderData(0, Qt::Horizontal, G("κωδικός"));
	////////packetsModel->setHeaderData(1, Qt::Horizontal, G("πλάτος"));
	////////packetsModel->setHeaderData(2, Qt::Horizontal, G("ύψος"));
	////////packetsModel->setHeaderData(3, Qt::Horizontal, G("μήκος"));
	////////packetsModel->setHeaderData(4, Qt::Horizontal, G("βάρος"));
	////////packetsModel->setHeaderData(5, Qt::Horizontal, G("προτ. τοποθ. πάνω"));
	////////packetsModel->setHeaderData(6, Qt::Horizontal, G("παραγγελία"));
	////////////
	////////file.close();
}
void packetsClass::updateVisual()
{
	int totalWeight=0;
	for (int i=0;i<packetsModel->rowCount();i++)
	{
		totalWeight+=packetsModel->data(packetsModel->index(i,5),Qt::DisplayRole).toInt();
	}
	totalWeightLabel->setText(QVariant(totalWeight).toString());
}
void packetsClass::onMainTimer()
{
}
