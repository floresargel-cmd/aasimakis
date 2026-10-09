#include "pch.h"
#include "bSelectS.h"
basketSelectForScheduleClass::basketSelectForScheduleClass(QWidget *p):QDialog(p)
{
	setWindowTitle(tr("Select cassettes for schedule."));
	resize(1200,600);
	//
	QHBoxLayout *mainLayout=new QHBoxLayout();
	setLayout(mainLayout);
	QVBoxLayout *leftLayout=new QVBoxLayout();mainLayout->addLayout(leftLayout);
	QVBoxLayout *rightLayout=new QVBoxLayout();mainLayout->addLayout(rightLayout);
	QHBoxLayout *rightButtonLayout=new QHBoxLayout();rightLayout->addLayout(rightButtonLayout);
	//left
	leftLayout->addWidget(new QLabel(tr("Schedule Date:")));
	shiftList=new QListWidget(this);
	shiftList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	shiftList->setSelectionMode(QAbstractItemView::SingleSelection);
	shiftList->setAlternatingRowColors(true);
	connect(shiftList,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(shiftListSelectionChanged(QListWidgetItem*,QListWidgetItem*)));
	leftLayout->addWidget(shiftList);
	//right
	rightLayout->addWidget(new QLabel(tr("Dies:")));
	diesList=new QListWidget(this);
	diesList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	diesList->setSelectionMode(QAbstractItemView::SingleSelection);
	diesList->setAlternatingRowColors(true);
	rightLayout->addWidget(diesList);

	addCassettesButton=new QToolButton(this);
	addCassettesButton->setText(tr("Add Selected baskets"));
	addCassettesButton->setIcon(QIcon(":add.png"));
	addCassettesButton->setIconSize(QSize(32,32));
	addCassettesButton->setStatusTip(tr("Adds all baskets from selected shift in the export list."));
	addCassettesButton->setToolTip(tr("Adds all baskets from selected shift in the export list."));
	addCassettesButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(addCassettesButton,SIGNAL(released()),this,SLOT(addCassettesButtonSlot()));
	rightButtonLayout->addWidget(addCassettesButton);
	addAllCassettesButton=new QToolButton(this);
	addAllCassettesButton->setText(tr("Add All"));
	addAllCassettesButton->setIcon(QIcon(":add.png"));
	addAllCassettesButton->setIconSize(QSize(32,32));
	addAllCassettesButton->setStatusTip(tr("Adds the selected baskets in the export list."));
	addAllCassettesButton->setToolTip(tr("Adds the selected baskets in the export list."));
	addAllCassettesButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	connect(addAllCassettesButton,SIGNAL(released()),this,SLOT(addAllCassettesButtonSlot()));
	rightButtonLayout->addWidget(addAllCassettesButton);
}
int basketSelectForScheduleClass::exec()
{
	updateShiftList();
	return QDialog::exec();
}
void basketSelectForScheduleClass::updateShiftList()
{
	QVector<QVariant> shiftsV=execQuery(QString("select distinct SchedDt from assimakisDieWarehouse order by SchedDt"),db);
	shiftList->clear();
	for (int i=0;i<shiftsV.count();i++)
	{
		shiftList->addItem(shiftsV[i].toString().replace("T"," "));
		shiftList->item(shiftList->count()-1)->setData(Qt::UserRole,shiftsV[i].toString());
	}
}
void basketSelectForScheduleClass::updateDiesList()
{
	QVector<QVector<QVariant>> diesVV=execTableQuery(QString("select DieNum,CopyNum,SchedSeq,Press,SchedShift from assimakisDieWarehouse where SchedDt=CAST('%1' AS smalldatetime) order by SchedDt,Press,SchedShift,SchedSeq").arg(selectedShift),db);
	diesList->clear();
	for (int i=0;i<diesVV.count();i++)
	{
		QString die=QString("%1-%2").arg(diesVV[i][0].toInt()).arg(diesVV[i][1].toInt());
		diesList->addItem(QString("%1 (%2) Press:%3 Shift:%4").arg(die).arg(diesVV[i][2].toString()).arg(diesVV[i][3].toInt()).arg(diesVV[i][4].toInt()));
		int cassette=getFirst(QString("select basket from basketsContents where contentName='%1'").arg(die),db).toInt();
		diesList->item(diesList->count()-1)->setData(Qt::UserRole,cassette);
		if (cassette==0)
		{
			diesList->item(diesList->count()-1)->setTextColor(QColor(200,0,0));
			diesList->item(diesList->count()-1)->setToolTip(tr("Die is not in warehouse"));
		}
		else
		{
			diesList->item(diesList->count()-1)->setTextColor(QColor(0,200,0));
			diesList->item(diesList->count()-1)->setToolTip(tr("Die is in cassete %1").arg(cassette));
		}
	}
}
//////////////////////////////////////////////////////slots
void basketSelectForScheduleClass::shiftListSelectionChanged(QListWidgetItem* selected,QListWidgetItem* deselected)
{
	if (selected==NULL)
		selectedShift="";
	else
		selectedShift=selected->data(Qt::UserRole).toString();
	updateDiesList();
}
void basketSelectForScheduleClass::addCassettesOfListWidgetItems(QList<QListWidgetItem*> items)
{
	for (int i=0;i<items.count();i++)
	{
		int cassette=items[i]->data(Qt::UserRole).toInt();
		if (cassette!=0)
		{
			int tId=getFirst(QString("select tId from exports where basket=%1").arg(cassette),db).toInt();
			if (tId<1)
				execQuery(QString("insert into exports (basket,priority) values(%1,%2)").arg(cassette).arg(getFirst(QString("select top 1 priority from exports order by priority desc"),db).toInt()+1),db);
		}
	}
	//call to this->parent()->updateFromDatabase() to update the exportwidget list, without relying on export widget's qtimer timeout (5secs) to update the list.
	//no further action was taken: both qtimer and the below line can call updateFromDatabase() "at the same time (within msecs)" in case user adds dies at the same time qtimer timeout fires
	//result: two unneccesary consecutives calls to updateFromDatabase() can happen.
	dynamic_cast<exportStationWidgetClass*>(parentWidget())->updateFromDatabase();
}
//	default behavior: all cassettes that contain dies that belongs to the selected shift are added to the export's list.
//	extended behavior:
//		1) only the cassette that contains the selected die from the selected schedule is added to the export's list. (already existing add-button)
//		2) all cassettes that belong to the selected shedule are added to export's list (new add-all-button added) (should this button be available even if some dies are selected?)
//		3) (?) multiselect dies that can be added via already existing button (1) should be implemented?

void basketSelectForScheduleClass::addCassettesButtonSlot()
{
	addCassettesOfListWidgetItems(diesList->selectedItems());
}
void basketSelectForScheduleClass::addAllCassettesButtonSlot()
{
	QList<QListWidgetItem*> items;
	for (int i=0;i<diesList->count();i++)
		items<<diesList->item(i);
	addCassettesOfListWidgetItems(items);
}
