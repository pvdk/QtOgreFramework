#ifndef QTOGRE_LOGENTRY_H_
#define QTOGRE_LOGENTRY_H_

#include "LogLevel.h"

#include <QString>
#include <QTime>
#include <QVariant>

namespace QtOgre
{
	// This could probably just be a struct but I'm using QObject as a base so that the model
	// can manage the pointers for us
	class LogEntry : public QObject
	{
		Q_OBJECT
	public:
		LogEntry(int line, const QString &file, const QString &msg, LogLevel level, QObject *parent = 0);
		LogLevel getLevel();
		QVariant getData(int column);

	private:
		int mLine;
		QString mFile;
		QString mMsg;
		QTime mTimestamp;
		LogLevel mLevel;		
	};
}
#endif