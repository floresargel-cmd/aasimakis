#pragma once

#include <QString>
#include <QtCore/QMutex>

namespace StackingPlan::Infrastructure::Logging
{
	class FileLogger
	{
	public:
		explicit FileLogger(const QString& logDirectory = "logs");

		void Info(const QString& message);
		void Warning(const QString& message);
		void Error(const QString& message);

	private:
		void Write(const QString& level, const QString& message);

	private:
		QString _logDirectory;
		QMutex _mutex;
	};
}
