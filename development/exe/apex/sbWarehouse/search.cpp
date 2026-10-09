#include "pch.h"
#include "search.h"
searchWidgetClass::searchWidgetClass(allPossClass *allPoss_,QWidget *p):QWidget(p)
{
	allPoss=allPoss_;
	selectedCassette=0;
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	mainLayout->addWidget(new QLabel(tr("List of cassettes:")));
	//list
	listView=new QListWidget(this);
	listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	listView->setAlternatingRowColors(true);
	connect(listView,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(listSelectionChangedSlot(QListWidgetItem*,QListWidgetItem*)));

	mainLayout->addWidget(listView);
	//filter
	filterLineEdit=new QLineEdit("",this);
	connect(filterLineEdit,SIGNAL(textEdited(const QString &)),this,SLOT(filterTextChangedSlot(const QString&)));
	filterLineEdit->setStyleSheet("QLineEdit{font-size:14px;}");
	mainLayout->addWidget(filterLineEdit);
	//QCompleter
	QVector<QVariant> ticketsV=execQuery("select ticket from pos order by ticket",mDb);
	QStringList ticketsStringList;
	for (int i=0;i<ticketsV.count();i++)
		ticketsStringList<<ticketsV[i].toString().trimmed();
	QCompleter *completer=new QCompleter(ticketsStringList,this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	filterLineEdit->setCompleter(completer);
	connect(completer,SIGNAL(activated(const QString&)),this,SLOT(filterTextChangedSlot(const QString&)));
	connect(completer,SIGNAL(highlighted(const QString&)),this,SLOT(filterTextChangedSlot(const QString&)));
	listView->blockSignals(true);
	updateList("");
	listView->blockSignals(false);
}
QString searchWidgetClass::tooltTipForCassette(int cassette)
{
	QString toolStr;
	toolStr+="<html><body bgcolor=\"#E6E6FA\"><font style='color:#333333'>"+tr("Cassette number:")+"</font><font style='color:#000077'>"+QString("%1").arg(cassette)+"</font><br>";
	toolStr+=tr("Located in cell:")+"</font><font style='color:#000077'>"+QString("%1").arg(getFirst(QString("select posId from pos where cassette=%1").arg(cassette),mDb).toInt())+"</font><br>";
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select ticket from pos where cassette=%1").arg(cassette),mDb);
	toolStr+=QString(QString("<table><tr>"));
	for (int i=0;i<dataVV.count();i++)
	{
		toolStr+=QString(QString("<td>"));
			QString("%1").arg(dataVV[i][0].toString().trimmed());
		toolStr+=QString(QString("</td>"));
	}
	toolStr+=QString(QString("</tr></table><br><br>"));
	toolStr+="</body></html>";
	return toolStr;
}
void searchWidgetClass::updateList(QString str)
{
	QString filter=str;
	listView->clear();
	if (!filter.isEmpty())
	{
		if (filter=="*")
		{
			QVector<QVariant> allCassettesArray=execQuery(QString("select ticket from pos where cassette<>0 order by cassette"),mDb);
			for (int i=0;i<allCassettesArray.count();i++)
			{
				listView->addItem(allCassettesArray[i].toString());
				listView->item(i)->setData(Qt::UserRole,allCassettesArray[i]);
				listView->item(listView->count()-1)->setToolTip(tooltTipForCassette(allCassettesArray[i].toInt()));
			}
		}
		else if ((filter.at(0)=='b')||(filter.at(0)=='B'))
		{
			int b=filter.remove("b").remove("B").toInt();
			if (getFirst(QString("select cassette from pos where cassette=%1").arg(b),mDb).toInt())
			{
				listView->addItem(QString("%1").arg(b));
				listView->item(0)->setData(Qt::UserRole,b);
				listView->item(listView->count()-1)->setToolTip(tooltTipForCassette(b));
			}
		}
		else
		{
			QVector<QVariant> allCassettesWithTicketArray=execQuery(QString("select cassette from pos where ticket='%1' order by cassette").arg(filter.toInt()),mDb);
			for (int i=0;i<allCassettesWithTicketArray.count();i++)
			{
				listView->addItem(allCassettesWithTicketArray[i].toString());
				listView->item(i)->setData(Qt::UserRole,allCassettesWithTicketArray[i]);
				listView->item(listView->count()-1)->setToolTip(tooltTipForCassette(allCassettesWithTicketArray[i].toInt()));
			}
		}
	}
	else //get empty cassettes
	{
		QVector<QVariant> allIdleEmptyCassettesArray=getIdleEmptyCassettes(mDb);
		for (int i=0;i<allIdleEmptyCassettesArray.count();i++)
		{
			listView->addItem(allIdleEmptyCassettesArray[i].toString());
			listView->item(i)->setData(Qt::UserRole,allIdleEmptyCassettesArray[i]);
			listView->item(listView->count()-1)->setToolTip(tooltTipForCassette(allIdleEmptyCassettesArray[i].toInt()));
		}
	}
	if (listView->count()>0)
	{
		listView->item(0)->setSelected(true);
		listView->setCurrentRow(0);
	}
}
//////////////////////////////////////////////////////slots
void searchWidgetClass::filterTextChangedSlot(const QString& str)
{
	updateList(str);
}
void searchWidgetClass::listSelectionChangedSlot(QListWidgetItem* selected,QListWidgetItem* deselected)
{
	if (deselected)
	{
		int prevCassette=deselected->data(Qt::UserRole).toInt();
		posClass *prevPos=allPoss->getCellWithCassette(prevCassette,false);
		if (prevPos)
			prevPos->setIsSelected(false);
	}
	int sel=0;
	if (selected==NULL)
		sel=0;
	else
		sel=selected->data(Qt::UserRole).toInt();
	if (selectedCassette!=sel)
	{
		selectedCassette=sel;
		if (selectedCassette)
		{
			posClass *pos=allPoss->getCellWithCassette(selectedCassette,false);
			if (pos)
			{
				pos->setIsSelected(true);
				emit posSelectedSignal(pos->getPosId());
			}
			else
				emit posSelectedSignal(0);
		}
		else
			emit posSelectedSignal(0);
		//allPoss->highLightPosWithCassette(selectedCassette);
	}
}
