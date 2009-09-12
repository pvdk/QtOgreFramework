#ifndef QTOGRE_LOG_H_
#define QTOGRE_LOG_H_

#include "ui_Log.h"

#include "LogLevel.h"

#include <QTextStream>
#include <QTime>

class QFile;

namespace QtOgre
{
	//forward declarations
	class LogEntry;
	class LogModel;
	class LogModelSortFilterProxy;

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

		QIcon &getIcon(LogLevel level);
		QColor &getForegroundColour(LogLevel level);

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
}

#endif /*QTOGRE_LOG_H_*/
