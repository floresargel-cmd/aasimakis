#include "dxf.h"
dxfGroupOfGraphicsItemClass::dxfGroupOfGraphicsItemClass():QGraphicsItem()
{
}
dxfGroupOfGraphicsItemClass::~dxfGroupOfGraphicsItemClass()
{
	for (int i=0;i<items.count();i++)
		delete items[i];
}
dxfGroupOfGraphicsItemClass::dxfGroupOfGraphicsItemClass(QString fileName_,double rotationAngle,bool stickToZeroY,QColor color_):QGraphicsItem()
{
	fileName=fileName_;
	color=color_;
	loadFromDxfFile(rotationAngle,stickToZeroY);
	//setColor(color);
}
void dxfGroupOfGraphicsItemClass::setColor(QColor col)
{
	for (int i=0;i<items.count();i++)
		items[i]->color=col;
}
void dxfGroupOfGraphicsItemClass::setPen(QPen p)
{
	for (int i=0;i<items.count();i++)
		items[i]->pen=p;
}
QPainterPath dxfGroupOfGraphicsItemClass::shape() const
{
	return pathOfAll;
}
QRectF dxfGroupOfGraphicsItemClass::boundingRect() const
{
	return rectOfAll; 
}
void dxfGroupOfGraphicsItemClass::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
	for (int i=0;i<items.count();i++)
	{
		items[i]->myPaint(painter);
	}
	//painter->drawLine(QLine(0.0,-1.0,0.0,1.0));
	//painter->drawLine(QLine(-1.0,0.0,1.0,0.0));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dxfGroupOfGraphicsItemClass::clearItems()
{
	for (int i=0;i<items.count();i++)
		delete items[i];
	items.clear();
	rectOfAll=QRectF();
	pathOfAll=QPainterPath();
}
void dxfGroupOfGraphicsItemClass::appendItem(dxfGroupOfGraphicsItemClass *gitem)
{
	for (int i=0;i<gitem->noOfItems();i++)
		appendItem(gitem->getItem(i));
}

void dxfGroupOfGraphicsItemClass::appendItem(myGraphicsItem *item)
{
	items.append(item);
	pathOfAll.addPath(item->path);
	rectOfAll=rectOfAll.united(item->path.boundingRect());
}
void dxfGroupOfGraphicsItemClass::appendItems(QList<myGraphicsItem*> items)
{
	for (int i=0;i<items.count();i++)
		appendItem(items[i]);
}
// original code comment out by gene

void dxfGroupOfGraphicsItemClass::loadFromDxfFile(double rotationAngle,bool stickToZeroY)
{
	QFileInfo fInfo(fileName);
	if (!fInfo.exists())
		uExit(tr("error"),tr("Could not find %1 file").arg(fileName));
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	if (!fInfo.isNativePath())
#else
	if (!fInfo.isWritable())
#endif
	{
		QString d=QString("qrc");
		QDir::current().mkdir(d);
		QString f=d+"/"+fInfo.fileName();
		QFile::copy(fileName,f);
		fileName=f;
	}
	try
	{
		dxfDataClass* dxfData = new dxfDataClass();//dxf reader
		DL_Dxf* dxf = new DL_Dxf();


		std::string fname = fileName.toLatin1();
		bool ans = dxf->in(fname, dxfData);
		if (!ans)
		{
			delete dxf;delete dxfData;return;
		}
		dxfData->calculate();
		QList<groupOfLinesClass> groupOfLines = dxfData->getAllGroupsOfLines();
		for (int i = 0;i < groupOfLines.count();i++)
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

// new code inserted by gene
/*
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

	try {
		dxfDataClass* dxfData = new dxfDataClass();  // DXF reader
		DL_Dxf* dxf = new DL_Dxf();

		std::string fname = fileName.toLatin1();
		bool ans = dxf->in(fname, dxfData);
		if (!ans) {
			delete dxf;
			delete dxfData;
			return;
		}

		dxfData->calculate();
		QList<groupOfLinesClass> groupOfLines = dxfData->getAllGroupsOfLines();

		for (int i = 0; i < groupOfLines.count(); i++) {
			groupOfLines[i].rotate(transformOriginPoint(), rotationAngle);
			appendItem(new myGraphicsItem(groupOfLines[i], color));
		}

		delete dxf;
		delete dxfData;

		// Optional: translate Y if needed
		if (stickToZeroY)
			setTransform(QTransform().translate(0.0, boundingRect().height() - boundingRect().bottom()), false);

		// NEW CODE: Scale to fit within 500x300 while preserving aspect ratio
		const QSizeF targetSize(500.0, 300.0);
		QRectF bounds = boundingRect();

		if (!bounds.isEmpty()) {
			qreal scaleX = targetSize.width() / bounds.width();
			qreal scaleY = targetSize.height() / bounds.height();
			qreal scaleFactor = qMin(scaleX, scaleY);  // Maintain aspect ratio

			// Compute offset to center scaled content
			qreal dx = (targetSize.width() - bounds.width() * scaleFactor) / 2.0;
			qreal dy = (targetSize.height() - bounds.height() * scaleFactor) / 2.0;

			QTransform transform;
			transform.translate(dx - bounds.left() * scaleFactor, dy - bounds.top() * scaleFactor);
			transform.scale(scaleFactor, scaleFactor);
			setTransform(transform, false);
		}

	}
	catch (const std::bad_alloc& e) {
		std::cerr << "Memory allocation failed: " << e.what() << '\n';
	}
}
*/ 
//end new code
QPainterPath dxfGroupOfGraphicsItemClass::getPath()
{
	
	return pathOfAll;
}