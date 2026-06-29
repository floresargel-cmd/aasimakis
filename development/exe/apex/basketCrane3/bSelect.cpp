#include "pch.h"
#include "bSelect.h"
basketSelectClass::basketSelectClass(QString title_,QString dbTable_,exportStationWidgetClass *exportStationW_):QDialog(exportStationW_)
{
	selectedBasketNum=0;
	title=title_;
	dbTable=dbTable_;
	exportStationW=exportStationW_;
	setWindowTitle(tr("Select a basket for %1.").arg(title));
	resize(1000,600);

	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	QVBoxLayout *upLayout=new QVBoxLayout();
	mainLayout->addLayout(upLayout);
	QHBoxLayout *downLayout=new QHBoxLayout();
	mainLayout->addLayout(downLayout);
	//up
	upLayout->addWidget(new QLabel(tr("List of all baskets:")));
	//mid
	basketListView=new QListWidget(this);
	upLayout->addWidget(basketListView);
	basketListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	basketListView->setSelectionMode(QAbstractItemView::SingleSelection);
	basketListView->setAlternatingRowColors(true);
	connect(basketListView,&QListWidget::currentItemChanged,this,&basketSelectClass::listSelectionChanged);

	downLayout->addWidget(addSelectedBtn=new QToolButton(this));
	addSelectedBtn->setText(tr("Add selected basket to export list"));
	addSelectedBtn->setIcon(QIcon(":add.png"));
	addSelectedBtn->setStatusTip(tr("Adds the selected baskets in the export list."));
	addSelectedBtn->setToolTip(tr("Adds the selected baskets in the export list."));
	connect(addSelectedBtn,SIGNAL(released()),this,SLOT(addSelectedSlot()));

	downLayout->addStretch(1);
	updateList();
}
int basketSelectClass::exec()
{
	updateList();
	return QDialog::exec();
}
#define selectIdleBasketsQuery QString("select basketTableId from positions where (posNumber>=%1 and posNumber<=%2) and basketTableId<>0 and basketTableId not in (select basket from %3) and basketTableId not in (select basket from %4) and posNumber+posIndex not in (select fromPosNumber+fromPosIndex from c3missions);").arg(bufferApos).arg(bufferCpos).arg(crane3ExportsDestackerPositions1).arg(crane3ExportsDestackerPositions2)
void basketSelectClass::updateList()
{
	basketListView->clear();
	QVector<QVariant> allBasketsV=execQuery(selectIdleBasketsQuery,bDb);
	for (int i=0;i<allBasketsV.count();i++)
	{
		int basketNumber=allBasketsV[i].toInt();
		basketListView->addItem(tr("%1").arg(basketNumber));
		basketListView->item(i)->setData(roleBasketNumber,basketNumber);
	}
}
//public slots
void basketSelectClass::updateListSlot()
{
	updateList();
}
void basketSelectClass::listSelectionChanged(QListWidgetItem* currentlySelected,QListWidgetItem* previouslySelected)
{
	if (currentlySelected==nullptr)
		selectedBasketNum=0;
	else
		selectedBasketNum=currentlySelected->data(roleBasketNumber).toInt();
}
void basketSelectClass::addSelectedSlot()
{
	addBasket(selectedBasketNum);
}
void basketSelectClass::addBasket(int bskt)
{ 
	QVector<QVariant> allAvailableBasketsV=execQuery(selectIdleBasketsQuery,bDb);
	for (int i=0;i<allAvailableBasketsV.count();i++)
	{
		if (bskt==allAvailableBasketsV[i].toInt())
		{
			execQuery(QString("insert into %1 (basket,priority) values(%2,%3)").arg(dbTable).arg(bskt).arg(getFirst(QString("select top 1 priority from %1 order by priority desc").arg(dbTable),bDb).toInt()+1),bDb);
			exportStationW->dbUpdateTimerSlot();
			updateList();
			return;
		}
	}
}

//QVector<QVariant> getIdleCassettes(QString dbName)
//{
//	return execQuery(QString("select cassette from pos where cassette<>0 and (cassette in (%1)) order by cassette").arg(selectIdleCassettes),dbName);
//}