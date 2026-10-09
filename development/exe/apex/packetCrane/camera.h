#ifndef CAMERA_H
#define CAMERA_H

#include <Common.h>
#include <Instance.h>
#include <Media.h>
#include <MediaPlayer.h>
#include <WidgetVideo.h>
#include <Video.h>
class cameraWidgetClass : public QWidget
{
	QWidget																*mainWidget;
	QVBoxLayout														*mainLayout;
	QHBoxLayout														*popUpLayout;
	QHBoxLayout														*zoomOutLayout;
	QHBoxLayout														*zoomInLayout;

	QScrollArea														*zoomInScrollArea;
	QWidget																*zoomOutWidget;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	VlcWidgetVideo												*videoWidget;
	VlcVideo															*video;
	VlcMediaPlayer												*player;
	VlcMedia															*media;
#endif
	QToolBar															*toolBar;
	QAction																*zoomAction;
	bool																	inZoomMode;
  Q_OBJECT
public:
	cameraWidgetClass(QString cameraIp,QString displayName,QWidget *parent=0);
	~cameraWidgetClass();
void cameraWidgetClass::takeSnapShot(QString fName);
private slots:
	void snapShotActionTriggeredSlot();
	void zoomActionTriggeredSlot();
	void connectActionTriggeredSlot();
};
#endif
