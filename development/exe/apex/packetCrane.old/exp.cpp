#include "exp.h"
exportsClass::exportsClass(QWidget *p):QDialog(p)
{
	setMinimumHeight(1000);
	setMinimumWidth(500);
	setWindowTitle(tr("Exports..."));
	QVBoxLayout *mainLayout=new QVBoxLayout();this->setLayout(mainLayout);
	QHBoxLayout *tablesLayout=new QHBoxLayout();mainLayout->addLayout(tablesLayout);
		QVBoxLayout *tablesLeftLayout=new QVBoxLayout();tablesLayout->addLayout(tablesLeftLayout);
		QVBoxLayout *tablesRightLayout=new QVBoxLayout();tablesLayout->addLayout(tablesRightLayout);
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);
	//
	tablesLeftLayout->addWidget(new QLabel(tr("Contents")));
	contentsTableWidget=new QTableWidget();tablesLeftLayout->addWidget(contentsTableWidget);
	contentsTableWidget->setAlternatingRowColors(true);
	contentsTableWidget->horizontalHeader()->setStretchLastSection(true);
	contentsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	contentsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	contentsTableWidget->setColumnCount(2);
	QStringList ch;ch<<tr("Packet")<<tr("cell");
	contentsTableWidget->setHorizontalHeaderLabels(ch);
	connect(contentsTableWidget,SIGNAL(itemSelectionChanged()),this,SLOT(contentsTableWidgetItemSelectionChangedSlot()));
	tablesLeftLayout->addWidget(addToExportsTableBtn=new QPushButton(QIcon(":/add.png"),""));
	connect(addToExportsTableBtn,SIGNAL(clicked()),this,SLOT(addToExportsTableBtnSlot()));

	tablesRightLayout->addWidget(new QLabel(tr("Exports")));
	exportsTableWidget=new QTableWidget();tablesRightLayout->addWidget(exportsTableWidget);
	exportsTableWidget->setAlternatingRowColors(true);
	exportsTableWidget->horizontalHeader()->setStretchLastSection(true);
	exportsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	exportsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	exportsTableWidget->setColumnCount(1);
	QStringList eh;eh<<tr("Packet");
	exportsTableWidget->setHorizontalHeaderLabels(eh);
	connect(exportsTableWidget,SIGNAL(itemSelectionChanged()),this,SLOT(exportsTableWidgetItemSelectionChangedSlot()));
	tablesRightLayout->addWidget(removeFromExportsTableBtn=new QPushButton(QIcon(":/remove.png"),""));
	connect(removeFromExportsTableBtn,SIGNAL(clicked()),this,SLOT(removeFromExportsTableSlot()));
	//buttons layout
	QPushButton *okPushButton=new QPushButton(tr("Ok"));btnsLayout->addWidget(okPushButton);
	connect(okPushButton,SIGNAL(clicked()),this,SLOT(accept()));
}
void exportsClass::show()
{
	updateGui();
	QDialog::show();
}
void exportsClass::updateGui()
{
	//contentsTableWidget
	QVector<QVariant> packetsVector=apexExecQuery(QString("select packetId from positions where packetId<>0 and packetId not in (select contents from exports where status=0) and posId<%1 order by packetId desc").arg(MAX_CELL_POS_ID));
	QVector<QVariant> posIdsVector=apexExecQuery(QString("select posId from positions where packetId<>0 and packetId not in (select contents from exports where status=0) and posId<%1 order by packetId desc").arg(MAX_CELL_POS_ID));
	contentsTableWidget->clearContents();
	contentsTableWidget->setRowCount(packetsVector.count());
	for (int i=0;i<packetsVector.count();i++)
	{
		contentsTableWidget->setItem(i,0,new QTableWidgetItem(packetsVector[i].toString(),Qt::DisplayRole));
		contentsTableWidget->setItem(i,1,new QTableWidgetItem(posIdsVector[i].toString(),Qt::DisplayRole));
	}
	addToExportsTableBtn->setEnabled(false);
	//contentsTableWidget
	QVector<QVariant> packetsEVector=apexExecQuery(QString("select contents from exports where status=0 order by orderSequence,contentsSequence"));
	exportsTableWidget->clearContents();
	exportsTableWidget->setRowCount(packetsEVector.count());
	for (int i=0;i<packetsEVector.count();i++)
		exportsTableWidget->setItem(i,0,new QTableWidgetItem(packetsEVector[i].toString()));
	removeFromExportsTableBtn->setEnabled(false);
}
void exportsClass::addToExportsTableBtnSlot()
{
	QList<QTableWidgetItem *> selected=contentsTableWidget->selectedItems();
	QVector<int> rowVector;
	for (int i=0;i<selected.count();i++)
	{
		int row=selected[i]->row();
		if (!rowVector.contains(row))
			rowVector<<row;
	}
	for (int i=0;i<rowVector.count();i++)
	{
		int row=rowVector[i];
		int posId=contentsTableWidget->item(row,1)->data(Qt::DisplayRole).toInt();
		int contents=GETONE(QString("select packetId from positions where posId=%1").arg(posId)).toInt();
		int type=GETONE(QString("select sb from packets where packetId=%1").arg(contents)).toInt();
		int status=0;
		int orderSequence=GETONE(QString("select max(orderSequence) from exports")).toInt()+1;
		int contentsSequence=1;
		apexExecQuery(QString("insert into exports (contents,type,status,orderSequence,contentsSequence) values(%1,%2,%3,%4,%5)").arg(contents).arg(type).arg(status).arg(orderSequence).arg(contentsSequence));
	}
	updateGui();
}
void exportsClass::removeFromExportsTableSlot()
{
	QList<QTableWidgetItem *> selected=exportsTableWidget->selectedItems();
	for (int i=0;i<selected.count();i++)
	{
		int row=selected[i]->row();
		int contents=exportsTableWidget->item(row,0)->data(Qt::DisplayRole).toInt();
		apexExecQuery(QString("delete from exports where contents=%1").arg(contents));
	}
	updateGui();
}
void exportsClass::contentsTableWidgetItemSelectionChangedSlot()
{
	QList<QTableWidgetItem *> selected=contentsTableWidget->selectedItems();
	if (selected.count()==0)
		addToExportsTableBtn->setEnabled(false);
	else
		addToExportsTableBtn->setEnabled(true);
}
void exportsClass::exportsTableWidgetItemSelectionChangedSlot()
{
	QList<QTableWidgetItem *> selected=exportsTableWidget->selectedItems();
	if (selected.count()==0)
		removeFromExportsTableBtn->setEnabled(false);
	else
		removeFromExportsTableBtn->setEnabled(true);
}