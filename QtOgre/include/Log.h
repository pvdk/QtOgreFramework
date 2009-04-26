#ifndef QTOGRE_LOG_H_
#define QTOGRE_LOG_H_

#include "ui_Log.h"

#include <QTextStream>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QTime>

class QFile;

namespace QtOgre
{
	//forward declarations
	class LogEntry;
	class LogModel;
	class LogModelSortFilterProxy;

	enum LogLevel
	{ 
		//These values are specifically chosen to be over 1000. This means they can be used as the 'type'
		//of a QTableWidgetItem. This lets us easily identify the message type of any given row.
		//In addition, they are powers of two in order to simplify the logic when filtering messages.
		LL_DEBUG = 1024,
		LL_INFO = 2048,
		LL_WARNING = 4096,
		LL_ERROR = 8192
	};

	class Log : public QWidget, private Ui::Log
	{
		Q_OBJECT

	public:
		Log(const QString& name, QWidget *parent = 0);
		~Log();

		void logMessage(const QString& message, LogLevel logLevel);

		//Messages which get logged do not appear in the log until the event
		//queue get processed to cause a GUI update. This normally happens once
		//per frame, but that may not be enough. It's certainly a problem during
		//initialisation, when many mesages get logged with no updates.
		//Setting this to true causes the log to force event processing every
		//time a message is logged.
		//TODO - should this be a Qt property?
		void setForceProcessEvents(bool forceProcessEvents);

		QIcon &get_icon(LogLevel level);
		QColor &get_fg_color(LogLevel level);

	private:
		//Private methods for writing HTML
		void writeHTMLHeader(void);
		void writeHTMLTail(void);

		//This is used as a bitfield to determine what message types are enabled
		int mVisibleMessageTypes;

		//Icons for the lefthand column
		QIcon debugIcon;
		QIcon infoIcon;
		QIcon warningIcon;
		QIcon errorIcon;

		//Colours used for the log viewer
		QColor bgColor;
		QColor debugColor;
		QColor infoColor;
		QColor warningColor;
		QColor errorColor;

		//Controls whether we force the processing of event on log updates
		bool mForceProcessEvents;

		//For writing the log to disk.
		QString mName;
		QFile* mFile;
		QTextStream mTextStream;

		//New Models
		LogModel* mLogModel; //holds all log data
		LogModelSortFilterProxy *mProxyModel; //proxy for sorting/filtering the mLogModel
		
		private slots:
			void writeMessageToHTML(LogEntry *entry);
			void computeVisibleMessageTypes(bool ignored);
	};


	// This could probably just be a struct but I'm using QObject as a base so that the model
	// can manage the pointers for us
	class LogEntry : public QObject {
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

	// A simple model that will let us customize display, sorting, and filtering of log messages
	class LogModel : public QAbstractTableModel {
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

	class LogModelSortFilterProxy : public QSortFilterProxyModel {
		Q_OBJECT
	public:
		LogModelSortFilterProxy(QObject *parent = 0);
		void setShowLineAndFile(bool show);
		void setVisisbleLevels(int level_bitmask) {
			mVisibleLevels = level_bitmask;
			invalidateFilter();
		}

	protected:
		bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
		bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;

	private:
		bool mShowLineAndFile;
		int mVisibleLevels;
	};


}

#endif /*QTOGRE_LOG_H_*/
