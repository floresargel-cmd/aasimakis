#pragma once

#include <QString>

class ILogger
{
public:
	virtual ~ILogger() = default;

	/// <summary>Writes an informational log entry.</summary>
	virtual void Info(const QString& message) = 0;

	/// <summary>Writes a warning log entry.</summary>
	virtual void Warn(const QString& message) = 0;

	/// <summary>Writes an error log entry.</summary>
	virtual void Error(const QString& message) = 0;
};
