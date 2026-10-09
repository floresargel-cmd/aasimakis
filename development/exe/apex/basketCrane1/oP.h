#ifndef OP_H
#define OP_H

#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "cG.h"

//#include "GL/GLU.h"
class pointClass
{
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;

	 pointClass()
	 {
		 x=y=z=0.;
	 };

	pointClass &operator=(GLfloat v)
	{
		x=v;
		y=v;
		z=v;
		return *this;
	}

	pointClass &operator=(const pointClass& t)
	{
		x=t.x;
		y=t.y;
		z=t.z;
		return *this;
	}

	bool operator==(const pointClass& t)
	{
		if (fabs(x-t.x)>FLT_EPSILON)
			return false;
		if (fabs(y-t.y)>FLT_EPSILON)
			return false;
		if (fabs(z-t.z)>FLT_EPSILON)
			return false;
		return true;
	}
};

class quadClass
{
public:
	pointClass p0;
	pointClass p1;
	pointClass p2;
	pointClass p3;
	pointClass v;
	quadClass()
	{
		p0=p1=p2=v=0;
	};
	quadClass(pointClass p0_,pointClass p1_,pointClass p2_,pointClass p3_,pointClass v_)
	{
		p0 = p0_;
		p1 = p1_;
		p2 = p2_;
		p3 = p3_;
		v = v_;
	};
	quadClass &operator=(const quadClass& t)
	{
		p0 = t.p0;
		p1 = t.p1;
		p2 = t.p2;
		p3 = t.p3;
		v = t.v;
		return *this;
	}
	QVector<pointClass> getVector()
	{
		QVector<pointClass> ans;
		ans.resize(3);
		ans[0] = p0;
		ans[1] = p1;
		ans[2] = p2;
		ans[3] = p3;
		return ans;
	}
};

class triangleClass
{
public:
	pointClass p0;
	pointClass p1;
	pointClass p2;
	pointClass v;

	triangleClass()
	{
		p0=p1=p2;;
	};

	triangleClass(pointClass p0_,pointClass p1_,pointClass p2_,pointClass v_)
	{
		p0 = p0_;
		p1 = p1_;
		p2 = p2_;
		v = v_;
	};

	triangleClass &operator=(const triangleClass& t)
	{
		p0 = t.p0;
		p1 = t.p1;
		p2 = t.p2;
		v = t.v;
		return *this;
	}

	QVector<pointClass> getVector()
	{
		QVector<pointClass> ans;
		ans.resize(3);
		ans[0] = p0;
		ans[1] = p1;
		ans[2] = p2;
		return ans;
	}
};

class openGLPartClass:public QObject
{
	Q_OBJECT

	int										noOfTriangles;
	int										noOfQuads;

	GLfloat									*triNormalsArray;
	GLfloat									*triVertexesArray;
	GLfloat									*triTexCoordArray;
	GLfloat									*quadNormalsArray;
	GLfloat									*quadVertexesArray;
	GLfloat									*quadTexCoordArray;
	GLfloat									xPos;
	GLfloat									yPos;
	GLfloat									zPos;

	bool									isMouseOver;
	bool									isSelected;
	//
	QColor 									pickingColor;
	QColor 									mouseOverColor;
	QColor 									selectedColor;

	GLint									triTexture;
	GLint									quadTexture;

public:
	openGLPartClass(QString fileName,GLint triTexture_,GLint quadTexture_);
	openGLPartClass(GLfloat txPos,GLfloat tyPos,GLfloat width,GLfloat height,GLint quadTexture_);
	~openGLPartClass();
	//
	void draw();
	void drawForPicking();
	void draw(GLfloat txPos,GLfloat tyPos,GLfloat tzPos);
	void drawForPicking(GLfloat txPos,GLfloat tyPos,GLfloat tzPos);
	void drawGeometry(bool drawForPicking);
 	void setOpenGlXpos(GLfloat pos);
 	void setOpenGlYpos(GLfloat pos);
 	void setOpenGlZpos(GLfloat pos);
 	float getOpenGlXpos();
 	float getOpenGlYpos();
 	float getOpenGlZpos();
	//openGlVectors
	GLfloat* getTriVertexesArray();
	GLfloat* getTriNormalsArray();
	GLfloat* getTriTexCoordArray();
	GLfloat* getQuadVertexesArray();
	GLfloat* getQuadNormalsArray();
	GLfloat* getQuadTexCoordArray();
	int getNumberOfTris();
	int getNumberOfQuad();
	//mouse
	bool mouseOver(QColor pickedColor);
	bool mouseClicked(QColor pickedColor);
	void setColorForPicking(QColor pickingColor);
	void setMouseOverColor(QColor c);
	void setSelectedColor(QColor c);
	private:
	void initialise(QString fileName);

signals:
	void openGlChanged();
	void mouseClicked();
};
#endif
