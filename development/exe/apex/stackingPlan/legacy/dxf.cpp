#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qglobal.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qdir.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qfile.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qfileinfo.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qlist.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qrect.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qcolor.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainter.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpainterpath.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qpen.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/painting/qtransform.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsitem.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/graphicsview/qgraphicsscene.h"
#include "../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../libs/_deploy/includes/utilities/Gu.h"
#include "dxf.h"
#include "dxfData.h"
#include <dl_dxf.h>
#include <exception>
#include <iostream>
#include <string>
dxfGroupOfGraphicsItemClass::dxfGroupOfGraphicsItemClass() :QGraphicsItem()
{
}
dxfGroupOfGraphicsItemClass::~dxfGroupOfGraphicsItemClass()
{
	for (int i = 0; i < items.count(); i++)
		delete items[i];
}
dxfGroupOfGraphicsItemClass::dxfGroupOfGraphicsItemClass(QString fileName_, double rotationAngle, bool stickToZeroY, QColor color_) :QGraphicsItem()
{
	fileName = fileName_;
	color = color_;
	loadFromDxfFile(rotationAngle, stickToZeroY);
	//setColor(color);
}
void dxfGroupOfGraphicsItemClass::setColor(QColor col)
{
	for (int i = 0; i < items.count(); i++)
		items[i]->color = col;
}
void dxfGroupOfGraphicsItemClass::setPen(QPen p)
{
	for (int i = 0; i < items.count(); i++)
		items[i]->pen = p;
}
QPainterPath dxfGroupOfGraphicsItemClass::shape() const
{
	return pathOfAll;
}
QRectF dxfGroupOfGraphicsItemClass::boundingRect() const
{
	return rectOfAll;
}
void dxfGroupOfGraphicsItemClass::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	for (int i = 0; i < items.count(); i++)
	{
		items[i]->myPaint(painter);
	}
	//painter->drawLine(QLine(0.0,-1.0,0.0,1.0));
	//painter->drawLine(QLine(-1.0,0.0,1.0,0.0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dxfGroupOfGraphicsItemClass::clearItems()
{
	for (int i = 0; i < items.count(); i++)
		delete items[i];
	items.clear();
	rectOfAll = QRectF();
	pathOfAll = QPainterPath();
}
void dxfGroupOfGraphicsItemClass::appendItem(dxfGroupOfGraphicsItemClass* gitem)
{
	for (int i = 0; i < gitem->noOfItems(); i++)
		appendItem(gitem->getItem(i));
}

void dxfGroupOfGraphicsItemClass::appendItem(myGraphicsItem* item)
{
	items.append(item);
	pathOfAll.addPath(item->path);
	rectOfAll = rectOfAll.united(item->path.boundingRect());
}
void dxfGroupOfGraphicsItemClass::appendItems(QList<myGraphicsItem*> items)
{
	for (int i = 0; i < items.count(); i++)
		appendItem(items[i]);
}
// original code comment out by gene

void dxfGroupOfGraphicsItemClass::loadFromDxfFile(double rotationAngle, bool stickToZeroY)
{
	QFileInfo fInfo(fileName);
	if (!fInfo.exists())
		uExit(tr("error"), tr("Could not find %1 file").arg(fileName));
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	if (!fInfo.isNativePath())
#else
	if (!fInfo.isWritable())
#endif
	{
		QString d = QString("qrc");
		QDir::current().mkdir(d);
		QString f = d + "/" + fInfo.fileName();
		QFile::copy(fileName, f);
		fileName = f;
	}
	try
	{
		dxfDataClass* dxfData = new dxfDataClass();//dxf reader
		DL_Dxf* dxf = new DL_Dxf();


		std::string fname = fileName.toStdString();
		bool ans = dxf->in(fname, dxfData);
		if (!ans)
		{
			delete dxf; delete dxfData; return;
		}

		int test = fileName.toLatin1().indexOf("basketSide", 0);
		if (test >= 0) {
			// This finds the basketSide(.dxf) drawing file.
			// If it's the basket, don't try to adjust the drawing position.
			dxfData->calculate(false);
		}
		else {
			// If it's not the basket, then it must be a profile drawing file.
			// Pass in a true.  It will iterate through all the lines in the
			// drawing and find the greatest X & Y offsets beyond the default
			// 500x300 bounding recrangle.  It will use these to reposition
			// the drawing.
			dxfData->calculate(true);
		}

		QList<groupOfLinesClass> groupOfLines = dxfData->getAllGroupsOfLines();
		for (int i = 0; i < groupOfLines.count(); i++)
		{
			groupOfLines[i].rotate(transformOriginPoint(), rotationAngle);
			//int colorIndex=groupOfLines[i].dxfColorIndex;
			//if ((colorIndex<0)||(colorIndex>255))
			//	colorIndex=1;
			//if (colorIndex==7)
			//	colorIndex=0;
			appendItem(new myGraphicsItem(groupOfLines[i], color));
		}
		delete dxf;
		delete dxfData;

		if (stickToZeroY)
			setTransform(QTransform().translate(0.0, boundingRect().height() - boundingRect().bottom()), false);
	}
	catch (const std::bad_alloc& e) {
		// Catch and report memory allocation failure
		std::cerr << "Memory allocation failed: " << e.what() << '\n';
	}
}



QPainterPath dxfGroupOfGraphicsItemClass::getPath()
{

	return pathOfAll;
}
