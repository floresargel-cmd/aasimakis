#ifndef PPL_H
#define PPL_H

#include <QLabel>
#include <QScrollArea>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <QWidget>

#include "pp.h"
#include <memory>

namespace StackingPlan
{
	namespace Application
	{
		namespace Services
		{
			class MainWindowOrchestrator;
		}
	}
}

class profileProductionLayoutClass : public QWidget
{
	Q_OBJECT

public:
	profileProductionLayoutClass();
	~profileProductionLayoutClass();

	void setNeedsToBeLoadedFromDatabase(int c, int force = 0);
	bool getNeedsToBeLoadedFromDatabase();
	void loadFromDatabase();
	void clearAllProfileProduction();
	void appendProfileProduction(profileProductionClass* p);
	void setOrchestrator(std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> orchestrator);
	void moveProfileLeft();
	void moveProfileRight();
	void deleteProfile();
	profileProductionClass* getSelectedProfileProduction();
	QVector<profileProductionClass*> getProfileProductionVector();
	profileProductionClass* getFirstVisible();
	void setVisibleProfile(int profile);

signals:
	void productionChangedSignal(bool);
	void productionProfileSelectedSignal(profileProductionClass*);

public slots:
	void productionChangedSlot(bool);
	void profileProductionClassSelected(profileProductionClass* pp, bool selected);

private:
	QVBoxLayout* mainLayout;
	QScrollArea* mainScrollArea;
	QVector<profileProductionClass*> profileProductionVector;
	int needsToBeLoadedFromDatabase;
	QLabel* moreProductionAvailable;
	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> _orchestrator;
};

#endif