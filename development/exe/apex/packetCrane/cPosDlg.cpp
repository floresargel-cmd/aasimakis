#include "pch.h"
#include "cPosDlg.h"
cPosDlgClass::cPosDlgClass(int posId_,int plcIndex_,int basketNum_,QString posDescription,allConvPositionsClass *all_,bool showBasketSpinBox):QDialog((QWidget*)all_)
{
	posId=posId_;
	plcIndex=plcIndex_;
	basketNum=basketNum_;
	allConvPositions=all_;
	//
	setMinimumWidth(600);
	setWindowTitle(tr("Position %1(%2)").arg(plcIndex).arg(posDescription));
	//
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	if (showBasketSpinBox)
	{
		QGridLayout *dataLayout=new QGridLayout();mainLayout->addLayout(dataLayout);
		dataLayout->addWidget(new QLabel(tr("Basket:")),0,0);
		basketSpinBox=new QSpinBox();
		dataLayout->addWidget(basketSpinBox,0,1);
		basketSpinBox->setMinimum(-INT_MAX);
		basketSpinBox->setMaximum(INT_MAX);
		basketSpinBox->setValue(basketNum);
		connect(basketSpinBox,SIGNAL(valueChanged(int)),this,SLOT(basketSpinBoxChangedSlot(int)));
		dataLayout->addWidget(basketSpinBox,0,1);
		basketOkLabel=new QLabel();
		dataLayout->addWidget(basketOkLabel,0,2);
		basketOkLabel->setMinimumWidth(400);
	}
	//
	QVBoxLayout *statusLayout=new QVBoxLayout();mainLayout->addLayout(statusLayout);
	bitVector<<new bitClass("conveyorState",plcIndex,0,bitClass::TYPE_CHANGE_COLOR,tr("Auto"),tr("Auto"),styleLabelG,styleLabelGr,"","","","",this);statusLayout->addWidget(bitVector.last());
	bitVector<<new bitClass("conveyorState",plcIndex,1,bitClass::TYPE_CHANGE_COLOR,tr("Manual"),tr("Manual"),styleLabelG,styleLabelGr,"","","","",this);statusLayout->addWidget(bitVector.last());
	bitVector<<new bitClass("conveyorState",plcIndex,6,bitClass::TYPE_CHANGE_COLOR,tr("Ready"),tr("Not ready"),styleLabelG,styleLabelR,"","","","",this);statusLayout->addWidget(bitVector.last());
	bitVector<<new bitClass("conveyorState",plcIndex,19,bitClass::TYPE_CHANGE_COLOR,tr("Ready for job"),tr("Not ready for job"),styleLabelG,styleLabelGr,"","","","",this);statusLayout->addWidget(bitVector.last());
	bitVector<<new bitClass("conveyorState",plcIndex,22,bitClass::TYPE_CHANGE_COLOR,tr("With basket"),tr("No basket"),styleLabelG,styleLabelGr,"","","","",this);statusLayout->addWidget(bitVector.last());
	//
	QHBoxLayout *btnsLayout=new QHBoxLayout();mainLayout->addLayout(btnsLayout);
	QPushButton *cancelBtn=new QPushButton(tr("Cancel"),this);
	connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
	btnsLayout->addWidget(cancelBtn);
	okBtn=new QPushButton(tr("Ok"),this);
	connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	btnsLayout->addWidget(okBtn);
	okBtn->setDefault(true);
	okBtn->setAutoDefault(true);
	mainLayout->addStretch(1);
}
cPosDlgClass::~cPosDlgClass()
{
}
int cPosDlgClass::exec()
{
	return QDialog::exec();
}
int cPosDlgClass::getBasketNum()
{
	return basketSpinBox->value();
}
void cPosDlgClass::accept()
{
	QDialog::accept();
}
void cPosDlgClass::reject()
{
	QDialog::reject();
}
void cPosDlgClass::setBasketNum(int b)
{
	if (basketSpinBox&&(basketSpinBox->value()!=b))
		basketSpinBox->setValue(b);
}
void cPosDlgClass::basketSpinBoxChangedSlot(int b)
{
	QString posStr;
	if (b>0)
		posStr=allConvPositions->getPosDescriptionWithBasket(b,posId);
	if ((b!=0)&&(posStr.length()>0))
	{
		basketOkLabel->setText(tr("Basket %1 is on position: %2").arg(b).arg(posStr));
		basketOkLabel->setStyleSheet("QLabel{color:red;}");
		okBtn->setEnabled(false);
	}
	else
	{
		okBtn->setEnabled(true);
		basketOkLabel->setText(tr(""));
		basketOkLabel->setStyleSheet("QLabel{color:blue;}");
	}
}
void cPosDlgClass::dataInPlcChanged(QString ip,QString table,int index,QVariant value)
{
	if (ip==packetCraneIp)
	{
		if ((table=="conveyorState")&&(index==plcIndex))
		{
			for (int i=0;i<bitVector.count();i++)
				bitVector[i]->updateValue(table,index,value.toInt());
		}
	}
}
