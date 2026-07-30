#include "mW.h"
#include "configuration/ConfigurationManager.h"
#include <QtPlugin>
#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);


#endif

//Q_IMPORT_PLUGIN(qsqlodbc)
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void messageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	if (msg.left(6)=="libpng")
		return;
	QMessageBox::critical(0,"Fatal",msg);
	return;
}
#else
void messageOutPut(QtMsgType type, const char *msg)
{
	QMessageBox::critical(0,"Fatal",msg);
}
#endif

#ifdef _DEBUG
//#include "vld.h"
#endif

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);  //moved here by gene
	using BasketCrane::Configuration::ConfigurationManager;
	ConfigurationManager& configuration = ConfigurationManager::Instance();

	QString configFileName = QCoreApplication::applicationDirPath() + "/basketCrane.ini";
	for (int i = 1; i < argc; ++i)
	{
		QString argument = QString::fromLocal8Bit(argv[i]);
		if (argument == "--config" && i + 1 < argc)
			configFileName = QString::fromLocal8Bit(argv[++i]);
		else if (argument.startsWith("--config="))
			configFileName = argument.mid(QString("--config=").length());
	}

	QString configError;
	if (!configuration.Load(configFileName, &configError))
	{
		QMessageBox::critical(0, "Configuration error", configError);
		return 2;
	}

	app.setStyle(configuration.Application().Style());
	app.setStyleSheet(
		"QWidget {"
		"  color: #e2e8f0;"
		"  font-family: \"Segoe UI\";"
		"  font-size: 10pt;"
		"}"
		"mainWindowClass, QDialog {"
		"  background-color: #0f172a;"
		"}"
		"QGroupBox {"
		"  background-color: rgba(15, 23, 42, 210);"
		"  border: 1px solid #334155;"
		"  border-radius: 8px;"
		"  margin-top: 12px;"
		"  padding: 10px 8px 8px 8px;"
		"  font-weight: 600;"
		"  color: #f8fafc;"
		"}"
		"QGroupBox::title {"
		"  subcontrol-origin: margin;"
		"  left: 10px;"
		"  padding: 0 5px;"
		"  color: #93c5fd;"
		"}"
		"QCheckBox, QRadioButton {"
		"  spacing: 7px;"
		"  min-height: 20px;"
		"  padding-bottom: 1px;"
		"  color: #e2e8f0;"
		"  background: transparent;"
		"}"
		"QCheckBox::indicator, QRadioButton::indicator {"
		"  width: 16px;"
		"  height: 16px;"
		"}"
		"QCheckBox::indicator {"
		"  border: 1px solid #64748b;"
		"  border-radius: 4px;"
		"  background: #1e293b;"
		"}"
		"QCheckBox::indicator:checked {"
		"  background: #0ea5e9;"
		"  border-color: #38bdf8;"
		"}"
		"QRadioButton::indicator {"
		"  border: 1px solid #64748b;"
		"  border-radius: 8px;"
		"  background: #1e293b;"
		"}"
		"QRadioButton::indicator:checked {"
		"  background: #0ea5e9;"
		"  border: 4px solid #dbeafe;"
		"}"
		"QPushButton {"
		"  min-height: 26px;"
		"  padding: 3px 12px;"
		"  color: #f8fafc;"
		"  background-color: #334155;"
		"  border: 1px solid #475569;"
		"  border-radius: 6px;"
		"  font-weight: 600;"
		"}"
		"QPushButton:hover {"
		"  background-color: #475569;"
		"  border-color: #64748b;"
		"}"
		"QPushButton:pressed {"
		"  background-color: #1e293b;"
		"}"
		"QPushButton[accent=\"true\"] {"
		"  background-color: #0284c7;"
		"  border-color: #38bdf8;"
		"}"
		"QPushButton[accent=\"true\"]:hover {"
		"  background-color: #0369a1;"
		"}"
		"QLineEdit, QSpinBox, QComboBox {"
		"  min-height: 26px;"
		"  padding: 2px 8px;"
		"  color: #f8fafc;"
		"  background-color: #1e293b;"
		"  border: 1px solid #475569;"
		"  border-radius: 5px;"
		"  selection-background-color: #0284c7;"
		"}"
		"QLineEdit:focus, QSpinBox:focus, QComboBox:focus {"
		"  border-color: #38bdf8;"
		"}"
		"QSpinBox {"
		"  padding-right: 20px;"
		"}"
		"QSpinBox::up-button, QSpinBox::down-button {"
		"  subcontrol-origin: border;"
		"  width: 18px;"
		"  background-color: #334155;"
		"  border-left: 1px solid #475569;"
		"}"
		"QSpinBox::up-button {"
		"  subcontrol-position: top right;"
		"  border-top-right-radius: 4px;"
		"}"
		"QSpinBox::down-button {"
		"  subcontrol-position: bottom right;"
		"  border-bottom-right-radius: 4px;"
		"}"
		"QTableView {"
		"  color: #e2e8f0;"
		"  background-color: #111827;"
		"  alternate-background-color: #1e293b;"
		"  gridline-color: #334155;"
		"  border: 1px solid #334155;"
		"  border-radius: 5px;"
		"  selection-background-color: #075985;"
		"}"
		"QHeaderView::section {"
		"  color: #cbd5e1;"
		"  background-color: #1e293b;"
		"  border: 0;"
		"  border-right: 1px solid #334155;"
		"  border-bottom: 1px solid #334155;"
		"  padding: 6px;"
		"  font-weight: 600;"
		"}"
		"QScrollBar:vertical {"
		"  width: 12px;"
		"  background: #0f172a;"
		"}"
		"QScrollBar::handle:vertical {"
		"  min-height: 24px;"
		"  background: #475569;"
		"  border-radius: 6px;"
		"}"
		"QToolTip {"
		"  color: #f8fafc;"
		"  background-color: #1e293b;"
		"  border: 1px solid #64748b;"
		"  padding: 5px;"
		"}"
	);
	const QStringList libraryPaths = configuration.Paths().LibraryPaths();
	for (int i = 0; i < libraryPaths.count(); ++i)
		qApp->addLibraryPath(libraryPaths[i]);
	// for( int x=0; x < QSqlDatabase::drivers().count(); x++)
	// 	LOG("SQL Drivers", "SQL driver:" + QSqlDatabase::drivers()[x]);
	// LOG("Qt plugins path", QLibraryInfo::location(QLibraryInfo::PluginsPath));

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	qDebug() << QCoreApplication::libraryPaths();
	#ifdef _DEBUG
		qInstallMessageHandler(messageOutPut);
	#endif
	//for (int x = 0; x < QCoreApplication::libraryPaths().count(); x++)
	// 	LOG("Library Path(s)", "library path:" + QCoreApplication::libraryPaths()[x]);

#else
	#ifdef _DEBUG
		qInstallMsgHandler(messageOutPut);
	#endif
	for (int x = 0; x < QCoreApplication::libraryPaths().count(); x++)
		LOG("Library Path(s)", "library path:" + QCoreApplication::libraryPaths()[x]);

#endif
	
	// Library/plugin paths are configured in basketCrane.ini.
	// LOG("Plugin Path(s)", localPlugins);
	// LOG("QLibraryInfo::PluginsPath:", QLibraryInfo::location(QLibraryInfo::PluginsPath));
	// for( int x=0; x < QCoreApplication::libraryPaths().count(); x++)
	//	LOG("SQL Drivers", "SQL driver:" + QCoreApplication::libraryPaths()[x]);
	// for (int x = 0; x < QSqlDatabase::drivers().count(); x++)
	//	LOG("Available SQL driver(s)", QSqlDatabase::drivers()[x]);

	// coment out by gene  QApplication app(argc, argv);
	QFileInfo pathInfo(argv[0]);

	if (configuration.Application().SingleInstance() &&
		(isExeRuningClass::isRunning(pathInfo.fileName())))
		uExit("Error",QString("The application %1 is already running.").arg(pathInfo.fileName()));

	mainWindowClass mainWindow;
	mainWindow.resize(
		configuration.Application().WindowWidth(),
		configuration.Application().WindowHeight());
	if (configuration.Application().StartMaximized())
		mainWindow.showMaximized();
	else
		mainWindow.show();
	
	return app.exec();
}
