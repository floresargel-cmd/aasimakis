#include "pch.h"
#include "bCont.h"
basketContentsClass::basketContentsClass(QWidget *w):QWidget(w)
{
	basketNum=0;
	QVBoxLayout *mainLayout=new QVBoxLayout(this);setLayout(mainLayout);
	//upLayout
	tableView=new QTableWidget();
	tableView->setAlternatingRowColors(true);
	tableView->horizontalHeader()->setStretchLastSection(true);
	tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->setColumnCount(12);
	tableView->setHorizontalHeaderLabels(QStringList()<<tr("Profile")<<tr("Quantity")<<tr("Length [mm]")<<tr("Temper")<<tr("So")<<tr("So Line")<<tr("CurrentFinCode")<<tr("StdPract")<<tr("CustShip")<<tr("NextDeptNum")<<tr("StopTime")<<tr("Destination"));
	mainLayout->addWidget(tableView,1);
}
basketContentsClass::~basketContentsClass()
{
}
void basketContentsClass::updateTable(int basketNum_)
{
	basketNum=basketNum_;
	updateTable();
}
void basketContentsClass::updateTable()
{
	if (basketNum==0)
	{
		tableView->clearContents();
		tableView->setRowCount(0);
		return;
	}
	tableView->clearContents();
	QVector<QVector<QVariant>> bContVV=execTableQuery(QString("%1 and RackDetail.RackId=%2 order by StopTime desc").arg(selectContentsFromEpicsStrG).arg(basketNum),eDb);
	tableView->setRowCount(bContVV.count());
	for (int i=0;i<bContVV.count();i++) 
	{
		craneContentsDataStruct contentsData(bContVV[i]);
		tableView->setItem(i,colProfile,new QTableWidgetItem(tr("%1").arg(contentsData.profile)));
		tableView->setItem(i,colQuantity,new QTableWidgetItem(tr("%1").arg(contentsData.quantity)));
		tableView->setItem(i,colLength,new QTableWidgetItem(tr("%1").arg(contentsData.length)));
		tableView->setItem(i,colTemper,new QTableWidgetItem(contentsData.temper));
		tableView->setItem(i,colSoNumber,new QTableWidgetItem(tr("%1").arg(contentsData.soNumber)));
		tableView->setItem(i,colSoLine,new QTableWidgetItem(tr("%1").arg(contentsData.soLine)));
		tableView->setItem(i,colCurrentFinCode,new QTableWidgetItem(tr("%1").arg(contentsData.CurrentFinCode)));
		tableView->setItem(i,colStdPract,new QTableWidgetItem(tr("%1").arg(contentsData.StdPract)));
		tableView->setItem(i,colCustShip,new QTableWidgetItem(tr("%1").arg(contentsData.CustShip)));
		tableView->setItem(i,colNextDeptNum,new QTableWidgetItem(tr("%1").arg(contentsData.NextDeptNum)));
		tableView->setItem(i,colStopTime,new QTableWidgetItem(tr("%1").arg(contentsData.StopTime)));
		tableView->setItem(i,colDestination,new QTableWidgetItem(getHoneyCombDestinationEnumClassStringG(contentsData.honeyCombDestinationEnum)));
	}
	tableView->resizeColumnsToContents();
	tableView->horizontalHeader()->setStretchLastSection(true);
}