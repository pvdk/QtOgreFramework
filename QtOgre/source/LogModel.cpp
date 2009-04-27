#include "LogModel.h"

#include "Log.h"
#include "LogEntry.h"

namespace QtOgre
{
	LogModel::LogModel(Log *log, QObject *parent)
		:QAbstractTableModel(parent)
		,m_entries(QList<LogEntry*>())
		,m_log(log)
	{
	}

	QVariant LogModel::data(const QModelIndex &index, int role) const {
		if (!index.isValid()) // bad index
			return QVariant();

		if (index.row() >= m_entries.size()) // row that we don't have in our model
			return QVariant();

		// valid index, so go fetch the entry object
		LogEntry *entry = m_entries.at(index.row());
		switch (role) {
			case Qt::DecorationRole: // called to show icons
				// we only want to decorate 1 column, so the icon doesn't show up for every piece of data
				if (index.column() == 0) { 
					// re-use the same QIcons that the Log owner created
					return m_log->get_icon(entry->get_level());
				} else {
					return QVariant();
				}
			case Qt::DisplayRole:
				return entry->get_data(index.column());
			case Qt::ForegroundRole:
				return m_log->get_fg_color(entry->get_level());
			case Qt::UserRole:
				return entry->get_level();
			case Qt::ToolTipRole:
				return entry->get_data(3);
			default: //some other role we don't really care about
				return QVariant();
		}
	}

	QVariant LogModel::headerData(int section, Qt::Orientation orientation, int role) const {
		if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
			return QVariant();
		switch (section) {
			case 0: return QVariant("Time");
			case 1: return QVariant("Line");
			case 2: return QVariant("File");
			case 3: return QVariant("Message");
			default: return QVariant("unset");
		}
	}

	void LogModel::append(int line, const QString &file, const QString &msg, LogLevel level) {
		emit layoutAboutToBeChanged(); // if we don't emit these signals, the view never knows to update
		LogEntry *entry = new LogEntry(line, file, msg, level, this);
		m_entries.append(entry);
		emit layoutChanged(); // causes a redraw
		emit entry_added(entry);
		//TODO: we could buffer appends up and only emit the change signals every Nth entry or every few seconds
		//this would probably be more efficient
	}
}