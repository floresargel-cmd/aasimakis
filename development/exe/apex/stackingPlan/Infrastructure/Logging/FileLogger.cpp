/*
 * FileLogger.cpp
 *
 * Implements the FileLogger class for the StackingPlan::Infrastructure::Logging namespace.
 * Provides thread-safe logging functionality to write informational, warning, and error messages
 * to daily log files in a specified directory.
 *
 * The logger creates a log directory if it does not exist. Each log entry is timestamped and
 * categorized by log level (INFO, WARN, ERROR). Log files are named by date (yyyy-MM-dd.log).
 *
 * Dependencies:
 * - QtCore (QDir, QFile, QIODevice, QTextStream, QCoreApplication, QMutex, QMutexLocker, QDate, QDateTime, QString)
 *
 * Usage:
 *   FileLogger logger("path/to/logs");
 *   logger.Info("Application started");
 *   logger.Warning("Potential issue detected");
 *   logger.Error("An error occurred");
 *
 * Thread Safety:
 *   All write operations are protected by a QMutex to ensure thread safety.
 *
 * Author: [Your Name]
 * Date: [Creation/Modification Date]
 */
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qdatetime.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qsettings.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qprocess.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qdir.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qfile.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qiodevice.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/io/qtextstream.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/kernel/qcoreapplication.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/thread/qmutex.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qdatetime.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstring.h"
#include "FileLogger.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qnamespace.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/tools/qstringlist.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/corelib/global/qglobal.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qapplication.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/widgets/kernel/qwidget.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/image/qpixmap.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qwindowdefs.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qscreen.h"
#include "../../../../../libs/#qt/qt.5.7.0.static.x32/qtbase/src/gui/kernel/qwindow.h"


namespace StackingPlan::Infrastructure::Logging
{
	QString FileLogger::CaptureAppScreenshot()
	{
		QWidget* mainWindow = QApplication::activeWindow();

		if (mainWindow == NULL)
		{
			const QWidgetList widgets = QApplication::topLevelWidgets();

			for (int i = 0; i < widgets.count(); ++i)
			{
				if (widgets[i] != NULL && widgets[i]->isVisible())
				{
					mainWindow = widgets[i];
					break;
				}
			}
		}

		if (mainWindow == NULL)
		{
			Write("WARN", "Screenshot capture skipped. No active or visible main window found.");
			return "";
		}

		const QString screenshotDir =
			QDir(QCoreApplication::applicationDirPath()).filePath("logs/screenshots");

		QDir().mkpath(screenshotDir);

		const QString screenshotPath =
			QDir(screenshotDir).filePath(
				"error_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".png");

		QApplication::processEvents();

		QWindow* window = mainWindow->windowHandle();

		if (window == NULL)
		{
			Write("WARN", "Screenshot capture failed. Main window handle is null.");
			return "";
		}

		QScreen* screen = window->screen();

		if (screen == NULL)
		{
			Write("WARN", "Screenshot capture failed. Screen is null.");
			return "";
		}

		QPixmap pixmap = screen->grabWindow(0);

		if (pixmap.isNull())
		{
			Write("WARN", "Screenshot capture failed. Grab returned null pixmap.");
			return "";
		}

		if (!pixmap.save(screenshotPath, "PNG"))
		{
			Write("WARN", "Screenshot capture failed. Could not save file: " + screenshotPath);
			return "";
		}

		Write("INFO", "Screenshot captured: " + screenshotPath);

		return screenshotPath;
	}

	static QDateTime _lastErrorEmailSentAt;
	/**
	 * Constructs a FileLogger instance.
	 * If logDirectory is empty, uses the application's directory and creates a "logs" subdirectory.
	 * Ensures the log directory exists.
	 *
	 * @param logDirectory The directory where log files will be stored.
	 */
	FileLogger::FileLogger(const QString& logDirectory)
	{
		if (logDirectory.isEmpty())
		{
			_logDirectory = QDir(QCoreApplication::applicationDirPath()).filePath("logs");
		}
		else
		{
			_logDirectory = logDirectory;
		}

		QDir().mkpath(_logDirectory);
	}

	/**
	 * Writes an informational message to the log file.
	 *
	 * @param message The message to log.
	 */
	void FileLogger::Info(const QString& message)
	{
		Write("INFO", message);
	}

	/**
	 * Writes a warning message to the log file.
	 *
	 * @param message The message to log.
	 */
	void FileLogger::Warning(const QString& message)
	{
		Write("WARN", message);
	}

	/**
	 * Writes an error message to the log file.
	 *
	 * @param message The message to log.
	 */
	void FileLogger::Error(const QString& message)
	{
		Write("ERROR", message);

		const int intervalMinutes =
			GetConfigValue("ErrorEmailIntervalMinutes", "15").toInt();

		const QDateTime now = QDateTime::currentDateTime();

		if (_lastErrorEmailSentAt.isValid())
		{
			const qint64 minutesSinceLastEmail =
				_lastErrorEmailSentAt.secsTo(now) / 60;

			if (minutesSinceLastEmail < intervalMinutes)
			{
				Write("INFO",
					QString("Error email skipped. Last email was sent %1 minute(s) ago. Interval is %2 minute(s).")
					.arg(minutesSinceLastEmail)
					.arg(intervalMinutes));

				return;
			}
		}

		SendErrorEmail(message);
		_lastErrorEmailSentAt = now;
	}

	/**
	 * Internal method to write a log entry with a specified level.
	 * Ensures thread safety using a mutex.
	 * Log entries are timestamped and appended to a daily log file.
	 *
	 * @param level The log level (e.g., "INFO", "WARN", "ERROR").
	 * @param message The message to log.
	 */
	void FileLogger::Write(const QString& level, const QString& message)
	{
		QMutexLocker locker(&_mutex);

		const QString filePath =
			QDir(_logDirectory).filePath(QDate::currentDate().toString("yyyy-MM-dd") + ".log");

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

		stream.flush();
		file.flush();
		file.close();
	}

	bool FileLogger::IsErrorEmailEnabled() const
	{
		return GetConfigValue("EnableErrorEmailNotification", "false")
			.compare("true", Qt::CaseInsensitive) == 0;
	}

	QString FileLogger::GetConfigValue(const QString& key, const QString& defaultValue) const
	{
		const QString iniPath =
			QDir(QCoreApplication::applicationDirPath()).filePath("app.ini");

		QSettings settings(iniPath, QSettings::IniFormat);

		return settings.value(key, defaultValue).toString();
	}

	void FileLogger::SendErrorEmail(const QString& message)
	{
		if (!IsErrorEmailEnabled())
		{
			return;
		}

		const QString screenshotPath = CaptureAppScreenshot();
		const QString smtpHost = GetConfigValue("SmtpHost");
		const QString smtpPort = GetConfigValue("SmtpPort", "25");
		const QString enableSsl = GetConfigValue("SmtpEnableSsl", "false");
		const QString from = GetConfigValue("SmtpFrom");
		const QString to = GetConfigValue("SmtpTo").trimmed();
		const QString subjectPrefix = GetConfigValue("SmtpSubjectPrefix", "[StackPlan Error]");

		if (smtpHost.isEmpty() || from.isEmpty() || to.isEmpty())
		{
			Write("WARN", "Error email notification skipped. SMTP config is incomplete.");
			return;
		}

		const QString subject = subjectPrefix + " " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

		QString body;
		body += "StackPlan application error detected.\r\n\r\n";
		body += "Time: " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "\r\n";
		body += "Machine/App Path: " + QCoreApplication::applicationDirPath() + "\r\n\r\n";
		body += "Error Message:\r\n";
		body += message;

		QString safeSubject = subject;
		safeSubject.replace("'", "''");

		QString safeBody = body;
		safeBody.replace("'", "''");

		QString script;
		script += "$smtpHost = '" + smtpHost + "';";
		script += "$smtpPort = " + smtpPort + ";";
		script += "$from = '" + from + "';";
		QStringList recipients =
			to.split(",", QString::SkipEmptyParts);

		QString psRecipients = "@(";

		for (int i = 0; i < recipients.count(); ++i)
		{
			psRecipients += "'" + recipients[i].trimmed() + "'";

			if (i < recipients.count() - 1)
			{
				psRecipients += ",";
			}
		}

		psRecipients += ")";

		script += "$to = " + psRecipients + ";";
		script += "$subject = '" + safeSubject + "';";
		script += "$body = @'\r\n" + safeBody + "\r\n'@;";
		script += "Send-MailMessage -SmtpServer $smtpHost -Port $smtpPort -From $from -To $to -Subject $subject -Body $body";

		if (!screenshotPath.isEmpty())
		{
			script += " -Attachments '" + screenshotPath + "'";
		}

		if (enableSsl.compare("true", Qt::CaseInsensitive) == 0)
		{
			script += " -UseSsl";
		}



		QProcess process;
		process.start("powershell.exe", QStringList()
			<< "-NoProfile"
			<< "-ExecutionPolicy" << "Bypass"
			<< "-Command" << script);

		if (!process.waitForFinished(15000))
		{
			process.kill();
			Write("WARN", "Error email notification timed out.");
			return;
		}

		if (process.exitCode() != 0)
		{
			const QString errorText = QString::fromLocal8Bit(process.readAllStandardError());
			Write("WARN", "Error email notification failed: " + errorText);
		}
	}
}