#ifndef BASKETCRANE_CONFIGURATION_SETTINGS_H
#define BASKETCRANE_CONFIGURATION_SETTINGS_H

#include <QString>
#include <QStringList>

namespace BasketCrane
{
namespace Configuration
{

class ConfigurationManager;

class EmailAlertSettings
{
public:
    EmailAlertSettings() : m_enabled(false), m_smtpPort(587), m_startTls(true),
        m_cooldownSeconds(3600), m_timeoutSeconds(20) {}
    bool Enabled() const { return m_enabled; }
    const QString& SmtpHost() const { return m_smtpHost; }
    int SmtpPort() const { return m_smtpPort; }
    bool StartTls() const { return m_startTls; }
    const QString& Username() const { return m_username; }
    const QString& Password() const { return m_password; }
    const QString& From() const { return m_from; }
    const QString& To() const { return m_to; }
    QString Subject() const { return m_subjectPrefix.isEmpty() ? "Basket Crane 1 Application Error" : m_subjectPrefix + "Application Error"; }
    int CooldownSeconds() const { return m_cooldownSeconds; }
    int TimeoutSeconds() const { return m_timeoutSeconds; }
    const QString& ValidationError() const { return m_validationError; }

private:
    friend class ConfigurationManager;
    bool m_enabled;
    QString m_smtpHost;
    int m_smtpPort;
    bool m_startTls;
    QString m_username, m_password, m_from, m_to;
    QString m_subjectPrefix;
    int m_cooldownSeconds, m_timeoutSeconds;
    QString m_validationError;
};

class ApplicationSettings
{
public:
	bool SingleInstance() const { return m_singleInstance; }
	const QString& Style() const { return m_style; }
	int WindowWidth() const { return m_windowWidth; }
	int WindowHeight() const { return m_windowHeight; }
	bool StartMaximized() const { return m_startMaximized; }
	bool ShowSettingsButton() const { return m_showSettingsButton; }
	const QString& EnvironmentMode() const { return m_environmentMode; }
	const QStringList& LiveEnabledHosts() const { return m_liveEnabledHosts; }

private:
	friend class ConfigurationManager;
	ApplicationSettings()
		: m_singleInstance(false),
		  m_windowWidth(0),
		  m_windowHeight(0),
		  m_startMaximized(false), m_showSettingsButton(true)
	{
	}
	bool m_singleInstance;
	QString m_style;
	int m_windowWidth;
	int m_windowHeight;
	bool m_startMaximized;
	bool m_showSettingsButton;
	QString m_environmentMode;
	QStringList m_liveEnabledHosts;
};

class PathSettings
{
public:
	const QStringList& LibraryPaths() const { return m_libraryPaths; }

private:
	friend class ConfigurationManager;
	QStringList m_libraryPaths;
};

class DatabaseProfile
{
public:
	const QString& Name() const { return m_name; }
	const QString& Dsn() const { return m_dsn; }
	const QString& User() const { return m_user; }
	const QString& Password() const { return m_password; }
	const QString& ConnectOptions() const { return m_connectOptions; }

private:
	friend class ConfigurationManager;
	QString m_name;
	QString m_dsn;
	QString m_user;
	QString m_password;
	QString m_connectOptions;
};

class DatabaseSettings
{
public:
	const QString& Driver() const { return m_driver; }
	const DatabaseProfile& MainProfile(bool useLiveProfile) const
	{
		return useLiveProfile ? m_mainLive : m_mainTest;
	}
	const DatabaseProfile& ApexProfile(bool useLiveProfile) const
	{
		return useLiveProfile ? m_apexLive : m_apexTest;
	}
	const QString& MainCatalog() const { return m_mainCatalog; }
	const QString& ApexCatalog() const { return m_apexCatalog; }
	bool CreateCatalogs() const { return m_createCatalogs; }

private:
	friend class ConfigurationManager;
	DatabaseSettings()
		: m_createCatalogs(false)
	{
	}
	QString m_driver;
	DatabaseProfile m_mainLive;
	DatabaseProfile m_mainTest;
	DatabaseProfile m_apexLive;
	DatabaseProfile m_apexTest;
	QString m_mainCatalog;
	QString m_apexCatalog;
	bool m_createCatalogs;
};

class PlcSettings
{
public:
	const QString& CraneIp() const { return m_craneIp; }
	const QString& ConveyorsIp() const { return m_conveyorsIp; }
	const QString& NewOvenIp() const { return m_newOvenIp; }
	const QString& CraneDescription() const { return m_craneDescription; }
	const QString& ConveyorsDescription() const { return m_conveyorsDescription; }
	const QString& NewOvenDescription() const { return m_newOvenDescription; }
	int CraneReadPort() const { return m_craneReadPort; }
	int CraneWritePort() const { return m_craneWritePort; }
	int ConveyorsReadPort() const { return m_conveyorsReadPort; }
	int ConveyorsWritePort() const { return m_conveyorsWritePort; }
	int EnabledPollingMilliseconds() const { return m_enabledPollingMilliseconds; }
	int StandbyPollingMilliseconds() const { return m_standbyPollingMilliseconds; }
	const QString& CraneHmiExecutable() const { return m_craneHmiExecutable; }
	const QString& ConveyorsHmiExecutable() const { return m_conveyorsHmiExecutable; }

private:
	friend class ConfigurationManager;
	PlcSettings()
		: m_craneReadPort(0),
		  m_craneWritePort(0),
		  m_conveyorsReadPort(0),
		  m_conveyorsWritePort(0),
		  m_enabledPollingMilliseconds(0),
		  m_standbyPollingMilliseconds(0)
	{
	}
	QString m_craneIp;
	QString m_conveyorsIp;
	QString m_newOvenIp;
	QString m_craneDescription;
	QString m_conveyorsDescription;
	QString m_newOvenDescription;
	int m_craneReadPort;
	int m_craneWritePort;
	int m_conveyorsReadPort;
	int m_conveyorsWritePort;
	int m_enabledPollingMilliseconds;
	int m_standbyPollingMilliseconds;
	QString m_craneHmiExecutable;
	QString m_conveyorsHmiExecutable;
};

class TimerSettings
{
public:
	int MainWindowMilliseconds() const { return m_mainWindowMilliseconds; }
	int PositionsMilliseconds() const { return m_positionsMilliseconds; }
	int AutoModeMilliseconds() const { return m_autoModeMilliseconds; }
	int CraneAnimationMilliseconds() const { return m_craneAnimationMilliseconds; }
	int StatusFadeMilliseconds() const { return m_statusFadeMilliseconds; }
	int EmulationMainMilliseconds() const { return m_emulationMainMilliseconds; }
	int EmulationCraneMainMilliseconds() const { return m_emulationCraneMainMilliseconds; }
	int EmulationMovingZStepMilliseconds() const { return m_emulationMovingZStepMilliseconds; }
	int EmulationStepMilliseconds() const { return m_emulationStepMilliseconds; }
	int EmulationAnimationMilliseconds() const { return m_emulationAnimationMilliseconds; }
	int EmulationConveyorMainMilliseconds() const { return m_emulationConveyorMainMilliseconds; }
	int EmulationConveyorPropagationMilliseconds() const { return m_emulationConveyorPropagationMilliseconds; }

private:
	friend class ConfigurationManager;
	TimerSettings()
		: m_mainWindowMilliseconds(0),
		  m_positionsMilliseconds(0),
		  m_autoModeMilliseconds(0),
		  m_craneAnimationMilliseconds(0),
		  m_statusFadeMilliseconds(0),
		  m_emulationMainMilliseconds(0),
		  m_emulationCraneMainMilliseconds(0),
		  m_emulationMovingZStepMilliseconds(0),
		  m_emulationStepMilliseconds(0),
		  m_emulationAnimationMilliseconds(0),
		  m_emulationConveyorMainMilliseconds(0),
		  m_emulationConveyorPropagationMilliseconds(0)
	{
	}
	int m_mainWindowMilliseconds;
	int m_positionsMilliseconds;
	int m_autoModeMilliseconds;
	int m_craneAnimationMilliseconds;
	int m_statusFadeMilliseconds;
	int m_emulationMainMilliseconds;
	int m_emulationCraneMainMilliseconds;
	int m_emulationMovingZStepMilliseconds;
	int m_emulationStepMilliseconds;
	int m_emulationAnimationMilliseconds;
	int m_emulationConveyorMainMilliseconds;
	int m_emulationConveyorPropagationMilliseconds;
};

class OperationSettings
{
public:
	int EmptyBufferMinimum() const { return m_emptyBufferMinimum; }
	int EmptyBufferMaximum() const { return m_emptyBufferMaximum; }
	int EmptyBufferDefault() const { return m_emptyBufferDefault; }
	bool Oven1Enabled() const { return m_oven1Enabled; }
	bool Oven2Enabled() const { return m_oven2Enabled; }
	bool Oven1Maximum24Feet() const { return m_oven1Maximum24Feet; }
	bool Oven2OnlyOver24Feet() const { return m_oven2OnlyOver24Feet; }
	bool ExportsBeforeImports() const { return m_exportsBeforeImports; }

private:
	friend class ConfigurationManager;
	OperationSettings()
		: m_emptyBufferMinimum(0),
		  m_emptyBufferMaximum(0),
		  m_emptyBufferDefault(0),
		  m_oven1Enabled(false),
		  m_oven2Enabled(false),
		  m_oven1Maximum24Feet(false),
		  m_oven2OnlyOver24Feet(false),
		  m_exportsBeforeImports(false)
	{
	}
	int m_emptyBufferMinimum;
	int m_emptyBufferMaximum;
	int m_emptyBufferDefault;
	bool m_oven1Enabled;
	bool m_oven2Enabled;
	bool m_oven1Maximum24Feet;
	bool m_oven2OnlyOver24Feet;
	bool m_exportsBeforeImports;
};

class VisualizationSettings
{
public:
	bool ShowBasketNumbers() const { return m_showBasketNumbers; }
	bool ShowLocationLabels() const { return m_showLocationLabels; }
	int BasketLabelFontPointSize() const { return m_basketLabelFontPointSize; }
	int LocationLabelFontPointSize() const { return m_locationLabelFontPointSize; }

private:
	friend class ConfigurationManager;
	VisualizationSettings()
		: m_showBasketNumbers(false),
		  m_showLocationLabels(false),
		  m_basketLabelFontPointSize(0),
		  m_locationLabelFontPointSize(0)
	{
	}
	bool m_showBasketNumbers;
	bool m_showLocationLabels;
	int m_basketLabelFontPointSize;
	int m_locationLabelFontPointSize;
};

class EmulationSettings
{
public:
	bool ForceDemoFillTables() const { return m_forceDemoFillTables; }
	int OvenStayMinimumSeconds() const { return m_ovenStayMinimumSeconds; }
	int OvenStayMaximumSeconds() const { return m_ovenStayMaximumSeconds; }
	int OvenStayDefaultSeconds() const { return m_ovenStayDefaultSeconds; }

private:
	friend class ConfigurationManager;
	EmulationSettings()
		: m_forceDemoFillTables(false),
		  m_ovenStayMinimumSeconds(0),
		  m_ovenStayMaximumSeconds(0),
		  m_ovenStayDefaultSeconds(0)
	{
	}
	bool m_forceDemoFillTables;
	int m_ovenStayMinimumSeconds;
	int m_ovenStayMaximumSeconds;
	int m_ovenStayDefaultSeconds;
};

class SecuritySettings
{
public:
	const QString& InitialAdminUser() const { return m_initialAdminUser; }
	const QString& InitialAdminPassword() const { return m_initialAdminPassword; }

private:
	friend class ConfigurationManager;
	QString m_initialAdminUser;
	QString m_initialAdminPassword;
};

}
}

#endif
