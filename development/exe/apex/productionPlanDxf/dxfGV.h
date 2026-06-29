#ifndef DXFGV_H
#define DXFGV_H
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTextStream>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QGraphicsView>
//
class dxfQGraphicsViewClass : public QGraphicsView
{
	double					minScaleFactor;
	double					maxScaleFactor;
	Q_OBJECT;
public:
	dxfQGraphicsViewClass(QWidget* parent=NULL,double minScaleFactor_=0.5,double maxScaleFactor_=0.1);
protected:
	//dist
	void resetView();
	void scaleView(double scaleFactor);
	//
	virtual void wheelEvent(QWheelEvent* ev);
};
#endif
