#ifndef BASKETCRANE_CONFIGURATION_CONFIGURATIONMANAGER_H
#define BASKETCRANE_CONFIGURATION_CONFIGURATIONMANAGER_H

#include "Settings.h"

class QSettings;

namespace BasketCrane
{
namespace Configuration
{

class ConfigurationManager
{
public:
	static ConfigurationManager& Instance();

	bool Load(const QString& fileName, QString *errorMessage = 0);
	bool IsLoaded() const { return m_isLoaded; }
	const QString& FileName() const { return m_fileName; }

	const ApplicationSettings& Application() const { return m_application; }
	const PathSettings& Paths() const { return m_paths; }
	const DatabaseSettings& Database() const { return m_database; }
	const PlcSettings& Plc() const { return m_plc; }
	const TimerSettings& Timers() const { return m_timers; }
	const OperationSettings& Operations() const { return m_operations; }
	const VisualizationSettings& Visualization() const { return m_visualization; }
	const EmulationSettings& Emulation() const { return m_emulation; }
	const SecuritySettings& Security() const { return m_security; }

private:
	ConfigurationManager();
	ConfigurationManager(const ConfigurationManager&);
	ConfigurationManager& operator=(const ConfigurationManager&);

	bool Validate(QSettings& settings, QString *errorMessage) const;
	QString ReadString(QSettings& settings, const QString& key) const;
	QString Expand(const QString& value) const;
	DatabaseProfile ReadDatabaseProfile(QSettings& settings, const QString& section) const;

	bool m_isLoaded;
	QString m_fileName;
	ApplicationSettings m_application;
	PathSettings m_paths;
	DatabaseSettings m_database;
	PlcSettings m_plc;
	TimerSettings m_timers;
	OperationSettings m_operations;
	VisualizationSettings m_visualization;
	EmulationSettings m_emulation;
	SecuritySettings m_security;
};

}
}

#endif
