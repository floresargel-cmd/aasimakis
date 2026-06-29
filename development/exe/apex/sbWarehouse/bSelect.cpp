#include "pch.h"
#include "bSelect.h"
cassetteSelectClass::cassetteSelectClass(QString title_,QString dbTable_,exportStationWidgetClass *exportStationW_):QDialog(exportStationW_)
{
	title=title_;
	dbTable=dbTable_;
	exportStationW=exportStationW_;
	setWindowTitle(tr("Select a cassette for %1.").arg(title));
	resize(1000,600);
	selectedCassetteNum=0;
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	QVBoxLayout *upLayout=new QVBoxLayout();mainLayout->addLayout(upLayout);
	QHBoxLayout *filterLayout=new QHBoxLayout();mainLayout->addLayout(filterLayout);
	QHBoxLayout *downLayout=new QHBoxLayout();mainLayout->addLayout(downLayout);
	//up
	upLayout->addWidget(new QLabel(tr("List of cassettes:")));
	//mid
	listView=new QListWidget(this);upLayout->addWidget(listView);
	listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	listView->setAlternatingRowColors(true);
	connect(listView,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(listSelectionChanged(QListWidgetItem*,QListWidgetItem*)));

	//filter
	profileFilter=new QLineEdit("",this);
	connect(profileFilter,SIGNAL(textEdited(const QString &)),this,SLOT(filterTextChangedSlot(const QString&)));
	profileFilter->setStyleSheet("QLineEdit{font-size:14px;}");
	filterLayout->addWidget(profileFilter);

	downLayout->addWidget(sbRadioBtn=new QRadioButton(tr("Sb")));
	sbRadioBtn->setChecked(true);
	connect(sbRadioBtn,SIGNAL(toggled(bool)),this,SLOT(updateListSlot()));
	downLayout->addWidget(cassetteRadioBtn=new QRadioButton(tr("Cassette")));
	connect(cassetteRadioBtn,SIGNAL(toggled(bool)),this,SLOT(updateListSlot()));

	downLayout->addWidget(addSelectedButton=new QToolButton(this));
	addSelectedButton->setText(tr("Add selected cassette"));
	addSelectedButton->setIcon(QIcon(":add.png"));
	addSelectedButton->setStatusTip(tr("Adds the selected cassettes in the export list."));
	addSelectedButton->setToolTip(tr("Adds the selected cassettes in the export list."));
	connect(addSelectedButton,SIGNAL(released()),this,SLOT(addSelectedSlot()));

	downLayout->addStretch(1);

	updateList();
}
int cassetteSelectClass::exec()
{
	updateList();
	return QDialog::exec();
}
void cassetteSelectClass::updateListSlot()
{
	updateList();
}
void cassetteSelectClass::updateList()
{
	listView->clear();
	QVector<QVector<QVariant>> allSbVV;
	QString filterTicket=profileFilter->text();
	if (filterTicket.isEmpty())
	{
		if (cassetteRadioBtn->isChecked())
			allSbVV=execTableQuery(QString("select cassette,sbNumber,ticket from pos where cassette in (%1) order by cassette").arg(QString("select cassette from pos where cassette<>0 and (cassette in (%1))").arg(selectIdleCassettes)),mDb);
		if (sbRadioBtn->isChecked())
			allSbVV=execTableQuery(QString("select cassette,sbNumber,ticket from pos where cassette in (%1) and sbNumber<>0 order by sbNumber").arg(QString("select cassette from pos where cassette<>0 and (cassette in (%1))").arg(selectIdleCassettes)),mDb);
	}
	else
	{
		allSbVV=execTableQuery(QString("select cassette,sbNumber,ticket from pos where cassette in (%1) and sbNumber<>0 and ticket like '%%2%' order by ticket").arg(QString("select cassette from pos where cassette<>0 and (cassette in (%1))").arg(selectIdleCassettes)).arg(filterTicket.toInt()),mDb);
	}
	for (int i=0;i<allSbVV.count();i++)
	{
		int cassette=allSbVV[i][0].toInt();
		int sbNumber=allSbVV[i][1].toInt();
		int ticket=allSbVV[i][2].toInt();
		if (cassetteRadioBtn->isChecked())
			listView->addItem(tr("%1\t%2\t%3").arg(cassette).arg(sbNumber).arg(ticket));
		if (sbRadioBtn->isChecked())
			listView->addItem(tr("%1\t%2\t%3").arg(sbNumber).arg(cassette).arg(ticket));
		listView->item(i)->setData(roleCassetteNumber,cassette);
	}
}
//////////////////////////////////////////////////////slots
void cassetteSelectClass::listSelectionChanged(QListWidgetItem* selected,QListWidgetItem* deselected)
{
	if (selected==NULL)
		selectedCassetteNum=0;
	else
		selectedCassetteNum=selected->data(roleCassetteNumber).toInt();
}

void cassetteSelectClass::addSelectedSlot()
{
	addCassette(selectedCassetteNum);
}
void cassetteSelectClass::addCassette(int b)
{
	QVector<QVariant> allIdleCassettesArray=getIdleCassettes(mDb);
	for (int i=0;i<allIdleCassettesArray.count();i++)
	{
		if (b==allIdleCassettesArray[i].toInt())
		{
			execQuery(QString("insert into %1 (cassette,priority) values(%2,(select(ifNull((select priority from (select priority from %1 order by priority desc limit 1 )as t),0))+1))").arg(dbTable).arg(b),mDb);
			int posId=getFirst(QString("select posId from pos where cassette=%1").arg(b),mDb).toInt();
			exportStationW->dbUpdateTimerSlot();
			updateList();
			logImportExportGlobal(getFirst(QString("select ticket from pos where posId=%1").arg(posId),mDb).toInt(),getFirst(QString("select sbNumber from pos where posId=%1").arg(posId),mDb).toInt(),"",title,logExportTrigger,mDb);
			return;
		}
	}
}
void cassetteSelectClass::filterTextChangedSlot(const QString&)
{
	updateList();
}