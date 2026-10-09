#include "mPos.h"
mainPositionClass::mainPositionClass(tcpServerClass *tcpServer_,QString title_,int plcPosId_,QVector<int>	plcIndexVector,QVector<int>	plcBitVector,QVector<int>	commandsVector,QWidget *p):QPushButton(p)
{
	setAutoFillBackground(true);
	tcpServer=tcpServer_;
	//setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(255, 255, 255)");
	title=title_;
	plcPosId=plcPosId_;
	setSelectedFromPosition(NULL);
	inMaintenanceMode=false;
	//
	QPushButton::setToolTip(QString("%1\nPosition id:%2").arg(title).arg(plcPosId));
	QHBoxLayout *mainLayout=new QHBoxLayout();mainLayout->setMargin(0);mainLayout->setContentsMargins(0,0,0,0);QPushButton::setLayout(mainLayout);

	//QHBoxLayout *posLayout=new QHBoxLayout();posLayout->setMargin(1);mainLayout->addLayout(posLayout);
	//QHBoxLayout *posLayout=new QHBoxLayout();posLayout->setMargin(1);mainLayout->addLayout(posLayout);
	//mainLayout->addWidget(new QLabel(title));
	//mainLayout->setContentsMargins(6,6,6,6);
	//QHBoxLayout *upLayout=new QHBoxLayout();mainLayout->addLayout(upLayout);
	//QVBoxLayout *downLayout=new QVBoxLayout();mainLayout->addLayout(downLayout);
//	QVBoxLayout *leftLayout=new QVBoxLayout();downLayout->addLayout(leftLayout);leftLayout->addWidget(new QLabel(QString().number(plcPosId)));
	//QVBoxLayout *rightLayout=new QVBoxLayout();downLayout->addLayout(rightLayout);
	mainPositionReset=new mainPositionResetClass(tcpServer,commandsVector,title,this);
	//
	for (int i=0;i<plcIndexVector.count()/3;i++)
	{
		positionClass *tPos=new positionClass(plcPosId+i+1,plcIndexVector[i*3],plcIndexVector[i*3+1],plcIndexVector[i*3+2],plcBitVector[i*3],plcBitVector[i*3+1],plcBitVector[i*3+2],commandsVector[i*3],commandsVector[i*3+1],commandsVector[i*3+2],p);
		positionsVector.append(tPos);
		tPos->setAlignment(Qt::AlignCenter);
		mainLayout->addWidget(tPos);
	}
	QPushButton::setMinimumSize(37*positionsVector.count(),32);
}
void mainPositionClass::setInMaintenanceMode(bool m)
{
	inMaintenanceMode=m;
}
void mainPositionClass::setSelectedFromPosition(positionClass *selectedFromPosition_)
{
	selectedFromPosition=selectedFromPosition_;
	QPushButton::setEnabled(false);
	if (!selectedFromPosition)
	{
		if ( !((plcPosId==100) || (plcPosId==200)) )
			QPushButton::setEnabled(true);
	}
	else
	{
		if ( !((plcPosId==600) || (plcPosId==610) || (plcPosId==550)) )
		{
			if (getNumberOfEmptyPos()>0)
			{
				//if (selectedFromPosition->getStatusEnum()==positionClass::STATUS_FULL_SB)
				//{
				//	if ( (plcPosId==500)||(plcPosId==510)||(plcPosId==540)||(plcPosId==110)||(plcPosId==210) )
				//		QPushButton::setEnabled(true);
				//}
				//else
					QPushButton::setEnabled(true);
			}
		}
	}
}
int mainPositionClass::getNumberOfSBs()
{
	int ans=0;
	for (int i=0;i<positionsVector.count();i++)
	{
		if (positionsVector[i]->getHasSB())
			ans++;
		else
			break;
	}
	return ans;
}

int mainPositionClass::getNumberOfEmptyPos()
{
	return positionsVector.count()-getNumberOfSBs();
}
void mainPositionClass::setPlcStatusForNoEmptyFullToTrue(int plcIndex,int plcBitIndex)
{
	for (int i=0;i<positionsVector.count();i++)
		positionsVector[i]->setPlcNoFullEmptyStatusBitToTrue(plcIndex,plcBitIndex);
}
void mainPositionClass::mouseReleaseEvent(QMouseEvent *e)
{
	QPushButton::mouseReleaseEvent(e);
	//
	if (e->button()==Qt::LeftButton)
	{
		if (!inMaintenanceMode)
		{
			if (selectedFromPosition)//look for empty
			{
				positionClass* emptyPos=NULL;
				for (int i=0;i<positionsVector.count();i++)
				{
					if (!positionsVector[i]->getHasSB())
					{
						emptyPos=positionsVector[i];
						break;
					}
				}
				if (emptyPos)
					emit clickedSignal(emptyPos);
			}
			else//look for full
			{
				int mainPositionResetExecValue=QDialog::Accepted;
				if ( (plcPosId==120) || (plcPosId==110) || (plcPosId==210) || (plcPosId==300) || (plcPosId==400))
				{
					mainPositionReset->hideNoBasket();
					mainPositionResetExecValue=mainPositionReset->exec();
				}
				if( (plcPosId==600)||(plcPosId==550))
					defineContents();
				if ((mainPositionResetExecValue==QDialog::Accepted)&&(!((plcPosId==600) || (plcPosId==550))))
				{
					positionClass* fullPos=NULL;
					for (int i=positionsVector.count()-1;i>=0;i--)
					{
						if (positionsVector[i]->getHasSB())
						{
							fullPos=positionsVector[i];
							break;
						}
					}
					if (fullPos)
						emit clickedSignal(fullPos);
				}
			}
		}
		else
			QMessageBox::information(this,tr("Information"),tr("Not active in maintenence mode!"));
	}
	else if (e->button()==Qt::RightButton)
	{
		if (inMaintenanceMode)
		{
			QString passStr=QInputDialog::getText(this,tr("Password neaded for right click operation"),tr("Please input the password"),QLineEdit::Password,"");
			if(passStr=="ass")
			{
				if (inMaintenanceMode)
				{
					mainPositionReset->showAll();
					mainPositionReset->exec();
				}
			}
			else
				QMessageBox::warning(0,tr("Warning"),tr("The password that you typed is incorrect"));
		}
	}
}
bool mainPositionClass::defineContents()
{
	if (plcPosId==600)
		mainPositionReset->hideFullBasket();
	if (plcPosId==550)
		mainPositionReset->hideNoBasket();

	int mainPositionResetExecValue=QDialog::Accepted;
	mainPositionResetExecValue=mainPositionReset->exec();
	if (mainPositionResetExecValue==QDialog::Accepted)
		return true;
	return false;
}
