#include "LogEntry.h"

namespace QtOgre
{
	LogEntry::LogEntry(int line, const QString &file, const QString &msg, LogLevel level, QObject *parent)
		:QObject(parent)
		,mLine(line)
		,mFile(file)
		,mMsg(msg)
		,mLevel(level)
		,mTimestamp(QTime::currentTime())
	{
	}

	LogLevel LogEntry::getLevel()
	{
		return mLevel;
	}

	QVariant LogEntry::getData(int column) 
	{
		switch(column)
		{
			case 0:
				return QVariant(mTimestamp);
			case 1:
				return QVariant(mLine);
			case 2:
				return QVariant(mFile);
			case 3:
				return QVariant(mMsg);
			default:
				return QVariant();
		}
	}
}