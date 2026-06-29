#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qglobal.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qnamespace.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qcoreevent.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qmimedata.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobject.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qobjectdefs.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qalgorithms.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qline.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qrect.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qvector.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/image/qimage.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/image/qpixmap.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qdrag.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qevent.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qpalette.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qbrush.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qcolor.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainter.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainterpath.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpen.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qtransform.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/text/qfont.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qdialog.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/dialogs/qmessagebox.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsitem.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsscene.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicssceneevent.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qapplication.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qboxlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qgridlayout.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/styles/qstyleoption.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qlabel.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/widgets/qpushbutton.h"

#include "../../common/tcpTux.h"
#include "../Application/Services/MainWindowOrchestrator.h"
#include "bs.h"
#include "dxf.h"
#include "dxfGV.h"
#include "mW.h"
#include "pl.h"
#include "pp.h"
#include "ppG.h"
#include "sp.h"

#include <climits>
#include <memory>
#include <tuxG.h>

namespace
{
	std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator> GetOrchestrator()
	{
		if (mainWindowClass::GetInstance() == NULL)
		{
			return std::shared_ptr<StackingPlan::Application::Services::MainWindowOrchestrator>();
		}

		return mainWindowClass::GetInstance()->getOrchestrator();
	}
}

int newnumber;
QString lastSonum = NULL;
QString currentSonum;
int combinedNumber;

basketProfileGroupClass::basketProfileGroupClass(profileProductionClass* profileProduction_)
{
	signed int lala = INT_MIN;
	Q_UNUSED(lala);

	setAcceptHoverEvents(true);
	setAcceptDrops(true);
	setAcceptedMouseButtons(Qt::LeftButton);

	needsToBeCalculated = 0;
	noOfProfiles = 0;
	length = 0.0;
	isMouseOver = 0;
	isSelected = 0;
	isDragging = 0;
	profileProduction = profileProduction_;
}

basketProfileGroupClass::~basketProfileGroupClass()
{
}

void basketProfileGroupClass::mousePressEvent(QGraphicsSceneMouseEvent*)
{
	if (!needsToBeCalculated)
	{
		setCursor(Qt::ClosedHandCursor);
	}
}

bool basketClass::hasProfileTableId(int tableId)
{
	for (int i = 0; i < profilesGroupsVector.count(); i++)
	{
		if (profilesGroupsVector[i] && profilesGroupsVector[i]->getTableId() == tableId)
		{
			return true;
		}
	}

	return false;
}

void basketProfileGroupClass::mouseMoveEvent(QGraphicsSceneMouseEvent* ev)
{
	if (needsToBeCalculated ||
		(QLineF(ev->screenPos(), ev->buttonDownScreenPos(Qt::LeftButton)).length() < QApplication::startDragDistance()))
	{
		return;
	}

	QDrag* drag = new QDrag(ev->widget());
	QMimeData* mime = new QMimeData;
	mime->setText(QString("%1").arg(getTableId()));
	drag->setMimeData(mime);

	double scale = 2.0;
	QRectF bounds = profileProduction->getProfileGraphics()->boundingRect();
	QPixmap pixmap(
		scale * bounds.width(),
		scale * bounds.height() + 20);

	QPainter painter(&pixmap);
	painter.fillRect(pixmap.rect(), QBrush(QColor(230, 230, 240)));
	painter.setPen(QPen(profileProduction->getColor()));

	QTransform transform;
	transform.translate(-bounds.left(), -bounds.top());
	transform.scale(scale, scale);
	painter.setTransform(transform);

	painter.drawPath(profileProduction->getProfilePath());

	painter.resetTransform();
	painter.setPen(QPen(QColor(0, 0, 100)));
	painter.setFont(QFont("Arial", 12));
	painter.drawText(
		0,
		scale * bounds.height(),
		scale * bounds.width(),
		20,
		Qt::AlignCenter,
		QString("%1").arg(profileProduction->getProfileCode()));
	painter.end();

	drag->setPixmap(pixmap);
	drag->exec();
	setCursor(Qt::OpenHandCursor);
}

void basketProfileGroupClass::mouseReleaseEvent(QGraphicsSceneMouseEvent* ev)
{
	Q_UNUSED(ev);
	setCursor(Qt::OpenHandCursor);
}

void basketProfileGroupClass::dragEnterEvent(QGraphicsSceneDragDropEvent* ev)
{
	if (ev->mimeData()->hasText() && (ev->mimeData()->text().toInt() != getTableId()))
	{
		ev->setAccepted(true);
		isDragging = true;
		update();
	}
	else
	{
		ev->setAccepted(false);
	}
}

void basketProfileGroupClass::dragLeaveEvent(QGraphicsSceneDragDropEvent* ev)
{
	Q_UNUSED(ev);
	isDragging = false;
	update();
}

void basketProfileGroupClass::dropEvent(QGraphicsSceneDragDropEvent* ev)
{
	isDragging = false;

	if (!ev->mimeData()->hasText())
	{
		update();
		return;
	}

	auto mw = mainWindowClass::GetInstance();
	auto orchestrator = mw ? mw->getOrchestrator() : nullptr;

	if (!orchestrator)
	{
		QMessageBox::critical(NULL, "Error", "Orchestrator not initialized");
		return;
	}

	int droppedTid = ev->mimeData()->text().toInt();
	int beforeTid = getTableId();

	QString droppedSoNum = orchestrator->GetSoNumberByTid(droppedTid);
	QString droppedDie = orchestrator->GetDieByTid(droppedTid);
	QString beforeSoNum = orchestrator->GetSoNumberByTid(beforeTid);

	QString msg = QString("Insert %1 (die %2) before %3?")
		.arg(droppedSoNum)
		.arg(droppedDie)
		.arg(beforeSoNum);

	if (QMessageBox::question(NULL, "Confirm", msg) == QMessageBox::Yes)
	{
		if (orchestrator->ReorderProductionSequence(beforeTid, droppedTid))
		{
			emit productionSequenceChangedSignal();
		}
		else
		{
			QMessageBox::critical(NULL, "Error", "Reorder failed");
		}
	}

	update();
}

int basketProfileGroupClass::getTableId()
{
	if (profileProduction)
	{
		return profileProduction->getProfilesEpicsTableId();
	}

	return 0;
}

double basketProfileGroupClass::getLength()
{
	return length;
}

void basketProfileGroupClass::setNeedsToBeCalculated(int needsToBeCalculated_)
{
	if (needsToBeCalculated != needsToBeCalculated_)
	{
		needsToBeCalculated = needsToBeCalculated_;
	}
}

void basketProfileGroupClass::addProfile(layerProfileGroupClass* layerProfileGroup)
{
	length = layerProfileGroup->getProfileProduction()->getProfileLength();
	layerProfilesVector << layerProfileGroup;
	noOfProfiles += layerProfileGroup->getNumberOfProfiles();
	path.addPath(layerProfileGroup->getPath());
}

QRectF basketProfileGroupClass::boundingRect() const
{
	return path.boundingRect();
}

void basketProfileGroupClass::paint(QPainter* painter, const QStyleOptionGraphicsItem* opt, QWidget* w)
{
	Q_UNUSED(opt);
	Q_UNUSED(w);

	if (profileProduction)
	{
		if (isMouseOver || isSelected)
		{
			painter->setPen(QPen(profileProduction->getColor().darker(200)));
		}
		else
		{
			painter->setPen(QPen(profileProduction->getColor()));
		}

		painter->drawPath(path);

		if (profileProduction->getProfileLength() > 7365)
		{
			painter->setPen(QPen(QColor(200, 0, 0), 50));
			painter->drawRect(path.boundingRect());
		}
	}
}

void basketProfileGroupClass::setMouseOverProfileProducton(profileProductionClass* p)
{
	if (profileProduction)
	{
		if (p)
		{
			if (p->getProfilesEpicsTableId() == profileProduction->getProfilesEpicsTableId())
			{
				isMouseOver = true;
			}
		}
		else
		{
			isMouseOver = false;
		}
	}
}

profileProductionClass* basketProfileGroupClass::getProfileProduction()
{
	return profileProduction;
}

QPainterPath basketProfileGroupClass::getProfilesPath()
{
	return path;
}

void basketProfileGroupClass::hoverEnterEvent(QGraphicsSceneHoverEvent* ev)
{
	if (!needsToBeCalculated)
	{
		emit mouseEnterSignal(profileProduction);
	}

	QGraphicsObject::hoverEnterEvent(ev);
}

void basketProfileGroupClass::hoverLeaveEvent(QGraphicsSceneHoverEvent* ev)
{
	if (!needsToBeCalculated)
	{
		emit mouseLeaveSignal(profileProduction);
	}

	QGraphicsObject::hoverLeaveEvent(ev);
}

void basketClass::addBasketNumberSceneLabel()
{
	if (basketNumberSceneText)
	{
		myScene->removeItem(basketNumberSceneText);
		delete basketNumberSceneText;
		basketNumberSceneText = NULL;
	}

	if (basketNumber != 0)
	{
		return;
	}

	if (displayBasketNumber == 1)
	{
		basketNumberSceneText =
			new QGraphicsSimpleTextItem(QString("Unassigned Basket"));
	}
	else {

		basketNumberSceneText =
			new QGraphicsSimpleTextItem(QString("Basket %1").arg(displayBasketNumber));
	}
	QFont basketFont("Arial", 12);
	basketNumberSceneText->setFont(basketFont);
	basketNumberSceneText->setBrush(QBrush(QColor(0, 0, 0)));
	basketNumberSceneText->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);

	QRectF basketBounds = basketGraphics->boundingRect();
	QRectF textBounds = basketNumberSceneText->boundingRect();

	double textX = basketBounds.center().x() - (textBounds.width() / 2.0);
	double textY = basketBounds.bottom() - 75;

	basketNumberSceneText->setPos(textX, textY);
	myScene->addItem(basketNumberSceneText);
}

void basketClass::refreshBasketNumberDisplay()
{
	int resolvedBasketId = -1;

	for (int i = 0; i < layersVector.count(); ++i)
	{
		if (layersVector[i])
		{
			const int candidate = layersVector[i]->getRealBasketId();
			if (candidate > 0)
			{
				resolvedBasketId = candidate;
				break;
			}
		}
	}

	if (resolvedBasketId > 0)
	{
		displayBasketNumber = resolvedBasketId;
	}
	else
	{
		displayBasketNumber = basketNumber + 1;
	}

	if (basketNumberFooterLabel)
	{
		basketNumberFooterLabel->setText(QString("No.%1").arg(basketNumber + 1));
	}

	addBasketNumberSceneLabel();
}

int basketClass::getDisplayBasketNumber() const
{
	return displayBasketNumber;
}

void basketClass::refreshBackgroundBrush()
{
	if (needsToBeCalculated)
	{
		QBrush b = QBrush(Qt::BDiagPattern);
		b.setColor(QColor(150, 150, 150));
		myQGraphicsView->setBackgroundBrush(b);
		return;
	}

	if (maxWeightLimitReached)
	{
		QBrush b = QBrush(QImage(":overWeight.png"));
		myQGraphicsView->setBackgroundBrush(b);
		return;
	}

	QBrush b = QBrush(Qt::SolidPattern);
	b.setColor(QColor(230, 230, 230));
	myQGraphicsView->setBackgroundBrush(b);
}

basketClass::basketClass(int basketNumber_, bool firstToComeOutOfTheStacker_, QWidget* p)
	: QWidget(p)
{
	setAutoFillBackground(true);
	basketNumber = basketNumber_;
	displayBasketNumber = basketNumber_ + 1;
	firstToComeOutOfTheStacker = firstToComeOutOfTheStacker_;
	isSelected = false;
	totalWeight = 0.0;
	tcpServer = NULL;
	maxWeightLimitReached = false;
	needsToBeCalculated = false;
	stackerPlcCombsWord = 0;
	basketNumberSceneText = NULL;
	basketNumberFooterLabel = NULL;
	basketMaxWeightTopLabel = NULL;

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->setMargin(0);
	setLayout(mainLayout);

	basketReadyBtn = NULL;
	auto orchestrator = GetOrchestrator();
	if (firstToComeOutOfTheStacker && orchestrator && orchestrator->GetHavePlcComms())
	{
		basketReadyBtn = new QPushButton(QString("Ready"));
		connect(basketReadyBtn, SIGNAL(clicked()), this, SLOT(basketReadyBtnSlot()));
		mainLayout->addWidget(basketReadyBtn);
	}

	// NEW: top-center basket max weight label
	basketMaxWeightTopLabel = new QLabel(this);
	basketMaxWeightTopLabel->setAlignment(Qt::AlignCenter);
	QFont basketMaxWeightFont = basketMaxWeightTopLabel->font();
	basketMaxWeightFont.setPointSize(12);
	basketMaxWeightFont.setBold(true);
	basketMaxWeightTopLabel->setFont(basketMaxWeightFont);
	basketMaxWeightTopLabel->setText(
		QString("Basket max weight: %1 Kgr").arg((double)MAX_WEIGHT_OF_BASKET, 0, 'f', 1));
	mainLayout->addWidget(basketMaxWeightTopLabel, 0, Qt::AlignHCenter);

	myScene = new QGraphicsScene(this);
	myQGraphicsView = new dxfQGraphicsViewClass(this, 5, 0.35);
	myQGraphicsView->setMinimumSize(600, 350);
	myQGraphicsView->setScene(myScene);
	myQGraphicsView->setContentsMargins(0, 0, 0, 0);

	QBrush b = QBrush(Qt::SolidPattern);
	b.setColor(QColor(230, 230, 230));
	myQGraphicsView->setBackgroundBrush(b);
	mainLayout->addWidget(myQGraphicsView);

	basketGraphics = new dxfGroupOfGraphicsItemClass(":/dxfs/basketSide.dxf", 0.0, true, QColor(75, 50, 50));
	myScene->addItem(basketGraphics);
	basketGraphics->setFlag(QGraphicsItem::ItemIsMovable, false);

	// Keep existing zoom behavior, but apply it only once.
	if (this->parentWidget() && this->parentWidget()->width() < 1366)
	{
		myQGraphicsView->scale(0.55, 0.55);
	}
	else
	{
		myQGraphicsView->scale(0.85, 0.85);
	}

	// Additional full-scene flip to undo the current mirrored output.
	QRectF sceneRect = myScene->itemsBoundingRect();
	QTransform t = myQGraphicsView->transform();
	t.translate(sceneRect.left() + sceneRect.right(), 0.0);
	t.scale(1.0, -1.0);
	myQGraphicsView->setTransform(t);

	myQGraphicsView->centerOn(sceneRect.center());

	QGridLayout* dataLayout = new QGridLayout();
	mainLayout->addLayout(dataLayout);

	basketNumberFooterLabel = new QLabel();
	basketNumberFooterLabel->setText(tr("No.%1").arg(basketNumber + 1));
	dataLayout->addWidget(basketNumberFooterLabel, 0, 0);

	totalWeightLabel = new QLabel(tr("..."));
	QFont theFont = totalWeightLabel->font();
	theFont.setPointSize(14);
	totalWeightLabel->setFont(theFont);
	dataLayout->addWidget(totalWeightLabel, 0, 1, Qt::AlignCenter);

	addBasketNumberSceneLabel();

	if (orchestrator)
	{
		setPrivileges(orchestrator->GetPrivilegeType());
	}
}

basketClass::~basketClass()
{
	delete basketGraphics;
	qDeleteAll(layersVector);
	layersVector.clear();
	qDeleteAll(profilesGroupsVector);
	profilesGroupsVector.clear();
}

void basketClass::enterEvent(QEvent* ev)
{
	Q_UNUSED(ev);
	setBackgroundRole(QPalette::Midlight);
}

void basketClass::leaveEvent(QEvent* ev)
{
	Q_UNUSED(ev);

	if (isSelected)
	{
		setBackgroundRole(QPalette::Highlight);
	}
	else
	{
		if (maxProfileLength > 7365)
		{
			setBackgroundRole(QPalette::Light);
		}
		else
		{
			setBackgroundRole(QPalette::Mid);
		}
	}
}

void basketClass::mousePressEvent(QMouseEvent* ev)
{
	Q_UNUSED(ev);

	if (isSelected)
	{
		emit selectedSignal(this, false);
	}
	else
	{
		emit selectedSignal(this, true);
	}
}

void basketClass::setIsSelected(bool s)
{
	isSelected = s;

	if (isSelected)
	{
		setBackgroundRole(QPalette::Highlight);
	}
	else
	{
		if (maxProfileLength > 7365)
		{
			setBackgroundRole(QPalette::Light);
		}
		else
		{
			setBackgroundRole(QPalette::Mid);
		}
	}
}

bool basketClass::getIsSelected()
{
	return isSelected;
}

int basketClass::getFirstProfileCode()
{
	if (layersVector.count() > 0)
	{
		return layersVector[0]->getFirstProfile();
	}

	return 0;
}

void basketClass::setTcpServer(tcpServerClass* tcpServer_)
{
	tcpServer = tcpServer_;
}

void basketClass::sendDatabaseSpacerDataToPlc(int firstIndex)
{
	if (!tcpServer)
	{
		return;
	}

	auto orchestrator = GetOrchestrator();
	if (orchestrator && orchestrator->GetHavePlcComms())
	{
		QVector<bool> combBitsArray;
		combBitsArray.resize(32);
		combBitsArray.fill(false);

		for (int i = 0; i <= 15; i++)
		{
			if (i < layersVector.count())
			{
				tcpServer->sendString(QString("%1|%2|%3")
					.arg(STACKER_TABLE_WRITE)
					.arg(firstIndex + i)
					.arg(layersVector[i]->getSpacersWord()));
				combBitsArray[i] = layersVector[i]->getIsOnComb();
			}
			else
			{
				tcpServer->sendString(QString("%1|%2|%3")
					.arg(STACKER_TABLE_WRITE)
					.arg(firstIndex + i)
					.arg(0));
			}
		}

		tcpServer->sendString(QString("%1|%2|%3")
			.arg(STACKER_TABLE_WRITE)
			.arg(firstIndex + 15)
			.arg(deprecatedCalculateInt(combBitsArray)));
	}
}

void basketClass::setStackerPlcWords(QVector<int> v)
{
	auto orchestrator = GetOrchestrator();

	if (orchestrator && orchestrator->GetHavePlcComms())
	{
		stackerPlcWordsVector = v;
	}
}

void basketClass::setStackerCombsWord(int v)
{
	auto orchestrator = GetOrchestrator();

	if (orchestrator && orchestrator->GetHavePlcComms())
	{
		stackerPlcCombsWord = v;
	}
}

int basketClass::getStackerPlcWord(int index)
{
	if (index < (stackerPlcWordsVector.count() + 1))
	{
		return stackerPlcWordsVector[index + 1];
	}

	return 0;
}

int basketClass::getStackerPlcCombsWord()
{
	return stackerPlcCombsWord;
}

int basketClass::getDatabaseWord(int index)
{
	if (index < layersVector.count())
	{
		return layersVector[index]->getSpacersWord();
	}

	return 0;
}

int basketClass::getCombNumberOfLayer(int index)
{
	if (index < layersVector.count())
	{
		return layersVector[index]->getCombNumber();
	}

	return 0;
}

void basketClass::calculate()
{

	totalWeight = 0.0;
	maxWeightLimitReached = (totalWeight > MAX_WEIGHT_OF_BASKET);
	refreshBackgroundBrush();
	maxProfileLength = 0.0;

	for (int i = 0; i < layersVector.count(); i++)
	{
		totalWeight += layersVector[i]->getTotalWeight();
		maxProfileLength = MAX(maxProfileLength, layersVector[i]->getMaxProfileLength());
	}

	// Bottom label (unchanged)
	totalWeightLabel->setText(QString("%1Kgr %2mm max")
		.arg(totalWeight, 8, 'f', 1)
		.arg(maxProfileLength, 8, 'f', 1));

	// 🔥 Top center label (UPDATED)
	if (basketMaxWeightTopLabel)
	{
		QString text = QString("Basket max weight: %1 Kgr | Current: %2 Kgr")
			.arg((double)MAX_WEIGHT_OF_BASKET, 0, 'f', 1)
			.arg(totalWeight, 0, 'f', 1);

		// Optional: make it red when exceeding
		if (totalWeight > MAX_WEIGHT_OF_BASKET)
		{
			basketMaxWeightTopLabel->setStyleSheet("color: red;");
		}
		else
		{
			basketMaxWeightTopLabel->setStyleSheet("color: black;");
		}

		basketMaxWeightTopLabel->setText(text);
	}
}

void basketClass::setNeedsToBeCalculated(bool needsToBeCalculated_)
{
	if (needsToBeCalculated != needsToBeCalculated_)
	{
		needsToBeCalculated = needsToBeCalculated_;
		refreshBackgroundBrush();
	}
}

void basketClass::setPrivileges(int privilegeType)
{
	if (basketReadyBtn)
	{
		if (!(privilegeType & PRIVILEGE_TYPE_STACKER))
		{
			basketReadyBtn->setEnabled(false);
		}
		else
		{
			basketReadyBtn->setEnabled(true);
		}
	}
}

void basketClass::basketReadyBtnSlot()
{
	auto mw = mainWindowClass::GetInstance();
	auto orchestrator = mw ? mw->getOrchestrator() : nullptr;

	if (!orchestrator)
	{
		QMessageBox::critical(this, "Error", "Orchestrator not available");
		return;
	}

	int basketId = orchestrator->GetBasketTableIdByPositionId(101);

	defineSpacersClass dlg(basketId, this);

	if (dlg.exec() == QDialog::Accepted)
	{
		orchestrator->MarkUsersNeedsUpdate();
	}
}

double basketClass::getTotalHeight()
{
	if (layersVector.count() == 0)
	{
		return FIRST_LAYER_HEIGTH;
	}

	return layersVector.last()->getTotalHeight();
}

double basketClass::getTotalWeight()
{
	return totalWeight;
}

int basketClass::getMaxUsedComb()
{
	int ans = 0;
	for (int i = 0; i < layersVector.count(); i++)
	{
		if (ans < layersVector[i]->getCombNumber())
		{
			ans = layersVector[i]->getCombNumber();
		}
	}

	return ans;
}

int basketClass::getNumberOfLayers()
{
	return layersVector.count();
}

profileLayerClass* basketClass::getLayer(int index)
{
	return layersVector[index];
}

bool basketClass::hasProfile(int code)
{
	for (int i = 0; i < layersVector.count(); i++)
	{
		if (layersVector[i]->hasProfile(code))
		{
			return true;
		}
	}

	return false;
}

bool basketClass::addEmptyLayerForProfiles(profileProductionClass* p, bool forceOnComb)
{
	double currentHeight = getTotalHeight();

	if (layersVector.count() == 0)
	{
		newnumber = layersVector.count();
	}

	if (lastSonum != p->getSoNum())
	{
		lastSonum = p->getSoNum();
		newnumber = 0;
	}

	combinedNumber = newnumber + ((basketNumber + 1) * 100);

	if (p->getIsOnComb() || forceOnComb || ((layersVector.count() > 0) && (layersVector.last()->getIsOnComb())))
	{
		int comb = getCombWithHeightMoreThan(currentHeight);
		if (comb > MAX_COMBS)
		{
			return false;
		}

		double layerHeight = getHeightOfComb(comb);
		if ((layerHeight + p->getProfileHeight()) < MAX_HEIGHT_OF_LAYERS_IN_A_BASKET)
		{
			layersVector << new profileLayerClass(combinedNumber, comb, layerHeight, myScene);
			newnumber++;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if ((currentHeight + SPACER_THIKNESS + p->getProfileHeight()) < MAX_HEIGHT_OF_LAYERS_IN_A_BASKET)
		{
			layersVector << new profileLayerClass(combinedNumber, 0, currentHeight, myScene);
			newnumber++;
		}
		else
		{
			return false;
		}
	}

	return true;
}

void basketClass::resetLastLayerWidth()
{
	if (!layersVector.isEmpty() && layersVector.last() != NULL)
	{
		layersVector.last()->resetTotalWidth();
	}
}

int basketClass::addProfiles(profileProductionClass* p, int number)
{
	if ((p->getWeightOfOneProfile() + totalWeight) > MAX_WEIGHT_OF_BASKET)
	{
		maxWeightLimitReached = true;
		calculate();
		refreshBackgroundBrush();
		return 0;
	}

	int totalProfilesThatWarePutIntoTheLayer = 0;
	bool freshLayer = false;

	if ((layersVector.count() == 0) ||
		(p->getIsToBePutOnNextLayer()) ||
		(p->getIsOnComb() && ((layersVector.count() > 0) && (!layersVector.last()->getIsOnComb()))))
	{
		if (!addEmptyLayerForProfiles(p, false))
		{
			return totalProfilesThatWarePutIntoTheLayer;
		}

		freshLayer = true;
	}

	int profilesLeftToPutIntoTheLayer = number;
	int safetyCounter = 0;




	calculate();

	int profilesThaCanBePutBecauseOfMaxWeightLimitation =
		(MAX_WEIGHT_OF_BASKET - totalWeight - (p->getSpacers().count(true) * SPACER_WEIGHT)) /
		p->getWeightOfOneProfile();

	if (profilesThaCanBePutBecauseOfMaxWeightLimitation <= 0)
	{
		maxWeightLimitReached = true;
		refreshBackgroundBrush();

	}

	int profilesToPutIntoTheLayer = profilesLeftToPutIntoTheLayer;

	if (profilesToPutIntoTheLayer > profilesThaCanBePutBecauseOfMaxWeightLimitation)
	{
		profilesToPutIntoTheLayer = profilesThaCanBePutBecauseOfMaxWeightLimitation;
	}

	int profilesThatWarePutIntoTheLayer =
		layersVector.last()->addProfiles(p, profilesToPutIntoTheLayer, freshLayer);

	// 🚨 CRITICAL FIX
	if (profilesThatWarePutIntoTheLayer <= 0)
	{
		bool created = addEmptyLayerForProfiles(p, !layersVector.last()->getCanSupportLayer());

		if (!created)
		{
			maxWeightLimitReached = true;
			refreshBackgroundBrush();

		}

		freshLayer = true;

		// 🚨 STILL ZERO AFTER NEW LAYER → STOP
		int retry =
			layersVector.last()->addProfiles(p, profilesToPutIntoTheLayer);



		profilesThatWarePutIntoTheLayer = retry;
	}

	profilesLeftToPutIntoTheLayer -= profilesThatWarePutIntoTheLayer;
	totalProfilesThatWarePutIntoTheLayer += profilesThatWarePutIntoTheLayer;





	calculate();

	if (maxProfileLength > 7365)
	{
		setBackgroundRole(QPalette::Light);
	}
	else
	{
		setBackgroundRole(QPalette::Mid);
	}

	return totalProfilesThatWarePutIntoTheLayer;
}

void basketClass::calculateGroups()
{
	for (int i = 0; i < layersVector.count(); i++)
	{
		QVector<layerProfileGroupClass*> layerGroups = layersVector[i]->getProfileGroups();
		for (int j = 0; j < layerGroups.count(); j++)
		{
			bool found = false;
			for (int k = 0; k < profilesGroupsVector.count(); k++)
			{
				if (profilesGroupsVector[k]->getTableId() == layerGroups[j]->getTableId())
				{
					profilesGroupsVector[k]->addProfile(layerGroups[j]);
					found = true;
					break;
				}
			}

			if (!found)
			{
				basketProfileGroupClass* b = new basketProfileGroupClass(layerGroups[j]->getProfileProduction());
				b->addProfile(layerGroups[j]);
				profilesGroupsVector << b;
			}
		}
	}

	refreshBasketNumberDisplay();
}

QVector<basketProfileGroupClass*> basketClass::getProfilesGroupsVector()
{
	return profilesGroupsVector;
}