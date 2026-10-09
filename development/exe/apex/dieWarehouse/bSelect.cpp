#include "pch.h"
#include "bSelect.h"
basketSelectClass::basketSelectClass(QString title_,QString dbTable_,exportStationWidgetClass *exportStationW_):QDialog(exportStationW_)
{
	title=title_;
	dbTable=dbTable_;
	exportStationW=exportStationW_;
	setWindowTitle(tr("Select a basket for %1.").arg(title));
	resize(1200,600);
	exportStationW=exportStationW_;
	selectedBasketNum=0;
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
	//filter
	QHBoxLayout *filterLayout=new QHBoxLayout();leftLayout->addLayout(filterLayout);
	profileFilter=new QLineEdit("",this);
	profileFilter->setToolTip(tr("If the filter text box is empty all empty baskets are been seen\nIf the filter is <*> then all baskets with contents are been seen\nIf the filter has character %, baskets are ordered by weigth\nIf a text is writen the baskets with this die are displayed"));
	connect(profileFilter,SIGNAL(textEdited(const QString &)),this,SLOT(filterTextChangedSlot(const QString&)));
	profileFilter->setStyleSheet("QLineEdit{font-size:14px;}");
	filterLayout->addWidget(profileFilter);

	addSelectedButton=new QToolButton(this);
	addSelectedButton->setText(tr("Add selected basket"));
	addSelectedButton->setIcon(QIcon(":add.png"));
	addSelectedButton->setIconSize(QSize(32,32));
	addSelectedButton->setStatusTip(tr("Adds the selected baskets in the export list."));
	addSelectedButton->setToolTip(tr("Adds the selected baskets in the export list."));
	connect(addSelectedButton,SIGNAL(released()),this,SLOT(addSelectedSlot()));
	filterLayout->addWidget(addSelectedButton);

	//QCompleter

	//QCompleter
	QVector<QVariant> contentNameV=execQuery("select distinct contentName from basketsContents order by contentName",db);
	QStringList contentNameVList;
	for (int i=0;i<contentNameV.count();i++)
		contentNameVList<<contentNameV[i].toString().trimmed();
	filterCompleterModel=new QStringListModel(contentNameVList);
	QCompleter *filterCompleter=new QCompleter(filterCompleterModel,this);
	filterCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	filterCompleter->setFilterMode(Qt::MatchContains);
	profileFilter->setCompleter(filterCompleter);
	connect(filterCompleter,SIGNAL(activated(const QString&)),this,SLOT(filterTextChangedSlot(const QString&)));
	connect(filterCompleter,SIGNAL(highlighted(const QString&)),this,SLOT(filterTextChangedSlot(const QString&)));
	//table
	basketContents=new basketContentsClass(this);
	//splitter
	QSplitter *mainSplitter=new QSplitter(Qt::Horizontal);
	mainSplitter->addWidget(leftW);
	mainSplitter->addWidget(basketContents);
	mainSplitter->setStretchFactor(1,1);
	QList<int> sizes;sizes<<450<<700;
	mainSplitter->setSizes(sizes);
	midLayout->addWidget(mainSplitter,1);
	profileFilter->setText("");
}
int basketSelectClass::exec()
{
	QVector<QVariant> contentNameV=execQuery("select distinct contentName from basketsContents order by contentName",db);
	QStringList list;
	for (int i=0;i<contentNameV.count();i++)
		list<<contentNameV[i].toString().trimmed();
	filterCompleterModel->setStringList(list);
	return QDialog::exec();
}
void basketSelectClass::updateList(QString str)
{
	QString filter=str;
	listView->clear();
	if (!filter.isEmpty())
	{
		if ( (filter.length()>1)&&((filter.at(0)=="b")||(filter.at(0)=="B"))  )
		{
			QString q=
			QString("select basket from pos where plcId<%1 and basket<>0").arg(cranePlcId)+
			QString("and basket not in (select basket from %1)") .arg(exportsTable)+
			QString("and basket not in (select basket from pos where plcId in (select fromPos from missions))")+
			QString("and basket not in (select basket from pos where plcId in (select toPos from missions))")+
			QString("and basket=%1").arg(filter.replace("b","").replace("B","").toInt())+
			QString("order by row,col");

			QVector<QVariant> allIdleBasketsArray=execQuery(q,db);
			for (int i=0;i<allIdleBasketsArray.count();i++)
			{
				listView->addItem(allIdleBasketsArray[i].toString());
				listView->item(listView->count()-1)->setData(Qt::UserRole,allIdleBasketsArray[i].toInt());
			}
		}
		else if (filter=="*")
		{
			QVector<QVariant> allIdleBasketsArray=getAllIdleBaskets();
			for (int i=0;i<allIdleBasketsArray.count();i++)
			{
				listView->addItem(allIdleBasketsArray[i].toString());
				listView->item(listView->count()-1)->setData(Qt::UserRole,allIdleBasketsArray[i].toInt());
			}
		}
		else if ((filter.at(0)=='%')&&(filter.length()>1)&&(filter.at(1)=='%'))
		{
			QVector<QVector<QVariant>> havyIdleBasketsArray=execTableQuery("with basketWeights (basket,weight) as (select basket,sum(weight) as weight from basketsContents inner join contentTypes on basketsContents.contentType=contentTypes.content group by basket ) select basketWeights.basket,weight,row,col from basketWeights inner join pos on basketWeights.basket=pos.basket order by weight,row,col",db);
			for (int i=0;i<havyIdleBasketsArray.count();i++)
			{
				int basketId=havyIdleBasketsArray[i][0].toInt();
				if ((basketId>=1)&&(basketId<=26))
				{
					listView->addItem(QString("%1\t(%2%)").arg(basketId).arg(100.*havyIdleBasketsArray[i][1].toDouble()/basketAvailableWeight,0,'f',1,' '));
					listView->item(listView->count()-1)->setData(Qt::UserRole,havyIdleBasketsArray[i][0].toInt());
				}
			}
			QVector<QVariant> allIdleBasketsArray=getIdleEmptyBaskets();
			for (int i=0;i<allIdleBasketsArray.count();i++)
			{
				int basketId=allIdleBasketsArray[i].toInt();
				if ((basketId>=1)&&(basketId<=26))
				{
					listView->addItem(QString("%1\t(0.0%)").arg(basketId));
					listView->item(listView->count()-1)->setData(Qt::UserRole,allIdleBasketsArray[i].toInt());
				}
			}
		}
		else if ((filter.at(0)=='%'))
		{
			QVector<QVector<QVariant>> havyIdleBasketsArray=execTableQuery("with basketWeights (basket,weight) as (select basket,sum(weight) as weight from basketsContents inner join contentTypes on basketsContents.contentType=contentTypes.content group by basket ) select basketWeights.basket,weight,row,col from basketWeights inner join pos on basketWeights.basket=pos.basket order by weight,row,col",db);
			for (int i=0;i<havyIdleBasketsArray.count();i++)
			{
				int basketId=havyIdleBasketsArray[i][0].toInt();
				if ((basketId<1)||(basketId>26))
				{
					listView->addItem(QString("%1\t(%2%)").arg(basketId).arg(100.*havyIdleBasketsArray[i][1].toDouble()/basketAvailableWeight,0,'f',1,' '));
					listView->item(listView->count()-1)->setData(Qt::UserRole,havyIdleBasketsArray[i][0].toInt());
				}
			}
			QVector<QVariant> allIdleBasketsArray=getIdleEmptyBaskets();
			for (int i=0;i<allIdleBasketsArray.count();i++)
			{
				int basketId=allIdleBasketsArray[i].toInt();
				if ((basketId<1)||(basketId>26))
				{
					listView->addItem(QString("%1\t(0.0%)").arg(basketId));
					listView->item(listView->count()-1)->setData(Qt::UserRole,allIdleBasketsArray[i].toInt());
				}
			}
		}

		else if (filter.trimmed().length()!=0)
		{
			QVector<QVariant> allIdleBasketsWithContentArray=getIdleBasketsWithContent(filter.trimmed());
			for (int i=0;i<allIdleBasketsWithContentArray.count();i++)
			{
				listView->addItem(QString("%1").arg(allIdleBasketsWithContentArray[i].toInt()));
				listView->item(listView->count()-1)->setData(Qt::UserRole,allIdleBasketsWithContentArray[i].toInt());
			}
		}
		else if (filter.trimmed().length()==0)
		{
			QVector<QVariant> allIdleEmptyBasketsArray=getIdleEmptyBaskets();
			for (int i=0;i<allIdleEmptyBasketsArray.count();i++)
			{
				listView->addItem(allIdleEmptyBasketsArray[i].toString());
				listView->item(listView->count()-1)->setData(Qt::UserRole,allIdleEmptyBasketsArray[i].toInt());
			}
		}
	}
	else //get empty baskets
	{
		QVector<QVariant> allIdleEmptyBasketsArray=getIdleEmptyBaskets();
		for (int i=0;i<allIdleEmptyBasketsArray.count();i++)
		{
			listView->addItem(allIdleEmptyBasketsArray[i].toString());
			listView->item(listView->count()-1)->setData(Qt::UserRole,allIdleEmptyBasketsArray[i].toInt());
		}
	}
	if (listView->count()>0)
	{
		listView->item(0)->setSelected(true);
		listView->setCurrentRow(0);
	}
	updateTable();
}
void basketSelectClass::updateTable()
{
	basketContents->updateData(selectedBasketNum);
}
//////////////////////////////////////////////////////slots
void basketSelectClass::listSelectionChanged(QListWidgetItem* selected,QListWidgetItem* deselected)
{
	if (selected==NULL)
		selectedBasketNum=0;
	else
		selectedBasketNum=selected->data(Qt::UserRole).toInt();
	updateTable();
	//
	if (selectedBasketNum!=0)
		addSelectedButton->setEnabled(true);
	else
		addSelectedButton->setEnabled(false);
}

void basketSelectClass::addSelectedSlot()
{
	addBasket(selectedBasketNum);
}
void basketSelectClass::addBasket(int b)
{
	QVector<QVariant> allIdleBasketsArray=getAllIdleBaskets();
	for (int i=0;i<allIdleBasketsArray.count();i++)
	{
		if (b==allIdleBasketsArray[i].toInt())
		{
			execQuery(QString("insert into %1 (basket,priority) values(%2,%3)").arg(dbTable).arg(b).arg(getFirst(QString("select top 1 priority from %1 order by priority desc").arg(dbTable),db).toInt()+1),db);
			exportStationW->dbUpdateTimerSlot();
			updateList(profileFilter->text());
			return;
		}
	}
}
void basketSelectClass::filterTextChangedSlot(const QString& str)
{
	updateList(str);
}
