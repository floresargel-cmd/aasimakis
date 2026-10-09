#ifndef CRCONVW_H
#define CRCONVW_H

#include "Gsw.h"
class craneConveyorWidgetClass;
#include "serv.h"
#include "hmiGuiWidgetsLib.h"
#include "gView.h"
#include "pos.h"
#include "triplePos.h"
#include "allPos.h"
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
	QLabel									*activeStepLabel;
	QString									activeFromDescription;
	QString									activeToDescription;
	int											activeStep;

	QLabel									*nextMissionLabel;
	QString									nextFromDescription;
	QString									nextToDescription;

	QString									logGuiName;
	QListWidget							*logGuiList;
	intStringMapClass				plcStepTexts;
	Q_OBJECT
public:
	craneConveyorWidgetClass(QString logGuiName_,QWidget *p=NULL);
	~craneConveyorWidgetClass();
	void loadFromDataBase();
	void addMessageToGui(QString mess,QString details,QString type);

	void setNextFrom(QString fDescription);
	void setNextTo(QString tDescription);

	void setActiveFrom(QString fDescription);
	void setActiveTo(QString tDescription);
	void setActiveStep(int s);
	QString getStepText(int s);
	QString getStepDescription(int s);
	void updateGui();
public slots:
	void popUpList();
};
#endif
