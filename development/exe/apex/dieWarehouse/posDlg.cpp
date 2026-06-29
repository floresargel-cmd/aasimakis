#include "pch.h"
#include "posDlg.h"
#include "allPos.h"
extern 	allPossClass *globalAllPosPtr;
posDlgClass::posDlgClass(int posId_,int basketNum_,QString description_,int column_,int row_,float plcXPos_,float plcYPos_,float plcZPos_,int type_,int activeTab,QWidget *p):QDialog(p)
{
	posId=posId_;
	description=description_;
	column=column_;
	row=row_;
	plcXPos=plcXPos_;
	plcYPos=plcYPos_;
	plcZPos=plcZPos_;
	basketNum=basketNum_;
	type=type_;
	//
	if (type&showPositionData)
	{
		QVector<QVector<QVariant>> dataVV=execTableQuery(QString("select dx,dy,dz from pos where plcId=%1").arg(posId),db);
		if (dataVV.count()==1)
		{
			plcXCorrection=dataVV[0][0].toFloat();
			plcYCorrection=dataVV[0][1].toFloat();
			plcZCorrection=dataVV[0][2].toFloat();
		}
	}
	//
	resize(1000,1000);
	setWindowTitle(tr("Position %1,%2 (%3,%4)").arg(posId).arg(description).arg(column).arg(row));
	QWidget *posWidget=new QWidget(this);
	//basket
	QGridLayout *dataWidgetLayout=new QGridLayout();
	dataWidgetLayout->addWidget(new QLabel(tr("basket:")),0,0);
	basketSpinBox=new QSpinBox();
	basketSpinBox->setMinimum(-INT_MAX);
	basketSpinBox->setMaximum(INT_MAX);
	basketSpinBox->setValue(basketNum);
	basketSpinBox->selectAll();
	basketSpinBox->setStyleSheet("QSpinBox{font-size:20px;border-radius:7px;border-color:#8888aa;padding-left:10px;padding-right:10px;padding-top:5px;padding-bottom:5px;color:#555599;}");
	basketSpinBox->setEnabled(type&basketNumberIsEditable);

	dataWidgetLayout->addWidget(basketSpinBox,0,1);
	connect(basketSpinBox,SIGNAL(valueChanged(int)),this,SLOT(basketSpinBoxValueChangedSlot(int)));
	basketInfoLabel=new QLabel(tr(""));basketInfoLabel->setMinimumWidth(200);
	basketInfoLabel->setStyleSheet("QLabel{color:red;}");
	dataWidgetLayout->addWidget(basketInfoLabel,0,2);
	lockedCheckBox=NULL;
	xCorrectionSpinBox=yCorrectionSpinBox=zCorrectionSpinBox=NULL;
	if (type&showPositionData)
	{
		lockedCheckBox=new QCheckBox(tr("locked"));
		dataWidgetLayout->addWidget(lockedCheckBox,1,0);
		lockedCheckBox->setChecked(getOne(QString("select locked from pos where plcId=%1").arg(posId),db).toBool());
		//corrections
		dataWidgetLayout->addWidget(new QLabel(tr("plc")),2,1);
		dataWidgetLayout->addWidget(new QLabel(tr("correction")),2,2);
		dataWidgetLayout->addWidget(new QLabel(tr("X")),3,0);dataWidgetLayout->addWidget(new QLabel(QString("%1").arg(plcXPos)),3,1);
		dataWidgetLayout->addWidget(new QLabel(tr("Y")),4,0);dataWidgetLayout->addWidget(new QLabel(QString("%1").arg(plcYPos)),4,1);
		dataWidgetLayout->addWidget(new QLabel(tr("Z")),5,0);dataWidgetLayout->addWidget(new QLabel(QString("%1").arg(plcZPos)),5,1);
		xCorrectionSpinBox=new QDoubleSpinBox();xCorrectionSpinBox->setMinimum(-75);xCorrectionSpinBox->setMaximum(75);xCorrectionSpinBox->setValue(plcXCorrection);dataWidgetLayout->addWidget(xCorrectionSpinBox,3,2);
		yCorrectionSpinBox=new QDoubleSpinBox();yCorrectionSpinBox->setMinimum(-75);yCorrectionSpinBox->setMaximum(75);yCorrectionSpinBox->setValue(plcYCorrection);dataWidgetLayout->addWidget(yCorrectionSpinBox,4,2);
		zCorrectionSpinBox=new QDoubleSpinBox();zCorrectionSpinBox->setMinimum(-75);zCorrectionSpinBox->setMaximum(75);zCorrectionSpinBox->setValue(plcZCorrection);dataWidgetLayout->addWidget(zCorrectionSpinBox,5,2);
	}
	dataWidgetLayout->setColumnStretch(3,1);
	//
	QWidget *basketWidget=NULL;
	if (type&showBasketData)
	{
		QVBoxLayout *posLogWidgetLayout=new QVBoxLayout();

		QTableWidget *posLogTable=new QTableWidget();posLogWidgetLayout->addWidget(posLogTable);
		posLogTable->setAlternatingRowColors(true);
		posLogTable->horizontalHeader()->setStretchLastSection(true);
		posLogTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		posLogTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		posLogTable->setColumnCount(3);
		posLogTable->setHorizontalHeaderLabels(QStringList()<<tr("basket")<<tr("info")<<tr("time"));
		posLogTable->setWordWrap (true);

		QVector<QVariant> basketV=execQuery(QString("select top 50 basket from logPos where pos=%1 order by tid desc").arg(posId),db);
		QVector<QVariant> timeV=execQuery(QString("select top 50 info from logPos where pos=%1 order by tid desc").arg(posId),db);
		QVector<QVariant> infoV=execQuery(QString("select top 50 time from logPos where pos=%1 order by tid desc").arg(posId),db);
		posLogTable->setRowCount(basketV.count());
		for (int i=0;i<basketV.count();i++)
		{
			posLogTable->setItem(i,0,new QTableWidgetItem(basketV[i].toString()));
			posLogTable->setItem(i,1,new QTableWidgetItem(timeV[i].toString().trimmed()));
			posLogTable->setItem(i,2,new QTableWidgetItem(infoV[i].toString().trimmed()));
		}
		QVBoxLayout *posWidgetLayout=new QVBoxLayout();
		posWidgetLayout->addLayout(dataWidgetLayout);
		posWidgetLayout->addLayout(posLogWidgetLayout);
		posWidget->setLayout(posWidgetLayout);
		//
		basketWidget=new QWidget(this);
		QVBoxLayout *basketWidgetLayout=new QVBoxLayout();

		basketContents=new basketContentsClass(this);
		basketWidgetLayout->addWidget(basketContents);

		basketLogTable=new QTableWidget();
		basketLogTable->setAlternatingRowColors(true);
		basketLogTable->horizontalHeader()->setStretchLastSection(true);
		basketLogTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		basketLogTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		basketLogTable->setColumnCount(3);
		basketLogTable->setHorizontalHeaderLabels(QStringList()<<tr("position")<<tr("info")<<tr("time"));
		basketLogTable->setWordWrap (true);
		basketLogTable->setMaximumHeight(100);
		basketWidgetLayout->addWidget(basketLogTable);
		basketWidget->setLayout(basketWidgetLayout);
	}
	//
	mainTabWidget=new QTabWidget(this);
	if (type&showBasketData)
		mainTabWidget->addTab(basketWidget,tr("basket %1 data").arg(basketNum));
	mainTabWidget->addTab(posWidget,tr("position data"));
	mainTabWidget->setCurrentIndex(activeTab);
	//
	QHBoxLayout *btnsLayout=new QHBoxLayout();
	okBtn=new QPushButton(tr("Ok"),this);
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	btnsLayout->addWidget(okBtn);

	updateBasketData();
	QVBoxLayout *mainLayout=new QVBoxLayout();
	mainLayout->addWidget(mainTabWidget);
	mainLayout->addLayout(btnsLayout);
	setLayout(mainLayout);
}
posDlgClass::~posDlgClass()
{
}
//void posDlgClass::setBarCodeReaderDlgClass(barCodeReaderDlgClass *b)
//{
//	basketContents->setBarCodeReaderDlg(b);
//}
void posDlgClass::lockPositionDataGui(bool l)
{
	if (lockedCheckBox)lockedCheckBox->setEnabled(!l);
	if (xCorrectionSpinBox)xCorrectionSpinBox->setEnabled(!l);
	if (yCorrectionSpinBox)yCorrectionSpinBox->setEnabled(!l);
	if (zCorrectionSpinBox)zCorrectionSpinBox->setEnabled(!l);
}
int posDlgClass::myExec(bool checkPassword)
{
	if (adminMode)
		return QDialog::exec();
	if (!checkPassword)
		return QDialog::exec();
	passwordClass *pass=new passwordClass(this);
	pass->exec();
	if (pass->getPasswordIsCorrect())
		return QDialog::exec();
	return 0;
}
int posDlgClass::exec()
{
	return QDialog::exec();
}
int posDlgClass::getBasketNum()
{
	return basketSpinBox->value();
}
void posDlgClass::accept()
{
	if (type&showPositionData)
	{
		execQuery(QString("update pos set dx=%1 where plcId=%2").arg(xCorrectionSpinBox->value()).arg(posId),db);
		execQuery(QString("update pos set dy=%1 where plcId=%2").arg(yCorrectionSpinBox->value()).arg(posId),db);
		execQuery(QString("update pos set dz=%1 where plcId=%2").arg(zCorrectionSpinBox->value()).arg(posId),db);
		execQuery(QString("update pos set locked=%1 where plcId=%2").arg(lockedCheckBox->isChecked()?1:0).arg(posId),db);
	}
	QDialog::accept();
}
void posDlgClass::reject()
{
	QDialog::reject();
}
void posDlgClass::basketSpinBoxValueChangedSlot(int b)
{
	QString posOfBasket=globalAllPosPtr->getPosOfBasket(b,posId);
	//int posOfBasket=getFirst(QString("select plcId from pos where basket=%1").arg(b),db).toInt();
	if ((b!=0)&&(posOfBasket.length()!=0))
	{
		basketInfoLabel->setText(tr("basket %1 is on position %2").arg(b).arg(posOfBasket));
		okBtn->setEnabled(false);
	}
	else
	{
		basketInfoLabel->setText(tr(""));
		okBtn->setEnabled(true);
	}
	basketNum=b;
	updateBasketData();
}
void posDlgClass::setBasketNum(int b)
{
	basketNum=b;
	basketSpinBox->setValue(basketNum);
	updateBasketData();
}
void posDlgClass::updateBasketData()
{
	if (type&showBasketData)
	{
		if (basketNum>0)
			mainTabWidget->setTabText(0,tr("basket %1 data").arg(basketNum));
		else
			mainTabWidget->setTabText(0,tr("No basket"));
		//
		if (basketNum!=0)
		{
			basketContents->updateData(basketNum);
			QVector<QVariant> posV=execQuery(QString("select top 50 pos from logPos where basket=%1 order by tid desc").arg(basketNum),db);
			QVector<QVariant> timeV=execQuery(QString("select top 50 info from logPos where basket=%1 order by tid desc").arg(basketNum),db);
			QVector<QVariant> infoV=execQuery(QString("select top 50 time from logPos where basket=%1 order by tid desc").arg(basketNum),db);
			basketLogTable->setRowCount(posV.count());
			for (int i=0;i<posV.count();i++)
			{
				basketLogTable->setItem(i,0,new QTableWidgetItem(posV[i].toString()));
				basketLogTable->setItem(i,1,new QTableWidgetItem(timeV[i].toString().trimmed()));
				basketLogTable->setItem(i,2,new QTableWidgetItem(infoV[i].toString().trimmed()));
			}
		}
		else
		{
			basketContents->updateData(0);
			basketLogTable->setRowCount(0);
		}
	}
}
