#include "Log.h"
#include "LogEntry.h"
#include "LogModel.h"
#include "LogModelSortFilterProxy.h"

#include <QHeaderView>
#include <QTime>
#include <QFile>
#include <QSortFilterProxyModel>

namespace QtOgre
{
	Log::Log(const QString& name, QWidget* parent)
	:QWidget(parent)
	,mForceProcessEvents(false)
	,mName(name)
	,mLogModel(new LogModel(this, this))
	,mProxyModel(new LogModelSortFilterProxy(this))
	{
		setupUi(this);
		
		//setup connections 
		connect(showDebugButton, SIGNAL(toggled(bool)), this, SLOT(computeVisibleMessageTypes(bool)));
		connect(showInformationButton, SIGNAL(toggled(bool)), this, SLOT(computeVisibleMessageTypes(bool)));
		connect(showWarningsButton, SIGNAL(toggled(bool)), this, SLOT(computeVisibleMessageTypes(bool)));
		connect(showErrorsButton, SIGNAL(toggled(bool)), this, SLOT(computeVisibleMessageTypes(bool)));
		connect(filterLineEdit, SIGNAL(textChanged(const QString&)), mProxyModel, SLOT(setFilterFixedString(const QString&)));
		connect(clearFilterButton, SIGNAL(pressed()), filterLineEdit, SLOT(clear()));
		connect(mLogModel, SIGNAL(entry_added(LogEntry*)), this, SLOT(writeMessageToHTML(LogEntry*)));
		
		//Create a sort/filter proxy of our log entries model so we can sort and filter it :)
		mProxyModel->setSourceModel(mLogModel); // the proxy should point to the real model
		mProxyModel->setFilterKeyColumn(3); // the message column (for filtering by regex) TODO: nuke the MAGIC NUMBER!
		mProxyModel->setDynamicSortFilter(true); // keep sort order updated when data changes
		logTable->setModel(mProxyModel); // the view points to our proxy
		logTable->setSortingEnabled(true); // let the view use the sorting of the proxy

		//Using this approach of resizing to contents seems to have
		//noticeable performance penalties when adding rows to the log.
		//logTable->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
		//logTable->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
		//logTable->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
		//logTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

		//So instead we hard code the sizes
		logTable->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
		logTable->horizontalHeader()->resizeSection(0, 100);
		logTable->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
		logTable->horizontalHeader()->resizeSection(1, 30);
		logTable->horizontalHeader()->resizeSection(2, 120);
		logTable->horizontalHeader()->resizeSection(3, 800);

		logTable->verticalHeader()->hide();
		//logTable->verticalHeader()->setResizeMode(QHeaderView::Fixed);
		//logTable->verticalHeader()->resizeSections(20);


		//We use .png images here, rather than .svg. They are frequently being created on-the-fly
		//and in large numbers, so maybe this makes a performance difference. Also, there seems
		//to be a bug with Qt's rendering of dialog-information.svg which shows up clearly
		//against the dark background of the log. Using a .png works around this.
		debugIcon = QIcon(QPixmap(QString::fromUtf8(":/images/icons/script-error.png")));
		infoIcon = QIcon(QPixmap(QString::fromUtf8(":/images/icons/dialog-information.png")));
		warningIcon = QIcon(QPixmap(QString::fromUtf8(":/images/icons/dialog-warning.png")));
		errorIcon = QIcon(QPixmap(QString::fromUtf8(":/images/icons/dialog-error.png")));

		//Currently we hard-code the colours for the various log levels
		bgColor.setRgb(0, 0, 0);
		debugColor.setRgb(255, 255, 255);
		infoColor.setRgb(64, 64, 255);
		warningColor.setRgb(255, 255, 64);
		errorColor.setRgb(255,64,64);

		//Set the widgets background to the colour we chose above.
		QPalette palette = logTable->palette();
		palette.setColor(QPalette::Active, QPalette::Base, bgColor);
		palette.setColor(QPalette::Inactive, QPalette::Base, bgColor);
		logTable->setPalette(palette);

		//Initial set up of which log levels are displayed
		computeVisibleMessageTypes(true);

		//Create a file to write this log to disk.
		QString filename;
		QTextStream(&filename) << "logs/" << mName << ".html";
		mFile = new QFile(filename, this);
		mFile->open(QFile::WriteOnly | QFile::Truncate | QFile::Text | QIODevice::Unbuffered);
		mTextStream.setDevice(mFile);

		//Write the opening HTML to the log file.
		writeHTMLHeader();
	}

	Log::~Log()
	{
		//Write the closing HTML to the log file.
		writeHTMLTail();
	}

	//Used by the model for determining the decoration to use for an entry
	QIcon &Log::getIcon(LogLevel level)
	{
		switch (level)
		{
			case LL_DEBUG:
				return debugIcon;
			case LL_INFO:
				return infoIcon;
			case LL_WARNING:
				return warningIcon;
			case LL_ERROR:
				return errorIcon;
			default:
				return errorIcon;
		}
	}

	//Used by the model for determining the foreground color to use for an entry
	QColor &Log::getForegroundColour(LogLevel level)
	{
		switch (level)
		{
			case LL_DEBUG:
				return debugColor;
			case LL_INFO:
				return infoColor;
			case LL_WARNING:
				return warningColor;
			case LL_ERROR:
				return errorColor;
			default:
				return debugColor;
		}
	}

	void Log::logMessage(const QString& message, LogLevel logLevel)
	{
		mLogModel->append(0, "file", message, logLevel);
		logTable->verticalHeader()->resizeSection(mLogModel->rowCount() - 1, 14);
		logTable->scrollToBottom();
		if(mForceProcessEvents)	{
			qApp->processEvents();
		}
	}

	void Log::computeVisibleMessageTypes(bool ignored)
	{
		mVisibleMessageTypes = 0;
		if(showDebugButton->isChecked()) { mVisibleMessageTypes |= LL_DEBUG; }
		if(showInformationButton->isChecked()) { mVisibleMessageTypes |= LL_INFO; }
		if(showWarningsButton->isChecked()) { mVisibleMessageTypes |= LL_WARNING; }
		if(showErrorsButton->isChecked()) { mVisibleMessageTypes |= LL_ERROR; }
		mProxyModel->setVisisbleLevels(mVisibleMessageTypes);
	}

	void Log::setForceProcessEvents(bool forceProcessEvents)
	{
		mForceProcessEvents = forceProcessEvents;
	}

	void Log::writeHTMLHeader(void)
	{
		mTextStream
			<< "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">" << endl
			<< "<html>" << endl
			<< "<head>" << endl
			<< "<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">" << endl
			<< "<title>Log File</title>" << endl
			<< "</head>" << endl
			<< "<body style=\"background-color: black; color: rgb(0, 0, 0);\">" << endl
			<< "<table style=\"text-align: left; width: 100%;\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">" << endl
			<< "<tbody>" << endl;
	}

	void Log::writeMessageToHTML(LogEntry *entry) 
	{
		QString colour;
		QString icon;
		switch(entry->getLevel())
		{
		case LL_DEBUG:
			colour = "white";
			icon = "script-error.png";
			break;
		case LL_INFO:
			colour = "blue";
			icon = "dialog-information.png";
			break;
		case LL_WARNING:
			colour = "yellow";
			icon = "dialog-warning.png";
			break;
		case LL_ERROR:
			colour = "red";
			icon = "dialog-error.png";
			break;
		}

		mTextStream 
			<< "<tr>"
			<< "<td style=\"text-align: center; width: 25px;\">"
			<< "<img src=\"images/" << icon << "\">"
			<< "</td>"
			<< "<td style=\"width: 90px;\"><span style=\"color: " << colour << ";\">"
			<< entry->getData(0).toTime().toString("hh:mm:ss a") << " - "
			<< "</span></td>"
			<< "<td><span style=\"color: " << colour << ";\">"
			<< entry->getData(3).toString()
			<< "</span></td></tr>" << endl;

	}

	void Log::writeHTMLTail(void)
	{
		mTextStream
			<< "</tbody>" << endl
			<< "</table>" << endl
			<< "</body>" << endl
			<< "</html>" << endl;
	}

	/*
	//Set the size of the row
	//logTable->setRowHeight(row, 20); is an alternative to the following
	logTable->verticalHeader()->setResizeMode(row, QHeaderView::Fixed);
	logTable->verticalHeader()->resizeSection(row, 20); 
	*/
}
