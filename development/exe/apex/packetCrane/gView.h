#ifndef GVIEW_H
#define GVIEW_H
//#include "gItem.h"
//
class myQGraphicsViewClass : public QGraphicsView
{
	bool						readyToPan;
	bool						isPanning;
	QPoint					panningStart;
	bool						isZoomable;
	Q_OBJECT;
public:
	myQGraphicsViewClass(bool isZoomable_=true,QWidget* parent=NULL);
	void setScene(QGraphicsScene *s);
  void resetView();
protected:
	//dist
	QGraphicsLineItem	*distLine;
  QPointF						*distFirstPoint;
	QGraphicsTextItem	*distText;

	void scaleView(double scaleFactor);
	//
	void keyPressEvent(QKeyEvent *ev);
  virtual void mousePressEvent(QMouseEvent* ev);
	virtual void mouseReleaseEvent(QMouseEvent* ev);
  virtual void mouseMoveEvent(QMouseEvent* ev);
  virtual void wheelEvent(QWheelEvent* ev);
signals:
	void keyPressedSignal(QKeyEvent *keyEv);
};
#endif
