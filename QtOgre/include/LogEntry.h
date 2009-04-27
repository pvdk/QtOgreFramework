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
		LogEntry(int line, const QString &file, const QString &msg, LogLevel level, QObject *parent = 0)
			:QObject(parent)
			,m_line(line)
			,m_file(file)
			,m_msg(msg)
			,m_level(level)
			,m_timestamp(QTime::currentTime())
		{}
		LogLevel get_level() {return m_level;}
		QVariant get_data(int column) {
			switch(column) {
				case 0: return QVariant(m_timestamp);
				case 1: return QVariant(m_line);
				case 2: return QVariant(m_file);
				case 3: return QVariant(m_msg);
				default: return QVariant();
			}
		}

	private:
		int m_line;
		QString m_file;
		QString m_msg;
		QTime m_timestamp;
		LogLevel m_level;
		
	};
}
#endif