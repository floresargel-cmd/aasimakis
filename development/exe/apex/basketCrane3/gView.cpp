#include "pch.h"
#include "gView.h"

myQGraphicsViewClass::myQGraphicsViewClass(bool isZoomable_,QWidget* parent) : QGraphicsView(parent)
{
	isZoomable=isZoomable_;
	setRenderHints(QPainter::Qt4CompatiblePainting);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	resetView();
}
void myQGraphicsViewClass::setScene(QGraphicsScene *s)
{
	QGraphicsView::setScene(s);
}
void myQGraphicsViewClass::resetView()
{
	resetTransform();
	setTransform(QTransform(1.,0.,0.,-1.,0.,0.),false);
}
void myQGraphicsViewClass::keyPressEvent(QKeyEvent *ev)
{
	if (scene()->focusItem())
		scene()->sendEvent(scene()->focusItem(),ev);
	emit keyPressedSignal(ev);
	ev->setAccepted(true);
}
void myQGraphicsViewClass::mousePressEvent(QMouseEvent* ev)
{
  if (ev->button()==Qt::MidButton)
  {
		readyToPan=true;
		isPanning=false;
		panningStart.setX(ev->x());
		panningStart.setY(ev->y());
  }
	QGraphicsView::mousePressEvent(ev);
}
void myQGraphicsViewClass::mouseMoveEvent(QMouseEvent* ev)
{
	//pan
	if (readyToPan)
	{
		isPanning=true;
		horizontalScrollBar()->setValue(horizontalScrollBar()->value()-(ev->x()-panningStart.x()));
		verticalScrollBar()->setValue(verticalScrollBar()->value()-(ev->y()- panningStart.y()));
		panningStart.setX(ev->x());
		panningStart.setY(ev->y());
	}
	else if (distFirstPoint)//distance
	{
#ifdef _DEBUG
		//pos
		QPoint intPos=ev->pos();
		QPointF mousePos=mapToScene(intPos);
		//line
		QLineF tline=distLine->line();
		tline.setP2(mousePos);
		distLine->setLine(tline);
		//text
		intPos.setX(intPos.x()+10);
		intPos.setY(intPos.y());
		QPointF mousePosPlussOffset=mapToScene(intPos);
		QString lStr=QString("x:%1 y=%2\ndist:%3\ndx:%4 dy=%5").arg(mousePos.x()).arg(mousePos.y()).arg(distLine->line().length(),3,'f',2).arg(distLine->line().dx(),3,'f',2).arg(distLine->line().dy(),3,'f',2);
		distText->setPlainText(lStr);
		distText->setPos(mousePosPlussOffset);
		//visible
		distLine->setVisible(true);
		distText->setVisible(true);
#endif
	}
	QGraphicsView::mouseMoveEvent(ev);
}
void myQGraphicsViewClass::mouseReleaseEvent(QMouseEvent *ev)
{
	if (readyToPan)
		readyToPan=false;
	//distance
	if (distFirstPoint)
	{
		delete distFirstPoint;
		distFirstPoint=nullptr;
		scene()->removeItem(distLine);
		scene()->removeItem(distText);
	}
	else
	{
#ifdef _DEBUG
		if ((isPanning==0)&&(ev->button()==Qt::MidButton))
		{
			QPen distPen=QPen(QBrush(QColor(0,0,150)),0.0,Qt::DashDotLine);
			distLine=new QGraphicsLineItem();
			distText=new QGraphicsTextItem();
			distText->setFont(QFont("Helvetica",12));
			distLine->setZValue(100.);
			distText->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
			distText->setZValue(100.);
			//point
			distFirstPoint=new QPointF();
			(*distFirstPoint)=mapToScene(ev->pos());
			//line
			QLineF tline=distLine->line();
			tline.setP1(*distFirstPoint);
			tline.setP2((*distFirstPoint)+QPointF(0.0001,0.0001));
			distLine->setLine(tline);
			scene()->addItem(distLine);
			scene()->addItem(distText);
		}
#endif
	}
	QGraphicsView::mouseReleaseEvent(ev);
}
void myQGraphicsViewClass::scaleView(double scaleFactor)
{
	qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
#ifndef _DEBUG
	if (factor<0.005||factor>1.0)
		return;
#endif
	scale(scaleFactor, scaleFactor);
}
void myQGraphicsViewClass::wheelEvent(QWheelEvent* ev)
{
	if (isZoomable)
		scaleView(pow((double)1.25,-ev->delta()/240.0));
}
