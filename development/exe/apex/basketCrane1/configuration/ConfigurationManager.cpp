#include "ConfigurationManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QProcessEnvironment>
#include <QRegExp>
#include <QSettings>

namespace BasketCrane
{
namespace Configuration
{

ConfigurationManager& ConfigurationManager::Instance()
{
	static ConfigurationManager instance;
	return instance;
}

ConfigurationManager::ConfigurationManager()
	: m_isLoaded(false)
{
}

bool ConfigurationManager::Load(const QString& fileName, QString *errorMessage)
{
	QFileInfo fileInfo(fileName);
	if (!fileInfo.exists() || !fileInfo.isFile())
	{
		if (errorMessage)
			*errorMessage = QString("Configuration file not found:\n%1").arg(fileInfo.absoluteFilePath());
		return false;
	}

	m_fileName = fileInfo.absoluteFilePath();
	QSettings settings(m_fileName, QSettings::IniFormat);
	if (settings.status() != QSettings::NoError)
	{
		if (errorMessage)
			*errorMessage = QString("Configuration file could not be read:\n%1").arg(m_fileName);
		m_fileName.clear();
		return false;
	}

	if (!Validate(settings, errorMessage))
	{
		m_fileName.clear();
		return false;
	}

	m_application.m_singleInstance = settings.value("Application/singleInstance").toBool();
	m_application.m_style = ReadString(settings, "Application/style");
	m_application.m_windowWidth = settings.value("Application/windowWidth").toInt();
	m_application.m_windowHeight = settings.value("Application/windowHeight").toInt();
	m_application.m_startMaximized = settings.value("Application/startMaximized").toBool();
	m_application.m_environmentMode = ReadString(settings, "Environment/mode").trimmed();
	m_application.m_liveEnabledHosts = ReadString(settings, "Environment/liveEnabledHosts")
		.split('|', QString::SkipEmptyParts);
	for (int i = 0; i < m_application.m_liveEnabledHosts.count(); ++i)
		m_application.m_liveEnabledHosts[i] = m_application.m_liveEnabledHosts[i].trimmed();

	m_paths.m_libraryPaths = ReadString(settings, "Paths/libraryPaths")
		.split('|', QString::SkipEmptyParts);
	for (int i = 0; i < m_paths.m_libraryPaths.count(); ++i)
		m_paths.m_libraryPaths[i] = QDir::cleanPath(m_paths.m_libraryPaths[i].trimmed());

	m_database.m_driver = ReadString(settings, "Database/driver");
	m_database.m_mainLive = ReadDatabaseProfile(settings, ReadString(settings, "Database/mainLiveProfile"));
	m_database.m_mainTest = ReadDatabaseProfile(settings, ReadString(settings, "Database/mainTestProfile"));
	m_database.m_apexLive = ReadDatabaseProfile(settings, ReadString(settings, "Database/apexLiveProfile"));
	m_database.m_apexTest = ReadDatabaseProfile(settings, ReadString(settings, "Database/apexTestProfile"));
	m_database.m_mainCatalog = ReadString(settings, "Database/mainCatalog");
	m_database.m_apexCatalog = ReadString(settings, "Database/apexCatalog");
	m_database.m_createCatalogs = settings.value("Database/createCatalogs").toBool();

	m_plc.m_craneIp = ReadString(settings, "PLC/craneIp");
	m_plc.m_conveyorsIp = ReadString(settings, "PLC/conveyorsIp");
	m_plc.m_newOvenIp = ReadString(settings, "PLC/newOvenIp");
	m_plc.m_craneDescription = ReadString(settings, "PLC/craneDescription");
	m_plc.m_conveyorsDescription = ReadString(settings, "PLC/conveyorsDescription");
	m_plc.m_newOvenDescription = ReadString(settings, "PLC/newOvenDescription");
	m_plc.m_craneReadPort = settings.value("PLC/craneReadPort").toInt();
	m_plc.m_craneWritePort = settings.value("PLC/craneWritePort").toInt();
	m_plc.m_conveyorsReadPort = settings.value("PLC/conveyorsReadPort").toInt();
	m_plc.m_conveyorsWritePort = settings.value("PLC/conveyorsWritePort").toInt();
	m_plc.m_enabledPollingMilliseconds = settings.value("PLC/enabledPollingMs").toInt();
	m_plc.m_standbyPollingMilliseconds = settings.value("PLC/standbyPollingMs").toInt();
	m_plc.m_craneHmiExecutable = ReadString(settings, "PLC/craneHmiExecutable");
	m_plc.m_conveyorsHmiExecutable = ReadString(settings, "PLC/conveyorsHmiExecutable");

	m_timers.m_mainWindowMilliseconds = settings.value("Timers/mainWindowMs").toInt();
	m_timers.m_positionsMilliseconds = settings.value("Timers/positionsMs").toInt();
	m_timers.m_autoModeMilliseconds = settings.value("Timers/autoModeMs").toInt();
	m_timers.m_craneAnimationMilliseconds = settings.value("Timers/craneAnimationMs").toInt();
	m_timers.m_statusFadeMilliseconds = settings.value("Timers/statusFadeMs").toInt();
	m_timers.m_emulationMainMilliseconds = settings.value("Timers/emulationMainMs").toInt();
	m_timers.m_emulationCraneMainMilliseconds = settings.value("Timers/emulationCraneMainMs").toInt();
	m_timers.m_emulationMovingZStepMilliseconds = settings.value("Timers/emulationMovingZStepMs").toInt();
	m_timers.m_emulationStepMilliseconds = settings.value("Timers/emulationStepMs").toInt();
	m_timers.m_emulationAnimationMilliseconds = settings.value("Timers/emulationAnimationMs").toInt();
	m_timers.m_emulationConveyorMainMilliseconds = settings.value("Timers/emulationConveyorMainMs").toInt();
	m_timers.m_emulationConveyorPropagationMilliseconds = settings.value("Timers/emulationConveyorPropagationMs").toInt();

	m_operations.m_emptyBufferMinimum = settings.value("Operations/emptyBufferMinimum").toInt();
	m_operations.m_emptyBufferMaximum = settings.value("Operations/emptyBufferMaximum").toInt();
	m_operations.m_emptyBufferDefault = settings.value("Operations/emptyBufferDefault").toInt();
	m_operations.m_oven1Enabled = settings.value("Operations/oven1Enabled").toBool();
	m_operations.m_oven2Enabled = settings.value("Operations/oven2Enabled").toBool();
	m_operations.m_oven1Maximum24Feet = settings.value("Operations/oven1Maximum24Feet").toBool();
	m_operations.m_oven2OnlyOver24Feet = settings.value("Operations/oven2OnlyOver24Feet").toBool();
	m_operations.m_exportsBeforeImports = settings.value("Operations/exportsBeforeImports").toBool();

	m_visualization.m_showBasketNumbers = settings.value("Visualization/showBasketNumbers").toBool();
	m_visualization.m_showLocationLabels = settings.value("Visualization/showLocationLabels").toBool();
	m_visualization.m_basketLabelFontPointSize = settings.value("Visualization/basketLabelFontPointSize").toInt();
	m_visualization.m_locationLabelFontPointSize = settings.value("Visualization/locationLabelFontPointSize").toInt();

	m_emulation.m_forceDemoFillTables = settings.value("Emulation/forceDemoFillTables").toBool();
	m_emulation.m_ovenStayMinimumSeconds = settings.value("Emulation/ovenStayMinimumSeconds").toInt();
	m_emulation.m_ovenStayMaximumSeconds = settings.value("Emulation/ovenStayMaximumSeconds").toInt();
	m_emulation.m_ovenStayDefaultSeconds = settings.value("Emulation/ovenStayDefaultSeconds").toInt();

	m_security.m_initialAdminUser = ReadString(settings, "Security/initialAdminUser");
	m_security.m_initialAdminPassword = ReadString(settings, "Security/initialAdminPassword");

	m_isLoaded = true;
	return true;
}

bool ConfigurationManager::Validate(QSettings& settings, QString *errorMessage) const
{
	QStringList requiredKeys;
	requiredKeys
		<< "Application/singleInstance" << "Application/style"
		<< "Application/windowWidth" << "Application/windowHeight" << "Application/startMaximized"
		<< "Environment/mode" << "Environment/liveEnabledHosts"
		<< "Paths/libraryPaths"
		<< "Database/driver" << "Database/mainLiveProfile" << "Database/mainTestProfile"
		<< "Database/apexLiveProfile" << "Database/apexTestProfile"
		<< "Database/mainCatalog" << "Database/apexCatalog" << "Database/createCatalogs"
		<< "PLC/craneIp" << "PLC/conveyorsIp" << "PLC/newOvenIp"
		<< "PLC/craneDescription" << "PLC/conveyorsDescription" << "PLC/newOvenDescription"
		<< "PLC/craneReadPort" << "PLC/craneWritePort"
		<< "PLC/conveyorsReadPort" << "PLC/conveyorsWritePort"
		<< "PLC/enabledPollingMs" << "PLC/standbyPollingMs"
		<< "PLC/craneHmiExecutable" << "PLC/conveyorsHmiExecutable"
		<< "Timers/mainWindowMs" << "Timers/positionsMs" << "Timers/autoModeMs"
		<< "Timers/craneAnimationMs" << "Timers/statusFadeMs"
		<< "Timers/emulationMainMs" << "Timers/emulationCraneMainMs"
		<< "Timers/emulationMovingZStepMs" << "Timers/emulationStepMs"
		<< "Timers/emulationAnimationMs" << "Timers/emulationConveyorMainMs"
		<< "Timers/emulationConveyorPropagationMs"
		<< "Operations/emptyBufferMinimum" << "Operations/emptyBufferMaximum"
		<< "Operations/emptyBufferDefault" << "Operations/oven1Enabled"
		<< "Operations/oven2Enabled" << "Operations/oven1Maximum24Feet"
		<< "Operations/oven2OnlyOver24Feet" << "Operations/exportsBeforeImports"
		<< "Visualization/showBasketNumbers" << "Visualization/showLocationLabels"
		<< "Visualization/basketLabelFontPointSize" << "Visualization/locationLabelFontPointSize"
		<< "Emulation/forceDemoFillTables" << "Emulation/ovenStayMinimumSeconds"
		<< "Emulation/ovenStayMaximumSeconds" << "Emulation/ovenStayDefaultSeconds"
		<< "Security/initialAdminUser" << "Security/initialAdminPassword";

	const QStringList profileSections = QStringList()
		<< settings.value("Database/mainLiveProfile").toString()
		<< settings.value("Database/mainTestProfile").toString()
		<< settings.value("Database/apexLiveProfile").toString()
		<< settings.value("Database/apexTestProfile").toString();
	for (int i = 0; i < profileSections.count(); ++i)
	{
		requiredKeys
			<< profileSections[i] + "/dsn"
			<< profileSections[i] + "/user"
			<< profileSections[i] + "/password";
	}

	QStringList missingKeys;
	for (int i = 0; i < requiredKeys.count(); ++i)
	{
		if (!settings.contains(requiredKeys[i]))
			missingKeys << requiredKeys[i];
	}

	if (!missingKeys.isEmpty())
	{
		if (errorMessage)
			*errorMessage = QString("Configuration file is missing required values:\n%1")
				.arg(missingKeys.join("\n"));
		return false;
	}

	QStringList invalidValues;
	const QString environmentMode = settings.value("Environment/mode").toString().trimmed();
	if (environmentMode.compare("Test", Qt::CaseInsensitive) != 0
		&& environmentMode.compare("Live", Qt::CaseInsensitive) != 0
		&& environmentMode.compare("LiveObserver", Qt::CaseInsensitive) != 0)
		invalidValues << "Environment/mode must be Test, Live, or LiveObserver";

	const QStringList positiveIntegerKeys = QStringList()
		<< "Application/windowWidth" << "Application/windowHeight"
		<< "PLC/enabledPollingMs" << "PLC/standbyPollingMs"
		<< "Timers/mainWindowMs" << "Timers/positionsMs" << "Timers/autoModeMs"
		<< "Timers/craneAnimationMs" << "Timers/statusFadeMs"
		<< "Timers/emulationMainMs" << "Timers/emulationCraneMainMs"
		<< "Timers/emulationMovingZStepMs" << "Timers/emulationStepMs"
		<< "Timers/emulationAnimationMs" << "Timers/emulationConveyorMainMs"
		<< "Timers/emulationConveyorPropagationMs"
		<< "Visualization/basketLabelFontPointSize" << "Visualization/locationLabelFontPointSize";
	for (int i = 0; i < positiveIntegerKeys.count(); ++i)
	{
		if (settings.value(positiveIntegerKeys[i]).toInt() <= 0)
			invalidValues << positiveIntegerKeys[i] + " must be greater than zero";
	}

	const QStringList portKeys = QStringList()
		<< "PLC/craneReadPort" << "PLC/craneWritePort"
		<< "PLC/conveyorsReadPort" << "PLC/conveyorsWritePort";
	for (int i = 0; i < portKeys.count(); ++i)
	{
		const int port = settings.value(portKeys[i]).toInt();
		if (port < 1 || port > 65535)
			invalidValues << portKeys[i] + " must be between 1 and 65535";
	}

	const int emptyBufferMinimum = settings.value("Operations/emptyBufferMinimum").toInt();
	const int emptyBufferDefault = settings.value("Operations/emptyBufferDefault").toInt();
	const int emptyBufferMaximum = settings.value("Operations/emptyBufferMaximum").toInt();
	if (emptyBufferMinimum > emptyBufferDefault || emptyBufferDefault > emptyBufferMaximum)
		invalidValues << "Operations empty-buffer values must satisfy minimum <= default <= maximum";

	const int ovenStayMinimum = settings.value("Emulation/ovenStayMinimumSeconds").toInt();
	const int ovenStayDefault = settings.value("Emulation/ovenStayDefaultSeconds").toInt();
	const int ovenStayMaximum = settings.value("Emulation/ovenStayMaximumSeconds").toInt();
	if (ovenStayMinimum > ovenStayDefault || ovenStayDefault > ovenStayMaximum)
		invalidValues << "Emulation oven-stay values must satisfy minimum <= default <= maximum";

	const QRegExp sqlIdentifier("^[A-Za-z_][A-Za-z0-9_]*$");
	if (!sqlIdentifier.exactMatch(settings.value("Database/mainCatalog").toString()))
		invalidValues << "Database/mainCatalog must be a valid SQL identifier";
	if (!sqlIdentifier.exactMatch(settings.value("Database/apexCatalog").toString()))
		invalidValues << "Database/apexCatalog must be a valid SQL identifier";

	if (!invalidValues.isEmpty())
	{
		if (errorMessage)
			*errorMessage = QString("Configuration file contains invalid values:\n%1")
				.arg(invalidValues.join("\n"));
		return false;
	}
	return true;
}

QString ConfigurationManager::ReadString(QSettings& settings, const QString& key) const
{
	return Expand(settings.value(key).toString());
}

QString ConfigurationManager::Expand(const QString& value) const
{
	QString expanded = value;
	expanded.replace("${APP_DIR}", QCoreApplication::applicationDirPath());
	expanded.replace("${CURRENT_DIR}", QDir::currentPath());
	expanded.replace("${CONFIG_DIR}", QFileInfo(m_fileName).absolutePath());

	const QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
	int start = expanded.indexOf("${ENV:");
	while (start >= 0)
	{
		const int end = expanded.indexOf('}', start);
		if (end < 0)
			break;
		const QString token = expanded.mid(start, end - start + 1);
		const QString variableName = expanded.mid(start + 6, end - start - 6);
		expanded.replace(token, environment.value(variableName));
		start = expanded.indexOf("${ENV:", start);
	}
	return expanded;
}

DatabaseProfile ConfigurationManager::ReadDatabaseProfile(
	QSettings& settings,
	const QString& section) const
{
	DatabaseProfile profile;
	profile.m_name = section;
	profile.m_dsn = ReadString(settings, section + "/dsn");
	profile.m_user = ReadString(settings, section + "/user");
	profile.m_password = ReadString(settings, section + "/password");
	profile.m_connectOptions = ReadString(settings, section + "/connectOptions");
	return profile;
}

}
}
