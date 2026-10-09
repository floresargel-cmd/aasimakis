#include "FileLogger.h"

#include <QtCore/QDate>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QMutexLocker>
#include <QtCore/QTextStream>

namespace StackingPlan::Infrastructure::Logging
{
	FileLogger::FileLogger(const QString& logDirectory)
		: _logDirectory(logDirectory)
	{
		QDir().mkpath(_logDirectory);
	}

	void FileLogger::Info(const QString& message)
	{
		Write("INFO", message);
	}

	void FileLogger::Warning(const QString& message)
	{
		Write("WARN", message);
	}

	void FileLogger::Error(const QString& message)
	{
		Write("ERROR", message);
	}

	void FileLogger::Write(const QString& level, const QString& message)
	{
		QMutexLocker locker(&_mutex);

		const QString filePath =
			_logDirectory + "/" + QDate::currentDate().toString("yyyy-MM-dd") + ".log";

		QFile file(filePath);
		if (!file.open(QIODevice::Append | QIODevice::Text))
		{
			return;
		}

		QTextStream stream(&file);
		stream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
			<< " [" << level << "] "
			<< message
			<< "\r\n";
	}
}
