#include <QMessageBox>
#include <QApplication>
#include "ap.h"
#include "configuration/ConfigurationManager.h"
#include "dbTableStatus.h"
#include "plcNetworkStatus.h"
#include "timerStatus.h"
#include "../common/execDBQuery.h"
#include <QHash>
#include <QSet>

GLfloat	*triVertexesForNormalBasketArray;
GLfloat	*triNormalsForNormalBasketArray;
GLfloat	*triTexCoordForNormalBasketArray;
GLfloat	*quadVertexesForNormalBasketArray;
GLfloat	*quadNormalsForNormalBasketArray;
GLfloat	*quadTexCoordForNormalBasketArray;

int numberOfTrisForNormalBasket;
int numberOfQuadForNormalBasket;

GLfloat	*triVertexesForScrapBasketArray;
GLfloat	*triNormalsForScrapBasketArray;
GLfloat	*triTexCoordForScrapBasketArray;
GLfloat	*quadVertexesForScrapBasketArray;
GLfloat	*quadNormalsForScrapBasketArray;
GLfloat	*quadTexCoordForScrapBasketArray;

int numberOfTrisForScrapBasket;
int numberOfQuadForScrapBasket;

GLfloat	*triVertexesForProfileArray;
GLfloat	*triNormalsForProfileArray;
GLfloat	*triTexCoordForProfileArray;
GLfloat	*quadVertexesForProfileArray;
GLfloat	*quadNormalsForProfileArray;
GLfloat	*quadTexCoordForProfileArray;

int numberOfTrisForProfile;
int numberOfQuadForProfile;

GLfloat	*triVertexesFor24ProfileArray;
GLfloat	*triNormalsFor24ProfileArray;
GLfloat	*triTexCoordFor24ProfileArray;
GLfloat	*quadVertexesFor24ProfileArray;
GLfloat	*quadNormalsFor24ProfileArray;
GLfloat	*quadTexCoordFor24ProfileArray;

int numberOfTrisFor24Profile;
int numberOfQuadFor24Profile;

GLfloat	*triVertexesFor30ProfileArray;
GLfloat	*triNormalsFor30ProfileArray;
GLfloat	*triTexCoordFor30ProfileArray;
GLfloat	*quadVertexesFor30ProfileArray;
GLfloat	*quadNormalsFor30ProfileArray;
GLfloat	*quadTexCoordFor30ProfileArray;

int numberOfTrisFor30Profile;
int numberOfQuadFor30Profile;

GLfloat	*triVertexesForEmptyPosArray;
GLfloat	*triNormalsForEmptyPosArray;
GLfloat	*triTexCoordForEmptyPosArray;
GLfloat	*quadVertexesForEmptyPosArray;
GLfloat	*quadNormalsForEmptyPosArray;
GLfloat	*quadTexCoordForEmptyPosArray;

int numberOfTrisForEmptyPos;
int numberOfQuadForEmptyPos;

// 300, 320, 340, 360, 380, 200, 230
#define EMPTY_BUFFER_X_1 -4.063f// old: -3.500f = 2.900f - 6.400f // new: old - 0.563 = -4.063
#define EMPTY_BUFFER_X_2  2.900f	// 310, 330, 350, 370, 390
// 800, 820, 840, 860, 880
#define EMPTY_BUFFER_X_3  -12.373f//old:-3.500f+6.400f // new: old - 8.310 = -4.063 - 8.310 = -12.373

#define EMPTY_BUFFER_Y_1 10.850f	// 300, 310, 800
#define EMPTY_BUFFER_Y_2  9.293f	// 320, 330, 820
#define EMPTY_BUFFER_Y_3  7.736f	// 340, 350, 840
#define EMPTY_BUFFER_Y_4  6.179f	// 360, 370, 860
#define EMPTY_BUFFER_Y_5  4.622f	// 380, 390, 880

//New aging oven
//#define EMPTY_BUFFER_Y_6  4.622f 
//#define EMPTY_BUFFER_Y_7  4.622f 
//#define EMPTY_BUFFER_Y_8  4.622f 

#define BUFFER_Y_1  2.150f		// 250, 270, 280, 240, 230, 260
#define BUFFER_Y_2  0.550f		// 210, 220, 200, 520, 510, 500

// #define BUFFER_X_1  -3.500f //
#define BUFFER_X_2   4.400f		// 240, 500  //+7.9 
#define BUFFER_X_3  14.680f		// 250, 510  //+10.
#define BUFFER_X_4  25.520f		// 260, 520  //+10.840
#define BUFFER_X_5  33.880f		// 270, 210  //+8.36
#define BUFFER_X_6  43.135f		// 220, 280, 290 - Age oven 2  //+9.255

// #define SCRAP_BUFFER_X   8.110f
// #define SCRAP_BUFFER_Y   8.055f
#define SCRAP_WORKING_X   27.000f
#define SCRAP_WORKING_Y   7.740f

// 400, 900, 410 -- Stacker 1
#define STACKER_Y					6.000f
#define BEFORE_STACKER_X			11.000f
#define STACKER_X					20.000f
#define AFTER_STACKER_X				33.000f

// 450, 910, 460 -- Stacker 2
#define STACKER2_Y					0.050f
#define STACKER2_Y_IN				-2.400f
#define BEFORE_STACKER2_X			3.000f
#define STACKER2_X					-4.063f
#define AFTER_STACKER2_X			-12.373f

#define OVEN_Y_1		 10.250f	// 120, 130, 150, 620, 630, 650, 680, 690
#define OVEN_Y_2		 8.450f		// 100, 110, 140, 600, 610, 640, 660, 670

// Age Oven 2  -- numbered north to south
#define BUFFER_Y_1_2  3.912f	// 290
#define BUFFER_Y_2_2  2.264f	// 280
#define BUFFER_Y_3_2  0.633f    // 220

// Age Oven 2
#define OVEN2_Y1		 3.912f		// 1010, 1040
#define OVEN2_Y2		 2.264f		// 1020, 1050
#define OVEN2_Y3		 0.633f		// 1030, 1060

// Age Oven 1
#define OVEN_ENTRY_X_1		 33.000f	// 100, 120
#define OVEN_ENTRY_X_2		 40.550f	// 110, 130
#define OVEN_ENTRY_X_3		 37.900f	// 140, 150

// Age Oven 1
#define OVEN_IN_X_1				 51.000f	// 600, 620
#define OVEN_IN_X_2				 58.500f	// 610, 630
#define OVEN_IN_X_3				 54.750f	// 640, 650, 1010, 1020, 1030

#define OVEN_OUT_X_1			 67.500f	// 660, 680, 1040, 1050, 1060
#define OVEN_OUT_X_2			 75.000f	// 670, 690  (cross transfer)

#define RETURNING_CONV_X_4		 43.000f	// 160
#define RETURNING_CONV_X_3		 52.300f	// 720
#define RETURNING_CONV_X_2		 59.900f	// 710
#define RETURNING_CONV_X_12		 63.000f	// 730
#define RETURNING_CONV_X_1		 67.400f	// 700
#define RETURNING_CONV_Y		 STACKER_Y	// 160, 700, 710, 720, 730

#define INOUT_CONV_X			 83.000f    // 500


allPositionsClass::allPositionsClass(tcpServerClass *conveyorsPlcTcpServer_, tcpServerClass *newOvenConveyorsPlcTcpServer_):QObject()
{
	conveyorsPlcTcpServer = conveyorsPlcTcpServer_;
	newOvenConveyorsPlcTcpServer = newOvenConveyorsPlcTcpServer_;
	selectedFromBasketPos = NULL;
	selectedToBasketPos = NULL;
	crane = NULL;
	plcBeforeStackerPosMadeStep = plcStackerPosMadeStep = false;
	plcBeforeStacker2PosMadeStep = plcStacker2PosMadeStep = false;
	//hasBasketInOven=false;
	incomingBasketOn500 = false;
	scrapBasketIsReady = false;
	takeBasketsFromStackerOut = false;
	takeBasketsFromStacker2Out = false;
	autoModeEnum = 0;
	returningBasketFromOut = -1;
	returningBasketFromOutValueChanged = 0;
	numberOfPosToBeKeptEmptyInBuffer = 2;
	exportBeforeImports = false;
	exportPositionListThatWasLastCleared = NULL;
	
	//emptyNormalBasket
	emptyNormalBasket = new openGLPartClass(QString(":/basket.stl"), WHITE_TEXTURE_TRIA, WHITE_TEXTURE_QUAD);
	triVertexesForNormalBasketArray = emptyNormalBasket->getTriVertexesArray();
	triNormalsForNormalBasketArray = emptyNormalBasket->getTriNormalsArray();
	triTexCoordForNormalBasketArray = emptyNormalBasket->getTriTexCoordArray();
	numberOfTrisForNormalBasket = emptyNormalBasket->getNumberOfTris();
	quadVertexesForNormalBasketArray = emptyNormalBasket->getQuadVertexesArray();
	quadNormalsForNormalBasketArray = emptyNormalBasket->getQuadNormalsArray();
	quadTexCoordForNormalBasketArray = emptyNormalBasket->getQuadTexCoordArray();
	numberOfQuadForNormalBasket = emptyNormalBasket->getNumberOfQuad();

	//scrapBasket
	scrapBasket = new openGLPartClass(QString(":/scrap.stl"), WHITE_TEXTURE_TRIA, WHITE_TEXTURE_QUAD);
	triVertexesForScrapBasketArray = scrapBasket->getTriVertexesArray();
	triNormalsForScrapBasketArray = scrapBasket->getTriNormalsArray();
	triTexCoordForScrapBasketArray = scrapBasket->getTriTexCoordArray();
	numberOfTrisForScrapBasket = scrapBasket->getNumberOfTris();
	quadVertexesForScrapBasketArray = scrapBasket->getQuadVertexesArray();
	quadNormalsForScrapBasketArray = scrapBasket->getQuadNormalsArray();
	quadTexCoordForScrapBasketArray = scrapBasket->getQuadTexCoordArray();
	numberOfQuadForScrapBasket = scrapBasket->getNumberOfQuad();

	//profile
	profile = new openGLPartClass(QString(":/profiles1m.stl"), WHITE_TEXTURE_TRIA, WHITE_TEXTURE_QUAD);
	triVertexesForProfileArray = profile->getTriVertexesArray();
	triNormalsForProfileArray = profile->getTriNormalsArray();
	triTexCoordForProfileArray = profile->getTriTexCoordArray();
	numberOfTrisForProfile = profile->getNumberOfTris();
	quadVertexesForProfileArray = profile->getQuadVertexesArray();
	quadNormalsForProfileArray = profile->getQuadNormalsArray();
	quadTexCoordForProfileArray = profile->getQuadTexCoordArray();
	numberOfQuadForProfile = profile->getNumberOfQuad();

	//profile24
	profile24 = new openGLPartClass(QString(":/24.stl"), WHITE_TEXTURE_TRIA, WHITE_TEXTURE_QUAD);
	triVertexesFor24ProfileArray = profile24->getTriVertexesArray();
	triNormalsFor24ProfileArray = profile24->getTriNormalsArray();
	triTexCoordFor24ProfileArray = profile24->getTriTexCoordArray();
	numberOfTrisFor24Profile = profile24->getNumberOfTris();
	quadVertexesFor24ProfileArray = profile24->getQuadVertexesArray();
	quadNormalsFor24ProfileArray = profile24->getQuadNormalsArray();
	quadTexCoordFor24ProfileArray = profile24->getQuadTexCoordArray();
	numberOfQuadFor24Profile = profile24->getNumberOfQuad();

	//profile30
	profile30 = new openGLPartClass(QString(":/30.stl"), WHITE_TEXTURE_TRIA, WHITE_TEXTURE_QUAD);
	triVertexesFor30ProfileArray = profile30->getTriVertexesArray();
	triNormalsFor30ProfileArray = profile30->getTriNormalsArray();
	triTexCoordFor30ProfileArray = profile30->getTriTexCoordArray();
	numberOfTrisFor30Profile = profile30->getNumberOfTris();
	quadVertexesFor30ProfileArray = profile30->getQuadVertexesArray();
	quadNormalsFor30ProfileArray = profile30->getQuadNormalsArray();
	quadTexCoordFor30ProfileArray = profile30->getQuadTexCoordArray();
	numberOfQuadFor30Profile = profile30->getNumberOfQuad();

	//ForEmptyPos
	emptyPos = new openGLPartClass(QString(":/arrow.stl"), WHITE_TEXTURE_TRIA, WHITE_TEXTURE_QUAD);
	triVertexesForEmptyPosArray = emptyPos->getTriVertexesArray();
	triNormalsForEmptyPosArray = emptyPos->getTriNormalsArray();
	triTexCoordForEmptyPosArray = emptyPos->getTriTexCoordArray();
	numberOfTrisForEmptyPos = emptyPos->getNumberOfTris();
	quadVertexesForEmptyPosArray = emptyPos->getQuadVertexesArray();
	quadNormalsForEmptyPosArray = emptyPos->getQuadNormalsArray();
	quadTexCoordForEmptyPosArray = emptyPos->getQuadTexCoordArray();
	numberOfQuadForEmptyPos = emptyPos->getNumberOfQuad();

	//emptyPositions//emptyPositions//emptyPositions//emptyPositions//emptyPositions//emptyPositions//emptyPositions//emptyPositions//emptyPositions//emptyPositions//emptyPositions
	emptyPositionList01 = new positionListClass(310, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_2, EMPTY_BUFFER_Y_1);
	connect(emptyPositionList01, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList02 = new positionListClass(330, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_2, EMPTY_BUFFER_Y_2);
	connect(emptyPositionList02, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList03 = new positionListClass(350, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_2, EMPTY_BUFFER_Y_3);
	connect(emptyPositionList03, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList04 = new positionListClass(370, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_2, EMPTY_BUFFER_Y_4);
	connect(emptyPositionList04, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList05 = new positionListClass(390, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_2, EMPTY_BUFFER_Y_5);
	connect(emptyPositionList05, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));

	emptyPositionList06 = new positionListClass(300, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_1, EMPTY_BUFFER_Y_1); 
	connect(emptyPositionList06, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList07 = new positionListClass(320, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_1, EMPTY_BUFFER_Y_2); 
	connect(emptyPositionList07, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList08 = new positionListClass(340, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_1, EMPTY_BUFFER_Y_3); 
	connect(emptyPositionList08, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList09 = new positionListClass(360, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_1, EMPTY_BUFFER_Y_4); 
	connect(emptyPositionList09, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList10 = new positionListClass(380, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_1, EMPTY_BUFFER_Y_5); 
	connect(emptyPositionList10, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	
	//New Empty Buffer
	emptyPositionList11 = new positionListClass(800, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_3, EMPTY_BUFFER_Y_1); 
	connect(emptyPositionList11, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList12 = new positionListClass(820, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_3, EMPTY_BUFFER_Y_2); 
	connect(emptyPositionList12, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList13 = new positionListClass(840, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_3, EMPTY_BUFFER_Y_3); 
	connect(emptyPositionList13, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList14 = new positionListClass(860, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_3, EMPTY_BUFFER_Y_4); 
	connect(emptyPositionList14, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	emptyPositionList15 = new positionListClass(880, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24, basketPositionClass::EPICS_POS_BASKET_NOWHERE, EMPTY_BUFFER_X_3, EMPTY_BUFFER_Y_5); 
	connect(emptyPositionList15, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));

	//New aging oven
	//emptyPositionList16 = new positionListClass(220, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BUFFER_X_6, BUFFER_Y_2_2); connect(emptyPositionList16, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	//emptyPositionList17 = new positionListClass(280, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BUFFER_X_6, BUFFER_Y_1_2); connect(emptyPositionList17, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	//emptyPositionList18 = new positionListClass(290, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BUFFER_X_6, BUFFER_Y_3_2); connect(emptyPositionList18, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	
	// 310, 330, 350, 370, 390
	emptyPositionList << emptyPositionList01;
	emptyPositionList01->addObstructingPositionList(emptyPositionList02);
	emptyPositionList01->addObstructingPositionList(emptyPositionList03);
	emptyPositionList01->addObstructingPositionList(emptyPositionList04);
	emptyPositionList01->addObstructingPositionList(emptyPositionList05);

	emptyPositionList << emptyPositionList02;
	emptyPositionList02->addObstructingPositionList(emptyPositionList03);
	emptyPositionList02->addObstructingPositionList(emptyPositionList04);
	emptyPositionList02->addObstructingPositionList(emptyPositionList05);

	emptyPositionList << emptyPositionList03;
	emptyPositionList03->addObstructingPositionList(emptyPositionList04);
	emptyPositionList03->addObstructingPositionList(emptyPositionList05);

	emptyPositionList << emptyPositionList04;
	emptyPositionList04->addObstructingPositionList(emptyPositionList05);

	emptyPositionList << emptyPositionList05;

	// 300, 320, 340, 360, 380
	emptyPositionList << emptyPositionList06;
	emptyPositionList06->addObstructingPositionList(emptyPositionList07);
	emptyPositionList06->addObstructingPositionList(emptyPositionList08);
	emptyPositionList06->addObstructingPositionList(emptyPositionList09);
	emptyPositionList06->addObstructingPositionList(emptyPositionList10);

	emptyPositionList << emptyPositionList07;
	emptyPositionList07->addObstructingPositionList(emptyPositionList08);
	emptyPositionList07->addObstructingPositionList(emptyPositionList09);
	emptyPositionList07->addObstructingPositionList(emptyPositionList10);

	emptyPositionList << emptyPositionList08;
	emptyPositionList08->addObstructingPositionList(emptyPositionList09);
	emptyPositionList08->addObstructingPositionList(emptyPositionList10);

	emptyPositionList << emptyPositionList09;
	emptyPositionList09->addObstructingPositionList(emptyPositionList10);

	emptyPositionList << emptyPositionList10;

	// 800, 820, 840, 860, 880
	emptyPositionList << emptyPositionList11; 
	emptyPositionList11->addObstructingPositionList(emptyPositionList12); 
	emptyPositionList11->addObstructingPositionList(emptyPositionList13);
	emptyPositionList11->addObstructingPositionList(emptyPositionList14); 
	emptyPositionList11->addObstructingPositionList(emptyPositionList15);

	emptyPositionList << emptyPositionList12; 
	emptyPositionList12->addObstructingPositionList(emptyPositionList13); 
	emptyPositionList12->addObstructingPositionList(emptyPositionList14); 
	emptyPositionList12->addObstructingPositionList(emptyPositionList15);

	emptyPositionList << emptyPositionList13; 
	emptyPositionList13->addObstructingPositionList(emptyPositionList14); 
	emptyPositionList13->addObstructingPositionList(emptyPositionList15);

	emptyPositionList << emptyPositionList14; 
	emptyPositionList14->addObstructingPositionList(emptyPositionList15);
	
	emptyPositionList << emptyPositionList15;

//New aging oven
	//emptyPositionList << emptyPositionList16; emptyPositionList16->addObstructingPositionList(emptyPositionList18); emptyPositionList16->addObstructingPositionList(emptyPositionList17);
	//emptyPositionList << emptyPositionList18; emptyPositionList18->addObstructingPositionList(emptyPositionList17);
	//emptyPositionList << emptyPositionList17;

	emptyPositionList02->addBehindPositionLists(emptyPositionList01);
	emptyPositionList03->addBehindPositionLists(emptyPositionList01); emptyPositionList03->addBehindPositionLists(emptyPositionList02);
	emptyPositionList04->addBehindPositionLists(emptyPositionList01); emptyPositionList04->addBehindPositionLists(emptyPositionList02); emptyPositionList04->addBehindPositionLists(emptyPositionList03);
	emptyPositionList05->addBehindPositionLists(emptyPositionList01); emptyPositionList05->addBehindPositionLists(emptyPositionList02); emptyPositionList05->addBehindPositionLists(emptyPositionList03); emptyPositionList05->addBehindPositionLists(emptyPositionList04);

	emptyPositionList07->addBehindPositionLists(emptyPositionList06);
	emptyPositionList08->addBehindPositionLists(emptyPositionList06); emptyPositionList08->addBehindPositionLists(emptyPositionList07);
	emptyPositionList09->addBehindPositionLists(emptyPositionList06); emptyPositionList09->addBehindPositionLists(emptyPositionList07); emptyPositionList09->addBehindPositionLists(emptyPositionList08);
	emptyPositionList10->addBehindPositionLists(emptyPositionList06); emptyPositionList10->addBehindPositionLists(emptyPositionList07); emptyPositionList10->addBehindPositionLists(emptyPositionList08); emptyPositionList10->addBehindPositionLists(emptyPositionList09);

	emptyPositionList12->addBehindPositionLists(emptyPositionList11);
	emptyPositionList13->addBehindPositionLists(emptyPositionList11); emptyPositionList13->addBehindPositionLists(emptyPositionList12);
	emptyPositionList14->addBehindPositionLists(emptyPositionList11); emptyPositionList14->addBehindPositionLists(emptyPositionList12); emptyPositionList14->addBehindPositionLists(emptyPositionList13);
	emptyPositionList15->addBehindPositionLists(emptyPositionList11); emptyPositionList15->addBehindPositionLists(emptyPositionList12); emptyPositionList15->addBehindPositionLists(emptyPositionList13); emptyPositionList15->addBehindPositionLists(emptyPositionList14);

	allActivePositionList << emptyPositionList;
	allPositionList << emptyPositionList;

	// 30 ft buffer pos=istions : 210, 250, 270
	buffer250Position30List = new positionListClass(250, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BUFFER_X_3, BUFFER_Y_1); 
	connect(buffer250Position30List, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	buffer270Position30List = new positionListClass(270, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BUFFER_X_5, BUFFER_Y_1); 
	connect(buffer270Position30List, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	buffer210Position30List = new positionListClass(210, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BUFFER_X_5, BUFFER_Y_2); 
	connect(buffer210Position30List, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));

	buffer30PositionList << buffer250Position30List << buffer270Position30List << buffer210Position30List; 

	allActivePositionList << buffer30PositionList;
	allPositionList << buffer30PositionList;
	bufferPositionList << buffer30PositionList;

	// New Empty Buffer 24ft - 200, 230, 240, 260
	bufferPositionList << emptyPositionList11 << emptyPositionList12 << emptyPositionList13 << emptyPositionList14 << emptyPositionList15 << emptyPositionList06 << emptyPositionList07 << emptyPositionList08 << emptyPositionList09 << emptyPositionList10;
	buffer240Position24List = new positionListClass(240,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24,basketPositionClass::EPICS_POS_BASKET_NOWHERE,BUFFER_X_2,BUFFER_Y_1);
	connect(buffer240Position24List, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *))); 
	buffer230Position24List = new positionListClass(230,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24,basketPositionClass::EPICS_POS_BASKET_NOWHERE,EMPTY_BUFFER_X_1,BUFFER_Y_1);
	connect(buffer230Position24List, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	buffer200Position24List = new positionListClass(200,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24,basketPositionClass::EPICS_POS_BASKET_NOWHERE,EMPTY_BUFFER_X_1,BUFFER_Y_2);
	connect(buffer200Position24List, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	buffer260Position24List = new positionListClass(260,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24,basketPositionClass::EPICS_POS_BASKET_NOWHERE,BUFFER_X_4,BUFFER_Y_1);
	connect(buffer260Position24List, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));

	buffer24PositionList << buffer240Position24List << buffer230Position24List << buffer200Position24List << buffer260Position24List;

	allActivePositionList << buffer24PositionList;
	allPositionList << buffer24PositionList;
	bufferPositionList << buffer24PositionList;

	// Destackers taken offline - physical positions will be different if brought back

	//buffer24PositionList//buffer24PositionList//buffer24PositionList//buffer24PositionList//buffer24PositionList//buffer24PositionList//buffer24PositionList//buffer24PositionList//buffer24PositionList
	//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions
	//toDeStacker24Position=new positionListClass(520,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24,basketPositionClass::EPICS_POS_BASKET_NOWHERE,BUFFER_X_4,BUFFER_Y_2);connect(toDeStacker24Position, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	//allActivePositionList << toDeStacker24Position;
	//allPositionList << toDeStacker24Position;
	////toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions//toStacker24Positions
	////toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions
	//toDeStacker30Position=new positionListClass(510,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24|positionListClass::TYPE_PROFILES30,basketPositionClass::EPICS_POS_BASKET_NOWHERE,BUFFER_X_3,BUFFER_Y_2);connect(toDeStacker30Position, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	//allActivePositionList<<toDeStacker30Position;
	//allPositionList<<toDeStacker30Position;
	////toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions//toStacker30Positions
	////returningFromDestackersPositions//returningFromDestackersPositions//returningFromDestackersPositions//returningPositions//returningPositions//returningPositions//returningPositions//returningPositions//returningPositions//returningPositions
	//returningFromDestackersPosition=new positionListClass(500,positionListClass::TYPE_NORMAL|positionListClass::TYPE_SCRAP|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24,basketPositionClass::EPICS_POS_BASKET_NEW,BUFFER_X_2,BUFFER_Y_2);connect(returningFromDestackersPosition, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	//allActivePositionList<<returningFromDestackersPosition;
	//allPositionList<<returningFromDestackersPosition;
	//returningFromDestackersPositions//returningFromDestackersPositions//returningFromDestackersPositions//returningFromDestackersPositions//returningFromDestackersPositions//returningFromDestackersPositions//returningFromDestackersPositions//returningFromDestackersPositions//returningFromDestackersPositions//returningFromDestackersPositions
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//beforeOvenPositions
	beforeOvenPositionList01 = new positionListClass(100,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24,basketPositionClass::EPICS_POS_BASKET_NOWHERE,OVEN_ENTRY_X_1,OVEN_Y_2);
	connect(beforeOvenPositionList01, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	beforeOvenPositionList02 = new positionListClass(110,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24,basketPositionClass::EPICS_POS_BASKET_NOWHERE,OVEN_ENTRY_X_2,OVEN_Y_2);
	connect(beforeOvenPositionList02, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	beforeOvenPositionList03 = new positionListClass(120,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24,basketPositionClass::EPICS_POS_BASKET_NOWHERE,OVEN_ENTRY_X_1,OVEN_Y_1);
	connect(beforeOvenPositionList03, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	beforeOvenPositionList04 = new positionListClass(130,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24,basketPositionClass::EPICS_POS_BASKET_NOWHERE,OVEN_ENTRY_X_2,OVEN_Y_1);
	connect(beforeOvenPositionList04, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	beforeOvenPositionList05 = new positionListClass(140,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24|positionListClass::TYPE_PROFILES30,basketPositionClass::EPICS_POS_BASKET_NOWHERE,OVEN_ENTRY_X_3,OVEN_Y_2);
	connect(beforeOvenPositionList05, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *))); 
	beforeOvenPositionList06 = new positionListClass(150,positionListClass::TYPE_NORMAL|positionListClass::TYPE_EMPTY|positionListClass::TYPE_PROFILES20|positionListClass::TYPE_PROFILES24|positionListClass::TYPE_PROFILES30,basketPositionClass::EPICS_POS_BASKET_NOWHERE,OVEN_ENTRY_X_3,OVEN_Y_1);
	connect(beforeOvenPositionList06, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *))); 

	//Added by Vahid, Before New Aging Oven -- note north to south per PLC programming
	beforeOvenPositionList07 = new positionListClass(290, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BUFFER_X_6, BUFFER_Y_1_2); 
	connect(beforeOvenPositionList07, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	beforeOvenPositionList08 = new positionListClass(280, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BUFFER_X_6, BUFFER_Y_2_2); 
	connect(beforeOvenPositionList08, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	beforeOvenPositionList09 = new positionListClass(220, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BUFFER_X_6, BUFFER_Y_3_2); 
	connect(beforeOvenPositionList09, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));

	// @@@@@ -- these beforeOvenPositionList0x is where we build up the obstruction paths.
	// @@@@@ -- AB - LCMIT-12497

	// beforeOvenPositionList01() is missing - it is adjacent to the crane 
	// without anything obstructing it.

	// beforeOvenPositionList02->addObstructingPositionList(beforeOvenPositionList01);
	// Before position 110, we have to check 160.  
	// 160 is never supposed to be more than 3 tall, but I don't trust it.
	// Remove the beforeOvenPositionList01 and replace it with an obstruction 
	// path consisting of cell 160 4 tall
	// 110: crane travels over 160 -- This is the NEW Line to replace the line above.
	// beforeOvenPositionList02->addObstructingPositionList(returningConvPositionList04);
	// Conveyor class not the same as the other classes... can't add 160 to this list. :-(
	// Have to trust that 160 is never over 3 tall.

	// beforeOvenPositionList03->addObstructingPositionList(beforeOvenPositionList01);  
	// beforeOvenPositionList03->addObstructingPositionList(beforeOvenPositionList05);
	// Before position 120, we have to check for 100.add
	// Ensure that 100 is not 4 tall.  If it is, then position 120 is blocked.
	// Remove the undeeded beforeOvenPositionList03 and replace it with an obstruction 
	// path consisting of cell 100 4 tall
	// 120: crane travels over 100 -- This is the NEW Line to replace the line(s) above.
	beforeOvenPositionList03->addObstructingPositionList(beforeOvenPositionList01);
	beforeOvenPositionList03->addObstructingPositionList(beforeOvenPositionList05);


	// beforeOvenPositionList04->addObstructingPositionList(beforeOvenPositionList01);  
	// beforeOvenPositionList04->addObstructingPositionList(beforeOvenPositionList03);  
	// beforeOvenPositionList04->addObstructingPositionList(beforeOvenPositionList05);
	// Before position 130, we have to check for 160 & 110.
	// Ensure that neither 160 nor 110 is not 4 tall.  If it is, then position 130 is blocked.
	// Remove the undeeded beforeOvenPositionList04 and replace it with an obstruction 
	// path consisting of cell 160 & 110 4 tall
	// 130: crane travels over 160 and 110 -- This is the NEW Line to replace the line(s) above.
	//		NOTE:	This is using a Conveyor Position List for pos 160.  I'm not sure what that 
	//				will do.  It may be bad, it may be fiiiiine.  I've been told it never
	//				stacks above 3 high, so should never be an obstable, but I still want to 
	//				monitor it and make sure it's not a problem before travelling over it.
	// beforeOvenPositionList04->addObstructingPositionList(returningConvPositionList04);
	beforeOvenPositionList04->addObstructingPositionList(beforeOvenPositionList02);
	beforeOvenPositionList04->addObstructingPositionList(beforeOvenPositionList05);

	

	beforeOvenPositionList06->addObstructingPositionList(beforeOvenPositionList01); 
	beforeOvenPositionList06->addObstructingPositionList(beforeOvenPositionList02);  // @@@@@ - LCMIT-12497 - added this line to ensure that 150 is blocked if 110 is blocked.
	beforeOvenPositionList06->addObstructingPositionList(beforeOvenPositionList05);

	// @@@@@ -- end of AB - LCMIT-12497 Obstruction path additions
	
	//Added by Vahid, Before New Aging Oven
	// beforeOvenPositionList07->addObstructingPositionList(beforeOvenPositionList08);  // beforeOvenPositionList07->addObstructingPositionList(beforeOvenPositionList09);
	// LOG("ap.cpp", "beforeOvenPositionList07->addObstructingPositionList(beforeOvenPositionList08);  // comment out line  above (gene)");
	beforeOvenPositionList09->addObstructingPositionList(beforeOvenPositionList08);  // comment out line  above (gene)

	// beforeOvenPositionList09->addObstructingPositionList(beforeOvenPositionList08);
/*	beforeOvenPositionList09->addBehindPositionLists(beforeOvenPositionList07);
	beforeOvenPositionList08->addBehindPositionLists(beforeOvenPositionList07); beforeOvenPositionList08->addBehindPositionLists(beforeOvenPositionList09);

	beforeOven2Positions << beforeOvenPositionList07; beforeOvenPositionList07->addObstructingPositionList(beforeOvenPositionList09); beforeOvenPositionList07->addObstructingPositionList(beforeOvenPositionList08);
	beforeOven2Positions << beforeOvenPositionList09; beforeOvenPositionList09->addObstructingPositionList(beforeOvenPositionList08);
	beforeOven2Positions << beforeOvenPositionList08;*/

	beforeOvenPositionList01->addConcurrentPositionLists(beforeOvenPositionList05);
	beforeOvenPositionList01->addConcurrentPositionLists(beforeOvenPositionList06);//den mporoun na ksefortothoun an einai 2 mprosta kai 30 feet piso
	beforeOvenPositionList02->addConcurrentPositionLists(beforeOvenPositionList05);
	beforeOvenPositionList02->addConcurrentPositionLists(beforeOvenPositionList06);//den mporoun na ksefortothoun an einai 2 mprosta kai 30 feet piso
	beforeOvenPositionList03->addConcurrentPositionLists(beforeOvenPositionList06);
	beforeOvenPositionList04->addConcurrentPositionLists(beforeOvenPositionList06);
	beforeOvenPositionList05->addConcurrentPositionLists(beforeOvenPositionList01);  beforeOvenPositionList05->addConcurrentPositionLists(beforeOvenPositionList02);
	beforeOvenPositionList06->addConcurrentPositionLists(beforeOvenPositionList03);  beforeOvenPositionList06->addConcurrentPositionLists(beforeOvenPositionList04);
	beforeOvenPositionList06->addConcurrentPositionLists(beforeOvenPositionList01);  beforeOvenPositionList06->addConcurrentPositionLists(beforeOvenPositionList02);//den mporoun na ksefortothoun an einai 2 mprosta kai 30 feet piso

	beforeOvenPositions << beforeOvenPositionList05 << beforeOvenPositionList06 << beforeOvenPositionList01 << beforeOvenPositionList02 << beforeOvenPositionList03 << beforeOvenPositionList04;
	beforeOvenPositions << beforeOvenPositionList07 << beforeOvenPositionList08 << beforeOvenPositionList09;
	allPositionList << beforeOvenPositionList01 << beforeOvenPositionList02 << beforeOvenPositionList03 << beforeOvenPositionList04 << beforeOvenPositionList05 << beforeOvenPositionList06;
	allPositionList << beforeOvenPositionList07 << beforeOvenPositionList08 << beforeOvenPositionList09;
	allActivePositionList << beforeOvenPositionList01 << beforeOvenPositionList02 << beforeOvenPositionList03 << beforeOvenPositionList04 << beforeOvenPositionList05 << beforeOvenPositionList06;
	allActivePositionList << beforeOvenPositionList07 << beforeOvenPositionList08 << beforeOvenPositionList09;

	//ovenPositions (inside)
	ovenPositionList01 = new positionListClass(600, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_IN_OVEN, OVEN_IN_X_1, OVEN_Y_2);//connect(ovenPositionList01, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	ovenPositionList02 = new positionListClass(610, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_IN_OVEN, OVEN_IN_X_2, OVEN_Y_2);//connect(ovenPositionList02, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	ovenPositionList03 = new positionListClass(620, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_IN_OVEN, OVEN_IN_X_1, OVEN_Y_1);//connect(ovenPositionList03, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	ovenPositionList04 = new positionListClass(630, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_IN_OVEN, OVEN_IN_X_2, OVEN_Y_1);//connect(ovenPositionList04, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	ovenPositionList05 = new positionListClass(640, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_IN_OVEN, OVEN_IN_X_3, OVEN_Y_2);//connect(ovenPositionList05, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	ovenPositionList06 = new positionListClass(650, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_IN_OVEN, OVEN_IN_X_3, OVEN_Y_1);//connect(ovenPositionList06, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	allPositionList << ovenPositionList01 << ovenPositionList02 << ovenPositionList03 << ovenPositionList04 << ovenPositionList05 << ovenPositionList06;
	//ovenPositions//ovenPositions//ovenPositions//ovenPositions//ovenPositions//ovenPositions

	// afterOvenPositions 
	afterOvenPositionList01 = new positionListClass(660, positionListClass::TYPE_NORMAL, basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN, OVEN_OUT_X_1, OVEN_Y_2); connect(afterOvenPositionList01, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	afterOvenPositionList02 = new positionListClass(670, positionListClass::TYPE_NORMAL, basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN, OVEN_OUT_X_2, OVEN_Y_2); connect(afterOvenPositionList02, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	afterOvenPositionList03 = new positionListClass(680, positionListClass::TYPE_NORMAL, basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN, OVEN_OUT_X_1, OVEN_Y_1); connect(afterOvenPositionList03, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	afterOvenPositionList04 = new positionListClass(690, positionListClass::TYPE_NORMAL, basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN, OVEN_OUT_X_2, OVEN_Y_1); connect(afterOvenPositionList04, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	allPositionList << afterOvenPositionList01 << afterOvenPositionList02 << afterOvenPositionList03 << afterOvenPositionList04;

	//returningConvPos
	returningConvPositionList01 = new positionListClass(700, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_NOWHERE, RETURNING_CONV_X_1, RETURNING_CONV_Y);  connect(returningConvPositionList01, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	returningConvPositionList02 = new positionListClass(710, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_NOWHERE, RETURNING_CONV_X_2, RETURNING_CONV_Y);  connect(returningConvPositionList02, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	returningConvPositionList03 = new positionListClass(720, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_NOWHERE, RETURNING_CONV_X_3, RETURNING_CONV_Y);  connect(returningConvPositionList03, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	returningConvPositionList04 = new positionListClass(160, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN, RETURNING_CONV_X_4, RETURNING_CONV_Y);
	connect(returningConvPositionList04, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	returningConvPositionList05 = new positionListClass(730, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_NOWHERE, RETURNING_CONV_X_12, RETURNING_CONV_Y);//connect(returningConvPositionList05, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	
	allActivePositionList << returningConvPositionList04;
	allPositionList << returningConvPositionList01 << returningConvPositionList02 << returningConvPositionList03 << returningConvPositionList04 << returningConvPositionList05;

	// new oven (inside and after)
	newOvenInPositionList01 = new positionListClass(1010, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_IN_OVEN, OVEN_IN_X_3, OVEN2_Y1); 
	connect(newOvenInPositionList01, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	newOvenInPositionList02 = new positionListClass(1020, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_IN_OVEN, OVEN_IN_X_3, OVEN2_Y2);
	connect(newOvenInPositionList02, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	newOvenInPositionList03 = new positionListClass(1030, positionListClass::TYPE_NONE, basketPositionClass::EPICS_POS_BASKET_IN_OVEN, OVEN_IN_X_3, OVEN2_Y3); 
	connect(newOvenInPositionList03, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));

	newOvenOutPositionList01 = new positionListClass(1040, positionListClass::TYPE_NORMAL, basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN, OVEN_OUT_X_1, OVEN2_Y1);
	connect(newOvenOutPositionList01, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	newOvenOutPositionList02 = new positionListClass(1050, positionListClass::TYPE_NORMAL, basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN, OVEN_OUT_X_1, OVEN2_Y2);
	connect(newOvenOutPositionList02, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	newOvenOutPositionList03 = new positionListClass(1060, positionListClass::TYPE_NORMAL, basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN, OVEN_OUT_X_1, OVEN2_Y3);
	connect(newOvenOutPositionList03, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	
	allPositionList << newOvenInPositionList01 << newOvenInPositionList02 << newOvenInPositionList03 << newOvenOutPositionList01 << newOvenOutPositionList02 << newOvenOutPositionList03;

	///stacker
	beforeStackerPosition = new positionListClass(400, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BEFORE_STACKER_X, STACKER_Y);
	connect(beforeStackerPosition, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	stackerPosition = new positionListClass(900, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, STACKER_X, STACKER_Y);
	connect(stackerPosition, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	afterStackerPosition = new positionListClass(410, 
			positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, 
			basketPositionClass::EPICS_POS_BASKET_BEFORE_OVEN, 
			AFTER_STACKER_X, STACKER_Y);
	connect(afterStackerPosition, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	allPositionList << beforeStackerPosition << stackerPosition << afterStackerPosition;
	allActivePositionList << beforeStackerPosition << afterStackerPosition;

	///stacker2
	beforeStacker2Position = new positionListClass(450, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY, basketPositionClass::EPICS_POS_BASKET_NOWHERE, BEFORE_STACKER2_X, STACKER2_Y); 
	connect(beforeStacker2Position, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	stacker2Position = new positionListClass(910, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, STACKER2_X, STACKER2_Y_IN); 
	connect(stacker2Position, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	afterStacker2Position = new positionListClass(460, 
			positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, 
			basketPositionClass::EPICS_POS_BASKET_BEFORE_OVEN, 
			AFTER_STACKER2_X, STACKER2_Y); 
	connect(afterStacker2Position, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	allPositionList << beforeStacker2Position << stacker2Position << afterStacker2Position;
	// allPositionList << beforeStacker2Position << afterStacker2Position;
	allActivePositionList << beforeStacker2Position << afterStacker2Position;

	// packaging in / out
	basketInOutPositionList = new positionListClass(500, positionListClass::TYPE_NORMAL | positionListClass::TYPE_EMPTY | positionListClass::TYPE_PROFILES20 | positionListClass::TYPE_PROFILES24 | positionListClass::TYPE_PROFILES30, basketPositionClass::EPICS_POS_BASKET_NOWHERE, INOUT_CONV_X, RETURNING_CONV_Y);
	connect(basketInOutPositionList, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	allPositionList << basketInOutPositionList;

	//scrap
	//scrapWorkingPosition = new positionListClass(420, positionListClass::TYPE_SCRAP, basketPositionClass::EPICS_POS_BASKET_NOWHERE, SCRAP_WORKING_X, SCRAP_WORKING_Y); 
	//connect(scrapWorkingPosition, SIGNAL(positionSelectedSignal(basketPositionClass *)), this, SLOT(positionSelectedSlot(basketPositionClass *)));
	//allPositionList << scrapWorkingPosition;
	//allActivePositionList << scrapWorkingPosition;

	//////////////////////////////////
	nextOvenLoadingCycleTemperEnum = NEXT_OVEN_CYCLE_TEMPER_185;
	int noOfBasketsWithTemper185 = 0;
	int noOfBasketsWithTemper230 = 0;

	// LOG("ap.cpp", "noOfBasketsWithTemper185 - List01");
	noOfBasketsWithTemper185 += (beforeOvenPositionList01->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList01->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList01->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList01->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList01->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList01->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList01->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList01->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	// LOG("ap.cpp", "noOfBasketsWithTemper185 - List02");
	noOfBasketsWithTemper185 += (beforeOvenPositionList02->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList02->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList02->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList02->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList02->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList02->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList02->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList02->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	// LOG("ap.cpp", "noOfBasketsWithTemper185 - List03");
	noOfBasketsWithTemper185 += (beforeOvenPositionList03->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList03->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList03->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList03->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList03->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList03->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList03->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList03->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	// LOG("ap.cpp", "noOfBasketsWithTemper185 - List04");
	noOfBasketsWithTemper185 += (beforeOvenPositionList04->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList04->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList04->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList04->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList04->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList04->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList04->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList04->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	// LOG("ap.cpp", "noOfBasketsWithTemper185 - List05");
	noOfBasketsWithTemper185 += (beforeOvenPositionList05->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList05->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList05->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList05->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList05->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList05->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList05->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList05->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	// LOG("ap.cpp", "noOfBasketsWithTemper185 - List06");
	noOfBasketsWithTemper185 += (beforeOvenPositionList06->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList06->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList06->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList06->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList06->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList06->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList06->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList06->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;

	if (noOfBasketsWithTemper185 > noOfBasketsWithTemper230) {
		nextOvenLoadingCycleTemperEnum = NEXT_OVEN_CYCLE_TEMPER_185;
	}

	nextOven2LoadingCycleTemperEnum = NEXT_OVEN_CYCLE_TEMPER_185;
	noOfBasketsWithTemper185 = 0;
	noOfBasketsWithTemper230 = 0;

	// LOG("ap.cpp", "noOfBasketsWithTemper185 - List07");
	noOfBasketsWithTemper185 += (beforeOvenPositionList07->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList07->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList07->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList07->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList07->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList07->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList07->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList07->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	// LOG("ap.cpp", "noOfBasketsWithTemper185 - List08");
	noOfBasketsWithTemper185 += (beforeOvenPositionList08->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList08->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList08->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList08->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList08->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList08->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList08->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList08->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	// LOG("ap.cpp", "noOfBasketsWithTemper185 - List09");
	noOfBasketsWithTemper185 += (beforeOvenPositionList09->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList09->getPosition(0)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList09->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList09->getPosition(1)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList09->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList09->getPosition(2)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;
	noOfBasketsWithTemper185 += (beforeOvenPositionList09->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_185) ? 1 : 0;		noOfBasketsWithTemper230 += (beforeOvenPositionList09->getPosition(3)->getTemperEnum() == basketClass::BASKET_TEMPER_230) ? 1 : 0;

	if (noOfBasketsWithTemper185 > noOfBasketsWithTemper230)
		nextOven2LoadingCycleTemperEnum = NEXT_OVEN_CYCLE_TEMPER_185;

	connect(&mainTimer, SIGNAL(timeout()), this, SLOT(mainTimerSlot()));
	mainTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().PositionsMilliseconds());

	//connect(&inOvenUpdateTimeTimer,SIGNAL(timeout()),this,SLOT(inOvenUpdateTimeTimerSlot()));
	//inOvenUpdateTimeTimer.start(1000);
	connect(&autoModeTimer, SIGNAL(timeout()), this, SLOT(autoModeTimerSlot()));

#ifdef EMULATE
	basketAfterStackerReadyTime.restart();
#endif
}

allPositionsClass::~allPositionsClass()
{
	for (int i = 0; i < allPositionList.count(); i++)
		delete allPositionList[i];

	delete emptyNormalBasket;
	delete scrapBasket;
	delete profile;
	delete profile24;
	delete profile30;
	delete emptyPos;
}

positionListClass	*allPositionsClass::getBeforeStackerPosition()
{
	return beforeStackerPosition;
}

positionListClass	*allPositionsClass::getAfterStackerPosition()
{
	return afterStackerPosition;
}

positionListClass	*allPositionsClass::getBeforeStacker2Position()
{
	return beforeStacker2Position;
}

positionListClass	*allPositionsClass::getAfterStacker2Position()
{
	return afterStacker2Position;
}

void allPositionsClass::setNextOvenLoadingCycleTemperEnum(int t)
{
	nextOvenLoadingCycleTemperEnum = t;
}

int allPositionsClass::getNextOvenLoadingCycleTemperEnum()
{
	return nextOvenLoadingCycleTemperEnum;
}

//Added by Vahid
void allPositionsClass::setNextOven2LoadingCycleTemperEnum(int t)
{
	nextOven2LoadingCycleTemperEnum = t;
}

int allPositionsClass::getNextOven2LoadingCycleTemperEnum()
{
	return nextOven2LoadingCycleTemperEnum;
}

void allPositionsClass::setExportBeforeImports(bool s)
{
	exportBeforeImports=s;
}

void allPositionsClass::setNumberOfPosToBeKeptEmptyInBuffer(int n)
{
	numberOfPosToBeKeptEmptyInBuffer=n;
}

void allPositionsClass::updateAfterStackerPositionContentsFromDatabase()
{
	refreshBasketDataCache(afterStackerPosition->getPosition(0)->getBasketTableId());
	afterStackerPosition->calculateBasketContentsTypeFromEpicsDatabase();
	afterStackerPosition->loadContentFromDatabase();
}

void allPositionsClass::recalculateBasketContentsFromCache()
{
	LOG("BASKET CACHE", QString("Recalculating displayed contents for %1 position lists")
		.arg(allPositionList.count()));
	for (int i = 0; i < allPositionList.count(); ++i)
	{
		if (allPositionList[i])
			allPositionList[i]->calculateBasketContentsTypeFromEpicsDatabase();
	}
	emit openGlChangedSignal();
}

void allPositionsClass::reloadBasketAssignmentsFromDatabase()
{
	// Contents refreshes cannot discover baskets added to an empty position.
	// Refresh assignments in Live mode too, without disturbing active missions.
	const bool observerMode = isDatabaseReadOnlyMode();
	setDbTableActive("positions", true);
	const QVector<QVector<QVariant> > rows = execDBTableQuery(
		"select posNumber, posIndex, basketTableId from positions order by posNumber, posIndex",
		MAIN_DB_CONNECTION);
	setDbTableIdle("positions");
	QHash<QString, int> assignments;
	QSet<QString> ambiguousAssignments;
	for (int i = 0; i < rows.count(); ++i)
	{
		if (rows[i].count() < 3 || rows[i][0].isNull() || rows[i][1].isNull() || rows[i][2].isNull())
			continue;
		const QString key = QString("%1:%2").arg(rows[i][0].toInt()).arg(rows[i][1].toInt());
		if (assignments.contains(key))
			ambiguousAssignments.insert(key);
		assignments.insert(key, rows[i][2].toInt());
	}

	int changed = 0;
	int protectedChanges = 0;
	for (int i = 0; i < allPositionList.count(); ++i)
	{
		if (!allPositionList[i])
			continue;
		const QVector<basketPositionClass*> positions = allPositionList[i]->getAllBasketPositions();
		for (int j = 0; j < positions.count(); ++j)
		{
			basketPositionClass* position = positions[j];
			if (!position)
				continue;
			const QString key = QString("%1:%2").arg(position->getNumber()).arg(position->getIndex());
			if (!assignments.contains(key) || ambiguousAssignments.contains(key))
				continue;
			const int basketId = assignments.value(key);
			if (position->getBasketTableId() != basketId)
			{
				if (!observerMode && crane && crane->isAssignmentReservedForMission(position, basketId))
				{
					++protectedChanges;
					continue;
				}
				LOG("BASKET ASSIGNMENT", QString("External assignment synchronized: position=%1/%2, basket=%3 -> %4")
					.arg(position->getNumber()).arg(position->getIndex()).arg(position->getBasketTableId()).arg(basketId));
				position->setBasketTableIdInMemory(basketId);
				++changed;
			}
		}
	}
	LOG("BASKET CACHE", QString("Position assignments refreshed: databaseRows=%1, changed=%2, missionProtected=%3, ambiguousPositions=%4")
		.arg(rows.count()).arg(changed).arg(protectedChanges).arg(ambiguousAssignments.count()));
}

//void allPositionsClass::updateAfterStacker2PositionContentsFromDatabase()
//{
//	afterStacker2Position->calculateBasketContentsTypeFromEpicsDatabase();
//	afterStacker2Position->loadContentFromDatabase();
//}

int allPositionsClass::getBasketTableIdOfScrapBasketPos()
{
	return 0;
	// return scrapWorkingPosition->getTopBasketTableId();
}

void allPositionsClass::setScrapBasketIsReady(bool r)
{
	scrapBasketIsReady = r;
	if (scrapBasketIsReady)
		takeScrapBasket();
}

bool allPositionsClass::isOvenEntranceFull()
{
	// @@@@@ -- AB -- We can have 4 baskets in each of the 6 before 
	// oven positions, so we need to check them in groups of 3 or 2 to determine 
	// if the oven entrance is full or not. If we check all 6 positions together, 
	// we might miss the case where the first 3 positions are full and the 
	// last 3 positions are empty, which would still mean that the oven entrance 
	// is full.
	// Nothing was changed here, I'm just questioning the logic.  Review this later.
	if ((beforeOvenPositionList01->getNumberOfBaskets() == 4) && (beforeOvenPositionList02->getNumberOfBaskets() == 4) && (beforeOvenPositionList03->getNumberOfBaskets() == 4) && (beforeOvenPositionList04->getNumberOfBaskets() == 4))
		return true;
	if ((beforeOvenPositionList03->getNumberOfBaskets() == 4) && (beforeOvenPositionList04->getNumberOfBaskets() == 4) && (beforeOvenPositionList05->getNumberOfBaskets() == 4))
		return true;
	if ((beforeOvenPositionList05->getNumberOfBaskets() == 4) && (beforeOvenPositionList06->getNumberOfBaskets() == 4))
		return true;

	return false;
}

//////////////////////////
//Added by Vahid
//////////////////////////
bool allPositionsClass::isOven2EntranceFull()
{
	if ((beforeOvenPositionList07->getNumberOfBaskets() == 4) && (beforeOvenPositionList08->getNumberOfBaskets() == 4) && (beforeOvenPositionList09->getNumberOfBaskets() == 4))
		return true;

	return false;
}
/////////////////////////

void allPositionsClass::addToAutoMode(int mode)
{
	autoModeEnum = autoModeEnum | mode;
	autoModeTimer.start(BasketCrane::Configuration::ConfigurationManager::Instance().Timers().AutoModeMilliseconds());
}

void allPositionsClass::removeFromAutoMode(int mode)
{
	autoModeEnum = autoModeEnum ^ mode;
	if (!autoModeEnum)
		autoModeTimer.stop();
}

void allPositionsClass::setTakeBasketsFromStackerOut(bool s)
{
	takeBasketsFromStackerOut = s;
}

void allPositionsClass::setTakeBasketsFromStacker2Out(bool s)
{
	takeBasketsFromStacker2Out = s;
}

void allPositionsClass::clearSelectedPositions()
{
	if (selectedFromBasketPos)
	{
		selectedFromBasketPos->setSelected(false);
		selectedFromBasketPos = NULL;
	}
	if (selectedToBasketPos)
	{
		selectedToBasketPos->setSelected(false);
		selectedToBasketPos = NULL;
	}
}

void allPositionsClass::takeScrapBasket()
{
	scrapBasketIsReady = true;
}

void allPositionsClass::unlockBasketAfterStacker()
{
	if (afterStackerPosition->getTopBasketPosition() && afterStackerPosition->getTopBasketPosition()->getLocked())
	{
		QDialog *dlg=new QDialog();
		dlg->setWindowIcon(QIcon(":/logo_16x16.png"));
		QVBoxLayout *mainLayout = new QVBoxLayout(); dlg->setLayout(mainLayout);
		QHBoxLayout *upLayout = new QHBoxLayout(); mainLayout->addLayout(upLayout);
		QHBoxLayout *downLayout = new QHBoxLayout(); mainLayout->addLayout(downLayout);
		QGroupBox *radioGroupBox = new QGroupBox("Define virtual contents in basket"); upLayout->addWidget(radioGroupBox);
		QHBoxLayout *radioBtnsLayout = new QHBoxLayout(); radioGroupBox->setLayout(radioBtnsLayout);
		QRadioButton *justUnlockRadio = new QRadioButton("Just unlock it"); radioBtnsLayout->addWidget(justUnlockRadio); justUnlockRadio->setChecked(true);
		QRadioButton *make24Radio = new QRadioButton("Add 24 feet profiles"); radioBtnsLayout->addWidget(make24Radio);
		QRadioButton *make30Radio = new QRadioButton("Add 30 feet profiles"); radioBtnsLayout->addWidget(make30Radio);

		QPushButton *cancelPushBtn = new QPushButton("Cancel"); downLayout->addWidget(cancelPushBtn);
		QPushButton *okPushBtn = new QPushButton("Ok"); downLayout->addWidget(okPushBtn);
		connect(cancelPushBtn, SIGNAL(clicked()), dlg, SLOT(reject()));
		connect(okPushBtn, SIGNAL(clicked()), dlg, SLOT(accept()));

		if (dlg->exec() == QDialog::Accepted)
		{
			afterStackerPosition->getTopBasketPosition()->setLocked(false);
			int basketTableId = afterStackerPosition->getTopBasketPosition()->getBasketTableId();
			setDbTableActive("basket_details", true);
			QVector<QVariant> tVector = execDBQuery(QString("select temper from basket_details where basketnum=%1 and temper is not Null").arg(basketTableId), APEX_DB_CONNECTION);
			setDbTableIdle("basket_details");

			QString temper="F";
			if (tVector.count() > 0)
				temper = tVector[0].toString();

			if (make24Radio->isChecked()) {
				setDbTableActive("basket_details", false);
				execDBQuery(QString("insert into basket_details (basketnum,dienum,sequence,SONum,SOItemNum,Lot,Pc,PcLen,Wt,LotCompleteFlag,PcPulled,temper) VALUES(%1,6666,6666,6666,6666,6666,6666,6666,6666,0,0,'%2')").arg(basketTableId).arg(temper),APEX_DB_CONNECTION);
				setDbTableIdle("basket_details");
			}

			if (make30Radio->isChecked()) {
				setDbTableActive("basket_details", false);
				execDBQuery(QString("insert into basket_details (basketnum,dienum,sequence,SONum,SOItemNum,Lot,Pc,PcLen,Wt,LotCompleteFlag,PcPulled,temper) VALUES(%1,7777,7777,7777,7777,7777,7777,7777,7777,0,0,'%2')").arg(basketTableId).arg(temper),APEX_DB_CONNECTION);
				setDbTableIdle("basket_details");
			}

			afterStackerPosition->getTopBasketPosition()->loadContentFromDatabase();
		}
		emit openGlChangedSignal();
	}
	//if (afterStackerPosition->getPosition(0)->hasBasket()&&afterStackerPosition->getPosition(0)->getLocked())
	//{
	//	QMessageBox *msgBox=new QMessageBox();
	//	msgBox->setWindowIcon(QIcon(":/logo_16x16.png"));
	//	msgBox->setInformativeText("Define the contents as 30 feet?");
	//	msgBox->setText("You are about to unlock the basket after the stacker.\nDo you want to define the contents as 30 feet contents?");
	//	msgBox->setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
	//	msgBox->setDefaultButton(QMessageBox::Yes);
	//	int ans=msgBox->exec();
	//	if (ans==QMessageBox::Yes)
	//	{
	//		int basektTableId=afterStackerPosition->getPosition(0)->getBasketTableId();
	//		QVector<QVariant> tVector=execQuery(QString("select temper from basket_details where basketnum=%1 and temper is not Null").arg(basektTableId),APEX_DB_CONNECTION);
	//		QString temper="F";
	//		if (tVector.count()>0)
	//			temper=tVector[0].toString();
	//		execQuery(QString("insert into basket_details (basketnum,dienum,sequence,SONum,SOItemNum,Lot,Pc,PcLen,Wt,LotCompleteFlag,PcPulled,temper) VALUES(%1,7777,7777,7777,7777,7777,7777,7777,7777,0,0,'%2')").arg(basektTableId).arg(temper),APEX_DB_CONNECTION);
	//		afterStackerPosition->getPosition(0)->loadContentFromDatabase();
	//	}
	//	if (ans!=QMessageBox::Cancel)
	//		afterStackerPosition->getPosition(0)->setLocked(false);
	//	emit openGlChangedSignal();
	//}
}

void allPositionsClass::unlockBasketReturningFromOven()
{
	if (returningConvPositionList04->getTopBasketPosition() && returningConvPositionList04->getTopBasketPosition()->getLocked())
	{
		QDialog *dlg=new QDialog();
		dlg->setWindowIcon(QIcon(":/logo_16x16.png"));
		QVBoxLayout *mainLayout=new QVBoxLayout();dlg->setLayout(mainLayout);
		QHBoxLayout *upLayout=new QHBoxLayout();mainLayout->addLayout(upLayout);
		QHBoxLayout *downLayout=new QHBoxLayout();mainLayout->addLayout(downLayout);
		QGroupBox *radioGroupBox=new QGroupBox("Define virtual contents in basket");upLayout->addWidget(radioGroupBox);
		QHBoxLayout *radioBtnsLayout=new QHBoxLayout();radioGroupBox->setLayout(radioBtnsLayout);
		QRadioButton *justUnlockRadio=new QRadioButton("Just unlock it");radioBtnsLayout->addWidget(justUnlockRadio);justUnlockRadio->setChecked(true);
		QRadioButton *make24Radio=new QRadioButton("Add 24 feet profiles");radioBtnsLayout->addWidget(make24Radio);
		QRadioButton *make30Radio=new QRadioButton("Add 30 feet profiles");radioBtnsLayout->addWidget(make30Radio);

		QPushButton *cancelPushBtn=new QPushButton("Cancel");downLayout->addWidget(cancelPushBtn);
		QPushButton *okPushBtn=new QPushButton("Ok");downLayout->addWidget(okPushBtn);
		connect(cancelPushBtn,SIGNAL(clicked()),dlg,SLOT(reject()));
		connect(okPushBtn,SIGNAL(clicked()),dlg,SLOT(accept()));

		if (dlg->exec() == QDialog::Accepted)
		{
			returningConvPositionList04->getTopBasketPosition()->setLocked(false);
			int basketTableId = returningConvPositionList04->getTopBasketPosition()->getBasketTableId();
			setDbTableActive("basket_details", true);
			QVector<QVariant> tVector = execDBQuery(QString("select temper from basket_details where basketnum=%1 and temper is not Null").arg(basketTableId), APEX_DB_CONNECTION);
			setDbTableIdle("basket_details");
			QString temper = "F";

			if (tVector.count()>0)
				temper = tVector[0].toString();
			if (make24Radio->isChecked()) {
				setDbTableActive("basket_details", false);
				execDBQuery(QString("insert into basket_details (basketnum,dienum,sequence,SONum,SOItemNum,Lot,Pc,PcLen,Wt,LotCompleteFlag,PcPulled,temper) VALUES(%1,6666,6666,6666,6666,6666,6666,6666,6666,0,0,'%2')").arg(basketTableId).arg(temper), APEX_DB_CONNECTION);
				setDbTableIdle("basket_details");
			}
			if (make30Radio->isChecked()) {
				setDbTableActive("basket_details", false);
				execDBQuery(QString("insert into basket_details (basketnum,dienum,sequence,SONum,SOItemNum,Lot,Pc,PcLen,Wt,LotCompleteFlag,PcPulled,temper) VALUES(%1,7777,7777,7777,7777,7777,7777,7777,7777,0,0,'%2')").arg(basketTableId).arg(temper), APEX_DB_CONNECTION);
				setDbTableIdle("basket_details");
			}

			returningConvPositionList04->getTopBasketPosition()->loadContentFromDatabase();
		}
		emit openGlChangedSignal();

		//QMessageBox *msgBox=new QMessageBox();
		//msgBox->setWindowIcon(QIcon(":/logo_16x16.png"));
		//msgBox->setInformativeText("Define the contents as 30 feet?");
		//msgBox->setText(QString("You are about to unlock the basket %1 that is returning from the oven.\nDo you want to define the contents as 30 feet contents?").arg(returningConvPositionList04->getTopBasketPosition()->getBasketTableId()));
		//msgBox->setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
		//msgBox->setDefaultButton(QMessageBox::Yes);
		//int ans=msgBox->exec();
		//if (ans==QMessageBox::Yes)
		//{
		//	int basektTableId=returningConvPositionList04->getTopBasketPosition()->getBasketTableId();
		//	QVector<QVariant> tVector=execQuery(QString("select temper from basket_details where basketnum=%1 and temper is not Null").arg(basektTableId),APEX_DB_CONNECTION);
		//	QString temper="F";
		//	if (tVector.count()>0)
		//		temper=tVector[0].toString();
		//	execQuery(QString("insert into basket_details (basketnum,dienum,sequence,SONum,SOItemNum,Lot,Pc,PcLen,Wt,LotCompleteFlag,PcPulled,temper) VALUES(%1,7777,7777,7777,7777,7777,7777,7777,7777,0,0,'%2')").arg(basektTableId).arg(temper),APEX_DB_CONNECTION);
		//	returningConvPositionList04->getTopBasketPosition()->loadContentFromDatabase();
		//}
		//if (ans!=QMessageBox::Cancel)
		//	returningConvPositionList04->getTopBasketPosition()->setLocked(false);
		//emit openGlChangedSignal();
	}
}

basketPositionClass* allPositionsClass::getPosition(int number, int index)
{
	for (int i = 0; i < allPositionList.count(); i++) {
		if (allPositionList[i]->getMainPosId() == number) {
			return allPositionList[i]->getPosition(index);
		}
	}

	return NULL;
}

basketPositionClass* allPositionsClass::getPositionByPlcId(int plcId)
{
	for (int i = 0; i < allPositionList.count(); ++i)
	{
		if (!allPositionList[i])
			continue;
		// A PLC mission may be in transit after its source DB slot has been
		// cleared and before its destination DB slot has been filled. Search
		// every physical position, not only positions that currently hold a basket.
		const QVector<basketPositionClass*> positions = allPositionList[i]->getAllBasketPositions();
		for (int j = 0; j < positions.count(); ++j)
		{
			if (positions[j] && positions[j]->getPlcId() == plcId)
				return positions[j];
		}
	}
	return NULL;
}

void allPositionsClass::positionSelectedSlot(basketPositionClass* b)
{
	if (selectedFromBasketPos)
	{
		basketPositionClass* from = selectedFromBasketPos;
		selectedToBasketPos = b;
		// Finish selection before move() can invoke callbacks or repaint the view.
		clearSelectedPositions();
		if (!b || !crane)
		{
			LOG("ERROR: CRANE MOVE", "Destination selection has no destination or crane");
			return;
		}
		LOG("CRANE MOVE", QString("Destination clicked: from=%1/%2 to=%3/%4")
			.arg(from->getNumber()).arg(from->getIndex()).arg(b->getNumber()).arg(b->getIndex()));
		QString rejectionReason;
		const bool accepted = crane->move(from, b, &rejectionReason);
		LOG("CRANE MOVE", QString("Destination click completed: accepted=%1").arg(accepted));
		if (!accepted)
		{
			QMessageBox* alert = new QMessageBox(QMessageBox::Warning, tr("Basket move rejected"),
				QString(), QMessageBox::Ok, QApplication::activeWindow());
			alert->setAttribute(Qt::WA_DeleteOnClose);
			alert->setTextFormat(Qt::PlainText);
			alert->setText(tr("Basket %1 could not be moved from position %2 to %3.")
				.arg(from->getBasketTableId()).arg(from->getNumber()).arg(b->getNumber()));
			alert->setInformativeText(rejectionReason + tr("\n\nThe move was not started. Check the basket contents and destination before trying again."));
			// Return to the event loop so PLC/status updates continue normally.
			alert->open();
		}
	}
	else
	{
		selectedFromBasketPos = b;
	}
}

void allPositionsClass::setCraneClass(craneClass *crane_)
{
	crane = crane_;
}

bool allPositionsClass::hasSelectedFromBasketPos()
{
	if (selectedFromBasketPos)
		return true;

	return false;
}

#include "uglyfont.h"

void allPositionsClass::draw(bool forPicking, bool forHighliting)
{
	if (!forHighliting)
	{
		if (selectedFromBasketPos == NULL)
		{
			if (forPicking)
			{
				for (int i = 0; i < allActivePositionList.count(); i++)
					allActivePositionList[i]->drawFreeToMoveBasket();
			}
			else
			{
				for (int i = 0; i < allPositionList.count(); i++)
					allPositionList[i]->drawAllPosWithBasket();
			}
		}
		else
		{
			if (forPicking)
			{
				for (int i = 0; i < allActivePositionList.count(); i++)
					allActivePositionList[i]->drawFreeToRecievePos(forPicking, selectedFromBasketPos);
			}
			else
			{
				for (int i = 0; i < allPositionList.count(); i++)
				{
					allPositionList[i]->drawAllPosWithBasket();
				}
				for (int i = 0; i < allActivePositionList.count(); i++)
				{
					allActivePositionList[i]->drawFreeToRecievePos(forPicking, selectedFromBasketPos);
				}
			}
		}
	//	if (hasBasketInOven)
		{
			glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
			glPushMatrix();
			glTranslatef(50.5f,9.0f,6.5f);
			glScalef(0.5f,0.5f,0.5f);
			glColor3f(1.0f,0.0f,1.0f);
			YsDrawUglyFont(timeLeftForOvenStr.toLatin1(),0);
			glPopMatrix();
			glPopAttrib();
		}

		for (int i = 0; i < allPositionList.count(); i++)
			allPositionList[i]->drawLockedToRecieve();
	}
	else
	{
		for (int i = 0; i < allPositionList.count(); i++)
			allPositionList[i]->drawAllPosWithBasket(forHighliting);
	}
}

void allPositionsClass::inOvenUpdateTimeTimerSlot()
{
	//if (hasBasketInOven)
	//	emit openGlChangedSignal();//to update the time above the oven
}

void allPositionsClass::mouseOver(QColor pickedColor)
{
	for (int i = 0; i < allPositionList.count(); i++)
		allPositionList[i]->mouseOver(pickedColor);
}

void allPositionsClass::mouseOverHighlitingMode(QColor pickedColor)
{
	for (int i = 0; i < allPositionList.count(); i++)
		allPositionList[i]->mouseOverHighlitingMode(pickedColor);
}

void allPositionsClass::mouseClicked(QColor pickedColor, bool rightClick)
{
	for (int i = 0; i < allPositionList.count(); i++)
		allPositionList[i]->mouseClicked(pickedColor, rightClick);
}

void allPositionsClass::openGlObjectChangedSlot()
{
	emit openGlChangedSignal();
}

bool allPositionsClass::isPosBeforeStackerEmpty()
{
	return plcIsBeforeStackerPosEmpty;
}

bool allPositionsClass::isPosBeforeStacker2Empty()
{
	return plcIsBeforeStacker2PosEmpty;
}

/////
void allPositionsClass::plcStatusN20_13Changed(int status)
{
	QVector<bool> statusBits = deprecatedCalculateBits(status);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Stacker 1 needs empty basket.
	plcIsBeforeStackerPosEmpty = statusBits[4] ? true : false;
		LOG("STACKER PLC", QString("N20[13].4=%1 | beforeStackerPosition.hasBasket=%2")
			.arg(plcIsBeforeStackerPosEmpty).arg(beforeStackerPosition->getPosition(0)->hasBasket()));
	if (plcIsBeforeStackerPosEmpty)
	{
		beforeStackerPosition->setLockRecieve(false);
		if (beforeStackerPosition->getPosition(0)->hasBasket())
			plcBeforeStackerPosMadeStep = true;
	}
	else
		beforeStackerPosition->setLockRecieve(true);

	//////////////////////////////////////////
	// Basket ready for pickup at stacker 1
	plcIsAfterStackerPosFull = statusBits[6] ? true : false;
		LOG("STACKER PLC", QString("N20[13].6=%1 | afterStackerPosition.hasBasket=%2")
			.arg(plcIsAfterStackerPosFull).arg(afterStackerPosition->getPosition(0)->hasBasket()));
	if (plcIsAfterStackerPosFull)
	{
		if (!afterStackerPosition->getPosition(0)->hasBasket())
		{
			plcStackerPosMadeStep = true;
			// afterStackerPosition->setLockRecieve(true);
		}
	}
	else
		afterStackerPosition->setLockRecieve(false);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Stacker 2 needs empty basket.
	//if (plcIsBeforeStacker2PosEmpty)
	plcIsBeforeStacker2PosEmpty = statusBits[17] ? true : false;

	LOG("STACKER PLC", QString("N20[13].17=%1 | beforeStacker2Position.hasBasket=%2")
		.arg(plcIsBeforeStacker2PosEmpty).arg(beforeStacker2Position->getPosition(0)->hasBasket()));

	if (plcIsBeforeStacker2PosEmpty)
	{
		beforeStacker2Position->setLockRecieve(false);
		if (beforeStacker2Position->getPosition(0)->hasBasket())
			plcBeforeStacker2PosMadeStep = true;
	}
	else
		beforeStacker2Position->setLockRecieve(true);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Basket ready for pickup at stacker 2
	// if (plcIsAfterStacker2PosFull)
	plcIsAfterStacker2PosFull = statusBits[18] ? true : false;
	LOG("STACKER PLC", QString("N20[13].18=%1 | afterStacker2Position.hasBasket=%2")
		.arg(plcIsAfterStacker2PosFull).arg(afterStacker2Position->getPosition(0)->hasBasket()));

	if (plcIsAfterStacker2PosFull)
	{
		if (afterStacker2Position->getPosition(0)->hasBasket())
		{
			plcStacker2PosMadeStep = true;
			// afterStacker2Position->setLockRecieve(true);
		}
	}
	else
		afterStacker2Position->setLockRecieve(false);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Destaacker return
	//if (statusBits[14])
	//{
	//	returningFromDestackersPosition->setLockRecieve(false);
	//	if ((!returningFromDestackersPosition->getPosition(0)->getIsLockedByCrane()) && (returningFromDestackersPosition->getPosition(0)->hasBasket()))
	//		returningFromDestackersPosition->getPosition(0)->setBasketTableId(0);
	//}
	//else
	//	returningFromDestackersPosition->setLockRecieve(true);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// New basket
	 // incomingBasketOn500 = statusBits[15] ? true : false;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Destackers
//	if (statusBits[10])
//	{
//		if (toDeStacker24Position->getPosition(0)->hasBasket() && (!toDeStacker24Position->getPosition(0)->getIsLockedByCrane()))
//		{
//			toDeStacker24Position->getPosition(0)->setBasketTableId(0);
//			exportPositionListThatWasLastCleared = toDeStacker24Position;
//			emit openGlChangedSignal();
//		}
//		toDeStacker24Position->setLockRecieve(false);
//	}
//#ifndef EMULATE
//	else
//	{
//		toDeStacker24Position->setLockRecieve(true);
//	}
//#endif
//	if (statusBits[12])
//	{
//		if (toDeStacker30Position->getPosition(0)->hasBasket()&&(!toDeStacker30Position->getPosition(0)->getIsLockedByCrane()))
//		{
//			toDeStacker30Position->getPosition(0)->setBasketTableId(0);
//			exportPositionListThatWasLastCleared=toDeStacker30Position;
//			emit openGlChangedSignal();
//		}
//		toDeStacker30Position->setLockRecieve(false);
//	}
//#ifndef EMULATE
//	else
//	{
//		toDeStacker30Position->setLockRecieve(true);
//	}
//#endif

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ovens
	if (!statusBits[7])
	{
		beforeOvenPositionList01->setLockRecieve(true);
		beforeOvenPositionList02->setLockRecieve(true);
		beforeOvenPositionList03->setLockRecieve(true);
		beforeOvenPositionList04->setLockRecieve(true);
		beforeOvenPositionList05->setLockRecieve(true);
		beforeOvenPositionList06->setLockRecieve(true);
		beforeOvenPositionList07->setLockRecieve(true);
		beforeOvenPositionList08->setLockRecieve(true);
		beforeOvenPositionList09->setLockRecieve(true);
	}
	else
	{
		beforeOvenPositionList01->setLockRecieve(false);
		beforeOvenPositionList02->setLockRecieve(false);
		beforeOvenPositionList03->setLockRecieve(false);
		beforeOvenPositionList04->setLockRecieve(false);
		beforeOvenPositionList05->setLockRecieve(false);
		beforeOvenPositionList06->setLockRecieve(false);
		beforeOvenPositionList07->setLockRecieve(false);
		beforeOvenPositionList08->setLockRecieve(false);
		beforeOvenPositionList09->setLockRecieve(false);
	}

	if (!statusBits[8])
		returningConvPositionList04->setLockRecieve(false);
	else
		returningConvPositionList04->setLockRecieve(false);

	// mainTimerSlot();  // AB - Stopping this manual call of the mainTimerSlot()
}

void allPositionsClass::newBasketInReturningFromDestackerPos(int returningBasketTableId)
{
	if (returningBasketFromOut != returningBasketTableId)
	{
		returningBasketFromOutValueChanged = 1;
		returningBasketFromOut = returningBasketTableId;
	}
}

bool allPositionsClass::getPlcIsBeforeStackerPosEmpty()
{
	return plcIsBeforeStackerPosEmpty;
}

bool allPositionsClass::getPlcIsAfterStackerPosFull()
{
	return plcIsAfterStackerPosFull;
}

bool allPositionsClass::getPlcIsBeforeStacker2PosEmpty()
{
	return plcIsBeforeStacker2PosEmpty;
}

bool allPositionsClass::getPlcIsAfterStacker2PosFull()
{
	return plcIsAfterStacker2PosFull;
}

positionListClass* allPositionsClass::getPositionListWithBasketOnFirstPos(int b)
{
	// Conveyor positions
	if (beforeOvenPositionList01->isTheBottomBasket(b))		return beforeOvenPositionList01;
	if (beforeOvenPositionList02->isTheBottomBasket(b))		return beforeOvenPositionList02;
	if (beforeOvenPositionList03->isTheBottomBasket(b))		return beforeOvenPositionList03;
	if (beforeOvenPositionList04->isTheBottomBasket(b))		return beforeOvenPositionList04;

	if (beforeOvenPositionList05->isTheBottomBasket(b))		return beforeOvenPositionList05;
	if (beforeOvenPositionList06->isTheBottomBasket(b))		return beforeOvenPositionList06;

	if (ovenPositionList01->isTheBottomBasket(b))			return ovenPositionList01;
	if (ovenPositionList02->isTheBottomBasket(b))			return ovenPositionList02;
	if (ovenPositionList03->isTheBottomBasket(b))			return ovenPositionList03;
	if (ovenPositionList04->isTheBottomBasket(b))			return ovenPositionList04;

	if (ovenPositionList05->isTheBottomBasket(b))			return ovenPositionList05;
	if (ovenPositionList06->isTheBottomBasket(b))			return ovenPositionList06;

	if (afterOvenPositionList01->isTheBottomBasket(b))		return afterOvenPositionList01;
	if (afterOvenPositionList02->isTheBottomBasket(b))		return afterOvenPositionList02;
	if (afterOvenPositionList03->isTheBottomBasket(b))		return afterOvenPositionList03;
	if (afterOvenPositionList04->isTheBottomBasket(b))		return afterOvenPositionList04;

	if (returningConvPositionList01->isTheBottomBasket(b))	return returningConvPositionList01;
	if (returningConvPositionList02->isTheBottomBasket(b))	return returningConvPositionList02;
	if (returningConvPositionList03->isTheBottomBasket(b))	return returningConvPositionList03;
	if (returningConvPositionList04->isTheBottomBasket(b))	return returningConvPositionList04;  // return from oven, 160
	if (returningConvPositionList05->isTheBottomBasket(b))	return returningConvPositionList05;  // 30 ft 1+2

	//new oven
	if (beforeOvenPositionList07->isTheBottomBasket(b))		return beforeOvenPositionList07;
	if (beforeOvenPositionList08->isTheBottomBasket(b))		return beforeOvenPositionList08;
	if (beforeOvenPositionList09->isTheBottomBasket(b))		return beforeOvenPositionList09;

	if (newOvenInPositionList01->isTheBottomBasket(b))		return newOvenInPositionList01;
	if (newOvenInPositionList02->isTheBottomBasket(b))		return newOvenInPositionList02;
	if (newOvenInPositionList03->isTheBottomBasket(b))		return newOvenInPositionList03;
	
	if (newOvenOutPositionList01->isTheBottomBasket(b))		return newOvenOutPositionList01;
	if (newOvenOutPositionList02->isTheBottomBasket(b))		return newOvenOutPositionList02;
	if (newOvenOutPositionList03->isTheBottomBasket(b))		return newOvenOutPositionList03;

	if (basketInOutPositionList->isTheBottomBasket(b))		return basketInOutPositionList;

	return NULL;
}

void allPositionsClass::moveBaskets(positionListClass* from, positionListClass* to)
{
	if ((!from) || (!to))
		return;

	if (from->getNumberOfBaskets() == 0)
		return;

	if (to->getNumberOfBaskets() != 0)
		return;

	if (from->getNumberOfPositions() != to->getNumberOfPositions())
		return;

	for (int i = 0; i < conveyorSourceMarkers.count(); i++)
		conveyorSourceMarkers[i]->setConveyorSource(false);
	for (int i = 0; i < conveyorTargetMarkers.count(); i++)
		conveyorTargetMarkers[i]->setConveyorTarget(false);
	conveyorSourceMarkers.clear();
	conveyorTargetMarkers.clear();

	for (int i = 0; i < from->getNumberOfPositions(); i++)
	{
		from->getPosition(i)->setConveyorSource(true);
		conveyorSourceMarkers << from->getPosition(i);
		to->getPosition(i)->setBasketTableId(from->getPosition(i)->getBasketTableId(), from->getPosition(i)->hasContents());
		if (to->getPosition(i)->hasBasket())
		{
			to->getPosition(i)->setConveyorTarget(true);
			conveyorTargetMarkers << to->getPosition(i);
		}
		from->getPosition(i)->setBasketTableId(0);
	}
}

int allPositionsClass::getNumberOf30feetEmptyPositionsInFullBuffer()
{
	int ans = 0;

	for (int i = 0; i < buffer30PositionList.count(); i++)
		ans += buffer30PositionList[i]->getNumberOfPositions() - buffer30PositionList[i]->getNumberOfBaskets();

	return ans;
}

positionListClass* allPositionsClass::getBasketPositionListFromFullBufferWithBasketId(int id)
{
	if (id > 0)
	{
		for (int i = 0; i < bufferPositionList.count(); i++)
		{
			if (bufferPositionList[i]->getBaskePositiontWithBasketId(id))
				return bufferPositionList[i];
		}
		//take baskets from the returningConvPositionList04
		if (returningConvPositionList04->getBaskePositiontWithBasketId(id))
			return returningConvPositionList04;
	}
	return NULL;
}

void allPositionsClass::conveyorsBasketMemoryChanged(int index, int bottomBasket)
{
	if (bottomBasket == 0) {
		if (index == 15)	loadPositionFromDatabase(afterOvenPositionList01);
		if (index == 16)	loadPositionFromDatabase(afterOvenPositionList02);
		if (index ==  7)	loadPositionFromDatabase(afterOvenPositionList03);
		if (index ==  8)	loadPositionFromDatabase(afterOvenPositionList04);

		return;
	}

	if (index == 9)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList01);
	if (index == 11)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList02);
	if (index == 1)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList03);
	if (index == 3)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList04);

	if (index == 10)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList05);
	if (index == 2)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList06);

	if (index == 12)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), ovenPositionList01);
	if (index == 14)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), ovenPositionList02);
	if (index == 4)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), ovenPositionList03);
	if (index == 6)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), ovenPositionList04);

	if (index == 13)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), ovenPositionList05);
	if (index == 5)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), ovenPositionList06);

	if (index == 15)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), afterOvenPositionList01);
	if (index == 16)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), afterOvenPositionList02);  // Cross transfer 1
	if (index == 7)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), afterOvenPositionList03);
	if (index == 8)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), afterOvenPositionList04);  // Cross transfer 2

	if (index == 17) {
		// Position 700, BasketCrane2's empty return position
		if (getPositionListWithBasketOnFirstPos(bottomBasket) != 0) {
			moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), returningConvPositionList01);
		}
		else {
			// if not moving full baskets from another position, then assume we are wanting to update display of empties being returned.
			loadPositionFromDatabase(returningConvPositionList01);
		}
	}
	if (index == 19)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), returningConvPositionList02);
	if (index == 20)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), returningConvPositionList03);
	if (index == 22)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), returningConvPositionList04);

	if (index == 18)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), returningConvPositionList05);  // 30 ft mid return
	// ? 21 --> 30ft return 2, does not appear to be used...

	//if ((ovenPositionList01->getNumberOfBaskets()+ovenPositionList02->getNumberOfBaskets()+ovenPositionList03->getNumberOfBaskets()+ovenPositionList04->getNumberOfBaskets()+ovenPositionList05->getNumberOfBaskets()+ovenPositionList06->getNumberOfBaskets())==0)
	//	hasBasketInOven=false;

	if (index == 44)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList07);
	if (index == 45)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList08);
	if (index == 46)	moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList09);

	// Stacker 2 full basket
	if (index == 39)	afterStacker2Position->getPosition(0)->setBasketTableId(bottomBasket);

	// Packaging in / out
	if (index == 27) {
		if (getPositionListWithBasketOnFirstPos(bottomBasket) != 0) 
		{
			// if we are moving full baskets, move the baskets to packaging in/out then clear them
			// assumption : if they are already in the database then they are full.
			// TODO: find a way to delay so animation works...
			moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), basketInOutPositionList);

			// a short delay to ensure database is updated, then clear it
			Sleep(250);
			loadPositionFromDatabase(basketInOutPositionList);

			// clear the conveyor position.
			Sleep(250);
			setPlcNetworkActive(PLC_NETWORK_CONVEYORS, false);
			conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(27).arg(0));
			setPlcNetworkIdle(PLC_NETWORK_CONVEYORS);
		}
		else
		{
			// if not moving full baskets from another position, then assume we are wanting to update display of empties being returned.
			loadPositionFromDatabase(basketInOutPositionList);
		}
	}

	emit openGlChangedSignal();
}

void allPositionsClass::newOvenConveyorsBasketMemoryChanged(int index, int bottomBasket)
{
	if (bottomBasket == 0) {
		if (index == 7)	loadPositionFromDatabase(newOvenOutPositionList01);
		if (index == 8)	loadPositionFromDatabase(newOvenOutPositionList02);
		if (index == 9)	loadPositionFromDatabase(newOvenOutPositionList03);

		return;
	}

	// commented out as now handled by 44-46 above.
	// if (index == 1)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList07);
	// if (index == 2)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList08);
	// if (index == 3)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), beforeOvenPositionList09);

	if (index == 4)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), newOvenInPositionList01);
	if (index == 5)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), newOvenInPositionList02);
	if (index == 6)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), newOvenInPositionList03);
	if (index == 7)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), newOvenOutPositionList01);
	if (index == 8)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), newOvenOutPositionList02);
	if (index == 9)		moveBaskets(getPositionListWithBasketOnFirstPos(bottomBasket), newOvenOutPositionList03);

	emit openGlChangedSignal();
}

void allPositionsClass::loadPositionFromDatabase(positionListClass* reloadThisLocation)
{
	for (int i = 0; i < reloadThisLocation->getNumberOfPositions(); i++) {
		reloadThisLocation->getPosition(i)->loadFromDataBase();
	}
}

void allPositionsClass::timeLeftForOvenTofinishChanged(int minutesValue)
{
	timeLeftForOvenStr.clear();

	int time = minutesValue;
	int hours = time / (60); time -= hours * (60);
	int minutes = time;

	if (hours > 0)
		timeLeftForOvenStr += QString("%1 hours ").arg(hours);

	if (minutes > 0)
		timeLeftForOvenStr += QString("%1 minutes ").arg(minutes);

	if (minutesValue > 0)
		timeLeftForOvenStr += "left";

	emit openGlChangedSignal();
}

void allPositionsClass::changeConveyorsBasketMemory(int plcPosId, int basket)
{
	setPlcNetworkActive(PLC_NETWORK_CONVEYORS, false);
	// Hand the basket ID to DMS at the oven loading position.
	if (plcPosId == 400 || plcPosId == 401)
		conveyorsPlcTcpServer->sendString(QString("N12|5|%1").arg(basket));

	if (plcPosId == 101)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(9).arg(basket));  // oven 1 sw
	if (plcPosId == 111)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(11).arg(basket)); // oven 1 se
	if (plcPosId == 121)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(1).arg(basket));  // oven 1 nw
	if (plcPosId == 131)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(3).arg(basket));  // oven 1 ne
	if (plcPosId == 141)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(10).arg(basket)); // oven 1 30ft s
	if (plcPosId == 151)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(2).arg(basket));  // oven 1 30ft n

	if (plcPosId == 601)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(12).arg(basket)); // in oven 1, sw 
	if (plcPosId == 611)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(14).arg(basket)); // in oven 1, se
	if (plcPosId == 621)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(4).arg(basket));  // in oven 1, nw
	if (plcPosId == 631)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(6).arg(basket));  // in oven 1, ne
	if (plcPosId == 641)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(13).arg(basket)); // in oven 1, 30ft s
	if (plcPosId == 651)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(5).arg(basket));  // in oven 1, 30ft N

	if (plcPosId == 661)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(15).arg(basket)); // after oven 1, s
	if (plcPosId == 671)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(16).arg(basket)); // cross transfer 2, s
	if (plcPosId == 681)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(7).arg(basket));  // after oven 1, n
	if (plcPosId == 691)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(8).arg(basket));  // cross transfer 1, n

	if (plcPosId == 701)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(17).arg(basket));  // return 4
	if (plcPosId == 711)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(19).arg(basket));  // return 3
	if (plcPosId == 721)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(20).arg(basket));  // return 2
	if (plcPosId == 161)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(22).arg(basket));  // return 1
	if (plcPosId == 731)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(18).arg(basket));  // return 1+2 (30ft)

	// New age oven -- ordered N to S 
	if (plcPosId == 291)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(44).arg(basket));  // oven 2 n
	if (plcPosId == 281)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(45).arg(basket));  // oven 2 mid
	if (plcPosId == 221)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(46).arg(basket));  // oven 2 s

	// Press 2 stacker (empty basket supplied to in)
	if (plcPosId == 451)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(38).arg(basket));
	// Press 2 Stacker (full basket delivered from out)
	if (plcPosId == 461)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(39).arg(basket));

	// Packaging in - out, the crane can't place here so this should never be hit
	if (plcPosId == 501)	conveyorsPlcTcpServer->sendString(QString("N11|%1|%2").arg(27).arg(basket));
	setPlcNetworkIdle(PLC_NETWORK_CONVEYORS);
}

void allPositionsClass::mainTimerSlot()
{
	LOG("mainTimerSlot called", "INFO: Step 1");
	setMainTimerFiring();
	// Pick up externally updated conveyor assignments even with auto mode off.
	// This includes the third basket on return conveyor 160.
	if (!autoModeEnum)
	{
		checkOvenUnloadPositions();
		emit openGlChangedSignal();
	}

	LOG("mainTimerSlot called", "INFO: Step 2");
	if (plcBeforeStackerPosMadeStep)
	{
		LOG("mainTimerSlot called", "INFO: Step 2.1");
		basketPositionClass* beforeStackerBasketPos = beforeStackerPosition->getFreeToMoveBasketPosition();

		if (beforeStackerBasketPos && (!beforeStackerBasketPos->getIsLockedByCrane()))
		{
			LOG("mainTimerSlot called", "INFO: Step 2.1.1");
			basketPositionClass* stackerBasketPos = stackerPosition->getFreeToRecievePos(beforeStackerBasketPos);
			if (beforeStackerBasketPos && stackerBasketPos)
			{
				LOG("mainTimerSlot called", "INFO: Step 2.1.1.1");
				int basketId = beforeStackerBasketPos->getBasketTableId();
				if (basketId > 0)
				{
					LOG("mainTimerSlot called", "INFO: Step 2.1.1.1.1");
					stackerBasketPos->setBasketTableId(basketId);
					beforeStackerBasketPos->setBasketTableId(0);
					plcBeforeStackerPosMadeStep = false;

					// Reset oven in / out times for basket
					setDbTableActive("basket_location", false);
					execDBQuery(QString("update basket_location set audittime = NULL where basketnum = %1 and (basketlocation = %2 or basketlocation = %3)")
						.arg(basketId).arg(basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN).arg(basketPositionClass::EPICS_POS_BASKET_IN_OVEN), APEX_DB_CONNECTION);
					setDbTableIdle("basket_location");

					emit openGlChangedSignal();

					basketNeededBeforeStackerTime.restart();
				}
				LOG("mainTimerSlot called", "INFO: Step 2.1.1.2");
			}
			LOG("mainTimerSlot called", "INFO: Step 2.1.2");
		}
		LOG("mainTimerSlot called", "INFO: Step 2.2");
	}

	LOG("mainTimerSlot called", "INFO: Step 3");
	if (plcStackerPosMadeStep)
	{
		LOG("mainTimerSlot called", "INFO: Step 3.1");
		basketPositionClass* stackerBasketPos = stackerPosition->getFreeToMoveBasketPosition();
		basketPositionClass* afterStackerBasketPos = afterStackerPosition->getFreeToRecievePos(stackerBasketPos);

		if (afterStackerBasketPos && (!afterStackerBasketPos->getIsLockedByCrane()))
		{
			LOG("mainTimerSlot called", "INFO: Step 3.1.1");
			if (stackerBasketPos && afterStackerBasketPos)
			{
				LOG("mainTimerSlot called", "INFO: Step 3.1.1.1");
				int basketTableId = stackerBasketPos->getBasketTableId();
				if (basketTableId > 0)
				{
					LOG("mainTimerSlot called", "INFO: Step 3.1.1.1.1");
					setDbTableActive("baskets", false);
					execDBQuery(QString("update baskets set secFilled = %1 where pid = %2").arg(QDateTime::currentDateTime().toTime_t()).arg(basketTableId), MAIN_DB_CONNECTION);
					setDbTableIdle("baskets");

					afterStackerBasketPos->setBasketTableId(basketTableId);
					afterStackerPosition->setLockRecieve(true);
					stackerBasketPos->setBasketTableId(0);
					plcStackerPosMadeStep = false;

					if (afterStackerBasketPos->getTemperEnum() == basketClass::BASKET_TEMPER_000) // no temper required
					{
						LOG("mainTimerSlot called", "INFO: Step 3.1.1.1.1.1");
						setDbTableActive("basket_location", false);
						execDBQuery(QString("update basket_location set audittime = CONVERT(datetime, CURRENT_TIMESTAMP) where basketnum = %1 and basketlocation = %2") // was just 3, TEMPER_000 deos not need aging.
								.arg(basketTableId).arg(basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN), APEX_DB_CONNECTION);
						setDbTableIdle("basket_location");
					}
#ifdef EMULATE
					//double length=3.+static_cast<double>(6.)*static_cast<double>(rand())/static_cast<double>(RAND_MAX);
					//execQuery(QString("insert into basket_details (basketnum,dienum,sequence,SONum,SOItemNum,Lot,Pc,PcLen,Wt,temper) VALUES('%1',7777,7777,7777,7777,7777,7777,%2,7777,'T1')").arg(basketTableId).arg(length*1000.),APEX_DB_CONNECTION);
					int noOfProfiles=1+static_cast<double>(4.0)*(static_cast<double>(rand())/static_cast<double>(RAND_MAX));
					for (int i=0;i<noOfProfiles;i++)
					{
						double length=3.+static_cast<double>(4.0)*static_cast<double>(rand())/static_cast<double>(RAND_MAX);
						int no=10+static_cast<double>(100.0)*(static_cast<double>(rand())/static_cast<double>(RAND_MAX));
						setDbTableActive("basket_details", false);
						execQuery(QString("insert into basket_details (basketnum,dienum,Pc,PcLen,temper) values(%1,%2,%3,%4,'%5')").arg(basketTableId).arg(rand()).arg(no).arg(length*1000.).arg("T1"),APEX_DB_CONNECTION);
						setDbTableIdle("basket_details");
					}
					afterStackerBasketPos->loadFromDataBase();
#endif
					emit openGlChangedSignal();

					basketAfterStackerReadyTime.restart();
					LOG("mainTimerSlot called", "INFO: Step 3.1.1.1.2");
				}
				LOG("mainTimerSlot called", "INFO: Step 3.1.1.2");
			}
			LOG("mainTimerSlot called", "INFO: Step 3.1.2");
		}
		LOG("mainTimerSlot called", "INFO: Step 3.2");
	}

	LOG("mainTimerSlot called", "INFO: Step 4");
	if (plcBeforeStacker2PosMadeStep)
	{
		LOG("mainTimerSlot called", "INFO: Step 4.1");
		basketPositionClass* beforeStacker2BasketPos = beforeStacker2Position->getFreeToMoveBasketPosition();

		if (beforeStacker2BasketPos && (!beforeStacker2BasketPos->getIsLockedByCrane()))
		{
			LOG("mainTimerSlot called", "INFO: Step 4.1.1");
			int basketTableId = beforeStacker2BasketPos->getBasketTableId();
			if (basketTableId > 0)
			{
				LOG("mainTimerSlot called", "INFO: Step 4.1.1.1");
				beforeStacker2BasketPos->setBasketTableId(0);
				plcBeforeStacker2PosMadeStep = false;

				// Reset oven in / out times for basket
				setDbTableActive("basket_location", false);
				execDBQuery(QString("update basket_location set audittime = NULL where basketnum = %1 and (basketlocation = %2 or basketlocation = %3)")
					.arg(basketTableId).arg(basketPositionClass::EPICS_POS_BASKET_IN_OVEN).arg(basketPositionClass::EPICS_POS_BASKET_AFTER_OVEN), APEX_DB_CONNECTION);
				setDbTableIdle("basket_location");

				emit openGlChangedSignal();

				basketNeededBeforeStackerTime.restart();
			}
			LOG("mainTimerSlot called", "INFO: Step 4.1.2");
		}
		LOG("mainTimerSlot called", "INFO: Step 4.2");

	}

	LOG("mainTimerSlot called", "INFO: Step 5");
	//if (plcStacker2PosMadeStep)
	//{
	//}

	// Destackers taken offline
//	if (returningBasketFromOutValueChanged == 1)
//	{
//		if ((!returningFromDestackersPosition->getPosition(0)->getIsLockedByCrane()) && (!crane->getHasActiveMission()))returningBasketFromOut
//		{
//			if (returningBasketFromOut != returningFromDestackersPosition->getPosition(0)->getBasketTableId())
//			{
//				if (returningBasketFromOut > 0)//normal basket
//				{
//					int pidInBasketsTable = GETONEATMOST(QString("select pid from baskets where pid=%1").arg(returningBasketFromOut)).toInt();
//					if (pidInBasketsTable == 0)
//						EXIT(tr("Error in baskets table"), tr("The basket %1 returning from destackers is does not exist in baskets table.").arg(returningBasketFromOut));
//
//					int positionNumberOfReturningBasketInPositionTable = GETONEATMOST(QString("select posNumber from positions where basketTableId=%1").arg(returningBasketFromOut)).toInt();
//					int positionIndexOfReturningBasketInPositionTable = GETONEATMOST(QString("select posIndex from positions where basketTableId=%1").arg(returningBasketFromOut)).toInt();
//					if (((positionNumberOfReturningBasketInPositionTable != 0) || (positionIndexOfReturningBasketInPositionTable != 0)) && (positionNumberOfReturningBasketInPositionTable != 500))
//						EXIT(tr("Error in positions table"), tr("The basket %1 returning from destackers is already in position %2-%3 according to the database.").arg(returningBasketFromOut).arg(positionNumberOfReturningBasketInPositionTable).arg(positionIndexOfReturningBasketInPositionTable));
//
//					execQuery(QString("delete from basket_details where basketnum = %1").arg(returningBasketFromOut), APEX_DB_CONNECTION);
//
//					returningFromDestackersPosition->getPosition(0)->setBasketTableId(returningBasketFromOut);
//					returningFromDestackersPosition->getPosition(0)->setLocked(false);
//					returningBasketFromOutValueChanged = 0;
//				}
//				else if (returningBasketFromOut==-999)//==-999 scrap basket
//				{
//#ifdef EMULATE
//					int availableScrapBasketIdInDb=GETONE(QString("select pid from baskets where pid not in (select basketTableId from positions) and type=%1 order by pid limit 1").arg(BASKET_SCRAP)).toInt();
//#else
//					int availableScrapBasketIdInDb=GETONE(QString("select top 1 pid from baskets where pid not in (select basketTableId from positions) and type=%1 order by pid").arg(BASKET_SCRAP)).toInt();
//#endif
//					if (availableScrapBasketIdInDb>0)
//						returningFromDestackersPosition->getPosition(0)->setBasketTableId(availableScrapBasketIdInDb);
//					returningFromDestackersPosition->getPosition(0)->setLocked(false);
//					returningBasketFromOutValueChanged=0;
//				}
//				else if (returningBasketFromOut == 0)
//				{
//					returningFromDestackersPosition->getPosition(0)->setLocked(false);
//					returningFromDestackersPosition->getPosition(0)->setBasketTableId(0);
//					returningBasketFromOutValueChanged = 0;
//				}
//			}
//			emit openGlChangedSignal();
//		}
//	}
	setMainTimerIdle();
	LOG("mainTimerSlot called", "INFO: Step 6");
}

////////////////////////////////////////////////////////////////////////////////
void allPositionsClass::beforeStacker()
{	
	if (crane->getHasActiveMission())  return;

	if (autoModeEnum & AUTO_BEFORE_STACKER)
	{
		basketPositionClass *fromPos = NULL;
		basketPositionClass *toPos = NULL;

		if (!beforeStackerPosition->getPosition(0)->hasBasket())
		{
			// returningConvPositionList04

			// Destacker offline
			//if (returningFromDestackersPosition->getPosition(0)->hasBasket() && returningFromDestackersPosition->getPosition(0)->isBasketNormal())
			//{
			//	fromPos = returningFromDestackersPosition->getFreeToMoveBasketPosition();
			//	toPos = beforeStackerPosition->getFreeToRecievePos(fromPos);
			//}

			if ((!fromPos) || (!toPos))
			{
				for (int i = emptyPositionList.count() - 1; i >= 0; i--)
				{
					fromPos = emptyPositionList[i]->getFreeToMoveBasketPosition();
					if (fromPos && (fromPos->getContentsEnum() == BASKET_CONTENS_NONE))
					{
						toPos = beforeStackerPosition->getFreeToRecievePos(fromPos);
						if ((fromPos) && (toPos))
							break;
					}
				}
			}

			if (fromPos && toPos)
				crane->move(fromPos, toPos);
		}
	}
}

void allPositionsClass::beforeStacker2()
{
	if (crane->getHasActiveMission())  return;

	if (autoModeEnum & AUTO_BEFORE_STACKER2)
	{
		basketPositionClass *fromPos = NULL;
		basketPositionClass *toPos = NULL;

		if (!beforeStacker2Position->getPosition(0)->hasBasket())
		{
			// Destacker offline
			//if (returningFromDestackersPosition->getPosition(0)->hasBasket() && returningFromDestackersPosition->getPosition(0)->isBasketNormal())
			//{
			//	fromPos = returningFromDestackersPosition->getFreeToMoveBasketPosition();
			//	toPos = beforeStackerPosition->getFreeToRecievePos(fromPos);
			//}

			if ((!fromPos) || (!toPos))
			{
				for (int i = emptyPositionList.count() - 1; i >= 0; i--)
				{
					fromPos = emptyPositionList[i]->getFreeToMoveBasketPosition();
					if (fromPos && (fromPos->getContentsEnum() == BASKET_CONTENS_NONE))
					{
						toPos = beforeStacker2Position->getFreeToRecievePos(fromPos);
						if ((fromPos) && (toPos))
							break;
					}
				}
			}

			if (fromPos && toPos) {
				crane->move(fromPos, toPos);

			}
		}
	}
}

//////////////////////////////////////////////////////
// Decide to send basket to Oven 1, Oven 2 or Buffer
// Added by Vahid
//////////////////////////////////////////////////////
bool allPositionsClass::BasketCanGoToOven1(basketPositionClass* fromPos)
{
	bool Oven1IsFull = false;
	bool Oven1NotAcceptsLongerThan24Feet = false;
	bool TempMatchesOven1Cycle = false;
	bool BasketLengthIsMoreThan24Feet = false;

	Oven1IsFull = isOvenEntranceFull();

	if (autoModeEnum & AUTO_OVEN1_NOT24)
		Oven1NotAcceptsLongerThan24Feet = true;

	if (fromPos->getContentsEnum() == BASKET_CONTENS_LESS_THAN_30)
		BasketLengthIsMoreThan24Feet = true;

	if (
		((fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_185) && (nextOvenLoadingCycleTemperEnum == NEXT_OVEN_CYCLE_TEMPER_185))
		||
		((fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_230) && (nextOvenLoadingCycleTemperEnum == NEXT_OVEN_CYCLE_TEMPER_230))
		)
		TempMatchesOven1Cycle = true;

	if ((!Oven1IsFull) && (TempMatchesOven1Cycle))
	{
		if (!Oven1NotAcceptsLongerThan24Feet)
		{
			return true;
		}
		else
		{
			if (!BasketLengthIsMoreThan24Feet)
				return true;
		}
	}
	return false;
}

bool allPositionsClass::BasketCanGoToOven2(basketPositionClass* fromPos)
{
	bool Oven2IsFull = false;
	bool Oven2AcceptsOnlyLongerThan24Feet = false;
	bool TempMatchesOven2Cycle = false;
	bool BasketLengthIsMoreThan24Feet = false;

	if (autoModeEnum & AUTO_OVEN2_ONLY24)
		Oven2AcceptsOnlyLongerThan24Feet = true;

	if (
		((fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_185) && (nextOven2LoadingCycleTemperEnum == NEXT_OVEN_CYCLE_TEMPER_185))
		||
		((fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_230) && (nextOven2LoadingCycleTemperEnum == NEXT_OVEN_CYCLE_TEMPER_230))
		)
		TempMatchesOven2Cycle = true;

	Oven2IsFull = isOven2EntranceFull();

	if (fromPos->getContentsEnum() == BASKET_CONTENS_LESS_THAN_30)
		BasketLengthIsMoreThan24Feet = true;

	if ((!Oven2IsFull) && (TempMatchesOven2Cycle))
	{
		if (!Oven2AcceptsOnlyLongerThan24Feet)
		{
			return true;
		}
		else
		{
			if (BasketLengthIsMoreThan24Feet)
				return true;
		}
	}
	return false;
}

int allPositionsClass::AgingOvensDecisionMaking(basketPositionClass* fromPos)
{

	// return:
		// 0: Buffer
		// 1: Aging Oven 1
		// 2: Aging Oven 2

	bool oven1IsEnabled = false;
	bool oven2IsEnabled = false;
	bool pressToOvenEnabled = false;

	int fromBasketNum = 0;
	int orgPress = 0;
	int returnCode = 0;

	if (autoModeEnum & AUTO_OVEN1_ENABLE)
		oven1IsEnabled = true;

	if (autoModeEnum & AUTO_OVEN2_ENABLE)
		oven2IsEnabled = true;

	if (autoModeEnum & AUTO_PRESSTOOVEN_ENABLE)
		pressToOvenEnabled = true;

	if (pressToOvenEnabled)
	{
		fromBasketNum = fromPos->getBasketNumber();

		// Get original pressdienum
		// QVector<QVariant> pressArrayy = execQuery(QString("select press from basket_details where basketnum=%1 and Pc<>0").arg(tableId), APEX_DB_CONNECTION);
		//QVector<QVariant> tVector = execQuery(QString("select temper from basket_details where basketnum=%1 and temper is not Null").arg(basektTableId), APEX_DB_CONNECTION);
		//QString temper = "F";

		//if (tVector.count() > 0)
		//	temper = tVector[0].toString();

		if (fromPos->getNumber() == afterStackerPosition->getPosition(0)->getNumber() && BasketCanGoToOven1(fromPos) && oven1IsEnabled)
			returnCode = 1;

		if (fromPos->getNumber() == afterStacker2Position->getPosition(0)->getNumber() && BasketCanGoToOven2(fromPos) && oven2IsEnabled)
			returnCode = 2;
	}

	if (!pressToOvenEnabled)
	{
		if (BasketCanGoToOven1(fromPos) && (oven1IsEnabled))
			returnCode = 1;

		if (BasketCanGoToOven2(fromPos) && (oven2IsEnabled))
			returnCode = 2;
	}

	return returnCode;
}

basketPositionClass *allPositionsClass::getEmptyPositionInTheEntranceOfTheOven2(basketPositionClass* fromPos)
{
	basketPositionClass *toPos = NULL;
	//           before oven2 positions
	//							7						
	//							9						
	//							8
	// LOG("ap.cpp", "1,3,2 Sequence -> 1,2,3");
	if (!toPos)  toPos = beforeOvenPositionList09->getFreeToRecievePos(fromPos);
	if (!toPos)  toPos = beforeOvenPositionList08->getFreeToRecievePos(fromPos);  // seq swapped to 1 2 3 . 08 07 swap will be 1 3 2 
	if (!toPos)  toPos = beforeOvenPositionList07->getFreeToRecievePos(fromPos); // swap this line with the next (gene)
//	if (!toPos)  toPos = beforeOvenPositionList08->getFreeToRecievePos(fromPos);

	return toPos;
}
//////////////////////////////////////////////////////
// End of code added by Vahid
//////////////////////////////////////////////////////

basketPositionClass *allPositionsClass::getEmptyPositionInTheEntranceOfTheOven(basketPositionClass* fromPos)
{
	basketPositionClass *toPos=NULL;
	//           before oven positions
	//										6
	//							3						4
	//							1						2
	//										5
	if (  
		((fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_185) && (nextOvenLoadingCycleTemperEnum != NEXT_OVEN_CYCLE_TEMPER_185))
		||
		((fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_230) && (nextOvenLoadingCycleTemperEnum != NEXT_OVEN_CYCLE_TEMPER_230))
		)
		return NULL;

	if ((fromPos->getContentsEnum() == BASKET_CONTENS_LESS_THAN_20) || (fromPos->getContentsEnum() == BASKET_CONTENS_LESS_THAN_24))
	{
		// if (!toPos)		toPos = beforeOvenPositionList04->getFreeToRecievePos(fromPos);  // @@@@@ -- change to 03 -- AB - LCMIT-12497
		// if (!toPos)		toPos = beforeOvenPositionList03->getFreeToRecievePos(fromPos);  // @@@@@ -- change to 04 -- AB - LCMIT-12497
		// if (!toPos)		toPos = beforeOvenPositionList02->getFreeToRecievePos(fromPos);  // @@@@@ -- change to 01 -- AB - LCMIT-12497
		// if (!toPos)		toPos = beforeOvenPositionList01->getFreeToRecievePos(fromPos);  // @@@@@ -- change to 02 -- AB - LCMIT-12497
		if (!toPos)		toPos = beforeOvenPositionList03->getFreeToRecievePos(fromPos);  // @@@@@ -- change to 03 -- AB - LCMIT-12497
		if (!toPos)		toPos = beforeOvenPositionList04->getFreeToRecievePos(fromPos);  // @@@@@ -- change to 04 -- AB - LCMIT-12497
		if (!toPos)		toPos = beforeOvenPositionList01->getFreeToRecievePos(fromPos);  // @@@@@ -- change to 01 -- AB - LCMIT-12497
		if (!toPos)		toPos = beforeOvenPositionList02->getFreeToRecievePos(fromPos);  // @@@@@ -- change to 02 -- AB - LCMIT-12497
		if (!toPos)		toPos = beforeOvenPositionList06->getFreeToRecievePos(fromPos);
		if (!toPos)		toPos = beforeOvenPositionList05->getFreeToRecievePos(fromPos);
	}
	else if (fromPos->getContentsEnum()==BASKET_CONTENS_LESS_THAN_30)
	{
		if (beforeOvenPositionList05->getNumberOfBaskets() < 3)
		{
			if (!toPos)		toPos = beforeOvenPositionList05->getFreeToRecievePos(fromPos);
			if (!toPos)		toPos = beforeOvenPositionList06->getFreeToRecievePos(fromPos);
		}
		else if (beforeOvenPositionList05->getNumberOfBaskets()==3)
		{
			if (!toPos)		toPos = beforeOvenPositionList06->getFreeToRecievePos(fromPos);
			if (!toPos)
			{
				if ((beforeOvenPositionList03->getNumberOfBaskets() == 4) && (beforeOvenPositionList04->getNumberOfBaskets() == 4))
					toPos = beforeOvenPositionList05->getFreeToRecievePos(fromPos);
			}
		}
		else if (beforeOvenPositionList06->getNumberOfBaskets() == 4)
		{
			if (!toPos)toPos = beforeOvenPositionList05->getFreeToRecievePos(fromPos);
		}
	}
	return toPos;
}

basketPositionClass* allPositionsClass::getHighlightedPos()
{
	basketPositionClass* ans;
	for (int i = 0; i < allPositionList.count(); i++)
	{
		ans = allPositionList[i]->getHighlightedPos();
		if (ans)
			return ans;
	}
	return NULL;
}

basketPositionClass* allPositionsClass::getFreePositionToRecieve(basketPositionClass* from, bool ignoreBasketsForExport)
{
	int nextBasketForExport = 0;
	basketPositionClass* ans = NULL;
	if (!from)
		return NULL;

//#ifdef EMULATE
//	int nextBasketForExport = GETONEATMOST(QString("select basketTableId from toEastConveyorExports order by priority limit 1")).toInt();
//#else
//	int nextBasketForExport = GETONEATMOST(QString("select top 1 basketTableId from toEastConveyorExports order by priority")).toInt();
//#endif

	positionListClass* positionListWithBasketForExport = NULL;

	for (int i = 0; i < bufferPositionList.count(); i++)
	{

		if ((!positionListWithBasketForExport) && (bufferPositionList[i]->getBaskePositiontWithBasketId(nextBasketForExport)))
			positionListWithBasketForExport = bufferPositionList[i];
		
		if (positionListWithBasketForExport)
			break;
	}

	/// check positionListWithBasketForExport
	for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
	{
		if ((bufferPositionList[i]->getNumberOfBaskets() == 0) &&
			(bufferPositionList[i] != positionListWithBasketForExport) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
			break;
	}

	for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
	{
		if ((bufferPositionList[i]->getNumberOfBaskets() == 1) &&
			(bufferPositionList[i] != positionListWithBasketForExport) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
			break;
	}

	for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
	{
		if ((bufferPositionList[i]->getNumberOfBaskets() == 2) &&
			(bufferPositionList[i] != positionListWithBasketForExport) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
			break;
	}

	for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
	{
		if ((bufferPositionList[i]->getNumberOfBaskets() == 3) &&
			(bufferPositionList[i] != positionListWithBasketForExport) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
			break;
	}

	if (ignoreBasketsForExport)	// do not check positionListWithBasketForExport
	{
		for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
		{
			if ((bufferPositionList[i]->getNumberOfBaskets() == 0) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
				break;
		}

		for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
		{
			if ((bufferPositionList[i]->getNumberOfBaskets() == 1) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
				break;
		}

		for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
		{
			if ((bufferPositionList[i]->getNumberOfBaskets() == 2) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
				break;
		}

		for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
		{
			if ((bufferPositionList[i]->getNumberOfBaskets() == 3) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
				break;
		}
	}

	return ans;
}

void allPositionsClass::setNextBasketForExport(int basketId)	//gui only
{
	for (int i = 0; i < bufferPositionList.count(); i++)
		bufferPositionList[i]->setNextBasketForExport(basketId);
}


// Destackers taken offline
//basketPositionClass* allPositionsClass::getFreePositionToRecieve(basketPositionClass* from, bool ignoreBasketsForExport)
//{
//	basketPositionClass* ans = NULL;
//	if (!from)
//		return NULL;
//
//#ifdef EMULATE
//	int nextBasketForAssimakis=GETONEATMOST(QString("select basketTableId FROM toAssimakisExports order by priority limit 1")).toInt();
//	int nextBasketForOmav=GETONEATMOST(QString("select basketTableId FROM toOmavExports order by priority limit 1")).toInt();
//#else
//	int nextBasketForAssimakis = GETONEATMOST(QString("select top 1 basketTableId FROM toAssimakisExports order by priority")).toInt();
//	int nextBasketForOmav = GETONEATMOST(QString("select top 1 basketTableId FROM toOmavExports order by priority")).toInt();
//#endif
//
//	positionListClass* possitionListWithBasketForAssimakis = NULL;
//	positionListClass* possitionListWithBasketForOmav = NULL;
//
//	for (int i = 0; i < bufferPositionList.count(); i++)
//	{
//
//		if ((!possitionListWithBasketForAssimakis) && (bufferPositionList[i]->getBaskePositiontWithBasketId(nextBasketForAssimakis)))
//			possitionListWithBasketForAssimakis = bufferPositionList[i];
//		if ((!possitionListWithBasketForOmav) && (bufferPositionList[i]->getBaskePositiontWithBasketId(nextBasketForOmav)))
//			possitionListWithBasketForOmav = bufferPositionList[i];
//		if (possitionListWithBasketForAssimakis&&possitionListWithBasketForOmav)
//			break;
//	}
//
//	/// check positionListWithBasketForAssimakis and possitionListWithBasketForOmav
//	for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
//	{
//		if ((bufferPositionList[i]->getNumberOfBaskets() == 0) && 
//				(bufferPositionList[i] != possitionListWithBasketForAssimakis) && (bufferPositionList[i] != possitionListWithBasketForOmav) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
//			break;
//	}
//
//	for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
//	{
//		if ((bufferPositionList[i]->getNumberOfBaskets() == 1) &&
//				(bufferPositionList[i] != possitionListWithBasketForAssimakis) && (bufferPositionList[i] != possitionListWithBasketForOmav) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
//			break;
//	}
//
//	for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
//	{
//		if ((bufferPositionList[i]->getNumberOfBaskets() == 2) && 
//				(bufferPositionList[i] != possitionListWithBasketForAssimakis) && (bufferPositionList[i] != possitionListWithBasketForOmav) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
//			break;
//	}
//
//	for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
//	{
//		if ((bufferPositionList[i]->getNumberOfBaskets() == 3) && 
//				(bufferPositionList[i] != possitionListWithBasketForAssimakis) && (bufferPositionList[i] != possitionListWithBasketForOmav) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
//			break;
//	}
//
//	if (ignoreBasketsForExport)	// do not check positionListWithBasketForAssimakis and possitionListWithBasketForOmav
//	{
//		for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
//		{
//			if ((bufferPositionList[i]->getNumberOfBaskets() == 0) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
//				break;
//		}
//
//		for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
//		{
//			if ((bufferPositionList[i]->getNumberOfBaskets() == 1) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
//				break;
//		}
//
//		for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
//		{
//			if ((bufferPositionList[i]->getNumberOfBaskets() == 2) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
//				break;
//		}
//
//		for (int i = 0; ((!ans) && (i < bufferPositionList.count())); i++)
//		{
//			if ((bufferPositionList[i]->getNumberOfBaskets() == 3) && (ans = bufferPositionList[i]->getFreeToRecievePos(from)))
//				break;
//		}
//	}
//
//	return ans;
//}

void allPositionsClass::setNextBasketForOmavDestacker(int basketId)	//gui only
{
	for (int i = 0; i < bufferPositionList.count(); i++)
		bufferPositionList[i]->setNextBasketForOmavDestacker(basketId);
}

void allPositionsClass::setNextBasketForAssimakisDestacker(int basketId)	//gui only
{
	for (int i = 0; i < bufferPositionList.count(); i++)
		bufferPositionList[i]->setNextBasketForAssimakisDestacker(basketId);
}

QVector<positionListClass*> allPositionsClass::getPosListsWithLessBaskets()
{
	QVector<positionListClass*> ans;

	for (int i = 0; i < bufferPositionList.count(); i++)
	{
		if (bufferPositionList[i]->getNumberOfBaskets() == 0)
			ans << bufferPositionList[i];
	}

	for (int i = 0; i < bufferPositionList.count(); i++)
	{
		if (bufferPositionList[i]->getNumberOfBaskets() == 1)
			ans << bufferPositionList[i];
	}

	for (int i = 0; i < bufferPositionList.count(); i++)
	{
		if (bufferPositionList[i]->getNumberOfBaskets() == 2)
			ans << bufferPositionList[i];
	}

	for (int i = 0; i < bufferPositionList.count(); i++)
	{
		if (bufferPositionList[i]->getNumberOfBaskets() == 3)
			ans << bufferPositionList[i];
	}

	for (int i = 0; i < bufferPositionList.count(); i++)
	{
		if (bufferPositionList[i]->getNumberOfBaskets() == 4)
			ans << bufferPositionList[i];
	}

	return ans;
}

QVector<positionListClass*> allPositionsClass::get24FPosListsWithLessBaskets()
{
	QVector<positionListClass*> ans;

	for (int i = 0; i < buffer24PositionList.count(); i++)
	{
		if (buffer24PositionList[i]->getNumberOfBaskets() == 0)
			ans << buffer24PositionList[i];
	}

	for (int i = 0; i < buffer24PositionList.count(); i++)
	{
		if (buffer24PositionList[i]->getNumberOfBaskets() == 1)
			ans << buffer24PositionList[i];
	}

	for (int i = 0; i < buffer24PositionList.count(); i++)
	{
		if (buffer24PositionList[i]->getNumberOfBaskets() == 2)
			ans << buffer24PositionList[i];
	}

	for (int i = 0; i < buffer24PositionList.count(); i++)
	{
		if (buffer24PositionList[i]->getNumberOfBaskets() == 3)
			ans << buffer24PositionList[i];
	}

	for (int i = 0; i < buffer24PositionList.count(); i++)
	{
		if (buffer24PositionList[i]->getNumberOfBaskets() == 4)
			ans << buffer24PositionList[i];
	}

	return ans;
}

QVector<positionListClass*> allPositionsClass::get30FPosListsWithLessBaskets()
{
	QVector<positionListClass*> ans;

	for (int i = 0; i < buffer30PositionList.count(); i++)
	{
		if (buffer30PositionList[i]->getNumberOfBaskets() == 0)
			ans << buffer30PositionList[i];
	}
	for (int i = 0; i < buffer30PositionList.count(); i++)
	{
		if (buffer30PositionList[i]->getNumberOfBaskets() == 1)
			ans << buffer30PositionList[i];
	}
	for (int i = 0; i < buffer30PositionList.count(); i++)
	{
		if (buffer30PositionList[i]->getNumberOfBaskets() == 2)
			ans << buffer30PositionList[i];
	}
	for (int i = 0; i < buffer30PositionList.count(); i++)
	{
		if (buffer30PositionList[i]->getNumberOfBaskets() == 3)
			ans << buffer30PositionList[i];
	}

	for (int i = 0; i < buffer30PositionList.count(); i++)
	{
		if (buffer30PositionList[i]->getNumberOfBaskets() == 4)
			ans << buffer30PositionList[i];
	}

	return ans;
}

////////////////////////////////////////////////////

void allPositionsClass::afterStacker()
{
	if (crane->getHasActiveMission())  return;

	basketPositionClass *fromPos = afterStackerPosition->getFreeToMoveBasketPosition();
	basketPositionClass *toPos = NULL;
	
	if (fromPos)
		fromPos->loadContentFromDatabase();

	if ((fromPos) && !fromPos->hasContents()){
		// load from Epics

	}
	
	if ((fromPos) && (fromPos->hasContents()))
	{
		// Destacker offline
		//if (takeBasketsFromStackerOut)
		//	toPos = returningFromDestackersPosition->getFreeToRecievePos(fromPos);
		//else if (fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_000)

		if (fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_000)
			toPos = getFreePositionToRecieve(fromPos, true);
		else if ((fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_185) || (fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_230))
		{
			//Changed by Vahid
			int WhichOven = AgingOvensDecisionMaking(fromPos);
			if (WhichOven == 1)
				toPos = getEmptyPositionInTheEntranceOfTheOven(fromPos);
			else
			{
				if (WhichOven == 2)
					toPos = getEmptyPositionInTheEntranceOfTheOven2(fromPos);
				else
					toPos = getFreePositionToRecieve(fromPos, true);
			}
		}
	}

	if (fromPos && toPos)
		crane->move(fromPos, toPos);
}

void allPositionsClass::afterStacker2()
{
	if (crane->getHasActiveMission())  return;

	basketPositionClass *fromPos = afterStacker2Position->getFreeToMoveBasketPosition();
	basketPositionClass *toPos = NULL;

	if (fromPos)
		fromPos->loadContentFromDatabase();

	if ((fromPos) && (fromPos->hasContents()))
	{
		// Destacker offline
		//if (takeBasketsFromStackerOut)
		//	toPos = returningFromDestackersPosition->getFreeToRecievePos(fromPos);

		//else if (fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_000)

		if (fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_000)
			toPos = getFreePositionToRecieve(fromPos, true);
		else if ((fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_185) || (fromPos->getTemperEnum() == basketClass::BASKET_TEMPER_230))
		{
			//Changed by Vahid
			int WhichOven = AgingOvensDecisionMaking(fromPos);
			if (WhichOven == 1) {
				toPos = getEmptyPositionInTheEntranceOfTheOven(fromPos);
			}
			else if (WhichOven == 2) {
				toPos = getEmptyPositionInTheEntranceOfTheOven2(fromPos);
			}
			else {
				toPos = getFreePositionToRecieve(fromPos, true);
			}
		}
	}

	if (fromPos && toPos)
		crane->move(fromPos, toPos);
}

void allPositionsClass::fromOven()
{
	//auto unlock basket
	if (crane->getHasActiveMission()) return;

	if (returningConvPositionList04->getTopBasketPosition() && returningConvPositionList04->getTopBasketPosition()->getLocked())
	{
		returningConvPositionList04->getTopBasketPosition()->setLocked(false);

		int basketTableId = returningConvPositionList04->getTopBasketPosition()->getBasketTableId();
		setDbTableActive("basket_details", true);
		QVector<QVariant> tVector = execDBQuery(QString("select temper from basket_details where basketnum=%1 and temper is not Null").arg(basketTableId), APEX_DB_CONNECTION);
		setDbTableIdle("basket_details");
		QString temper = "F";

		if (tVector.count() > 0)
			temper = tVector[0].toString();

		if (returningConvPositionList04->getTopBasketPosition()->getContentsEnum() == BASKET_CONTENS_LESS_THAN_20) {
			setDbTableActive("basket_details", false);
			execDBQuery(QString("insert into basket_details (basketnum,dienum,sequence,SONum,SOItemNum,Lot,Pc,PcLen,Wt,LotCompleteFlag,PcPulled,temper) VALUES(%1,6666,6666,6666,6666,6666,6666,6666,6666,0,0,'%2')").arg(basketTableId).arg(temper), APEX_DB_CONNECTION);
			setDbTableIdle("basket_details");
		}

		if (returningConvPositionList04->getTopBasketPosition()->getContentsEnum() == BASKET_CONTENS_LESS_THAN_24) {
			setDbTableActive("basket_details", false);
			execDBQuery(QString("insert into basket_details (basketnum,dienum,sequence,SONum,SOItemNum,Lot,Pc,PcLen,Wt,LotCompleteFlag,PcPulled,temper) VALUES(%1,7777,7777,7777,7777,7777,7777,7777,7777,0,0,'%2')").arg(basketTableId).arg(temper), APEX_DB_CONNECTION);
			setDbTableIdle("basket_details");
		}

		returningConvPositionList04->getTopBasketPosition()->loadContentFromDatabase();
	}

	//unload oven
	if (crane->getHasActiveMission())  return;

	if (getNumberOf30feetEmptyPositionsInFullBuffer() > numberOfPosToBeKeptEmptyInBuffer)
	{
		basketPositionClass* fromOvenBasketPos = returningConvPositionList04->getFreeToMoveBasketPosition();
		crane->move(fromOvenBasketPos, getFreePositionToRecieve(fromOvenBasketPos, false));
	}

	emit openGlChangedSignal();
}

void allPositionsClass::exports()
{
	if (crane->getHasActiveMission())	return;

	//Destackers offline
	return;

//#ifdef EMULATE
//	int basketForAssimakis=GETONEATMOST(QString("select basketTableId FROM toAssimakisExports order by priority limit 1")).toInt();setNextBasketForAssimakisDestacker(basketForAssimakis);
//	int basketForOmav=GETONEATMOST(QString("select basketTableId FROM toOmavExports order by priority limit 1")).toInt();setNextBasketForOmavDestacker(basketForOmav);
//#else
//	int basketForAssimakis=GETONEATMOST(QString("select top 1 basketTableId FROM toAssimakisExports order by priority")).toInt();setNextBasketForAssimakisDestacker(basketForAssimakis);
//	int basketForOmav=GETONEATMOST(QString("select top 1 basketTableId FROM toOmavExports order by priority")).toInt();setNextBasketForOmavDestacker(basketForOmav);
//#endif
//
//	if (((basketForAssimakis>0) && (exportPositionListThatWasLastCleared != toDeStacker24Position)) || (basketForOmav == 0) || (!toDeStacker24Position->getPosition(0)->hasBasket()))// to assimakis
//	{
//		positionListClass* fromPositionList=getBasketPositionListFromFullBufferWithBasketId(basketForAssimakis);
//		if (fromPositionList)
//		{
//			basketPositionClass* fromBasketPosition=fromPositionList->getBaskePositiontWithBasketId(basketForAssimakis);
//			if (fromBasketPosition==fromPositionList->getFreeToMoveBasketPosition())//the basket is on top of the stack, take it to destacker
//			{
//				basketPositionClass* toBasketPosition=toDeStacker24Position->getFreeToRecievePos(fromBasketPosition);
//				if ((fromBasketPosition&&toBasketPosition)&&(fromBasketPosition->getContentsEnum()!=BASKET_CONTENTS_LESS_THAN_30)&&(fromBasketPosition->getContentsEnum()!=BASKET_CONTENTS_MORE_THAN_30))
//					crane->move(fromBasketPosition,toBasketPosition);
//			}
//			else//rearange
//			{
//				basketPositionClass* fromBasketPosition=fromPositionList->getFreeToMoveBasketPosition();
//				basketPositionClass* toBasketPosition=getFreePositionToRecieve(fromBasketPosition,false);
//				if (fromBasketPosition&&toBasketPosition)
//					crane->move(fromBasketPosition,toBasketPosition);
//			}
//		}
//#ifdef EMULATE
//		else
//			execQuery(QString("delete FROM toAssimakisExports where basketTableId=%1").arg(basketForAssimakis),MAIN_DB_CONNECTION);
//#endif
//	}
//
//	if (crane->getHasActiveMission())	return;

	// Destackers offline
//	if (basketForOmav > 0)//to omav
//	{
//		positionListClass* fromPositionList=getBasketPositionListFromFullBufferWithBasketId(basketForOmav);
//		if (fromPositionList)
//		{
//			basketPositionClass* fromBasketPosition=fromPositionList->getBaskePositiontWithBasketId(basketForOmav);
//			if (fromBasketPosition==fromPositionList->getFreeToMoveBasketPosition())//the basket is on top of the stack, take it to destacker
//			{
//				basketPositionClass* toBasketPosition=toDeStacker30Position->getFreeToRecievePos(fromBasketPosition);
//				if (fromBasketPosition&&toBasketPosition)
//					crane->move(fromBasketPosition,toBasketPosition);
//			}
//			else//rearange
//			{
//				basketPositionClass* fromBasketPosition = fromPositionList->getFreeToMoveBasketPosition();
//				basketPositionClass* toBasketPosition = getFreePositionToRecieve(fromBasketPosition,false);
//				if (fromBasketPosition && toBasketPosition)
//					crane->move(fromBasketPosition, toBasketPosition);
//			}
//		}
//#ifdef EMULATE
//		else
//			execQuery(QString("delete FROM toOmavExports where basketTableId=%1").arg(basketForOmav),MAIN_DB_CONNECTION);
//#endif
//	}
}

void allPositionsClass::returningEmpty()  // Imports
{
	if (crane->getHasActiveMission())	return;

	if (autoModeEnum & AUTO_RETURNING_EMPTY)
	{
		if (returningConvPositionList04->getPosition(0)->hasBasket() && (!returningConvPositionList04->getTopBasketPosition()->getLocked()) && (!returningConvPositionList04->getTopBasketPosition()->hasContents()))
		{
			basketPositionClass* fromBasketPosition = returningConvPositionList04->getFreeToMoveBasketPosition();
			basketPositionClass* toBasketPosition = NULL;

			if (autoModeEnum & AUTO_BEFORE_STACKER)
			{
				if (fromBasketPosition->isBasketNormal())
					toBasketPosition = beforeStackerPosition->getFreeToRecievePos(fromBasketPosition);
			}

			if (!toBasketPosition)
				if (autoModeEnum & AUTO_BEFORE_STACKER2)
					{
						if (fromBasketPosition->isBasketNormal())
							toBasketPosition = beforeStacker2Position->getFreeToRecievePos(fromBasketPosition);
					}

			if (!toBasketPosition)
			{
				for (int i = 0; i < emptyPositionList.count(); i++)
				{
					toBasketPosition = emptyPositionList[i]->getFreeToRecievePos(fromBasketPosition);
					if (toBasketPosition)
						break;
				}
			}

			if (fromBasketPosition && toBasketPosition)
				crane->move(fromBasketPosition, toBasketPosition);
		}
	}
	return; 

	// Destackers offline
	//if (autoModeEnum & AUTO_RETURNING_EMPTY)
	//{
	//	if (returningFromDestackersPosition->getPosition(0)->hasBasket() && (!returningFromDestackersPosition->getPosition(0)->getLocked()))
	//	{
	//		basketPositionClass* fromBasketPosition=returningFromDestackersPosition->getFreeToMoveBasketPosition();
	//		basketPositionClass* toBasketPosition=NULL;
	//		if (autoModeEnum & AUTO_BEFORE_STACKER)
	//		{
	//			if (fromBasketPosition->isBasketNormal())
	//				toBasketPosition = beforeStackerPosition->getFreeToRecievePos(fromBasketPosition);
	//			else 
	//				toBasketPosition = scrapWorkingPosition->getFreeToRecievePos(fromBasketPosition);
	//		}
	//		if (!toBasketPosition)
	//		{
	//			for (int i=0;i<emptyPositionList.count();i++)
	//			{
	//				toBasketPosition=emptyPositionList[i]->getFreeToRecievePos(fromBasketPosition);
	//				if (toBasketPosition)
	//					break;
	//			}
	//		}
	//		if (fromBasketPosition&&toBasketPosition)
	//			crane->move(fromBasketPosition,toBasketPosition);
	//	}
	//}
}

void allPositionsClass::scrapBasketOut()
{
	//if (scrapBasketIsReady)
	//{
	//	basketPositionClass* fromBasketPosition = scrapWorkingPosition->getFreeToMoveBasketPosition();
	//	//basketPositionClass* toBasketPosition = returningFromDestackersPosition->getFreeToRecievePos(fromBasketPosition);  // Destacker offline
	//	basketPositionClass* toBasketPosition = scrapWorkingPosition->getFreeToMoveBasketPosition();

	//	if (fromBasketPosition && toBasketPosition && fromBasketPosition != toBasketPosition)
	//		crane->move(fromBasketPosition, toBasketPosition);
	//}
	return;
}

void allPositionsClass::bufferToOvenByPress()
{
	// LOG("ap.cpp", "bufferToOvenByPress");
	if (crane->getHasActiveMission()) {
		// LOG("ap.cpp", "crane has active mission, returning");
		return;
	}

	//get basket for oven
	positionListClass* fromPositionList = NULL;
	basketPositionClass* fromBasketPosition = NULL;
	int WhichOven = 0;


	int basketTableId = returningConvPositionList04->getTopBasketPosition()->getBasketTableId();
	// LOG("ap.cpp", QString("basketTableId: %1").arg(basketTableId));

	// LOG("ap.cpp", "executing query to get temper");
	setDbTableActive("basket_details", true);
	QVector<QVariant> tVector = execDBQuery(QString("select temper from basket_details where basketnum=%1 and temper is not Null").arg(basketTableId), APEX_DB_CONNECTION);
	setDbTableIdle("basket_details");
	// LOG("ap.cpp", QString("query executed, tVector count: %1").arg(tVector.count()));

	QString temper = "F";
	if (tVector.count() > 0) {
		// LOG("ap.cpp", "temper: " + temper);
		temper = tVector[0].toString();
	} else {
		// LOG("ap.cpp", "no temper found, using default 'F'");
	}

	return;
}

void allPositionsClass::fromBufferToOven()
{
	if (crane->getHasActiveMission())
		return;

	//get basket for oven
	positionListClass* fromPositionList = NULL;
	basketPositionClass* fromBasketPosition = NULL;
	int WhichOven = 0;

	for (int i = 0; i < bufferPositionList.count(); i++)
	{
		basketPositionClass *anagedBasketPos = bufferPositionList[i]->getPositionWithAnagedBasket();
		if (anagedBasketPos)
		{
			//Changed by Vahid
			WhichOven = AgingOvensDecisionMaking(anagedBasketPos);
			if (WhichOven != 0)
			{
				fromPositionList = bufferPositionList[i];
				fromBasketPosition = anagedBasketPos;
				break;
			}
			/*if (
			((anagedBasketPos->getTemperEnum()==basketClass::BASKET_TEMPER_185)&&(nextOvenLoadingCycleTemperEnum==allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_185))
			||
			((anagedBasketPos->getTemperEnum()==basketClass::BASKET_TEMPER_230)&&(nextOvenLoadingCycleTemperEnum==allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_230))
			)
			{
			fromPositionList=bufferPositionList[i];
			fromBasketPosition=anagedBasketPos;
			break;
			}*/
		}
	}

	if ((!fromPositionList) && (!fromBasketPosition))
	{
		for (int i = 0; i<emptyPositionList.count(); i++)
		{
			basketPositionClass *anagedBasketPos = emptyPositionList[i]->getPositionWithAnagedBasket();
			if (anagedBasketPos)
			{
				//Cahnged by Vahid
				WhichOven = AgingOvensDecisionMaking(anagedBasketPos);
				if (WhichOven != 0)
				{
					fromPositionList = bufferPositionList[i];
					fromBasketPosition = anagedBasketPos;
					break;
				}

				/*if (
				((anagedBasketPos->getTemperEnum()==basketClass::BASKET_TEMPER_185)&&(nextOvenLoadingCycleTemperEnum==allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_185))
				||
				((anagedBasketPos->getTemperEnum()==basketClass::BASKET_TEMPER_230)&&(nextOvenLoadingCycleTemperEnum==allPositionsClass::NEXT_OVEN_CYCLE_TEMPER_230))
				)
				{
				fromPositionList=emptyPositionList[i];
				fromBasketPosition=anagedBasketPos;
				break;
				}*/
			}
		}
	}
	////
	if (fromPositionList && fromBasketPosition)
	{
		if (fromBasketPosition == fromPositionList->getFreeToMoveBasketPosition())
		{
			basketPositionClass* toBasketPosition = NULL;
			if (WhichOven == 1)
				toBasketPosition = getEmptyPositionInTheEntranceOfTheOven(fromBasketPosition);
			else
			{
				if (WhichOven == 2)
					toBasketPosition = getEmptyPositionInTheEntranceOfTheOven2(fromBasketPosition);
			}

			if (fromBasketPosition && toBasketPosition)
				crane->move(fromBasketPosition, toBasketPosition);
		}
		else
		{
			basketPositionClass* fromBasketPosition = fromPositionList->getFreeToMoveBasketPosition();
			basketPositionClass* toBasketPosition = getFreePositionToRecieve(fromBasketPosition, false);
			if (fromBasketPosition && toBasketPosition)
				crane->move(fromBasketPosition, toBasketPosition);
		}
	}
}

void allPositionsClass::toEmptyBuffer()								//when the normal buffer is full then take the 20 feet baskets that come from oven there
{
	if (crane->getHasActiveMission())	return;

	if (getNumberOf30feetEmptyPositionsInFullBuffer() > numberOfPosToBeKeptEmptyInBuffer)		return;

	basketPositionClass* fromOvenBasketPos = returningConvPositionList04->getFreeToMoveBasketPosition();

	if ((!fromOvenBasketPos) || (fromOvenBasketPos->getContentsEnum() != BASKET_CONTENS_LESS_THAN_20)) return;

	//if the basket returning is allready selected to be exported then do not take it to the empty buffer -- destacckers tak3n offline
	//QVector<QVariant> idsInExportTables = execDBQuery(QString("select id from toAssimakisExports where basketTableId=%1 UNION select id from toOmavExports where basketTableId=%1").arg(fromOvenBasketPos->getBasketTableId()), MAIN_DB_CONNECTION);
	//if (idsInExportTables.count() != 0)		return;	//if on an export list leave it there

	basketPositionClass* toPos = NULL;
	basketPositionClass* tempPos = NULL;

	if (tempPos = emptyPositionList10->getFreeToRecievePos(fromOvenBasketPos))			toPos = tempPos;
	else if (tempPos = emptyPositionList09->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList08->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList07->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList06->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList05->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList04->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList03->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList02->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList01->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList15->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList14->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList13->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList12->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;
	else if (tempPos = emptyPositionList11->getFreeToRecievePos(fromOvenBasketPos))		toPos = tempPos;

	if (toPos)
		crane->move(fromOvenBasketPos, toPos);
}

void allPositionsClass::fromEmptyBufferTofullBuffer() //when idle take the 20 feet baskets from empty baskets buffer to normal buffer
{
	if (crane->getHasActiveMission())	return;

	if (getNumberOf30feetEmptyPositionsInFullBuffer() <= numberOfPosToBeKeptEmptyInBuffer)	return;

	basketPositionClass* fromPos = NULL;
	basketPositionClass* tempPos = NULL;

	if ((tempPos = emptyPositionList11->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))			fromPos = tempPos;
	else if ((tempPos = emptyPositionList12->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList13->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList14->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList15->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList01->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList02->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList03->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList04->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList05->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList06->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList07->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList08->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList09->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;
	else if ((tempPos = emptyPositionList10->getFreeToMoveBasketPosition()) && (tempPos->hasContents()))	fromPos = tempPos;

	crane->move(fromPos, getFreePositionToRecieve(fromPos, false));
}

void allPositionsClass::move24FeetFrom30FeetPosTo24FPos()
{
	if (crane->getHasActiveMission())return;

	//find the next export baskets
#ifdef EMULATE
	// int nextBasketForAssimakis=GETONEATMOST(QString("select basketTableId FROM toAssimakisExports order by priority limit 1")).toInt();
	// int nextBasketForOmav=GETONEATMOST(QString("select basketTableId FROM toOmavExports order by priority limit 1")).toInt();
#else
	// int nextBasketForAssimakis=GETONEATMOST(QString("select top 1 basketTableId FROM toAssimakisExports order by priority")).toInt();
	// int nextBasketForOmav=GETONEATMOST(QString("select top 1 basketTableId FROM toOmavExports order by priority")).toInt();
#endif

	//from basket
	basketPositionClass* fromPos=NULL;

	//find a <24feet that is on top
	for (int i = 0; i < buffer30PositionList.count(); i++)
	{
		basketPositionClass* tFromPos = buffer30PositionList[i]->getFreeToMoveBasketPosition();
		if (tFromPos)
		{
			if ((tFromPos->getContentsEnum() == BASKET_CONTENS_LESS_THAN_24) || (tFromPos->getContentsEnum() == BASKET_CONTENS_LESS_THAN_20) || (tFromPos->getContentsEnum() == BASKET_CONTENS_NONE))
			{
				fromPos = tFromPos;
				break;
			}
		}
	}

	//pos to take the basket
	//positionListClass* positionListWithBasketForAssimakis = NULL;
	//positionListClass* positionListWithBasketForOmav = NULL;

	//for (int i = 0; i < buffer24PositionList.count(); i++)
	//{
	//	if ((!positionListWithBasketForAssimakis) && (buffer24PositionList[i]->getBaskePositiontWithBasketId(nextBasketForAssimakis)))
	//		positionListWithBasketForAssimakis = buffer24PositionList[i];

	//	if ((!positionListWithBasketForOmav) && (buffer24PositionList[i]->getBaskePositiontWithBasketId(nextBasketForOmav)))
	//		positionListWithBasketForOmav = buffer24PositionList[i];

	//	if (positionListWithBasketForAssimakis && positionListWithBasketForOmav)
	//		break;
	//}

	basketPositionClass* toPos = NULL;

	if (fromPos)
	{
		QVector<positionListClass*> posList24F = get24FPosListsWithLessBaskets();
		for (int i = 0; i < posList24F.count(); i++)
		{
			//if ((posList24F[i] != positionListWithBasketForAssimakis) && (posList24F[i] != positionListWithBasketForOmav))
			//{
				toPos = posList24F[i]->getFreeToRecievePos(fromPos);
				if (toPos)
					break;
			//}
		}
	}

	if (fromPos && toPos)
		crane->move(fromPos, toPos);
}
#ifdef EMULATE
bool nextExportIsForOmav=false;
#endif

void allPositionsClass::checkOvenUnloadPositions()
{
	// check if oven unload positions have been updated (by something other then me...)
	// return conveyor
	loadPositionFromDatabase(returningConvPositionList01); // 700 east returning conveyor
	loadPositionFromDatabase(returningConvPositionList04); // 160 wast returning conveyor

	// oven 1
	loadPositionFromDatabase(afterOvenPositionList01);
	loadPositionFromDatabase(afterOvenPositionList02);
	loadPositionFromDatabase(afterOvenPositionList03);
	loadPositionFromDatabase(afterOvenPositionList04);

	// oven 2
	loadPositionFromDatabase(newOvenOutPositionList01);
	loadPositionFromDatabase(newOvenOutPositionList02);
	loadPositionFromDatabase(newOvenOutPositionList03);
}

void allPositionsClass::autoModeTimerSlot()
{
	setAutoModeTimerFiring();
	int timeToWaitForBasketToStacker = 1000 * 30;  // 30 seconds
#ifdef EMULATE
	timeToWaitForBasketToStacker=3*1000;
#endif

	if (crane->getHasActiveMission()) { setAutoModeTimerIdle(); return; }

	// Update image from database
	checkOvenUnloadPositions();

	//if (autoModeEnum & AUTO_SCRAP)
	//	scrapBasketOut();

	int beforeStackerTime = basketNeededBeforeStackerTime.elapsed();
	int afterStackerTime = basketAfterStackerReadyTime.elapsed();

	if ((autoModeEnum & AUTO_BEFORE_STACKER) && (beforeStackerTime > timeToWaitForBasketToStacker))	//wait for some time
		beforeStacker();

	if ((autoModeEnum & AUTO_BEFORE_STACKER2) && (beforeStackerTime > timeToWaitForBasketToStacker))
		beforeStacker2();

	if ((autoModeEnum & AUTO_AFTER_STACKER) && (afterStackerTime > timeToWaitForBasketToStacker))	//wait for some time
		afterStacker();

	if ((autoModeEnum & AUTO_AFTER_STACKER2) && (afterStackerTime > timeToWaitForBasketToStacker))	//wait for some time
		afterStacker2();

	if (autoModeEnum & AUTO_PRESSTOOVEN_ENABLE)
	{
		// LOG("ap.cpp", "autoModeTimerSlot - AUTO_PRESSTOOVEN_ENABLE is on, calling  bufferToOvenByPress");
		bufferToOvenByPress();
		// LOG("ap.cpp", "autoModeTimerSlot - Done AUTO_PRESSTOOVEN_ENABLE is on, calling  bufferToOvenByPress");
	}

	if (exportBeforeImports)
	{
		if (autoModeEnum & AUTO_EXPORTS)
			exports();

		if (autoModeEnum & AUTO_RETURNING_EMPTY)
			returningEmpty();
	}
	else
	{
		if (autoModeEnum & AUTO_RETURNING_EMPTY)
			returningEmpty();

		if (autoModeEnum & AUTO_EXPORTS)
			exports();
	}

	if (autoModeEnum & AUTO_UNLOAD_OVEN)
	{
		fromOven();
		if (autoModeEnum & AUTO_USE_EMPTYBASKETS_POS)
			toEmptyBuffer();
		//if (autoModeEnum&AUTO_FROM_BUFFER_TO_OVEN)
		//	fromBufferToOven();
	}

	//if (autoModeEnum & AUTO_FROM_BUFFER_TO_OVEN && !autoModeEnum & AUTO_PRESSTOOVEN_ENABLE)
	if ((autoModeEnum & AUTO_FROM_BUFFER_TO_OVEN) && !(autoModeEnum & AUTO_PRESSTOOVEN_ENABLE))
		fromBufferToOven();

	if (autoModeEnum & AUTO_EMPTY_EMPTYBASKETS_POS)
		fromEmptyBufferTofullBuffer();

	if (autoModeEnum & AUTO_MOVE_24F_FROM30F_TO_24F_POS)
		move24FeetFrom30FeetPosTo24FPos();

	if (!crane->getHasActiveMission())//if nothing else to do feed stacker  even if the time has not passed
	{
		if (autoModeEnum & AUTO_BEFORE_STACKER)
			beforeStacker();

		if (autoModeEnum & AUTO_BEFORE_STACKER2)
			beforeStacker2();

		if (autoModeEnum & AUTO_AFTER_STACKER)
			afterStacker();

		if (autoModeEnum & AUTO_AFTER_STACKER2)
			afterStacker2();
	}

#ifdef EMULATE
	if (crane->getHasActiveMission()) { setAutoModeTimerIdle(); return; }
	//to oven
	int noOfFree24PosBeforeOven=0;
	int noOfFree30PosBeforeOven=0;
	if (beforeOvenPositionList05->getNumberOfBaskets()>0)
		noOfFree30PosBeforeOven+=(4-beforeOvenPositionList05->getNumberOfBaskets());
	else
	{
		noOfFree24PosBeforeOven+=(4-beforeOvenPositionList01->getNumberOfBaskets());
		noOfFree24PosBeforeOven+=(4-beforeOvenPositionList02->getNumberOfBaskets());
		if ((beforeOvenPositionList01->getNumberOfBaskets()==0)&&(beforeOvenPositionList02->getNumberOfBaskets()==0))
			noOfFree30PosBeforeOven+=4;
	}	
	if (beforeOvenPositionList06->getNumberOfBaskets()>0)
	{
		if ((beforeOvenPositionList01->getNumberOfBaskets()<4)&&(beforeOvenPositionList05->getNumberOfBaskets()<4))
			noOfFree30PosBeforeOven+=(4-beforeOvenPositionList06->getNumberOfBaskets());
	}
	else
	{
		if ((beforeOvenPositionList01->getNumberOfBaskets()<4)&&(beforeOvenPositionList05->getNumberOfBaskets()<4))
		{
			noOfFree24PosBeforeOven+=(4-beforeOvenPositionList03->getNumberOfBaskets());
			noOfFree24PosBeforeOven+=(4-beforeOvenPositionList04->getNumberOfBaskets());
			if ((beforeOvenPositionList03->getNumberOfBaskets()==0)&&(beforeOvenPositionList04->getNumberOfBaskets()==0))
				noOfFree30PosBeforeOven+=4;
		}
	}
	if (((noOfFree24PosBeforeOven+noOfFree30PosBeforeOven)==0)||((noOfFree30PosBeforeOven==0)&&(afterStackerPosition->getPosition(0)->getContentsEnum()==BASKET_CONTENS_LESS_THAN_30)))
	{
		setPlcNetworkActive(PLC_NETWORK_CONVEYORS, false);
		conveyorsPlcTcpServer->sendString(QString("basketsForOvenReady|0|0"));
		setPlcNetworkIdle(PLC_NETWORK_CONVEYORS);
		//inOvenTime.restart();
		//hasBasketInOven=true;
	}
	//ask for exports
	QVector<basketPositionClass*> availableBaskets;
	for (int i=buffer24PositionList.count()-1;i>=0;i--)
		availableBaskets<<buffer24PositionList[i]->getBasketPositions();
	for (int i=buffer30PositionList.count()-1;i>=0;i--)
		availableBaskets<<buffer30PositionList[i]->getBasketPositions();
	QVector<QVariant> basketsInQueue;
	setDbTableActive("toAssimakisExports", true);
	basketsInQueue<<execQuery("select basketTableId from toAssimakisExports order by priority",MAIN_DB_CONNECTION);
	setDbTableIdle("toAssimakisExports");
	setDbTableActive("toOmavExports", true);
	basketsInQueue<<execQuery("select basketTableId from toOmavExports order by priority",MAIN_DB_CONNECTION);
	setDbTableIdle("toOmavExports");
	for (int i=0;i<basketsInQueue.count();i++)
	{
		for (int j=0;j<availableBaskets.count();j++)
		{
			if (basketsInQueue[i].toInt()==availableBaskets[j]->getBasketTableId())
			{
				availableBaskets.remove(j);
				break;
			}
		}
	}
	///
	if (availableBaskets.count() > 0)
	{
		int randomIndex = static_cast<double>(availableBaskets.count()) * static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
		if (nextExportIsForOmav)
		{
			basketPositionClass* basket = availableBaskets[randomIndex];
#ifdef EMULATE
			setDbTableActive("toOmavExports", false);
			execQuery(QString("insert into toOmavExports (basketTableId, priority) values(%1,	%2)").arg(basket->getBasketTableId()).arg(GETONEATMOST(QString("select priority FROM toOmavExports order by priority desc limit 1")).toInt() + 1), MAIN_DB_CONNECTION);
			setDbTableIdle("toOmavExports");
#endif
			nextExportIsForOmav = false;
		}
		else
		{
			basketPositionClass* basket = availableBaskets[randomIndex];
			if ((basket->getContentsEnum() == BASKET_CONTENS_LESS_THAN_24) || (basket->getContentsEnum() == BASKET_CONTENS_LESS_THAN_20))
			{
#ifdef EMULATE
				setDbTableActive("toAssimakisExports", false);
				execQuery(QString("insert into toAssimakisExports (basketTableId, priority) values(%1, %2)").arg(basket->getBasketTableId()).arg(GETONEATMOST(QString("select priority FROM toAssimakisExports order by priority desc limit 1")).toInt() + 1), MAIN_DB_CONNECTION);
				setDbTableIdle("toAssimakisExports");
#endif
				nextExportIsForOmav = true;
			}
		}
	}
#endif

	setAutoModeTimerIdle();
}
