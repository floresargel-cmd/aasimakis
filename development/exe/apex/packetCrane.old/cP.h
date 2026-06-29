#ifndef CP_H
#define CP_H

#include <QtGui>
#include <QMainWindow>
#include <QList.h>
#include <QtOpenGL>
#include <math.h>
#include "pcG.h"
#include "../common/apexG.h"
#include "p.h"
#include "cellCont.h"
class cellPositionClass:public packetPositionClass			
{
protected:
	int															cassette;
	int															acceptableContents;
	int															storagingTypeOfCassette;
	bool														isLocked;
	int															row;
	int															col;
	//
	bool														isCasseteOnCell;
//
	Q_OBJECT
public:
	cellPositionClass(int posId,int packetId_,int cassette_,int acceptableContents_,int storagingTypeOfCassette_,int isLocked_,int col_,int row_,QWidget *p);
	~cellPositionClass();
	//
	void draw();
	void mouseClicked(QColor pickedColor);
	//
	void saveToDatabase();
	void loadFromDatabase();
	void loadContentsFromDatabase();
	bool hasCassette();
	void setCassette(int cas);
	int getCassette();
	int getStoragingTypeOfCassette();
	int getRow();
	int getColumn();
	void showDlg();
signals:
	void cellDataChangedSignal(int cell);
public slots:
	void onCellDataChangedSlot(int cell);
};
#endif
