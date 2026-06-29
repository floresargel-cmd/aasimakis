#pragma once

#include <QString>

struct DatabaseConnectionSetting
{
	QString connectionName;
	QString driver;
	QString dsn;
	QString username;
	QString password;
	QString initialCatalog;
};

struct SerialPortSetting
{
	QString portName;
	int baudRate = 9600;
	int timeoutMs = 50;
};

struct ProcessSetting
{
	QString executablePath;
	QString workingDirectory;
};

struct AppSettings
{
	DatabaseConnectionSetting mainDb;
	DatabaseConnectionSetting strapingDb;
	DatabaseConnectionSetting epicsDb;
	SerialPortSetting barcodePort;
	ProcessSetting strapingProcess;

	bool useLocalDb = false;
	bool useEpics = false;
	bool emulate = false;
	bool enableVbPlcComms = false;
	bool enableTuxPlcComms = false;
};