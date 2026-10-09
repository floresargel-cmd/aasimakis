#include "he.h"
helpClass::helpClass(QWidget *p):QDialog(p)
{
	setWindowTitle(tr("Help-production plan sfotware"));
	resize(1300,800);
	QVBoxLayout *mainLayout=new QVBoxLayout();setLayout(mainLayout);
	QTextBrowser *txtBrowser=new QTextBrowser(this);mainLayout->addWidget(txtBrowser);
	//txtBrowser->setHtml("http://www.fyfeeurope.com/index.html");
	//txtBrowser->setSearchPaths(QStringList()<<":/");
  //txtBrowser->setSource(QUrl(":/test.htm"));
	//QUrl *u=new QUrl("R://development//2012//exe//qt.5.0.1.soft//apex//productionPlan//resources//test.htm");
	//txtBrowser->setSource(*u);
	//txtBrowser->setSearchPaths(QStringList()<<QDir::currentPath());
	//txtBrowser->setSource(QUrl(QString("file:///")+QDir::currentPath()+"/test.html"));
	txtBrowser->setSearchPaths(QStringList()<<"qrc:/help");
	txtBrowser->setSource(QUrl("qrc:/help/index.html"));
}
helpClass::~helpClass()
{
}
