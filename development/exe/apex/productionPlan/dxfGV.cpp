#include "dxfGV.h"

dxfQGraphicsViewClass::dxfQGraphicsViewClass(QWidget* parent,double minScaleFactor_,double maxScaleFactor_) : QGraphicsView(parent) 
{
	readyToPan=isPanning=0;
	minScaleFactor=minScaleFactor_;
	maxScaleFactor=maxScaleFactor_;
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);

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
void dxfQGraphicsViewClass::mousePressEvent(QMouseEvent* ev) 
{
	if (ev->button()==Qt::MidButton)
	{
		readyToPan=1;
		isPanning=0;
		panningStart.setX(ev->x());
		panningStart.setY(ev->y());
	}
	QGraphicsView::mousePressEvent(ev);
}
void dxfQGraphicsViewClass::mouseReleaseEvent(QMouseEvent *ev)
{
	if (readyToPan)
		readyToPan=0;
	QGraphicsView::mouseReleaseEvent(ev);
}
void dxfQGraphicsViewClass::mouseMoveEvent(QMouseEvent* ev)
{
	//pan
	if (readyToPan)
	{
		isPanning=1;
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (ev->x() - panningStart.x()));
		verticalScrollBar()->setValue(verticalScrollBar()->value() - (ev->y() -  panningStart.y()));
		panningStart.setX(ev->x());
		panningStart.setY(ev->y());
	}
	QGraphicsView::mouseMoveEvent(ev);
}
