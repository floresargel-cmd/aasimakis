#include "fl.h"
faultsListsClass::faultsListsClass(tcpServerClass *tcpServer_,QWidget *p):QWidget(p)
{
	QHBoxLayout *mainLayout=new QHBoxLayout();setLayout(mainLayout);
	QListWidget *faultsList=new QListWidget();mainLayout->addWidget(faultsList);
	QListWidget *statusesList=new QListWidget();mainLayout->addWidget(statusesList);
	N24_00=new myFaultListWidgetItem(tr("Alarm:M-Aux & O/L OK Motor Running Load Conveyor"														));faultsList->addItem(N24_00);
	N24_01=new myFaultListWidgetItem(tr("Alarm:M-Aux & O/L OK Motor Running Oven Conveyor"														));faultsList->addItem(N24_01);
	N24_02=new myFaultListWidgetItem(tr("Alarm:M-Aux & O/L OK Motor Running Unload Conveyor 1"												));faultsList->addItem(N24_02);
	N24_03=new myFaultListWidgetItem(tr("Alarm:M-Aux & O/L OK Motor Running Unload Conveyor 2"												));faultsList->addItem(N24_03);
	N24_04=new myFaultListWidgetItem(tr("Alarm:M-Aux & O/L OK Motor Running Cross Trans. Conveyor 1"									));faultsList->addItem(N24_04);
	N24_05=new myFaultListWidgetItem(tr("Alarm:M-Aux & O/L OK Motor Running Cross Trans. Conveyor 2"									));faultsList->addItem(N24_05);
	N24_06=new myFaultListWidgetItem(tr("Alarm:M-Aux & O/L OK Motor Running Return Conveyor 1"												));faultsList->addItem(N24_06);
	N24_07=new myFaultListWidgetItem(tr("Alarm:M-Aux & O/L OK Motor Running Return Conveyor 2"												));faultsList->addItem(N24_07);
	N24_08=new myFaultListWidgetItem(tr("Alarm:M-Aux & O/L OK Motor Running Return Conveyor 3"												));faultsList->addItem(N24_08);
	N24_09=new myFaultListWidgetItem(tr("Alarm:M-Aux & O/L OK Motor Running Return Conveyor 4"												));faultsList->addItem(N24_09);
	N24_10=new myFaultListWidgetItem(tr("Alarm:Inverter fault ( master drive) "																				));faultsList->addItem(N24_10);
	N24_11=new myFaultListWidgetItem(tr("Alarm:Inverter fault ( slave drive) "																				));faultsList->addItem(N24_11);
	N24_12=new myFaultListWidgetItem(tr("Alarm:Cross transfer conveyor 1 limit switch"																));faultsList->addItem(N24_12);
	N24_13=new myFaultListWidgetItem(tr("Alarm:Cross transfer conveyor 2 limit switch"																));faultsList->addItem(N24_13);
	N24_14=new myFaultListWidgetItem(tr("Alarm:Time for command exceeded"																							));faultsList->addItem(N24_14);
	N24_15=new myFaultListWidgetItem(tr("Alarm:Cross transfer must reset"																							));faultsList->addItem(N24_15);
	N24_16=new myFaultListWidgetItem(tr("Alarm N11[24].16"																														));faultsList->addItem(N24_16);
	N24_17=new myFaultListWidgetItem(tr("Alarm N11[24].17"																														));faultsList->addItem(N24_17);
	N24_18=new myFaultListWidgetItem(tr("Alarm N11[24].18"																														));faultsList->addItem(N24_18);
	N24_19=new myFaultListWidgetItem(tr("Alarm N11[24].19"																														));faultsList->addItem(N24_19);
	N24_20=new myFaultListWidgetItem(tr("Alarm N11[24].20"																														));faultsList->addItem(N24_20);
	N24_21=new myFaultListWidgetItem(tr("Alarm N11[24].21"																														));faultsList->addItem(N24_21);
	N24_22=new myFaultListWidgetItem(tr("Alarm N11[24].22"																														));faultsList->addItem(N24_22);
	N24_23=new myFaultListWidgetItem(tr("Alarm N11[24].23"																														));faultsList->addItem(N24_23);
	N24_24=new myFaultListWidgetItem(tr("Alarm N11[24].24"																														));faultsList->addItem(N24_24);
	N24_25=new myFaultListWidgetItem(tr("Alarm N11[24].25"																														));faultsList->addItem(N24_25);
	N24_26=new myFaultListWidgetItem(tr("Alarm N11[24].26"																														));faultsList->addItem(N24_26);
	N24_27=new myFaultListWidgetItem(tr("Alarm N11[24].27"																														));faultsList->addItem(N24_27);
	N24_28=new myFaultListWidgetItem(tr("Alarm N11[24].28"																														));faultsList->addItem(N24_28);
	N24_29=new myFaultListWidgetItem(tr("Alarm N11[24].29"																														));faultsList->addItem(N24_29);
	N24_30=new myFaultListWidgetItem(tr("Alarm N11[24].30"																														));faultsList->addItem(N24_30);
	N24_31=new myFaultListWidgetItem(tr("Alarm N11[24].31"																														));faultsList->addItem(N24_31);
	
	N25_00=new myFaultListWidgetItem(tr("Alarm:Basket positions 1a,1b,1c  load conveyor "															));faultsList->addItem(N25_00);
	N25_01=new myFaultListWidgetItem(tr("Alarm:Basket positions 2a,2b,2c  load conveyor "															));faultsList->addItem(N25_01);
	N25_02=new myFaultListWidgetItem(tr("Alarm:Basket positions 1a,1b,1c  oven  conveyor "														));faultsList->addItem(N25_02);
	N25_03=new myFaultListWidgetItem(tr("Alarm:Basket positions 2a,2b,2c  oven  conveyor "														));faultsList->addItem(N25_03);
	N25_04=new myFaultListWidgetItem(tr("Alarm:Basket position on unload conveyor 1"																	));faultsList->addItem(N25_04);
	N25_05=new myFaultListWidgetItem(tr("Alarm:Basket position on cross tranfer conveyor 1"														));faultsList->addItem(N25_05);
	N25_06=new myFaultListWidgetItem(tr("Alarm:Basket position on unload conveyor 2"																	));faultsList->addItem(N25_06);
	N25_07=new myFaultListWidgetItem(tr("Alarm:Basket position cross tranfer conveyor 2"															));faultsList->addItem(N25_07);
	N25_08=new myFaultListWidgetItem(tr("Alarm:Basket position return conveyor 1"																			));faultsList->addItem(N25_08);
	N25_09=new myFaultListWidgetItem(tr("Alarm:Basket position return conveyor 2 (30 ft)"															));faultsList->addItem(N25_09);
	N25_10=new myFaultListWidgetItem(tr("Alarm:Basket position return conveyor 2"																			));faultsList->addItem(N25_10);
	N25_11=new myFaultListWidgetItem(tr("Alarm:Basket position return conveyor 3"																			));faultsList->addItem(N25_11);
	N25_12=new myFaultListWidgetItem(tr("Alarm:Basket position return conveyor 3 (30 ft)"															));faultsList->addItem(N25_12);
	N25_13=new myFaultListWidgetItem(tr("Alarm:Basket position return conveyor 4"																			));faultsList->addItem(N25_13);

	N26_00=new myStatusListWidgetItem(tr("Status:Condition oven entrance door not open "															));statusesList->addItem(N26_00);
	N26_01=new myStatusListWidgetItem(tr("Status:Condition oven entrance door not open "															));statusesList->addItem(N26_01);
	N26_02=new myStatusListWidgetItem(tr("Status:Condition cross trans. conveyor 1-2 not in oven position"						));statusesList->addItem(N26_02);
	N26_03=new myStatusListWidgetItem(tr("Status:Condition oven exit door not open "																	));statusesList->addItem(N26_03);
	N26_04=new myStatusListWidgetItem(tr("Status:Condition cross trans. conveyor 1 not oven position "								));statusesList->addItem(N26_04);
	N26_05=new myStatusListWidgetItem(tr("Status:Condition cross trans. conveyor 2 not oven position "								));statusesList->addItem(N26_05);
	N26_06=new myStatusListWidgetItem(tr("Status:Condition Cross trans. conveyor 2 not in return position"						));statusesList->addItem(N26_06);
	N26_07=new myStatusListWidgetItem(tr("Status:Condition Cross trans. conveyor 2 not in return position"						));statusesList->addItem(N26_07);

	N26_13=new myStatusListWidgetItem(tr("Status:Condition Cross trans. conveyor 1 not in return position "						));statusesList->addItem(N26_13);
	N26_14=new myStatusListWidgetItem(tr("Status:Condition Cross trans. conveyor 1 not in return position "						));statusesList->addItem(N26_14);
	N26_15=new myStatusListWidgetItem(tr("Status:Condition no permission from basket crane on loading conveyors"			));statusesList->addItem(N26_15);
	N26_16=new myStatusListWidgetItem(tr("Status:Condition no permission from basket crane on return conveyor 4 (N22)"));statusesList->addItem(N26_16);

	N26_17=new myStatusListWidgetItem(tr("Status N11[26].17"																													));statusesList->addItem(N26_17);
	N26_18=new myStatusListWidgetItem(tr("Status N11[26].18"																													));statusesList->addItem(N26_18);
	N26_19=new myStatusListWidgetItem(tr("Status N11[26].19"																													));statusesList->addItem(N26_19);
	N26_20=new myStatusListWidgetItem(tr("Status N11[26].20"																													));statusesList->addItem(N26_20);
	N26_21=new myStatusListWidgetItem(tr("Status N11[26].21"																													));statusesList->addItem(N26_21);
	N26_22=new myStatusListWidgetItem(tr("Status N11[26].22"																													));statusesList->addItem(N26_22);
	N26_23=new myStatusListWidgetItem(tr("Status N11[26].23"																													));statusesList->addItem(N26_23);
	N26_24=new myStatusListWidgetItem(tr("Status N11[26].24"																													));statusesList->addItem(N26_24);
	N26_25=new myStatusListWidgetItem(tr("Status N11[26].25"																													));statusesList->addItem(N26_25);
	N26_26=new myStatusListWidgetItem(tr("Status N11[26].26"																													));statusesList->addItem(N26_26);
	N26_27=new myStatusListWidgetItem(tr("Status N11[26].27"																													));statusesList->addItem(N26_27);
	N26_28=new myStatusListWidgetItem(tr("Status N11[26].28"																													));statusesList->addItem(N26_28);
	N26_29=new myStatusListWidgetItem(tr("Status N11[26].29"																													));statusesList->addItem(N26_29);
	N26_30=new myStatusListWidgetItem(tr("Status N11[26].30"																													));statusesList->addItem(N26_30);
	N26_31=new myStatusListWidgetItem(tr("Status N11[26].31"																													));statusesList->addItem(N26_31);
}
void faultsListsClass::plcStatusChanged(int index,int value)
{
	if (index==24)
	{
		QVector<bool> bitsVector=deprecatedCalculateBits(value);
		N24_00->setHidden(!bitsVector[ 0]);
		N24_01->setHidden(!bitsVector[ 1]);
		N24_02->setHidden(!bitsVector[ 2]);
		N24_03->setHidden(!bitsVector[ 3]);
		N24_04->setHidden(!bitsVector[ 4]);
		N24_05->setHidden(!bitsVector[ 5]);
		N24_06->setHidden(!bitsVector[ 6]);
		N24_07->setHidden(!bitsVector[ 7]);
		N24_08->setHidden(!bitsVector[ 8]);
		N24_09->setHidden(!bitsVector[ 9]);
		N24_10->setHidden(!bitsVector[10]);
		N24_11->setHidden(!bitsVector[11]);
		N24_12->setHidden(!bitsVector[12]);
		N24_13->setHidden(!bitsVector[13]);
		N24_14->setHidden(!bitsVector[14]);
		N24_15->setHidden(!bitsVector[15]);
		N24_16->setHidden(!bitsVector[16]);
		N24_17->setHidden(!bitsVector[17]);
		N24_18->setHidden(!bitsVector[18]);
		N24_19->setHidden(!bitsVector[19]);
		N24_20->setHidden(!bitsVector[20]);
		N24_21->setHidden(!bitsVector[21]);
		N24_22->setHidden(!bitsVector[22]);
		N24_23->setHidden(!bitsVector[23]);
		N24_24->setHidden(!bitsVector[24]);
		N24_25->setHidden(!bitsVector[25]);
		N24_26->setHidden(!bitsVector[26]);
		N24_27->setHidden(!bitsVector[27]);
		N24_28->setHidden(!bitsVector[28]);
		N24_29->setHidden(!bitsVector[29]);
		N24_30->setHidden(!bitsVector[30]);
		N24_31->setHidden(!bitsVector[31]);
	}
	if (index==25)
	{
		QVector<bool> bitsVector=deprecatedCalculateBits(value);
		N25_00->setHidden(!bitsVector[ 0]);
		N25_01->setHidden(!bitsVector[ 1]);
		N25_02->setHidden(!bitsVector[ 2]);
		N25_03->setHidden(!bitsVector[ 3]);
		N25_04->setHidden(!bitsVector[ 4]);
		N25_05->setHidden(!bitsVector[ 5]);
		N25_06->setHidden(!bitsVector[ 6]);
		N25_07->setHidden(!bitsVector[ 7]);
		N25_08->setHidden(!bitsVector[ 8]);
		N25_09->setHidden(!bitsVector[ 9]);
		N25_10->setHidden(!bitsVector[10]);
		N25_11->setHidden(!bitsVector[11]);
		N25_12->setHidden(!bitsVector[12]);
		N25_13->setHidden(!bitsVector[13]);
	}
	if (index==26)
	{
		QVector<bool> bitsVector=deprecatedCalculateBits(value);
		N26_00->setHidden(!bitsVector[ 0]);
		N26_01->setHidden(!bitsVector[ 1]);
		N26_02->setHidden(!bitsVector[ 2]);
		N26_03->setHidden(!bitsVector[ 3]);
		N26_04->setHidden(!bitsVector[ 4]);
		N26_05->setHidden(!bitsVector[ 5]);
		N26_06->setHidden(!bitsVector[ 6]);
		N26_07->setHidden(!bitsVector[ 7]);
		N26_13->setHidden(!bitsVector[13]);
		N26_14->setHidden(!bitsVector[14]);
		N26_15->setHidden(!bitsVector[15]);
		N26_16->setHidden(!bitsVector[16]);

		N26_17->setHidden(!bitsVector[17]);
		N26_18->setHidden(!bitsVector[18]);
		N26_19->setHidden(!bitsVector[19]);
		N26_20->setHidden(!bitsVector[20]);
		N26_21->setHidden(!bitsVector[21]);
		N26_22->setHidden(!bitsVector[22]);
		N26_23->setHidden(!bitsVector[23]);
		N26_24->setHidden(!bitsVector[24]);
		N26_25->setHidden(!bitsVector[25]);
		N26_26->setHidden(!bitsVector[26]);
		N26_27->setHidden(!bitsVector[27]);
		N26_28->setHidden(!bitsVector[28]);
		N26_29->setHidden(!bitsVector[29]);
		N26_30->setHidden(!bitsVector[30]);
		N26_31->setHidden(!bitsVector[31]);
	}
}
