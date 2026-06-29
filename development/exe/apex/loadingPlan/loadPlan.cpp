#include "loadPlan.h"
loadingPlanClass::loadingPlanClass(packetsClass *packets_,optionsClass *options_,QWidget *p)
{
	packets=packets_;
	options=options_;
	//orders
	QVector<QColor> orderColors;
	orderColors.append(QColor(100,0,0));
	orderColors.append(QColor(0,100,0));
	orderColors.append(QColor(0,0,100));
	orderColors.append(QColor(100,100,0));
	orderColors.append(QColor(0,100,100));
	orderColors.append(QColor(100,0,100));
	//////////////////////////////////////////
	////////initialPacketsColumnArray/////////
	//////////////////////////////////////////
	QVector<packetColumnClass*>		initialPacketsColumnArray;
	for (int i=0;i<packets->getNoOfPackets();i++)
	{
		int orderId=packets->getPacket(i)->getOrderId();
		int loadingColIndex=-1;
		for (int j=0;j<initialPacketsColumnArray.size();j++)
		{
			if (initialPacketsColumnArray[j]->getOrderIdOfFirstPacket()==orderId)
			{
				loadingColIndex=j;
				break;
			}
		}
		if (loadingColIndex<0)
		{
			packetColumnClass *tmpPacketsColumn=new packetColumnClass(Qt::AlignHCenter,Qt::AlignTop,QString(tr("Order "))+QVariant(orderId).toString(),0);
			tmpPacketsColumn->setBackgroundRole(QPalette::Dark);
			initialPacketsColumnArray.append(tmpPacketsColumn);
			loadingColIndex=initialPacketsColumnArray.size()-1;
		}
		if ( (orderId<orderColors.size()) && (orderId>=0)  )
			packets->getPacket(i)->setColor(orderColors[orderId]);
		initialPacketsColumnArray[loadingColIndex]->appendPacket(packets->getPacket(i));
		packets->getPacket(i)->setInitialParentColumn(initialPacketsColumnArray[loadingColIndex]);
		packets->getPacket(i)->show();
	}
	QWidget *initialOrdersQWidget=new QWidget();

	QHBoxLayout *initialOrdersLayout=new QHBoxLayout();
	initialOrdersQWidget->setLayout(initialOrdersLayout);

	QScrollArea *initialOrdersScrollArea=new QScrollArea();
	initialOrdersLayout->addWidget(initialOrdersScrollArea);

	initialOrdersScrollArea->viewport()->setBackgroundRole(QPalette::Dark);
	initialOrders=new groupOfColumnsClass(initialOrdersScrollArea,initialPacketsColumnArray,this,groupOfColumnsClass::ORIGINAL_GROUP,false,this);
	initialOrders->calculate();
	initialOrders->calculateGeometry();
	//////////////////
	//group1Orders
	//////////////////
	QWidget *group1OrdersQWidget=new QWidget();

	QHBoxLayout *group1OrdersLayout=new QHBoxLayout();
	group1OrdersQWidget->setLayout(group1OrdersLayout);

	QScrollArea *group1OrdersScrollArea=new QScrollArea();
	group1OrdersLayout->addWidget(group1OrdersScrollArea);

	group1OrdersScrollArea->viewport()->setBackgroundRole(QPalette::Dark);

	QVector<packetColumnClass*>		group1PacketsColumnArray;
	packetColumnClass *column1=new packetColumnClass(Qt::AlignRight,Qt::AlignBottom,QString(tr("column 1")),options->getMaxAvailableHeight());column1->setBackgroundRole(QPalette::Dark);group1PacketsColumnArray.append(column1);
	packetColumnClass *column2=new packetColumnClass(Qt::AlignRight,Qt::AlignBottom,QString(tr("column 2")),options->getMaxAvailableHeight());column2->setBackgroundRole(QPalette::Dark);group1PacketsColumnArray.append(column2);
	packetColumnClass *column3=new packetColumnClass(Qt::AlignHCenter,Qt::AlignBottom,QString(tr("column 3")),options->getMaxAvailableHeight());column3->setBackgroundRole(QPalette::Dark);group1PacketsColumnArray.append(column3);
	packetColumnClass *column4=new packetColumnClass(Qt::AlignLeft,Qt::AlignBottom,QString(tr("column 4")),options->getMaxAvailableHeight());column4->setBackgroundRole(QPalette::Dark);group1PacketsColumnArray.append(column4);
	packetColumnClass *column5=new packetColumnClass(Qt::AlignLeft,Qt::AlignBottom,QString(tr("column 5")),options->getMaxAvailableHeight());column5->setBackgroundRole(QPalette::Dark);group1PacketsColumnArray.append(column5);
	forGroupOrders=new groupOfColumnsClass(group1OrdersScrollArea,group1PacketsColumnArray,this,groupOfColumnsClass::FOR_GROUP,true,this);
	forGroupOrders->setMaxWeight(options->getMaxWeightFor());
	forGroupOrders->setMaxHeight(options->getMaxAvailableHeight());
	forGroupOrders->calculateGeometry();
	//
	//////////////////
	//group1Orders
	//////////////////
	QWidget *group2OrdersQWidget=new QWidget();

	QHBoxLayout *group2OrdersLayout=new QHBoxLayout();
	group2OrdersQWidget->setLayout(group2OrdersLayout);

	QScrollArea *group2OrdersScrollArea=new QScrollArea();
	group2OrdersLayout->addWidget(group2OrdersScrollArea);

	group2OrdersScrollArea->viewport()->setBackgroundRole(QPalette::Dark);

	QVector<packetColumnClass*>		group2PacketsColumnArray;
	packetColumnClass *column6 =new packetColumnClass(Qt::AlignRight,Qt::AlignBottom,QString(tr("column 6")),options->getMaxAvailableHeight());	column6->setBackgroundRole(QPalette::Dark); group2PacketsColumnArray.append(column6);
	packetColumnClass *column7 =new packetColumnClass(Qt::AlignRight,Qt::AlignBottom,QString(tr("column 7")),options->getMaxAvailableHeight());	column7->setBackgroundRole(QPalette::Dark); group2PacketsColumnArray.append(column7);
	packetColumnClass *column8 =new packetColumnClass(Qt::AlignHCenter,Qt::AlignBottom,QString(tr("column 8")),options->getMaxAvailableHeight());column8->setBackgroundRole(QPalette::Dark); group2PacketsColumnArray.append(column8);
	packetColumnClass *column9 =new packetColumnClass(Qt::AlignLeft,Qt::AlignBottom,QString(tr("column 9")),options->getMaxAvailableHeight());	column9->setBackgroundRole(QPalette::Dark); group2PacketsColumnArray.append(column9);
	packetColumnClass *column10=new packetColumnClass(Qt::AlignLeft,Qt::AlignBottom,QString(tr("column 10")),options->getMaxAvailableHeight());	column10->setBackgroundRole(QPalette::Dark);group2PacketsColumnArray.append(column10);
	aftGroupOrders=new groupOfColumnsClass(group2OrdersScrollArea,group2PacketsColumnArray,this,groupOfColumnsClass::AFT_GROUP,true,this);
	aftGroupOrders->setMaxWeight(options->getMaxWeightAft());
	aftGroupOrders->setMaxHeight(options->getMaxAvailableHeight());
	aftGroupOrders->calculateGeometry();
	//
	optimizer=new optimizerClass(this,this);
	//
	QSplitter *upSplitter=new QSplitter();
	upSplitter->addWidget(group1OrdersQWidget);
	upSplitter->addWidget(optimizer);
	upSplitter->addWidget(group2OrdersQWidget);
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	mainLayout->addWidget(upSplitter);
	//
	setLayout(mainLayout);
	connect(&mainTimer, SIGNAL(timeout()), this, SLOT(onMainTimer()));
	mainTimer.start(1*1000);
}
void loadingPlanClass::exportLoading()
{
	QString stmnt;
	int activeLoading=getActiveLoadingId();
	if (activeLoading==0)
		return;
	//if active return
	stmnt=QString("SELECT loadingId from loadings where status<>1 and loadingId=")+QString().number(activeLoading);
	QVector<QVariant> activeLoadingsArray=execQuery(stmnt,whDb);
	if (activeLoadingsArray.count()>0)
	{
		QMessageBox::critical(0,"Error",tr("Order %1 has allready started").arg(activeLoading));
		return;
	}
	//remove if exist 
	stmnt="DELETE FROM loadings where loadingId="+QString().number(activeLoading);
	execQuery(stmnt,whDb);
	//
	forGroupOrders->exportLoading(getActiveLoadingId());
	aftGroupOrders->exportLoading(getActiveLoadingId());
}
void loadingPlanClass::printLabels()
{
	QString stmnt;
	int selectedLoading=packets->getActiveLoadingId();
	if (selectedLoading==0)
		return;
	//remove if exist 
	stmnt="DELETE FROM printingOrder where loadingid="+QString().number(selectedLoading);
	execQuery(stmnt,whDb);
	//get order

	stmnt=QString("insert into printingOrder (loadingid,code,ord) values (")+QString().number(selectedLoading)+",:code,:ord)";
	QSqlQuery query(QSqlDatabase::database(whDb));
	query.prepare(stmnt);
	int labelCounter=0;
	for (int i=0;i<packets->getNoOfPackets();i++)
	{
		packetWidgetClass* pw=packets->getPacket(i);
		query.bindValue(":code",pw->getCode());
		query.bindValue(":ord",pw->getManualExportOrder());
		if (!(query.exec()))
			uExit("Error in database",query.lastError().text());
	}
}
void loadingPlanClass::startManualLoading()
{
	//QFile file("logg.txt");
	//if (!file.open(QIODevice::Append|QIODevice::Text))
	//	return;
	//bool ans=false;
	////int activeLoading=packets->getActiveLoadingId();
	//int noOfPacketsToTakeOut=QInputDialog::getInt(this,tr("Question"),tr("Till what row to export?"),packets->getNoOfPackets(),1,packets->getNoOfPackets(),1,&ans);
	//if (!ans)
	//	return;
	//QTextStream out(&file);
	//QString stmnt;
	//int selectedLoading=packets->getActiveLoadingId();
	//if (selectedLoading==0)
	//	return;
	//out<<"selectedLoading:"<<selectedLoading<<"\n";out.flush();
	//
	////outOrderCounter 
	//int outOrderCounter=0;
	//stmnt=QString("select top 1 outOrder from ")+manualExportsTable+" order by outOrder desc";
	//QVector<QVariant> outOrderArray=execQuery(stmnt,mainConnectionName);
	//if (outOrderArray.count()==1)
	//	outOrderCounter=outOrderArray[0].toInt();
	//out<<"outOrderCounter:"<<outOrderCounter<<"\n";out.flush();
	////cassete 
	//stmnt=QString("select position from ")+positionsTable+" where packet=:packet";
	//QSqlQuery casseteQuery(QSqlDatabase::database(mainConnectionName));
	//casseteQuery.prepare(stmnt);
	////get order
	//stmnt=QString("insert into ")+manualExportsTable+" (cassete,outOrder,packet) values (:cassete,:outOrder,:packet)";
	//QSqlQuery insertQuery(QSqlDatabase::database(mainConnectionName));
	//insertQuery.prepare(stmnt);

	//for (int i=0;i<packets->getNoOfPackets();i++)
	//{
	//	packetWidgetClass* pw=packets->getPacket(i);
	//	if (pw->getManualExportOrder()==0)
	//	{
	//		QMessageBox::critical(0,G("Σφλμα"),G("Το ")+QString().number(pw->getCode())+G(" δεν εχει δηλωμενη σειρα εξωδου"));
	//		return;
	//	}
	//}

	//int manualExportOrder=0;
	//int cassete;
	//while (manualExportOrder<noOfPacketsToTakeOut/*packets->getNoOfPackets()*/)
	//{
	//	packetWidgetClass* pw=NULL;
	//	for (int i=0;i<packets->getNoOfPackets();i++)
	//	{
	//		pw=packets->getPacket(i);
	//		if (pw->getManualExportOrder()==(manualExportOrder+1))
	//		{
	//			manualExportOrder++;
	//			out<<"manualExportOrder:"<<manualExportOrder<<"\n";out.flush();
	//			break;
	//		}
	//	}
	//	if (pw==NULL)
	//	{
	//		break;
	//	}
	//	//casseteQuery
	//	casseteQuery.bindValue(":packet",pw->getCode());
	//	casseteQuery.exec();
	//	out<<"casseteQuery:"<<casseteQuery.lastQuery()<<"\n";out.flush();
	//	cassete=0;
	//	if (casseteQuery.next())
	//		cassete=casseteQuery.value(0).toInt();
	//	else
	//	{
	//		continue;	
	//	}
	//	out<<"cassete:"<<cassete<<"\n";out.flush();
	//	//allready in _manualExports?
	//	QVector<QVariant> outOrderV=execQuery(QString("select outOrder from %1 where packet=%2").arg(manualExportsTable).arg(pw->getCode()),mainConnectionName);
	//	//insertQuery
	//	if ((cassete!=0)&&(outOrderV.count()==0))
	//	{
	//		insertQuery.bindValue(":cassete",cassete);
	//		insertQuery.bindValue(":outOrder",++outOrderCounter);
	//		insertQuery.bindValue(":packet",pw->getCode());
	//		if (!(insertQuery.exec()))
	//		{
	//			out<<"casseteQuery:"<<insertQuery.lastQuery()<<"\n";out.flush();
	//			EXIT(G("Πρόβλημα στην βάση::startManualLoading()"),insertQuery.lastError().text()+" "+insertQuery.lastQuery());
	//		}
	//	}
	//}
	//file.close();
}
//void loadingPlanClass::exportToManual()
//{
//	forGroupOrders->exportToStation(getActiveLoadingId());
//	aftGroupOrders->exportToStation(getActiveLoadingId());
////	forGroupOrders->exportPacketsToManual();
////	aftGroupOrders->exportPacketsToManual();
//}
void loadingPlanClass::calculate()
{
	initialOrders->calculate();
	forGroupOrders->calculate();
	aftGroupOrders->calculate();
}
groupOfColumnsClass* loadingPlanClass::getForGroupOfOrders()
{
	return forGroupOrders;
}
groupOfColumnsClass* loadingPlanClass::getAftGroupOfOrders()
{
	return aftGroupOrders;
}
void loadingPlanClass::onMainTimer()
{
}
int loadingPlanClass::getActiveLoadingId()
{
	return packets->getActiveLoadingId();
}
//optim
void loadingPlanClass::loadPacket(int index,int col)
{
	packets->getPacket(index)->removeFromParent();
	if (col<5)
		forGroupOrders->appendPacket(packets->getPacket(index),col);
	else
		aftGroupOrders->appendPacket(packets->getPacket(index),col-5);
	calculate();
}
void loadingPlanClass::clearPackets()
{
	forGroupOrders->clearPackets();
	aftGroupOrders->clearPackets();
}
void loadingPlanClass::clearManualExportOrder()
{
	forGroupOrders->clearManualExportOrder();
	aftGroupOrders->clearManualExportOrder();
}

void loadingPlanClass::exportToPdf()
{
	QDir::current().mkdir("reports");
	QString pdfFile=QDir::current().absolutePath()+QString("/reports/loading.%1.%2").arg(getActiveLoadingId()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd")+QTime::currentTime().toString(".hh.mm.ss.pdf"));
	QString date=QDateTime::currentDateTime().toString("yyyy/MM/dd")+QTime::currentTime().toString(" hh:mm:ss");
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFileName(pdfFile);
	printer.setOrientation(QPrinter::Landscape);
	printer.setPageSize(QPrinter::A4);
	QPainter painter;
	painter.begin(&printer);
	forGroupOrders->exportToPdf(&painter,getActiveLoadingId(),date);
	printer.newPage();
	aftGroupOrders->exportToPdf(&painter,getActiveLoadingId(),date);
	painter.end();
		//str=G("Φόρτωση:%1 πίσω ντάνα ").arg(getActiveLoadingId());
		//HPDF_Page_TextRect(page,100.0,930.0,600.,60.,str.toLocal8Bit().data(),HPDF_TALIGN_CENTER,NULL);
		//str=QDateTime::currentDateTime().toString("yyyy-MM-dd")+QTime::currentTime().toString(" hh:mm");
		//HPDF_Page_TextRect(page,100.0,910.0,600.,60.,str.toLocal8Bit().data(),HPDF_TALIGN_CENTER,NULL);
		//HPDF_Page_EndText(page);
		//
		//HPDF_Page_BeginText(page);
		//str=G("%1 δεματα   %2kgr X:%3m Y:%4m").arg(aftGroupOrders->getNoOfPackets()).arg(aftGroupOrders->getWeight()).arg(double(aftGroupOrders->getXCG())/1000.).arg(double(aftGroupOrders->getYCG())/1000.);
		//HPDF_Page_SetFontAndSize(page,font,10);
		//HPDF_Page_TextRect(page,100.0,60.0,600.,60.,str.toLocal8Bit().data(),HPDF_TALIGN_CENTER,NULL);
		//HPDF_Page_EndText(page);

		QDesktopServices::openUrl(pdfFile);
}
/////////////////////////////////////////////////////////////////////////////////////////////////
///// pdf  ///// pdf  ///// pdf  ///// pdf  ///// pdf  ///// pdf  ///// pdf  ///// pdf  ///// pdf  
/////////////////////////////////////////////////////////////////////////////////////////////////
