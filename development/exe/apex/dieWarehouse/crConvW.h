#ifndef CRCONVW_H
#define CRCONVW_H

#include "Gd.h"
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "triplePos.h"
#include "plcBit.h"
#include "allPos.h"
class craneConveyorWidgetClass;
#include "crConv.h"
class intStringMapClass
{
	QVector<int>						indexV;
	QVector<QString>				textV;
	QVector<QString>				descriptionV;
public:
	intStringMapClass(){}
	~intStringMapClass(){}
	void add(int index,QString text,QString description)
	{
		indexV<<index;
		textV<<text;
		descriptionV<<description;
	}
	QString getTextOfIndex(int id)
	{
		QString ans;
		int i=indexV.indexOf(id);
		if (i>=0)
			ans=textV[i];
		return ans;
	}
	QString getDescriptionOfIndex(int id)
	{
		QString ans;
		int i=indexV.indexOf(id);
		if (i>=0)
			ans=descriptionV[i];
		return ans;
	}
};
class craneConveyorWidgetClass : public QGroupBox
{
	QLabel									*activeMissionLabel;
	QLabel									*activeCraneStepLabel;
	QLabel									*activeHooksStepLabel;
	QString									activeFromDescription;
	QString									activeToDescription;

	QLabel									*nextMissionLabel;
	QString									nextFromDescription;
	QString									nextToDescription;

	QString									logGuiName;
	QListWidget							*logGuiList;
	int											activeCraneStep;
	intStringMapClass				plcCraneStepTexts;
	int											activeHooksStep;
	intStringMapClass				plcHooksStepTexts;
	Q_OBJECT
public:
	craneConveyorWidgetClass(QWidget *p=NULL);
	~craneConveyorWidgetClass();
	void loadFromDataBase();
	void addMessageToGui(QString mess,QString details,QString type);

	void setNextFrom(QString fDescription);
	void setNextTo(QString tDescription);

	void setActiveFrom(QString fDescription);
	void setActiveTo(QString tDescription);

	void setActiveCraneStep(int s);
	QString getCraneStepText(int s);
	QString getCraneStepDescription(int s);

	void setActiveHooksStep(int s);
	QString getHooksStepText(int s);
	QString getHooksStepDescription(int s);


	void updateGui();
public slots:
	void popUpList();
};
#endif
