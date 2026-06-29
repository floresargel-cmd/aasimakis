#ifndef PPG_H
#define PPG_H

#include "../common/apexG.h"

#define BASKET_CRANE_DB_CONNECTION QString("basketCraneConnection")
#define DXFCOLOR(color) QColor(dxfColors[color][0]*255.,dxfColors[color][1]*255.,dxfColors[color][2]*255.)

#define LOCAL_DB
#undef LOCAL_DB
#define STACKER_TABLE_WRITE "XIN"
#define STACKER_TABLE_READ "XOUT"
//
#define MAX_HEIGHT_OF_LAYERS_IN_A_BASKET 750.
#define MAX_WEIGHT_OF_BASKET 1750.//net weight
#define MAX_LAYER_WIDTH 1200
#define FIRST_LAYER_HEIGTH 117.48
#define SPACER_WIDTH 1350.
#define SPACER_THIKNESS 19.2
#define SPACER_WEIGHT 2.6

#define PROFILE_SINGLE_LENGTH 1
#define PROFILE_DOUBLE_LENGTH 2

#define MAX_COMBS 13
#define COMB_LAYER_HEIGTH_01 117.5
#define COMB_LAYER_HEIGTH_02 167.5
#define COMB_LAYER_HEIGTH_03 217.5
#define COMB_LAYER_HEIGTH_04 267.5
#define COMB_LAYER_HEIGTH_05 317.5
#define COMB_LAYER_HEIGTH_06 367.5
#define COMB_LAYER_HEIGTH_07 417.5
#define COMB_LAYER_HEIGTH_08 467.5
#define COMB_LAYER_HEIGTH_09 517.5
#define COMB_LAYER_HEIGTH_10 567.5
#define COMB_LAYER_HEIGTH_11 617.5
#define COMB_LAYER_HEIGTH_12 667.5
static int getCombWithHeightMoreThan(double h)
{
	     if (COMB_LAYER_HEIGTH_01>h)return  1;
	else if (COMB_LAYER_HEIGTH_02>h)return  2;
	else if (COMB_LAYER_HEIGTH_03>h)return  3;
	else if (COMB_LAYER_HEIGTH_04>h)return  4;
	else if (COMB_LAYER_HEIGTH_05>h)return  5;
	else if (COMB_LAYER_HEIGTH_06>h)return  6;
	else if (COMB_LAYER_HEIGTH_07>h)return  7;
	else if (COMB_LAYER_HEIGTH_08>h)return  8;
	else if (COMB_LAYER_HEIGTH_09>h)return  9;
	else if (COMB_LAYER_HEIGTH_10>h)return 10;
	else if (COMB_LAYER_HEIGTH_11>h)return 11;
	else if (COMB_LAYER_HEIGTH_12>h)return 12;
	return 13;
}
static double getHeightOfComb(int c)
{
	     if (c== 1)return COMB_LAYER_HEIGTH_01;
	else if (c== 2)return COMB_LAYER_HEIGTH_02;
	else if (c== 3)return COMB_LAYER_HEIGTH_03;
	else if (c== 4)return COMB_LAYER_HEIGTH_04;
	else if (c== 5)return COMB_LAYER_HEIGTH_05;
	else if (c== 6)return COMB_LAYER_HEIGTH_06;
	else if (c== 7)return COMB_LAYER_HEIGTH_07;
	else if (c== 8)return COMB_LAYER_HEIGTH_08;
	else if (c== 9)return COMB_LAYER_HEIGTH_09;
	else if (c==10)return COMB_LAYER_HEIGTH_10;
	else if (c==11)return COMB_LAYER_HEIGTH_11;
	else if (c==12)return COMB_LAYER_HEIGTH_12;
	return COMB_LAYER_HEIGTH_12+1000.;
}
static int globalColorIndex=-1;
static QColor getGlobalColor()
{
  globalColorIndex++;
       if(globalColorIndex== 0) return QColor(29,139,209);
	else if(globalColorIndex== 1) return QColor(143,188,139);
	else if(globalColorIndex== 2) return QColor(210,180,140);
	else if(globalColorIndex== 3) return QColor(32,178,170);
	else if(globalColorIndex== 4) return QColor(176,196,222);
	else if(globalColorIndex== 5) return QColor(221,160,221);
	else if(globalColorIndex== 6) return QColor(99,0,99);
	else if(globalColorIndex== 7) return QColor(255,130,132);
	else if(globalColorIndex== 8) return QColor(0,101,206);
	else if(globalColorIndex== 9) return QColor(206,207,255);
	else if(globalColorIndex==10) return QColor(0,0,132);
	else if(globalColorIndex==11) return QColor(255,0,255);
	else if(globalColorIndex==12) return QColor(255,255,0);
	else if(globalColorIndex==13) return QColor(0,255,255);
	else if(globalColorIndex==14) return QColor(132,0,132);
	else if(globalColorIndex==15) return QColor(132,0,0);
	else if(globalColorIndex==16) return QColor(0,0,255);
	else if(globalColorIndex==17) return QColor(0,207,255);
	else if(globalColorIndex==18) return QColor(206,255,255);
	else if(globalColorIndex==19) return QColor(156,207,255);
	else if(globalColorIndex==20) return QColor(255,154,206);
	else if(globalColorIndex==21) return QColor(206,154,255);
	else if(globalColorIndex==22) return QColor(255,207,156);
	else if(globalColorIndex==23) return QColor(49,101,255);
	else if(globalColorIndex==24) return QColor(49,207,206);
	else if(globalColorIndex==25) return QColor(255,207,0);
	else if(globalColorIndex==26) return QColor(255,101,0);
	else {globalColorIndex=-1;return QColor(0,0,0);}
}
//
#define MAX(a,b) ((a>b)?a:b)
#define MIN(a,b) ((a<b)?a:b)
static double getMinXDistanceOfPaths(QPainterPath p1,QPainterPath p2)
{
	double stepDist=0.;
	double DX=1.;
	double b1=p1.boundingRect().width();
	double b2=p2.boundingRect().width();
	double MAXDIST=MAX(p1.boundingRect().width(),p2.boundingRect().width());
	double STEP=0.5*MIN(p1.boundingRect().width(),p2.boundingRect().width());
	//STEP=1.0;
	bool found=false;
	for (stepDist=-1.0;stepDist<(MAXDIST+STEP+2.);stepDist+=STEP)
	{
		if (p1.intersects(p2.translated(-stepDist,0.0)))
			break;
		found=true;
	}
	if (!found)
		return 0.0;
	double maxDist=stepDist;
	double minDist=stepDist-STEP;
	double dist=0.;
	double validDist=0.;
	double oldValidDist=0.;
	for (int i=0;i<50;i++)
	{
		dist=0.5*(maxDist+minDist);
		if (p1.intersects(p2.translated(-dist,0.0)))
			maxDist=dist;
		else
		{
			minDist=dist;
			validDist=dist;
			if ((i!=0)&&(fabs(validDist-oldValidDist)<0.1))
				break;
			oldValidDist=validDist;
		}
	}
	return validDist;
}
#endif

