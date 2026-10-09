#ifndef CG_H
#define CG_H
#include <QMessageBox>
#ifdef VB_PLC_COMMS
#include "../common/tcp.h"
#endif
#ifdef TUX_PLC_COMMS
#include "../common/tcpTux.h"
#endif
#include"..\G.h"
#include "../common/apexG.h"
#define EMULATE
#undef EMULATE
extern int redForPicking;
extern int greenForPicking;
extern int blueForPicking;
static QColor getColorForPicking()
{
	if (redForPicking<255)
		return QColor(++redForPicking,greenForPicking,blueForPicking);
	if (greenForPicking<255)
		return QColor(redForPicking,++greenForPicking,blueForPicking);
	if (blueForPicking<255)
		return QColor(redForPicking,greenForPicking,++blueForPicking);
	QMessageBox::critical(0,"error","colorForPicking>255x255x255");
	return QColor(0,0,0);
}

////
#include <QtOpenGL>
static QString openGlErrorCode()
{
	int err=glGetError();
	if (err==GL_NO_ERROR)
		return "GL_NO_ERROR";
	if (err==GL_INVALID_ENUM)
		return "GL_INVALID_ENUM";
	if (err==GL_INVALID_VALUE)
		return "GL_INVALID_VALUE";
	if (err==GL_INVALID_OPERATION)
		return "GL_INVALID_OPERATION";
	if (err==GL_STACK_OVERFLOW)
		return "GL_STACK_OVERFLOW";
	if (err==GL_STACK_UNDERFLOW)
		return "GL_STACK_UNDERFLOW";
	if (err==GL_OUT_OF_MEMORY)
		return "GL_OUT_OF_MEMORY";
	return "GL_NO_ERROR";
}
#endif
