#include "pch.h"
#include "search.h"
searchWidgetClass::searchWidgetClass(allPossClass *allPoss_,QWidget *p):QWidget(p)
{
	allPoss=allPoss_;
	selectedBasket=0;
	QVBoxLayout *mainLayout=new QVBoxLayout();
	setLayout(mainLayout);
	mainLayout->addWidget(new QLabel(tr("List of baskets:")));
	//list
	listView=new QListWidget(this);
	listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	listView->setSelectionMode(QAbstractItemView::SingleSelection);
	listView->setAlternatingRowColors(true);
	connect(listView,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(listSelectionChangedSlot(QListWidgetItem*,QListWidgetItem*)));

	mainLayout->addWidget(listView);
	//filter
	filterLineEdit=new QLineEdit("",this);
	filterLineEdit->setToolTip(tr("If the filter text box is empty all empty baskets are been seen\nIf the filter is <*> then all baskets with contents are been seen\nIf the filter has the leter b and a number, the basket with this number is been seen\nIf a text is writen the baskets with this profile are been seen"));
	connect(filterLineEdit,SIGNAL(textEdited(const QString &)),this,SLOT(filterTextChangedSlot(const QString&)));
	filterLineEdit->setStyleSheet("QLineEdit{font-size:14px;}");
	mainLayout->addWidget(filterLineEdit);
	//QCompleter
	QVector<QVariant> dstinctProfilesV=execQuery("select distinct contentName from basketsContents order by contentName",db);
	QStringList profileStringList;
	for (int i=0;i<dstinctProfilesV.count();i++)
		profileStringList<<dstinctProfilesV[i].toString().trimmed();
	QCompleter *completer=new QCompleter(profileStringList,this);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	filterLineEdit->setCompleter(completer);
	connect(completer,SIGNAL(activated(const QString&)),this,SLOT(filterTextChangedSlot(const QString&)));
	connect(completer,SIGNAL(highlighted(const QString&)),this,SLOT(filterTextChangedSlot(const QString&)));
	updateList("");
}
QString searchWidgetClass::tooltTipForBasket(int basket)
{

	QString toolStr;
	return toolStr;
	toolStr+="<html><body bgcolor=\"#E6E6FA\"><font style='color:#333333'>"+tr("Basket number:")+"</font><font style='color:#000077'>"+QString("%1").arg(basket)+"</font><br>";
	toolStr+=tr("Located in cell:")+"</font><font style='color:#000077'>"+QString("%1").arg(getFirst(QString("select plcId from pos where basket=%1").arg(basket),db).toInt())+"</font><br>";
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select profile,quantity,length from basketsContents where basket=%1").arg(basket),db);
	toolStr+=QString(QString("<table><tr>"));
	for (int i=0;i<dataVV.count();i++)
	{
		toolStr+=QString(QString("<td>"));
		toolStr+=
			QString("%1<br>").arg(dataVV[i][0].toString().trimmed())+
			tr("pieces:%1<br>").arg(dataVV[i][1].toInt())+
			tr("length:%1mm<br>").arg(dataVV[i][2].toInt())
			;
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
			QVector<QVariant> allBasketsArray=execQuery(QString("select basket from pos where basket<>0  order by basket"),db);
			for (int i=0;i<allBasketsArray.count();i++)
			{
				listView->addItem(allBasketsArray[i].toString());
				listView->item(i)->setData(Qt::UserRole,allBasketsArray[i]);
				listView->item(listView->count()-1)->setToolTip(tooltTipForBasket(allBasketsArray[i].toInt()));
			}
		}
		else if ((filter.at(0)=='b')||(filter.at(0)=='B'))
		{
			int b=filter.remove("b").toInt();
			if (getFirst(QString("select basket from pos where basket=%1").arg(b),db).toInt())
			{
				listView->addItem(QString("%1").arg(b));
				listView->item(0)->setData(Qt::UserRole,b);
				listView->item(listView->count()-1)->setToolTip(tooltTipForBasket(b));
			}
		}
		else
		{
			QVector<QVariant> allBasketsWithProfileArray=execQuery(QString("select distinct basket from basketsContents where contentName='%1' order by basket").arg(filter),db);
			for (int i=0;i<allBasketsWithProfileArray.count();i++)
			{
				listView->addItem(allBasketsWithProfileArray[i].toString());
				listView->item(i)->setData(Qt::UserRole,allBasketsWithProfileArray[i]);
				listView->item(listView->count()-1)->setToolTip(tooltTipForBasket(allBasketsWithProfileArray[i].toInt()));
			}
		}
	}
	else //get empty baskets
	{
		QVector<QVariant> allIdleEmptyBasketsArray=getIdleEmptyBaskets();
		for (int i=0;i<allIdleEmptyBasketsArray.count();i++)
		{
			listView->addItem(allIdleEmptyBasketsArray[i].toString());
			listView->item(listView->count()-1)->setToolTip(tooltTipForBasket(allIdleEmptyBasketsArray[i].toInt()));
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
	int sel=0;
	if (selected==NULL)
		sel=0;
	else
		sel=selected->data(Qt::UserRole).toInt();
	if (selectedBasket!=sel)
	{
		selectedBasket=sel;
		allPoss->highLightPosWithBasket(selectedBasket);
	}
}
