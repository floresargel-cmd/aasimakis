#ifndef bSelect_h
#define bSelect_h

class basketSelectClass;
#include "Gb3.h"
#include "expWdg.h"

class basketSelectClass:public QDialog
{
	int												selectedBasketNum;
	QString										title;
	QString										dbTable;
	QListWidget								*basketListView;
	QRadioButton							*basketRadioBtn;
	QRadioButton							*shippingBasketRadioBtn;
	exportStationWidgetClass  *exportStationW;
	QToolButton 							*addSelectedBtn;
  enum
	{
		roleBasketNumber=Qt::UserRole,
	};
	Q_OBJECT
public:
	basketSelectClass(QString title_,QString dbTable_,exportStationWidgetClass *exportStationW_);
	void updateList();
	void addBasket(int bskt);
	int exec();
public slots:
	void updateListSlot();
	void listSelectionChanged(QListWidgetItem* currentlySelected,QListWidgetItem* previouslySelected);
	void addSelectedSlot();
};
#endif
