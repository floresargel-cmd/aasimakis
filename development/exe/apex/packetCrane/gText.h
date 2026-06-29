#ifndef G_TEXT_H
#define G_TEXT_H

class myGraphicsTextItem : public QObject,public QGraphicsItem
{
private:
	QRectF									myBoundRect;
	QTextOption							textOp;
	QString									text;
	QFont										font;
	QPen										pen;
	Q_OBJECT
protected:
public:
enum 
{ 
	Type=UserType+3 
};
public:
	myGraphicsTextItem(QString txt,QRectF r,QFont f,QColor col,QGraphicsScene *scene);
	~myGraphicsTextItem();
	void setText(const QString &txt);
	void setText(const QString &txt,QColor c);
	void setColor(QColor c);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *ev);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev);
protected:
};											
#endif
