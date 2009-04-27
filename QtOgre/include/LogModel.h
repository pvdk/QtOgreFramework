#ifndef QTOGRE_LOGMODEL_H_
#define QTOGRE_LOGMODEL_H_

#include "LogLevel.h"

#include <QAbstractTableModel>

namespace QtOgre
{
	class Log;
	class LogEntry;

	// A simple model that will let us customize display, sorting, and filtering of log messages
	class LogModel : public QAbstractTableModel
	{
		Q_OBJECT

	public:
		// takes a pointer to the owning log object (right now just for fetching icons & colors)
		LogModel(Log *log, QObject *parent = 0);

		int rowCount(const QModelIndex &parent = QModelIndex()) const {return m_entries.length();}
		int columnCount(const QModelIndex &parent = QModelIndex()) const {return 4;}

		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

		public slots:
			//TODO: make slots for debug, warning, etc...
			void append(int line, const QString &file, const QString &msg, LogLevel level);
	
	signals:
		// slots can't return values, so we'll fire this signal when items are added and let Qt handle
	    // dispatching
		void entry_added(LogEntry*);

	private:
		QList<LogEntry*> m_entries;
		Log *m_log;
	};
}
#endif