#include "mW.h"
#include "configuration/ConfigurationManager.h"
#include "generated/build_version.h"
#include "../common/applogger.h"
#include <QtPlugin>
#include <QtGlobal>
#include <cstdlib>
#include <exception>

#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
Q_IMPORT_PLUGIN (QWindowsIntegrationPlugin);


#endif

//Q_IMPORT_PLUGIN(qsqlodbc)
static void logTerminate()
{
	applog_write(APPLOG_SEV_ERROR, "UNHANDLED EXCEPTION", "std::terminate was called");
	std::abort();
}

#ifdef _DEBUG
//#include "vld.h"
#endif

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);  //moved here by gene
	QCoreApplication::setOrganizationName("Lorval Capital Management");
	QCoreApplication::setApplicationName("ApexBasketCrane");
	QCoreApplication::setApplicationVersion(APEX_BASKET_CRANE_VERSION);
	applog_initialize("basket-crane", 30);
	applog_installQtMessageHandler();
	std::set_terminate(logTerminate);
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
		applog_write(APPLOG_SEV_ERROR, "CONFIGURATION", configError);
		QMessageBox::critical(0, "Configuration error", configError);
		applog_shutdown();
		return 2;
	}

	app.setStyle(configuration.Application().Style());
	app.setStyleSheet(
		"QWidget {"
		"  color: #e2e8f0;"
		"  font-family: \"Segoe UI\";"
		"  font-size: 9.5pt;"
		"}"
		"mainWindowClass, QDialog {"
		"  background-color: #0b1220;"
		"}"
		"#appHeader {"
		"  background-color: #111827;"
		"  border-bottom: 1px solid #263449;"
		"}"
		"#brandLogo {"
		"  color: #e11145;"
		"  font-size: 16pt;"
		"  font-weight: 400;"
		"  background: transparent;"
		"}"
		"#applicationTitle {"
		"  color: #f8fafc;"
		"  font-size: 12pt;"
		"  font-weight: 600;"
		"}"
		"#applicationSubtitle, #hostIdentity {"
		"  color: #94a3b8;"
		"  font-size: 8.5pt;"
		"}"
		"#systemMode {"
		"  min-height: 22px;"
		"  padding: 2px 10px;"
		"  border-radius: 11px;"
		"  font-size: 8.5pt;"
		"  font-weight: 700;"
		"}"
		"#systemMode[mode=\"live\"] {"
		"  color: #bbf7d0; background: #14532d; border: 1px solid #22c55e;"
		"}"
		"#systemMode[mode=\"test\"] {"
		"  color: #fde68a; background: #422006; border: 1px solid #d97706;"
		"}"
		"#systemMode[mode=\"observer\"] {"
		"  color: #dbeafe; background: #1e3a8a; border: 1px solid #60a5fa;"
		"}"
		"QGroupBox {"
		"  background-color: rgba(17, 24, 39, 232);"
		"  border: 1px solid #2d3b50;"
		"  border-radius: 8px;"
		"  margin-top: 14px;"
		"  padding: 10px 8px 8px 8px;"
		"  font-weight: 600;"
		"  color: #f8fafc;"
		"}"
		"QGroupBox::title {"
		"  subcontrol-origin: margin;"
		"  left: 10px;"
		"  padding: 0 5px;"
		"  color: #7dd3fc;"
		"  font-size: 9pt;"
		"}"
		"QCheckBox, QRadioButton {"
		"  spacing: 7px;"
		"  min-height: 20px;"
		"  padding-bottom: 1px;"
		"  color: #e2e8f0;"
		"  background: transparent;"
		"}"
		"QCheckBox:hover, QRadioButton:hover { color: #ffffff; }"
		"QCheckBox:disabled, QRadioButton:disabled { color: #64748b; }"
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
		"QPushButton[warning=\"true\"] {"
		"  color: #fef3c7; background-color: #78350f; border-color: #b45309;"
		"}"
		"QPushButton[warning=\"true\"]:hover {"
		"  background-color: #92400e; border-color: #f59e0b;"
		"}"
		"QPushButton:disabled {"
		"  color: #64748b; background-color: #1e293b; border-color: #334155;"
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

	applog_write(APPLOG_SEV_INFO, "APPLICATION",
		"Qt library paths=" + QCoreApplication::libraryPaths().join(";"));
	
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

	int result = 0;
	{
		mainWindowClass mainWindow;
		mainWindow.resize(
			configuration.Application().WindowWidth(),
			configuration.Application().WindowHeight());
		if (configuration.Application().StartMaximized())
			mainWindow.showMaximized();
		else
			mainWindow.show();

		try {
			result = app.exec();
		}
		catch (const std::exception& error) {
			applog_write(APPLOG_SEV_ERROR, "UNHANDLED EXCEPTION", QString::fromLocal8Bit(error.what()));
			result = 1;
		}
		catch (...) {
			applog_write(APPLOG_SEV_ERROR, "UNHANDLED EXCEPTION", "Unknown exception escaped the event loop");
			result = 1;
		}
	}
	applog_shutdown();
	return result;
}
