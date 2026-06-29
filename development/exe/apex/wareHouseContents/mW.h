#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QtGui>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPrinter>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVector>
#include <QAction>

#include "../common/apexG.h"
#include "gC.h"
class mainWindowClass : public QWidget				
{												
	QTimer								mainTimer;
	groupOfCellsClass			*eastGroup0;
	groupOfCellsClass			*eastGroup1;
	groupOfCellsClass			*eastGroup2;
	groupOfCellsClass			*westGroup0;
	groupOfCellsClass			*westGroup1;
	groupOfCellsClass			*westGroup2;
Q_OBJECT								
public:										
	mainWindowClass();							
private:									
	void createLetter();					
	QAction *printAction;					
	QTabWidget *letters;					
public slots:								
	void onMainTimer();						
	void printEastBtnSlot();						
	void printWestBtnSlot();						
	void excellBtnSlot();						
	void qtAboutSlot();
};											
#endif
