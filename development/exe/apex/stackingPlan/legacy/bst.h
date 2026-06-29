#ifndef BST_H
#define BST_H

#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qboxlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qscrollarea.h"
#include "../../common/tcpTux.h"

#include "bs.h"
#include "ppl.h"

class basketStackingClass : public QWidget
{
	Q_OBJECT

public:
	explicit basketStackingClass(QWidget* p);
	~basketStackingClass();

	void calculateLayout(profileProductionLayoutClass* profileProductionLayout);
	void setNeedsToBeCalculated(int c, int force = 1);
	void clearAllBaskets();
	void appendBasket(basketClass* b);
	basketClass* getFirstVisible();
	void setVisibleBasketWithProfile(int profile);
	void stackerPlcMemoryChanged(QString table, int index, QString value);
	void setTcpServer(tcpServerClass* tcpServer_);
	QVector<basketClass*> getBasketsVector();
	void setVisibleBasketWithProfileTableId(int tableId);

signals:
	void basketSelectedSignal(basketClass*);

public slots:
	void basketClassSelected(basketClass* b, bool selected);

private:
	tcpServerClass* tcpServer;
	QHBoxLayout* mainLayout;
	QScrollArea* mainScrollArea;
	QWidget* mainScrollAreaWidget;
	QVector<basketClass*> basketsVector;
	QVector<int> stackerPlcWordsVector;
	int stackerPlcCombsWord;
	int needsToBeCalculated;
	QLabel* alertLabel;
};

#endif