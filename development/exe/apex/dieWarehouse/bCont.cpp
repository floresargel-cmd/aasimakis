#include "pch.h"
#include "bCont.h"
extern specialDiesClass * specialDiesG;
extern QStringList allColorList;

#define lineEditR					 QString("QLineEdit{color:red;}")
#define lineEditG					 QString("QLineEdit{color:green;}")

basketContentsClass::basketContentsClass(QWidget *w):QWidget(w)
{
	QVBoxLayout *mainLayout=new QVBoxLayout(this);mainLayout->setMargin(0);setLayout(mainLayout);
	QHBoxLayout *labelsLayout=new QHBoxLayout();labelsLayout->setMargin(2);mainLayout->addLayout(labelsLayout);
	QHBoxLayout *upLayout=new QHBoxLayout();upLayout->setMargin(2);mainLayout->addLayout(upLayout);
	QHBoxLayout *btnsLayout=new QHBoxLayout();btnsLayout->setMargin(2);mainLayout->addLayout(btnsLayout);
	tableWdgt=new QTableWidget();
	tableWdgt->setAlternatingRowColors(true);
	tableWdgt->setSelectionBehavior(QAbstractItemView::SelectItems);
	tableWdgt->setIconSize(QSize(100,100));
	tableWdgt->setColumnCount(5);
	tableWdgt->setHorizontalHeaderLabels(QStringList()<<tr("Die/tool")<<tr("Type")<<tr("Weight[libs/Kgr]")<<tr("Width[inches/mm]")<<tr("Shift"));
	tableWdgt->horizontalHeaderItem(4)->setIcon(QIcon(":/shift.png"));
	tableWdgt->horizontalHeaderItem(4)->setTextAlignment(Qt::AlignLeft);
	//connect(tableWdgt,SIGNAL(cellChanged(int,int)),this,SLOT(tableCellChangedSlot(int,int)));
	upLayout->addWidget(tableWdgt);
	//
	labelsLayout->addWidget(new QLabel(tr("Weight :")));
	weightLabel=new QLabel("...");
	labelsLayout->addWidget(weightLabel);
	labelsLayout->addWidget(new QLabel(tr("Width :")));
	widthLabel=new QLabel("...");
	labelsLayout->addWidget(widthLabel);
	//
	saveBtn=new QPushButton(QIcon(":/save.png"),tr("Save"),this);
	connect(saveBtn,SIGNAL(clicked()),this,SLOT(saveBtnClickedSlot()));
	btnsLayout->addWidget(saveBtn);
	contentComboMapper=new QSignalMapper(this);
	connect(contentComboMapper,SIGNAL(mapped(int)),this,SLOT(contentComboMapperSlot(int)));
	contentLineEditMapper=new QSignalMapper(this);
	connect(contentLineEditMapper,SIGNAL(mapped(int)),this,SLOT(contentLineEditMapperSlot(int)));
}
basketContentsClass::~basketContentsClass()
{

}
void basketContentsClass::updateData(int basketNum_)
{
	basketNum=basketNum_;
	updateData();
}
void basketContentsClass::saveBtnClickedSlot()
{
	save();
	updateData();
}
void basketContentsClass::save()
{
	for (int i=0;i<(tableWdgt->rowCount()-1);i++)
	{
		int tId=tableWdgt->verticalHeaderItem(i)->data(Qt::UserRole).toInt();
		bool isOk=tableWdgt->verticalHeaderItem(i)->data(Qt::UserRole+1).toBool();
		QString contentName=((QLineEdit*)tableWdgt->cellWidget(i,0))->text();
		QString contentType=((QComboBox*)(tableWdgt->cellWidget(i,1)))->currentText();
		if (tId>0)
		{
			if (isOk)
				execQuery(QString("update basketsContents set contentName='%1',contentType='%2' where tId=%3").arg(contentName).arg(contentType).arg(tId),db);
			else
				execQuery(QString("delete from  basketsContents where tId=%1").arg(tId),db);
		}
		else
		{
			if ((isOk)&&(basketNum!=0))
				execQuery(QString("insert into basketsContents (contentName,contentType,basket) values ('%1','%2',%3)").arg(contentName).arg(contentType).arg(basketNum),db);
		}
	}
	updateData();
}
void basketContentsClass::fillTableRow(int row,int tId,QString contentName,QString contentType)
{
	tableWdgt->blockSignals(true);
	QComboBox *contentComboBox=new QComboBox();
	double weight=0.;
	double width=0.;
	bool isSpare=(contentType=="spare part");
	if (typeOfBasket(basketNum)==cellTypeNormal)
	{
		if (contentType!=contentNone)
		{
			weight=contentSmallWeightMap[contentType];
			width=contentSmallWidthMap[contentType];
		}
		QMapIterator<QString,double> itter(contentSmallWeightMap);
		contentComboBox->addItem(contentNone,"");
		while (itter.hasNext()) 
		{
			itter.next();
			QString cont=itter.key();
			contentComboBox->addItem(QString("%1").arg(cont),cont);
		}
	}
	if (typeOfBasket(basketNum)==cellTypeBig)
	{
		if (contentType!=contentNone)
		{
			weight=contentAllWeightMap[contentType];
			width =contentAllWidthMap[contentType];
		}
		QMapIterator<QString,double> itter(contentAllWeightMap);
		contentComboBox->addItem(contentNone,"");
		while (itter.hasNext()) 
		{
			itter.next();
			QString cont=itter.key();
			contentComboBox->addItem(QString("%1").arg(cont),cont);
		}
	}
	contentComboBox->setCurrentText(contentType);
	connect(contentComboBox,SIGNAL(currentIndexChanged(int)),contentComboMapper,SLOT(map()));
	contentComboMapper->setMapping(contentComboBox,row);
	//
	QTableWidgetItem *rowHeaderItem=new QTableWidgetItem(QString("%1").arg(row+1));
	rowHeaderItem->setData(Qt::UserRole,tId);
	tableWdgt->setVerticalHeaderItem(row,rowHeaderItem);
	tableWdgt->setCellWidget(row,1,contentComboBox);
	QLineEdit *contentLineEdit=new QLineEdit(contentName);
	if (!isSpare)
		contentLineEdit->setInputMask("9999-90; ");
	connect(contentLineEdit,SIGNAL(textChanged(QString)),contentLineEditMapper,SLOT(map()));
	contentLineEditMapper->setMapping(contentLineEdit,row);
	tableWdgt->setCellWidget(row,0,contentLineEdit); 
	tableWdgt->setItem(row,2,new QTableWidgetItem(QString("%1/%2").arg(toLibs(weight),0,'f',1,' ').arg(weight,0,'f',1,' '))); 
	tableWdgt->setItem(row,3,new QTableWidgetItem(QString("%1/%2").arg(toInches(width),0,'f',1,' ').arg(width,0,'f',1,' ')));
	tableWdgt->setItem(row,4,new QTableWidgetItem(QString("%1/%2").arg(toInches(width),0,'f',1,' ').arg(width,0,'f',1,' ')));

	scheduleStruct productionShiftResults=getProductionShiftOfDie(((QLineEdit*)tableWdgt->cellWidget(row,0))->text());;
	tableWdgt->setItem(row,4,new QTableWidgetItem(productionShiftResults.timestamp));
	//scheduleStruct productionShiftResults;
	//productionShiftResults=getProductionShiftOfDie(((QLineEdit*)tableWdgt->cellWidget(row,0))->text());
	//
	//((QLineEdit*)tableWdgt->cellWidget(row,4))->setReadOnly(true);
	//((QLineEdit*)tableWdgt->cellWidget(row,4))->setFrame(false);
	//((QLineEdit*)tableWdgt->cellWidget(row,4))->setText(productionShiftResults.timestamp);

	if(productionShiftResults.timestamp=="None")
		tableWdgt->item(row,4)->setTextColor(QColor(150,150,150));
	else if (productionShiftResults.newDayShift)
		tableWdgt->item(row,4)->setTextColor(QColor(100,200,100));
	else
		tableWdgt->item(row,4)->setTextColor(QColor(200,100,100));
	tableWdgt->resizeColumnToContents(4);
	tableWdgt->item(row,2)->setData(Qt::UserRole,weight);
	tableWdgt->item(row,2)->setFlags(tableWdgt->item(row,2)->flags()^Qt::ItemIsEditable);
	tableWdgt->item(row,3)->setData(Qt::UserRole,width);
	tableWdgt->item(row,3)->setFlags(tableWdgt->item(row,3)->flags()^Qt::ItemIsEditable);
	//tableWdgt->item(row,4)->setFlags(tableWdgt->item(row,4)->flags()^Qt::ItemIsEditable);
	tableWdgt->blockSignals(false);
}
void basketContentsClass::updateData()
{
	setWindowTitle(tr("Basket %1").arg(basketNum));
	tableWdgt->blockSignals(true);
	tableWdgt->clearContents();
	tableWdgt->setRowCount(0);
	if (basketNum==0)
		return;
	QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select tId,contentName,contentType from basketsContents where basket=%1").arg(basketNum),db);
	tableWdgt->setRowCount(dataVV.count()+1);
	for (int i=0;i<dataVV.count();i++) 
	{
		fillTableRow(i,dataVV[i][0].toInt(),dataVV[i][1].toString().trimmed(),dataVV[i][2].toString().trimmed());
	}
	tableWdgt->sortItems(4);
	fillTableRow(dataVV.count(),-1,"",contentNone);
	tableWdgt->resizeRowsToContents();
	tableWdgt->resizeColumnsToContents();
	tableWdgt->blockSignals(false);
	checkRows();
}

bool basketContentsClass::checkRows()
{
	tableWdgt->blockSignals(true);
	bool isSpare=false;
	if (getFirst(QString("select count(tId) from basketsContents where contentType='spare part' and basket=%1").arg(basketNum),db).toInt()>0)//spare parts basket
		isSpare=true;
	double totalWeight=0.;
	double totalWidth=0.;
	bool isContentOk=true;
	QStringList diesInCasseteL;
	for (int i=0;i<(tableWdgt->rowCount());i++)
	{
		QComboBox *contentComboBox=(QComboBox*)tableWdgt->cellWidget(i,1);
		if (contentComboBox->currentText()=="spare part")
			isSpare=true;
		QString errorToolStr;
		QLineEdit *contentLineEdit=((QLineEdit*)tableWdgt->cellWidget(i,0));
		bool isRowOk=true;
		if (!isSpare)
		{
			if (!contentLineEdit->hasAcceptableInput())
			{
				errorToolStr+=tr("Name is not correct, record will be deleted\n");
				isRowOk=false;
				isContentOk=false;
			}

			if(isRowOk && (!specialDiesG->isCombinationValid(basketNum,contentLineEdit->text())))
			{
				
				errorToolStr+=tr("%1\n").arg(specialDiesG->getErrorStr());
				isRowOk=false;
				isContentOk=false;
			}

			int tId=tableWdgt->verticalHeaderItem(i)->data(Qt::UserRole).toInt();
			int noOfRecords=getOne(QString("select count(tId) from basketsContents where contentName='%1'").arg(contentLineEdit->text()),db).toInt();
			if (((tId>0)&&(noOfRecords>1))||((tId<0)&&(noOfRecords>0)))
			{
				errorToolStr+=tr("%1 is allready defined in warehouse\n").arg(contentLineEdit->text());
				isRowOk=false;
				isContentOk=false;
			}
			if (((QComboBox*)(tableWdgt->cellWidget(i,1)))->currentText()==contentNone)
			{
				errorToolStr+=tr("Type is null, record will be deleted\n");
				isRowOk=false;
				isContentOk=false;
			}
			if(isRowOk)
			{
				if(diesInCasseteL.contains(contentLineEdit->text()))
				{
					errorToolStr+=tr("Die %1 Already Exists In Previous Row And Will Be Deleted\n").arg(contentLineEdit->text());
					isRowOk=false;
					isContentOk=false;
				}
				else
					diesInCasseteL.append(contentLineEdit->text());
			}
		}
		//
		tableWdgt->verticalHeaderItem(i)->setData(Qt::UserRole+1,isRowOk);
		tableWdgt->verticalHeaderItem(i)->setTextColor(isRowOk?Qt::darkBlue:Qt::red);
		contentLineEdit->setStyleSheet(isRowOk?lineEditG:lineEditR);
		tableWdgt->verticalHeaderItem(i)->setToolTip(errorToolStr);
		totalWeight+=tableWdgt->item(i,2)->data(Qt::UserRole).toDouble();
		totalWidth+=tableWdgt->item(i,3)->data(Qt::UserRole).toDouble();
	}
	weightLabel->setText(tr("%1%").arg(100.*totalWeight/basketAvailableWeight,0,'f',1,' '));
	weightLabel->setToolTip(tr("%1 libs of maximum %2 libs").arg(toLibs(totalWeight)).arg(toLibs(basketAvailableWeight)));
	widthLabel->setText(tr("%1%").arg(100.*totalWidth/basketAvailableWidth,0,'f',1,' '));
	widthLabel->setToolTip(tr("%1 inches of maximum %2 inches").arg(toInches(totalWidth)).arg(toInches(basketAvailableWidth)));
	saveBtn->setEnabled(totalWeight<basketAvailableWeight);
	tableWdgt->resizeRowsToContents();
	tableWdgt->resizeColumnsToContents();
	tableWdgt->blockSignals(false);
	emit stateChangedSignal(totalWeight<basketAvailableWeight);
	return isContentOk;
}
void basketContentsClass::contentLineEditMapperSlot(int row)
{
	scheduleStruct productionShiftResults;
	productionShiftResults=getProductionShiftOfDie(((QLineEdit*)tableWdgt->cellWidget(row,0))->text());
	tableWdgt->item(row,4)->setText(productionShiftResults.timestamp);
	if(productionShiftResults.timestamp=="None")
		tableWdgt->item(row,4)->setTextColor(QColor(150,150,150));
	else if (productionShiftResults.newDayShift)
		tableWdgt->item(row,4)->setTextColor(QColor(100,200,100));
	else
		tableWdgt->item(row,4)->setTextColor(QColor(200,100,100));

	if(checkRows())
	{
		tableWdgt->insertRow(tableWdgt->rowCount());
		fillTableRow(tableWdgt->rowCount()-1,-1,"",contentNone);
	}
}
void basketContentsClass::contentComboMapperSlot(int row)
{
	QComboBox *contentComboBox=(QComboBox*)tableWdgt->cellWidget(row,1);
	if (contentComboBox)
	{
		int tId=tableWdgt->verticalHeaderItem(row)->data(Qt::UserRole).toInt();
		fillTableRow(row,tId,((QLineEdit*)tableWdgt->cellWidget(row,0))->text(),contentComboBox->currentText());
	}

	if(checkRows())
	{
		tableWdgt->insertRow(tableWdgt->rowCount());
		fillTableRow(tableWdgt->rowCount()-1,-1,"",contentNone);
	}
}

scheduleStruct basketContentsClass::getProductionShiftOfDie(QString die)
{
	scheduleStruct temp;
	temp.timestamp="None";
	if(die.split('-').count()!=2 || die.split('-')[0].count()<1 || die.split('-')[1].count()<1)
		return temp;
	
	//getFirst without comparing SchedDt to CurrentTime() or getDate(), schedDt Column is not correct. We should display the correct SchedDt time that is the current/next
	// schedDt time.
	//However, even the DateDiff(dieSchedtDate(),currentTime())>0 condition alone can produce incorrect results, bacause of delays caused by errors, etc.
	//example:
	//	current time: 10:01 | export's list contains: cassete(10)->dies{3304->schedDt(10:45),8975->schedDt(10:45)}
	//	(also lets suppose die 3304 has also! been scheduled for 11:15 except for 10:45 and die 8975 has no other scheduled date)
	//  bCont SchedDt column shows correctly that both dies are scheduled for 10:45
	//	in case of an error that will result in 30 minutes delay, the current situation is as follows:
	//	current time: 10:31 (restart after error) | export's list contains: cassete(10)->dies{3304->schedDt(10:45),8975->schedDt(10:45)}
	//	but bCont SchedDt column now shows die 3304 has been scheduled for 11:15 instead of 10:45 as a result of delay. 
	//	It might be necessary instead of relying on DateDiff() to find the current schedule of a die, to keep the schedDt in a seperate column
	//  at the export's table, after the cassette has been insterted into exports lists. This way, even after several restarts and delays, the bCont widget 
	//	will be able to search if there is an entry in assimakisDieWarehouse view, that matches one or more dies that are being displayed together with the schedDt
	//	that corresponds to the cassette that is on the exports list.

	//This function adds a number (a signed integer!) to a datepart of an input date, and returns a modified date/time value. For example, you can use this function to find the date that is 7000 minutes from today: number = 7000, datepart = minute, date = today.

	//ama vrei aurianh, ginetai return me ena keno stin arxh gia na vgainoun prwta logw ascii sort
	//ama einai xthesinh, kanoniko return
	//ama den vrei oute aurio, oute xthes, return None gt to N einai meta kai apo arithmous kai apo to keno sotn ascii
	temp.timestamp=getFirst(QString("select SchedDt from assimakisDieWarehouse where DieNum=%1 AND CopyNum=%2 and ( SchedDt between GETDATE() and DATEADD(day,5,GETDATE()) ) order by SchedDt").arg(die.split('-')[0]).arg(die.split('-')[1]),db).toString();
	//if not exist in the next 24hours, then check previous 24hours
	if(temp.timestamp=="0")
	{
		temp.timestamp=getFirst(QString("select SchedDt from assimakisDieWarehouse where DieNum=%1 AND CopyNum=%2 and ( SchedDt between DATEADD(day,-1,GETDATE()) and GETDATE() ) order by SchedDt DESC").arg(die.split('-')[0]).arg(die.split('-')[1]),db).toString();
		temp.newDayShift=false;
		//if not exist in the previous 24hours, then NONE
		if(temp.timestamp=="0")
		{
			temp.timestamp="None";
			return temp;
		}
		temp.timestamp=temp.timestamp.replace("T"," ");
		return temp;
	}
	//exists in the next 24hours 
	temp.newDayShift=true;
	temp.timestamp=(temp.timestamp.replace("T"," "));
	return temp;
}
