#include "oW.h"

int redForPicking=0;
int greenForPicking=0;
int blueForPicking=0;

GLfloat *animateArrowDz;
GLfloat dz[]={0.3f, 0.2f, 0.1f, 0.05f, 0.0f};

bool initalized = false;

int	animateArrowDzFrame;

GLfloat	quadVertexesForGlassArray[] = {49.000f, 7.590f, 0.250f,  66.500, 7.590f, 0.250f,  66.500, 7.590f, 6.050f,   49.000, 7.590f, 6.050f,  //oven
																	/*	17.500f,4.750f,0.0f,      26.950f,4.750f,0.0f,    26.950f,4.750f,2.200f, 17.500f,4.750f,2.200f,  */     };//destacker
int numberOfQuadForGlass = 1;

static QGLFormat modernOpenGlFormat()
{
	QGLFormat format = QGLFormat::defaultFormat();
	format.setDepth(true);
	format.setDoubleBuffer(true);
	format.setSampleBuffers(true);
	format.setSamples(4);
	format.setSwapInterval(1);
	return format;
}

openGLWidgetClass::openGLWidgetClass(tcpServerClass *cranePlcTcpServer_,tcpServerClass *conveyorsPlcTcpServer_,tcpServerClass *newOvenConveyorsPlcTcpServer_,QWidget *parent, QGLWidget *shareWidget,GLfloat zoomMin_,GLfloat zoomMax_,GLfloat zRotMin_,GLfloat zRotMax_,GLfloat elevateMin_,GLfloat elevateMax_):QGLWidget(modernOpenGlFormat(), parent, shareWidget)
{
	basketPositionClass::setLabelWidget(this);
	cranePlcTcpServer = cranePlcTcpServer_;
	conveyorsPlcTcpServer = conveyorsPlcTcpServer_;
	newOvenConveyorsPlcTcpServer = newOvenConveyorsPlcTcpServer_;
	zoomMin = zoomMin_; zoomMax = zoomMax_; zoom = 0.34f;
	zRotMin = zRotMin_; zRotMax = zRotMax_; zRotate = 35.8f;
	elevateMin = elevateMin_; elevateMax = elevateMax_; elevate = 51.2f;
	animateArrowDzFrame = 0;
	animateArrowDz = dz;
	setMouseTracking(true);
	xTranslate = -25.;
	yTranslate = -6.;
	highlightedPosition = NULL;
	area = new openGLPartClass(QString(":/craneArea.stl"), WHITE_TEXTURE_TRIA, WHITE_TEXTURE_QUAD);
	
	crane = new craneClass();
	connect(crane,SIGNAL(openGlChangedSignal()),this,SLOT(openGlPartChangedPosSlot()));

	allPositions = new allPositionsClass(conveyorsPlcTcpServer, newOvenConveyorsPlcTcpServer);
	allPositions->setCraneClass(crane);

	crane->setallPositions(allPositions);
	crane->setCranePlcTcpServer(cranePlcTcpServer);
	crane->setConveyorsPlcTcpServer(conveyorsPlcTcpServer);

	connect(allPositions, SIGNAL(openGlChangedSignal()), this, SLOT(openGlPartChangedPosSlot()));
	connect(crane, SIGNAL(scrapBasketPositionChangedSignal(int)), this, SLOT(scrapBasketPositionChangedSlot(int)));
	connect(crane, SIGNAL(missionFromAfterStackerPositionFinishedSignal()), this, SLOT(missionFromAfterStackerPositionFinishedSlot()));
	connect(crane, SIGNAL(missionFromAfterStacker2PositionFinishedSignal()), this, SLOT(missionFromAfterStacker2PositionFinishedSlot()));

	emit scrapBasketPositionChangedSignal(allPositions->getBasketTableIdOfScrapBasketPos());
	//
	crane->loadFromDatabase();
}

openGLWidgetClass::~openGLWidgetClass()
{
	basketPositionClass::setLabelWidget(NULL);
	delete crane;
	delete area;
	delete allPositions;
}

float openGLWidgetClass::getCraneX()
{
	return crane->getPlcX();
}

float openGLWidgetClass::getCraneY()
{
	return crane->getPlcY();
}

float openGLWidgetClass::getCraneZ()
{
	return crane->getPlcZ();
}

void openGLWidgetClass::addToAutoMode(int mode)
{
	allPositions->addToAutoMode(mode);
}

void openGLWidgetClass::setTakeBasketsFromStackerOut(bool s)
{
	allPositions->setTakeBasketsFromStackerOut(s);
}

void openGLWidgetClass::setTakeBasketsFromStacker2Out(bool s)
{
	allPositions->setTakeBasketsFromStacker2Out(s);
}

void openGLWidgetClass::removeFromAutoMode(int mode)
{
	allPositions->removeFromAutoMode(mode);
}

void openGLWidgetClass::setExportBeforeImports(bool s)
{
	allPositions->setExportBeforeImports(s);
}

void openGLWidgetClass::setNumberOfPosToBeKeptEmptyInBuffer(int n)
{
	allPositions->setNumberOfPosToBeKeptEmptyInBuffer(n);
}

int openGLWidgetClass::getBasketTableIdOfScrapBasketPos()
{
	return allPositions->getBasketTableIdOfScrapBasketPos();
}

void openGLWidgetClass::takeScrapBasket()
{
	allPositions->takeScrapBasket();
}

void openGLWidgetClass::unlockBasketAfterStacker()
{
	allPositions->unlockBasketAfterStacker();
}

void openGLWidgetClass::unlockBasketReturningFromOven()
{
	allPositions->unlockBasketReturningFromOven();
}

void openGLWidgetClass::scrapBasketPositionChangedSlot(int b)
{
	emit scrapBasketPositionChangedSignal(b);
}

void openGLWidgetClass::missionFromAfterStackerPositionFinishedSlot()
{
	emit missionFromAfterStackerPositionFinishedSignal();
}

void openGLWidgetClass::missionFromAfterStacker2PositionFinishedSlot()
{
	emit missionFromAfterStacker2PositionFinishedSignal();
}

int openGLWidgetClass::getNextOvenLoadingCycleTemperEnum()
{
	return allPositions->getNextOvenLoadingCycleTemperEnum();
}

void openGLWidgetClass::setNextOvenLoadingCycleTemperEnum(int t)
{
	return allPositions->setNextOvenLoadingCycleTemperEnum(t);
}
// Added by Vahid
void openGLWidgetClass::setNextOven2LoadingCycleTemperEnum(int t)
{
	return allPositions->setNextOven2LoadingCycleTemperEnum(t);
}

void openGLWidgetClass::recalculateBasketContentsFromCache()
{
	if (!allPositions)
		return;
	allPositions->reloadBasketAssignmentsFromDatabase();
	if (crane)
		crane->synchronizeObserverMissionFromPlc();
	allPositions->recalculateBasketContentsFromCache();
	if (crane)
		crane->recalculateCarriedBasketContentsFromCache();
	updateGL();
}

void openGLWidgetClass::signalFromCranePlc(QString table, int index, QString value)
{
	if (table == "N20")
	{
		crane->plcMemoryChanged(index, value.toInt());

		if (index == 12)
			crane->plcStatusChanged(value.toInt());

		if (index == 13)
			allPositions->plcStatusN20_13Changed(value.toInt());

		if (index == 15)
		 	crane->plcStatusN20_15Changed(value.toInt());

		//if (index==16)
		//	crane->plcStatusN2016Changed(value.toInt());

		if (index == 17)
			allPositions->newBasketInReturningFromDestackerPos(value.toInt());
	}
	else if (table == "F21")
	{
		if (index == 10)
			crane->setPlcX(46.698 - value.toFloat() / 1000.);

		else if (index == 11)
			crane->setPlcY(value.toFloat() / 1000.);

		else if (index == 9)
			crane->setPlcZ(-value.toFloat() / 1000.);
	}
}

void openGLWidgetClass::signalFromConveyorsPlc(QString table, int index, QString value)
{
	if (table == "N11") 
	{
		if (((index >= 1) && (index <= 22)) || (index == 27))
			allPositions->conveyorsBasketMemoryChanged(index, value.toInt());

		// press 2 in / out and new oven loading conveyors
		// if ((table == "N11") && ((index == 39) || (index == 38) || ((index >= 44) && (index <= 46))))
		if ((index == 39) || (index == 38) || ((index >= 44) && (index <= 46)))
			allPositions->conveyorsBasketMemoryChanged(index, value.toInt());

		if (index == 35)
			allPositions->timeLeftForOvenTofinishChanged(value.toInt());
	}
}

void openGLWidgetClass::signalFromNewOvenConveyorsPlc(QString table, int index, QString value)
{
	if ((table == "N11") && (index >= 4) && (index <= 9))  // 1 to 3 now being accessed via oldoven, 44-46
		allPositions->newOvenConveyorsBasketMemoryChanged(index, value.toInt());
}

//int openGLWidgetClass::getBasketTableIdOfBasketAfterStacker()
//{
//	return allPositions->getBasketTableIdOfBasketAfterStacker();
//}

void openGLWidgetClass::openGlPartChangedPosSlot()
{
	LOG("openGLWidgetClass", "INFO: Starting: openGlPartChangedPosSlot - OpenGL part changed position...");
	if( !initalized)
	{
		LOG("openGLWidgetClass", "WARNING: openGlPartChangedPosSlot - OpenGL part changed position but OpenGL is not initialized yet...");
		return;
	}
	updateGL();
	LOG("openGLWidgetClass", "INFO: Completed: openGlPartChangedPosSlot - OpenGL part changed position...");
}

void openGLWidgetClass::initializeGL()
{
	LOG("openGLWidgetClass", "INFO: Starting: initializeGL - Initializing OpenGL...");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DITHER);
#ifdef GL_MULTISAMPLE
	if (format().sampleBuffers())
		glEnable(GL_MULTISAMPLE);
#endif
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-10.0, +10.0, -10.0, +10.0, -1000.0, 1000.0);

	bindTexture(QPixmap(QString(":/whiteTextureTria.png")), GL_TEXTURE_2D);//WHITE_TEXTURE_TRIA
	bindTexture(QPixmap(QString(":/whiteTextureQuad.png")), GL_TEXTURE_2D);//WHITE_TEXTURE_QUAD
	bindTexture(QPixmap(QString(":/orangeTextureTria.png")), GL_TEXTURE_2D);//ORANGE_TEXTURE_TRIA
	bindTexture(QPixmap(QString(":/orangeTextureQuad.png")), GL_TEXTURE_2D);//ORANGE_TEXTURE_QUAD

	bindTexture(QPixmap(QString(":/packet24Tria.png")), GL_TEXTURE_2D);//TEXTURE_FOR_24_TRIA
	bindTexture(QPixmap(QString(":/packet24Quad.png")), GL_TEXTURE_2D);//TEXTURE_FOR_24_QUAD
	bindTexture(QPixmap(QString(":/packet30Tria.png")), GL_TEXTURE_2D);//TEXTURE_FOR_30_TRIA
	bindTexture(QPixmap(QString(":/packet30Quad.png")), GL_TEXTURE_2D);//TEXTURE_FOR_30_QUAD
	
	bindTexture(QPixmap(QString(":/arrowTexture.png")), GL_TEXTURE_2D);//ARROW_TEXTURE

	initalized = true;
	LOG("openGLWidgetClass", "INFO: Completed: initializeGL - Initializing OpenGL...");
}

void openGLWidgetClass::paintGL()
{
	paintGLMine(false);
}

void openGLWidgetClass::paintGLMine(bool forPicking, bool forHighliting)
{
	if ((forPicking) || (forHighliting))
	{
		qglClearColor(QColor(0, 0, 0));
		glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glDisable(GL_DITHER);
#ifdef GL_MULTISAMPLE
		glDisable(GL_MULTISAMPLE);
#endif
	}
	else
	{
		qglClearColor(QColor(7, 13, 24));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DITHER);
#ifdef GL_MULTISAMPLE
		if (format().sampleBuffers())
			glEnable(GL_MULTISAMPLE);
#endif
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glLoadIdentity();
	glScalef(zoom, zoom, zoom);
	glRotatef(-elevate, 1., 0., 0.);
	glRotatef(zRotate, 0., 0., 1.);
	glTranslatef(xTranslate, yTranslate, 0.);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if (forPicking)
	{
		if (!crane->getHasActiveMission())
			allPositions->draw(true, forHighliting);
	}
	else if (forHighliting)
	{
			allPositions->draw(true, forHighliting);
	}
	else
	{
		allPositions->draw(false, forHighliting);
		crane->openGlDraw();
		crane->openGlDrawMissionMarkers();
		
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		glColor3ub(100, 116, 139);
		area->draw();
		glPopAttrib();
		///glass
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		//glEnableClientState(GL_VERTEX_ARRAY);
		glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		glColor4f(0.22f, 0.74f, 0.97f, 0.18f);
		glVertexPointer(3,GL_FLOAT,0,&quadVertexesForGlassArray[0]);
		glDrawArrays(GL_QUADS,0,numberOfQuadForGlass*4);
		glPopAttrib();
		//glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);	
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
	glOrtho(-10.0, +10.0, -10.0, +10.0, -1000.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void openGLWidgetClass::mousePressEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::RightButton)
		lastMousePos = ev->pos();
	if (ev->button() == Qt::MiddleButton)
		midleMousePos = ev->pos();
	//if (ev->button()==Qt::LeftButton)
	//	selectItem(ev,true,false);
}

void openGLWidgetClass::selectItem(QMouseEvent *ev, bool leftClick, bool rightClick, bool forHighliting)
{
	 paintGLMine(true,forHighliting);

	 GLubyte  pixel[3]; 
	 GLint viewport[4];

	 glGetIntegerv(GL_VIEWPORT, viewport);
	 glReadPixels(ev->x(), viewport[3] - ev->y() + 2 * viewport[1], 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void*)pixel);

	 if (!forHighliting)
	 {
		 QToolTip::hideText();
		 allPositions->mouseOverHighlitingMode(QColor((int)pixel[0], (int)pixel[0], (int)pixel[0]));
		 if (leftClick)
		 {
			 if (!crane->getHasActiveMission())
			 {
				 if ((pixel[0] > 0) || (pixel[1] > 0) || (pixel[2] > 0))
					 allPositions->mouseClicked(QColor((int)pixel[0], (int)pixel[1], (int)pixel[2]), false);
				 else
					 allPositions->clearSelectedPositions();
			 }
		 }
		 else if (rightClick)
		 {
			 if ((pixel[0] > 0) || (pixel[1] > 0) || (pixel[2] > 0))
				 allPositions->mouseClicked(QColor((int)pixel[0], (int)pixel[1], (int)pixel[2]), true);
		 }
		 else
		 {
			 crane->mouseOver(QColor((int)pixel[0], (int)pixel[1], (int)pixel[2]));
			 if (!crane->getHasActiveMission())
				 allPositions->mouseOver(QColor((int)pixel[0], (int)pixel[1], (int)pixel[2]));
		 }
	 }
	 else
	 {
		 allPositions->mouseOver(QColor((int)pixel[0], (int)pixel[0], (int)pixel[0]));
		 allPositions->mouseOverHighlitingMode(QColor((int)pixel[0], (int)pixel[1], (int)pixel[2]));
		 basketPositionClass *newHighlightedPos = allPositions->getHighlightedPos();
		 if (newHighlightedPos)
		 {
			 if (highlightedPosition)
			 {
				 if ((newHighlightedPos->getNumber() != highlightedPosition->getNumber()) || (newHighlightedPos->getIndex() != highlightedPosition->getIndex()))
					 toolTipText = newHighlightedPos->getToolTipText();
			 }
			 else
				 toolTipText = newHighlightedPos->getToolTipText();
			 QToolTip::showText(QPoint(ev->x() + 20, ev->y()), toolTipText);
		 }
		 else
			 QToolTip::hideText();

		 highlightedPosition = newHighlightedPos;
	 }
	 updateGL();
}

void openGLWidgetClass::mouseMoveEvent(QMouseEvent *ev)
{
	if (ev->buttons() == Qt::RightButton)
	{
		zRotate += (ev->x() - lastMousePos.x()) / 10.;
		elevate -= (ev->y() - lastMousePos.y()) / 10.;

		if (zRotate<zRotMin)
			zRotate = zRotMin;
		if (zRotate>zRotMax)
			zRotate = zRotMax;

		if (elevate<elevateMin)
			elevate = elevateMin;
		if (elevate>elevateMax)
			elevate = elevateMax;
		updateGL();
		lastMousePos = ev->pos();
	}
	else if (ev->buttons() == Qt::MiddleButton)
	{
		xTranslate += (ev->x() - midleMousePos.x()) / 10.;

		if (xTranslate<-100.f)	xTranslate = -100.f;
		if (xTranslate>50.f)	xTranslate = 50.f;
		
		yTranslate -= (ev->y() - midleMousePos.y()) / 10.;

		if (yTranslate<-10.f)	yTranslate = -10.f;
		if (yTranslate>20.f)	yTranslate = 20.f;
		
		updateGL();
		midleMousePos = ev->pos();
	}
	else
		selectItem(ev, false, false, ev->modifiers() & Qt::ControlModifier);
	return;
}

void openGLWidgetClass::wheelEvent(QWheelEvent *ev)
{
	zoom -= (ev->delta() / 2000.);
	if (zoom < zoomMin)
		zoom = zoomMin;
	if (zoom > zoomMax)
		zoom = zoomMax;
	updateGL();
}

void openGLWidgetClass::mouseReleaseEvent(QMouseEvent *ev)
{
	if (!(ev->modifiers() & Qt::ControlModifier))
		selectItem(ev, ev->button() == Qt::LeftButton, ev->button() == Qt::RightButton, false);
}
