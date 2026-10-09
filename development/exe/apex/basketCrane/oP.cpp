#include "op.h"
openGLPartClass::openGLPartClass(QString fileName,GLint triTexture_,GLint quadTexture_):QObject()
{
	isMouseOver=isSelected=false;
	triTexture=triTexture_;
	quadTexture=quadTexture_;
	//
	pickingColor=getColorForPicking();
	mouseOverColor=QColor(0,150,0);
	selectedColor=QColor(0,0,255);
	//
	xPos=yPos=zPos=0.f;
	initialise(fileName);
}
openGLPartClass::openGLPartClass(GLfloat txPos,GLfloat tyPos,GLfloat width,GLfloat height,GLint quadTexture_)
{
	//isMouseOver=isSelected=false;
	//triTexture=0;
	//quadTexture=quadTexture_;
	//xPos=yPos=zPos=0.f;
	////
	//pickingColor=getColorForPicking();
	//mouseOverColor=QColor(0,150,0);
	//selectedColor=QColor(0,0,255);

	//noOfTriangles=0;
	//noOfQuads=1;
	//quadNormalsArray=new GLfloat[12];
	//quadVertexesArray=new GLfloat[12];
	//quadTexCoordArray=new GLfloat[8];
	//quadNormalsArray[0]=0.0f;quadNormalsArray[ 1]=0.0f;quadNormalsArray[ 2]=1.0f;
	//quadNormalsArray[3]=0.0f;quadNormalsArray[ 4]=0.0f;quadNormalsArray[ 5]=1.0f;
	//quadNormalsArray[6]=0.0f;quadNormalsArray[ 7]=0.0f;quadNormalsArray[ 8]=1.0f;
	//quadNormalsArray[9]=0.0f;quadNormalsArray[10]=0.0f;quadNormalsArray[11]=1.0f;
	//quadVertexesArray[ 0]=txPos;
	//quadVertexesArray[ 1]=tyPos;
	//quadVertexesArray[ 2]=0.0f;

	//quadVertexesArray[ 3]=txPos+width;
	//quadVertexesArray[ 4]=tyPos;
	//quadVertexesArray[ 5]=0.0f;

	//quadVertexesArray[ 6]=txPos+width;
	//quadVertexesArray[ 7]=tyPos+height;
	//quadVertexesArray[ 8]=0.0f;

	//quadVertexesArray[ 9]=txPos;
	//quadVertexesArray[10]=tyPos+height;
	//quadVertexesArray[11]=0.0f;

	//quadTexCoordArray[0]=0;quadTexCoordArray[1]=0;
	//quadTexCoordArray[2]=1;quadTexCoordArray[3]=0;
	//quadTexCoordArray[4]=1;quadTexCoordArray[5]=1;
	//quadTexCoordArray[6]=0;quadTexCoordArray[7]=1;
}
openGLPartClass::~openGLPartClass()
{
	if (triNormalsArray) delete triNormalsArray;
	if (triVertexesArray) delete triVertexesArray;
	if (triTexCoordArray) delete triTexCoordArray;
	if (quadNormalsArray) delete quadNormalsArray;
	if (quadVertexesArray) delete quadVertexesArray;
	if (quadTexCoordArray) delete quadTexCoordArray;
}
bool mergTris(QVector<pointClass> t1,QVector<pointClass> t2,quadClass *q,pointClass vector1,pointClass vector2)
{
	QVector<pointClass> res;
	for (int i=0;i<t1.count();i++)
	{
		for (int j=0;j<t2.count();j++)
		{
			if (t1[i]==t2[j])
			{
				 res.append(t1[i]);
				 t1.remove(i);t2.remove(j);i--;j--;
				 break;
			}
		}
	}
	if ((t1.count()==1)&&(t2.count()==1))
	{
		res.append(t1[0]);res.append(t2[0]);
		if (vector1==vector2)
		{
			q->p0=res[0];q->p1=res[2];q->p2=res[1];q->p3=res[3];q->v=vector1;
			return true;
		}
	}
	return false;
}
void openGLPartClass::initialise(QString fileName)
{

	QFile file(fileName);
	if (!file.open (QIODevice::ReadOnly | QIODevice::Text))
		  exit(tr("Error reading file"),tr("The file %1 is inaccessible").arg(fileName));
	QString line;QStringList lineArray;
	file.readLine();//solid
	QVector<triangleClass> triangles;
	while(!file.atEnd())
	{
		triangleClass tri;
		line=file.readLine();
		if (line=="endsolid")
			break;
		lineArray=line.split(' ',QString::SkipEmptyParts);//facet normal
		tri.v.x=lineArray[2].toFloat();
		tri.v.y=lineArray[3].toFloat();
		tri.v.z=lineArray[4].toFloat();
		file.readLine();//outer loop
		line=file.readLine();lineArray=line.split(' ',QString::SkipEmptyParts);//vertex 0
		tri.p0.x=lineArray[1].toFloat();
		tri.p0.y=lineArray[2].toFloat();
		tri.p0.z=lineArray[3].toFloat();
		line=file.readLine();lineArray=line.split(' ',QString::SkipEmptyParts);//vertex 1
		tri.p1.x=lineArray[1].toFloat();
		tri.p1.y=lineArray[2].toFloat();
		tri.p1.z=lineArray[3].toFloat();
		line=file.readLine();lineArray=line.split(' ',QString::SkipEmptyParts);//vertex 2
		tri.p2.x=lineArray[1].toFloat();
		tri.p2.y=lineArray[2].toFloat();
		tri.p2.z=lineArray[3].toFloat();
		file.readLine();//endloop
		file.readLine();//endfacet
		triangles.append(tri);
	}
	file.close();
	//eliminate tris
	QVector<quadClass> quads;
	quadClass q;
	for (int i=0;i<triangles.count();i++)
	{
		for (int j=i;j<triangles.count();j++)
		{
			bool ans=mergTris(triangles[i].getVector(),triangles[j].getVector(),&q,triangles[i].v,triangles[i].v);
			if (ans)
			{
				triangles.remove(j);
				triangles.remove(i);
				i--;j--;
				quads.append(q);
				break;
			}
		}
	}
	//
	//////////////////////triNormalsArray  =(GLfloat *) malloc(sizeof(GLfloat)*triangles.count()*3*3);
	//////////////////////triVertexesArray =(GLfloat *) malloc(sizeof(GLfloat)*triangles.count()*3*3);
	//////////////////////triTexCoordArray =(GLfloat *) malloc(sizeof(GLfloat)*triangles.count()*2*3);
	//////////////////////quadNormalsArray =(GLfloat *) malloc(sizeof(GLfloat)*quads.count()*3*4);
	//////////////////////quadVertexesArray=(GLfloat *) malloc(sizeof(GLfloat)*quads.count()*3*4);
	//////////////////////quadTexCoordArray=(GLfloat *) malloc(sizeof(GLfloat)*quads.count()*2*4);
	triNormalsArray=triVertexesArray=triTexCoordArray=quadNormalsArray=quadVertexesArray=quadTexCoordArray=NULL;
	if(triangles.count()>0)
	{
			triNormalsArray=new GLfloat[triangles.count()*3*3];
			triVertexesArray=new GLfloat[triangles.count()*3*3];
			triTexCoordArray=new GLfloat[triangles.count()*2*3];
	}
	if(quads.count()>0)
	{
		quadNormalsArray =new GLfloat[quads.count()*3*4];
		quadVertexesArray=new GLfloat[quads.count()*3*4];
		quadTexCoordArray=new GLfloat[quads.count()*2*4];
	}
	int v=0;int n=0;int t=0;
	noOfTriangles=triangles.count();
	for (int i=0;i<noOfTriangles;i++)
	{
		triNormalsArray[n++]=triangles[i].v.x;
		triNormalsArray[n++]=triangles[i].v.y;
		triNormalsArray[n++]=triangles[i].v.z;
		triNormalsArray[n++]=triangles[i].v.x;
		triNormalsArray[n++]=triangles[i].v.y;
		triNormalsArray[n++]=triangles[i].v.z;
		triNormalsArray[n++]=triangles[i].v.x;
		triNormalsArray[n++]=triangles[i].v.y;
		triNormalsArray[n++]=triangles[i].v.z;

		triVertexesArray[v++]=triangles[i].p0.x;
		triVertexesArray[v++]=triangles[i].p0.y;
		triVertexesArray[v++]=triangles[i].p0.z;
		triVertexesArray[v++]=triangles[i].p1.x;
		triVertexesArray[v++]=triangles[i].p1.y;
		triVertexesArray[v++]=triangles[i].p1.z;
		triVertexesArray[v++]=triangles[i].p2.x;
		triVertexesArray[v++]=triangles[i].p2.y;
		triVertexesArray[v++]=triangles[i].p2.z;
		triTexCoordArray[t++]=0;triTexCoordArray[t++]=0;
		triTexCoordArray[t++]=1;triTexCoordArray[t++]=0;
		triTexCoordArray[t++]=0;triTexCoordArray[t++]=1;
	}
	v=0;n=0;t=0;
	noOfQuads=quads.count();
	for (int i=0;i<noOfQuads;i++)
	{
		quadNormalsArray[n++]=quads[i].v.x;
		quadNormalsArray[n++]=quads[i].v.y;
		quadNormalsArray[n++]=quads[i].v.z;
		quadNormalsArray[n++]=quads[i].v.x;
		quadNormalsArray[n++]=quads[i].v.y;
		quadNormalsArray[n++]=quads[i].v.z;
		quadNormalsArray[n++]=quads[i].v.x;
		quadNormalsArray[n++]=quads[i].v.y;
		quadNormalsArray[n++]=quads[i].v.z;
		quadNormalsArray[n++]=quads[i].v.x;
		quadNormalsArray[n++]=quads[i].v.y;
		quadNormalsArray[n++]=quads[i].v.z;
		quadVertexesArray[v++]=quads[i].p0.x;
		quadVertexesArray[v++]=quads[i].p0.y;
		quadVertexesArray[v++]=quads[i].p0.z;
		quadVertexesArray[v++]=quads[i].p1.x;
		quadVertexesArray[v++]=quads[i].p1.y;
		quadVertexesArray[v++]=quads[i].p1.z;
		quadVertexesArray[v++]=quads[i].p2.x;
		quadVertexesArray[v++]=quads[i].p2.y;
		quadVertexesArray[v++]=quads[i].p2.z;
		quadVertexesArray[v++]=quads[i].p3.x;
		quadVertexesArray[v++]=quads[i].p3.y;
		quadVertexesArray[v++]=quads[i].p3.z;

		quadTexCoordArray[t++]=0;quadTexCoordArray[t++]=0;
		quadTexCoordArray[t++]=1;quadTexCoordArray[t++]=0;
		quadTexCoordArray[t++]=1;quadTexCoordArray[t++]=1;
		quadTexCoordArray[t++]=0;quadTexCoordArray[t++]=1;
	}
	//noOfTriangles=noOfQuads=5;
}
GLfloat* openGLPartClass::getTriVertexesArray(){return triVertexesArray;}
GLfloat* openGLPartClass::getTriNormalsArray(){return triNormalsArray;}
GLfloat* openGLPartClass::getTriTexCoordArray(){return triTexCoordArray;}
GLfloat* openGLPartClass::getQuadVertexesArray(){return quadVertexesArray;}
GLfloat* openGLPartClass::getQuadNormalsArray(){return quadNormalsArray;}
GLfloat* openGLPartClass::getQuadTexCoordArray(){return quadTexCoordArray;}
int openGLPartClass::getNumberOfTris(){return noOfTriangles;}
int openGLPartClass::getNumberOfQuad(){return noOfQuads;}

void openGLPartClass::draw()
{
	glPushMatrix();
	drawGeometry(false);
	glPopMatrix();
}
void openGLPartClass::drawForPicking()
{
	glPushMatrix();
	glPushAttrib( GL_COLOR_BUFFER_BIT	|GL_CURRENT_BIT	);
	glColor3ub(pickingColor.red(),pickingColor.green(),pickingColor.blue());
	drawGeometry(true);
	glPopAttrib();
	glPopMatrix();
}
void openGLPartClass::draw(GLfloat txPos,GLfloat tyPos,GLfloat tzPos)
{
	xPos=txPos;
	yPos=tyPos;
	zPos=tzPos;
	draw();
}
void openGLPartClass::drawForPicking(GLfloat txPos,GLfloat tyPos,GLfloat tzPos)
{
	xPos=txPos;
	yPos=tyPos;
	zPos=tzPos;
	drawForPicking();
}

void openGLPartClass::drawGeometry(bool drawForPicking)
{
	glTranslatef(xPos,yPos,zPos);
	if (isMouseOver&&(!drawForPicking))
	{
		glPushAttrib( GL_COLOR_BUFFER_BIT	|GL_CURRENT_BIT	);
		glColor3ub(mouseOverColor.red(),mouseOverColor.green(),mouseOverColor.blue());
	}
	else if (isSelected&&(!drawForPicking))
	{
		glPushAttrib( GL_COLOR_BUFFER_BIT	|GL_CURRENT_BIT	);
		glColor3ub(selectedColor.red(),selectedColor.green(),selectedColor.blue());
	}
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if (noOfTriangles>0)
	{
		if (!drawForPicking)
			glBindTexture(GL_TEXTURE_2D,triTexture);
		glVertexPointer(3,GL_FLOAT,0,&triVertexesArray[0]);
		glNormalPointer(GL_FLOAT,0,&triNormalsArray[0]);
		glTexCoordPointer(2,GL_FLOAT,0,&triTexCoordArray[0]);
		QString a=openGlErrorCode();
	}

	if (noOfQuads>0)
	{
		if (!drawForPicking)
			glBindTexture(GL_TEXTURE_2D,quadTexture);
		glVertexPointer(3,GL_FLOAT,0,&quadVertexesArray[0]);
		glNormalPointer(GL_FLOAT,0,&quadNormalsArray[0]);
		glTexCoordPointer(2,GL_FLOAT,0,&quadTexCoordArray[0]);
		glDrawArrays(GL_QUADS,0,noOfQuads*4);
	}

	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);
	if (isMouseOver||isSelected)
		glPopAttrib();
}
bool openGLPartClass::mouseOver(QColor pickedColor)
{
	if (pickedColor==pickingColor)
		isMouseOver=true;
	else
		isMouseOver=false;
	return isMouseOver;
}
bool openGLPartClass::mouseClicked(QColor pickedColor)
{
	if (pickedColor==pickingColor)
	{
		isSelected=true;////////kanei mono to emit, to selected to apofasizei o mpampas tou
		emit mouseClicked();
		return true;
	}
	return false;
}
//
void openGLPartClass::setMouseOverColor(QColor c)
{
	mouseOverColor=c;
}
void openGLPartClass::setSelectedColor(QColor c)
{
	selectedColor=c;
}
void openGLPartClass::setColorForPicking(QColor c)
{
	pickingColor=c;
}

//
void openGLPartClass::setOpenGlXpos(GLfloat pos)
{
	xPos=pos;
	emit openGlChanged();
}
void openGLPartClass::setOpenGlYpos(GLfloat pos)
{
	yPos=pos;
	emit openGlChanged();
}
void openGLPartClass::setOpenGlZpos(GLfloat pos)
{
	zPos=pos;
	emit openGlChanged();
}
float openGLPartClass::getOpenGlXpos()
{
	return xPos;
}
float openGLPartClass::getOpenGlYpos()
{
	return yPos;
}
float openGLPartClass::getOpenGlZpos()
{
	return zPos;
}
