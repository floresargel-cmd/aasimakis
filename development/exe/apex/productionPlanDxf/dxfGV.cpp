#include "dxfGV.h"

dxfQGraphicsViewClass::dxfQGraphicsViewClass(QWidget* parent,double minScaleFactor_,double maxScaleFactor_) : QGraphicsView(parent) 
{
	minScaleFactor=minScaleFactor_;
	maxScaleFactor=maxScaleFactor_;
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	resetView();
}
void dxfQGraphicsViewClass::resetView()
{
	resetTransform();
	setTransform(QTransform(1.,0.,0.,-1.,0.,0.),false);
}
void dxfQGraphicsViewClass::scaleView(double scaleFactor)
 {
	qreal factor=transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if ((factor>minScaleFactor)||(factor<maxScaleFactor))
		return;
	scale(scaleFactor, scaleFactor);
 }
void dxfQGraphicsViewClass::wheelEvent(QWheelEvent* ev)
{
	scaleView(pow((double)1.25,-ev->delta()/240.0));
}