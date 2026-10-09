#include "pch.h"
#include "camera.h"
cameraWidgetClass::cameraWidgetClass(QString cameraIp,QString displayName,QWidget *parent):QWidget(parent)
{
	inZoomMode=false;
	mainLayout=new QVBoxLayout();mainLayout->setMargin(1);setLayout(mainLayout);
	mainWidget=new QWidget();mainLayout->addWidget(mainWidget);
	QVBoxLayout *mainWidgetLayout=new QVBoxLayout();mainWidgetLayout->setMargin(1);mainWidget->setLayout(mainWidgetLayout);
	zoomInScrollArea=new QScrollArea();mainWidgetLayout->addWidget(zoomInScrollArea);
	zoomOutWidget=new QWidget();mainWidgetLayout->addWidget(zoomOutWidget);

	zoomInScrollArea->setWidgetResizable(true);
	zoomInScrollArea->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
	QWidget *scrollAreaWidget=new QWidget();
	zoomInScrollArea->setWidget(scrollAreaWidget);
	zoomInLayout=new QHBoxLayout();zoomInLayout->setMargin(0);scrollAreaWidget->setLayout(zoomInLayout);

	zoomOutLayout=new QHBoxLayout();zoomOutLayout->setMargin(0);zoomOutWidget->setLayout(zoomOutLayout);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	videoWidget=new VlcWidgetVideo();zoomOutLayout->addWidget(videoWidget);
	//videoWidget->setMinimumSize(1280/10,720/10);
	videoWidget->setMinimumSize(640/10,480/10);

	QStringList args;
	args
				<< "--intf=dummy"
				<< "--no-media-library"
				<< "--no-one-instance"
				<< "--no-plugins-cache"
				<< "--no-stats"
				<< "--no-osd"
				<< "--no-loop"
				<< "--no-video-title-show"
				<< "--no-sout-audio"

				<< "--high-priority"
				//<< "--rtsp-http"
				//<< "--rtp-timeout"<<"1"
				//<< "--logo-file"<<"R:\\development\\\exe\\test\\camera\\resources\\hmi\\assimakis.png"
				<< "--network-caching"<<"250"
				//<< "--network-synchronisation"
				//<< "--live-caching"<<"50"
				//<< "--clock-jitter"<<"200"
				//<< " --rtsp-frame-buffer-size"<<"100000"
				//<< "--live-caching"<<"5000"
				//<< "--clock-synchro"<<"1"
				//<< "--network-synchronisation"
				<< "--drop-late-frames"
				//<< "--skip-frames"
				;


	VlcInstance *instance = new VlcInstance(args, this);
	player = new VlcMediaPlayer(instance);
	player->setVideoWidget(videoWidget);
	media=new VlcMedia(cameraIp,instance);
	video=new VlcVideo(player);
	player->open(media);
	//
	toolBar=new QToolBar(zoomOutWidget);toolBar->setMinimumHeight(40);
	zoomAction=new QAction(QIcon(":/cameraZoomIn.png"),"",this);
	QAction *snapShotAction=new QAction(QIcon(":/snapShot.png"),"",this);
	QAction *connectShotAction=new QAction(QIcon(":/connect.png"),"",this);
	toolBar->addAction(zoomAction);
	toolBar->addAction(snapShotAction);
	toolBar->addAction(connectShotAction);
	zoomInScrollArea->hide();

	connect(zoomAction,SIGNAL(triggered()),this,SLOT(zoomActionTriggeredSlot()));
	connect(snapShotAction,SIGNAL(triggered()),this,SLOT(snapShotActionTriggeredSlot()));
	connect(connectShotAction,SIGNAL(triggered()),this,SLOT(connectActionTriggeredSlot()));
#endif
}
cameraWidgetClass::~cameraWidgetClass()
{
}

void cameraWidgetClass::takeSnapShot(QString fName)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	video->takeSnapshot(fName);
#endif
}
void cameraWidgetClass::snapShotActionTriggeredSlot()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	takeSnapShot(QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss.png"));
#endif
}
void cameraWidgetClass::zoomActionTriggeredSlot()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	inZoomMode=!inZoomMode;
	if (inZoomMode)
	{
		videoWidget->setMinimumSize(1280,720);
		videoWidget->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
		zoomOutLayout->removeWidget(videoWidget);
		zoomInLayout->addWidget(videoWidget);

		toolBar->setParent(zoomInScrollArea);
		zoomAction->setIcon(QIcon(":/cameraZoomOut.png"));

		zoomInScrollArea->show();
		zoomOutWidget->hide();
	}
	else
	{
		videoWidget->setMinimumSize(1280/10,720/10);
		videoWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
		zoomInLayout->removeWidget(videoWidget);
		zoomOutLayout->addWidget(videoWidget);

		toolBar->setParent(zoomOutWidget);
		zoomAction->setIcon(QIcon(":/cameraZoomIn.png"));

		zoomInScrollArea->hide();
		zoomOutWidget->show();
	}
#endif
}
void cameraWidgetClass::connectActionTriggeredSlot()
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	player->open(media);
#endif
}
