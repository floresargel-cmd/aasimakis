#ifndef SP_H
#define SP_H

#include <QList.h>
#include <QMainWindow>
#include <QPushButton>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtGui>
class basketClass;

#include "ppG.h"
#include "dxfGV.h"
#include "dxf.h"
#include "ls.h"
#include "bs.h"
class defineSpacersClass : public QDialog
{
	QVBoxLayout* layersLayout;
	QGridLayout* profilesQuandityLayout;
	QVector<QSpinBox*>								profileCodesSpinBoxVector;
	QVector<int>											epicsTableIdVector;
	QVector<QSpinBox*>								profileQuantitySpinBoxVector;
	QVector<int>											profileQuantityThatIsNeededAccordingToTheLayout;
	QSpinBox* basketNumberSpinBox;
	QPushButton* confirmBtn;
	//
	int																basketDBTableId;
	QVector<layerOfSpacersClass*>			layerOfSpacersClassVector;
	//
	Q_OBJECT
public:
	defineSpacersClass(int basketDBTableId_, basketClass* b);
	~defineSpacersClass();
	//
	int getBasketNumber();
	void updateSpacersFromBasket(basketClass* b);
	//int getActiveLayers();
	//layerOfSpacersClass* getLayer(int index);
public slots:
	void confirmBtnSlot();
};
#endif
