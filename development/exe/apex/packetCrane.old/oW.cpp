#include "oW.h"
int lastColorForPicking=0;
QVector<QColor> colorsForPicking;
extern QSplashScreen *splash;
openGLWidgetClass::openGLWidgetClass(QWidget *parent, QGLWidget *shareWidget,GLfloat zoomMin_,GLfloat zoomMax_,GLfloat zRotMin_,GLfloat zRotMax_,GLfloat elevateMin_,GLfloat elevateMax_):QGLWidget(parent, shareWidget)
{
	//colorsForPicking
	QFile file(QString(":/colorsForPicking.txt"));
	file.open (QIODevice::ReadOnly|QIODevice::Text);
	QString line;QStringList lineArray;
	file.readLine();
	while(!file.atEnd()) 
	{
		line=file.readLine();lineArray=line.split(' ',QString::SkipEmptyParts);
		colorsForPicking.append(QColor(lineArray[0].toInt(),lineArray[1].toInt(),lineArray[2].toInt()));
	}
	//
	zoomMin=zoomMin_;zoomMax=zoomMax_;zoom=0.16f;	
	zRotMin=zRotMin_;zRotMax=zRotMax_;zRotate=74.0f;
	elevateMin=elevateMin_;elevateMax=elevateMax_;elevate=67.1f;
	setMouseTracking(true);
	xTranslate=-45.f;
	yTranslate=14.7f;
	baseOpenGl=new openGLPartClass(QString("base"),BASE_TEXTURE_QUAD);
	cellsOpenGl=new openGLPartClass(QString("cells"),CELL_TEXTURE);
	//

	crane=new craneClass(this);
	connect(crane,SIGNAL(openGlChangedSignal()),this,SLOT(openGlPartChangedPosSlot()));

	fork=new forkClass(this);
	connect(fork,SIGNAL(openGlChangedSignal()),this,SLOT(openGlPartChangedPosSlot()));

	allPosisions=new allPosisionsClass(this);
	connect(allPosisions,SIGNAL(openGlChangedSignal()),this,SLOT(openGlPartChangedPosSlot()));
	splash->showMessage(tr("Loading from database..."), Qt::AlignRight | Qt::AlignBottom, QColor(150, 150, 200));
	allPosisions->setConveyorPosition24Class(crane->getConveyorPosition24());
	allPosisions->setConveyorPosition25Class(crane->getConveyorPosition25());
	allPosisions->setPositionCraneClass(crane->getCranePosition());
	crane->setAllPositionsClass(allPosisions);
	fork->setPosition24(crane->getConveyorPosition24());
	fork->setPosition25(crane->getConveyorPosition25());
	//
	manager=new managerClass(allPosisions,crane,crane->getCranePosition(),crane->getConveyorPosition24(),crane->getConveyorPosition25(),fork,fork->getForkPosition(),fork->getPositionSb(),fork->getPositionPa(),fork->getPositionIm(),this);
	fork->setPositionSbExit(manager->getPositionSbExit());
	fork->setPositionPacketExit(manager->getPositionPacketExit());
	fork->setPositionStoreExit(manager->getPositionStoreExit());

	crane->loadFromDatabase();
	fork->loadFromDatabase();
}
openGLWidgetClass::~openGLWidgetClass()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
{
	delete baseOpenGl;
	delete cellsOpenGl;
	delete crane;
	delete fork;
	delete allPosisions;
	delete manager;
}
void openGLWidgetClass::enableAutoMissions(bool enable)
{
	manager->enableAutoMissions(enable);
}
craneClass* openGLWidgetClass::getCraneClass()
{
	return crane;
}
forkClass* openGLWidgetClass::getForkClass()
{
	return fork;
}
allPosisionsClass* openGLWidgetClass::getAllPosClass()
{
	return allPosisions;
}
void openGLWidgetClass::startTimers()
{
	manager->startTimers();
}
void openGLWidgetClass::setExportsDestination(managerClass::exportsDestinationEnum e)
{
	manager->setExportsDestination(e);
}
void openGLWidgetClass::setPlcClass(plcClass *plc_)
{
	plc=plc_;
	manager->setPlcClass(plc);
	allPosisions->setPlcClass(plc);
	crane->setPlcClass(plc);
	fork->setPlcClass(plc);
}
float openGLWidgetClass::getPacketCraneX()
{
	return crane->getPlcX();
}
float openGLWidgetClass::getPacketCraneY()
{
	return crane->getPlcY();
}
float openGLWidgetClass::getPacketCraneZ()
{
	return crane->getPlcZ();
}
float openGLWidgetClass::getForkCraneX()
{
	return fork->getPlcX();
}
float openGLWidgetClass::getForkCraneY()
{
	return fork->getPlcY();
}
float openGLWidgetClass::getForkCraneZ()
{
	return fork->getPlcZ();
}
void openGLWidgetClass::signalFromCranePlc(QString table,int index,QString value)
{
	if (table=="N150")
	{
		if (index==2)
			crane->plcStatusChanged(value.toInt());
		else if (index==10)
			crane->plcCassetteOnCraneChanged(value.toInt());
	}
	if (table=="N152")
	{
		if (index==0)
		{
			QVector<bool> v=deprecatedCalculateBits(value.toInt());
			if (v[31])
				crane->plcCraneIsCommingOutWithCassette();
		}
	}
	else if (table=="F151")
	{
		if (index==0)
			crane->setPlcX(value.toFloat()/1000.);
		else if (index==1)
			crane->setPlcZ(value.toFloat()/1000.);
		else if (index==2)
			crane->setPlcY(value.toFloat()/1000.);
	}
}
void openGLWidgetClass::signalFromForkPlc(QString table,int index,QString value)
{
	if (table=="N160")
	{
		if (index==2)
			fork->plcStatusChanged(value.toInt());
	}
	else if (table=="F161")
	{
		if (index==0)
			fork->setPlcY(value.toFloat()/1000.);
		else if (index==1)
			fork->setPlcZ(-value.toFloat()/1000.);
		else if (index==2)
			fork->setPlcX(value.toFloat()/1000.);
	}
}
void openGLWidgetClass::openGlPartChangedPosSlot()
{
	updateGL();
}
void openGLWidgetClass::initializeGL()
{
	glEnable(GL_DEPTH_TEST);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable (GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPolygonMode( GL_FRONT, GL_FILL );
	glOrtho(-100.0, +100.0, -100.0, +100.0, -1000.0, 1000.0);
//
	bindTexture(QPixmap(QString(":/baseTexture.png")), GL_TEXTURE_2D);				//BASE_TEXTURE
	bindTexture(QPixmap(QString(":/cellTexture.png")), GL_TEXTURE_2D);				//CELL_TEXTURE
	bindTexture(QPixmap(QString(":/cassetteTexture.png")), GL_TEXTURE_2D);		//CASSETTE_TEXTURE
	bindTexture(QPixmap(QString(":/packetTexture.png")), GL_TEXTURE_2D);			//PACKET_TEXTURE
	bindTexture(QPixmap(QString(":/craneBaseTexture.png")), GL_TEXTURE_2D);		//CRANE_BASE_TEXTURE
	bindTexture(QPixmap(QString(":/craneCarrierTexture.png")), GL_TEXTURE_2D);//CRANE_CARRIER_TEXTURE
}

void openGLWidgetClass::paintGL()
{
	paintGLMine(false);
}
void openGLWidgetClass::paintGLMine(bool forPicking)
{
	if (forPicking)
	{
	  qglClearColor(QColor(0,0,0));
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		qglClearColor(QColor(0,0,0));
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	glLoadIdentity();
	glScalef(zoom,zoom,zoom);
	glRotatef(-elevate,1.,0.,0.);
	glRotatef(zRotate,0.,0.,1.);
	glTranslatef(xTranslate,yTranslate,0.);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	///
	if (forPicking)
	{
		allPosisions->draw(forPicking);
		crane->drawForPicking();
		fork->drawForPicking();
	}
	else
	{
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		glColor3ub(150,150,175);
		baseOpenGl->draw();
		cellsOpenGl->draw();
		glPopAttrib();

		allPosisions->draw(forPicking);
		crane->draw();
		fork->draw();
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glFlush();
	return;
}
void openGLWidgetClass::resizeGL(int width, int height)
{
  int side = qMax(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5.0, +5.0, -5.0, +5.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void openGLWidgetClass::mousePressEvent(QMouseEvent *ev)
{
	if (ev->button()==Qt::RightButton)
		lastMousePos=ev->pos();
	if (ev->button()==Qt::MiddleButton)
		midleMousePos=ev->pos();
}
void openGLWidgetClass::selectItem(QMouseEvent *ev,bool leftClick)
{
	 paintGLMine(true);
	 GLubyte  pixel[3];GLint viewport[4];
	 glGetIntegerv(GL_VIEWPORT, viewport);
	 glReadPixels(ev->x(),viewport[3]-ev->y()+2*viewport[1],1,1,GL_RGB,GL_UNSIGNED_BYTE ,(void*)pixel);
	 if (leftClick)
	 {
		 allPosisions->mouseClicked(QColor((int)pixel[0],(int)pixel[1],(int)pixel[2]));
		 crane->mouseClicked(QColor((int)pixel[0],(int)pixel[1],(int)pixel[2]));
		 fork->mouseClicked(QColor((int)pixel[0],(int)pixel[1],(int)pixel[2]));
	 }
	 else
	 {
		 allPosisions->mouseOver(QColor((int)pixel[0],(int)pixel[1],(int)pixel[2]));
		 crane->mouseOver(QColor((int)pixel[0],(int)pixel[1],(int)pixel[2]));
		 fork->mouseOver(QColor((int)pixel[0],(int)pixel[1],(int)pixel[2]));
	}
	updateGL();
}

void openGLWidgetClass::mouseMoveEvent(QMouseEvent *ev)
{
	if (ev->buttons()==Qt::RightButton)
	{
		zRotate+=(ev->x()-lastMousePos.x())/10.;
		elevate-=(ev->y()-lastMousePos.y())/10.;

		if (zRotate<zRotMin)
			zRotate=zRotMin;	
		if (zRotate>zRotMax)
			zRotate=zRotMax;

		if (elevate<elevateMin)
			elevate=elevateMin;	
		if (elevate>elevateMax)
			elevate=elevateMax;
		updateGL();
		lastMousePos=ev->pos();
	}
	else if (ev->buttons()==Qt::MiddleButton)
	{
		xTranslate-=(ev->y()-midleMousePos.y())/10.;
		if (xTranslate<-50.f)xTranslate=-50.f;
		if (xTranslate>50.f)xTranslate=50.f;

		yTranslate-=(ev->x()-midleMousePos.x())/10.;
		if (yTranslate<-50.f)yTranslate=-50.f;
		if (yTranslate>50.f)yTranslate=50.f;
		updateGL();
		midleMousePos=ev->pos();
	}
	else
		selectItem(ev,false);
	return;
}
void openGLWidgetClass::wheelEvent(QWheelEvent *ev)
{
	zoom-=(ev->delta()/2000.);
	if (zoom<zoomMin)
		zoom=zoomMin;	
	if (zoom>zoomMax)
		zoom=zoomMax;
	updateGL();
}
void openGLWidgetClass::mouseReleaseEvent(QMouseEvent *ev)
{
	 selectItem(ev,ev->button()==Qt::LeftButton);
}
