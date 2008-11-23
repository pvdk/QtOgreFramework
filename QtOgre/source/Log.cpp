#include "Log.h"

#include <QDir>
#include <QHeaderView>
#include <QTime>

namespace QtOgre
{
	Log::Log(const QString& name, QWidget* parent)
	:QWidget(parent)
	,mForceProcessEvents(false)
	,mName(name)
	{
		setupUi(this);

		//Using this approach of resizing to contents seems to have
		//noticable performance penalties when adding rows to the log.
		//logTable->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
		//logTable->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
		//logTable->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
		//logTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

		//So instead we hard code the sizes
		logTable->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
		logTable->horizontalHeader()->resizeSection(0, 20);
		logTable->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
		logTable->horizontalHeader()->resizeSection(1, 60);
		logTable->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
		logTable->horizontalHeader()->resizeSection(2, 10000);

		logTable->horizontalHeader()->hide();
		logTable->verticalHeader()->hide();

		//We use .png images here, rather than .svg. They are frequesntly being created on-the-fly
		//and in large numbers, so maybe this makes a performance difference. Also, there seems
		//to be a bug with Qt's rendering of dialog-information.svg which shows up clearly
		//against the dark background of the log. Using a .png works around this.
		debugIcon = QIcon(QPixmap(QString::fromUtf8(":/images/icons/script-error.png")));
		infoIcon = QIcon(QPixmap(QString::fromUtf8(":/images/icons/dialog-information.png")));
		warningIcon = QIcon(QPixmap(QString::fromUtf8(":/images/icons/dialog-warning.png")));
		errorIcon = QIcon(QPixmap(QString::fromUtf8(":/images/icons/dialog-error.png")));

		//Currently we hard-code the colours for the various log levels
		backgroundColor.setRgb(0, 0, 0);
		debugColor.setRgb(255, 255, 255);
		infoColor.setRgb(64, 64, 255);
		warningColor.setRgb(255, 255, 64);
		errorColor.setRgb(255,64,64);

		//Set the widgets background to the colour we chose above.
		QPalette palette = logTable->palette();
		palette.setColor(QPalette::Active, QPalette::Base, backgroundColor);
		palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
		logTable->setPalette(palette);

		//Initial set up of which log levels are displayed
		computeVisibleMessageTypes();

		//Create a file to write this log to disk.
		QDir::current().mkdir("logs");
		QString filename;
		QTextStream(&filename) << "logs/" << mName << ".html";
		mFile = new QFile(filename, this);
		mFile->open(QFile::WriteOnly | QFile::Truncate | QFile::Text | QIODevice::Unbuffered);
		mTextStream.setDevice(mFile);

		//Write the opening XML to the log file.
		//mTextStream << "<Log>" << endl;
		writeHTMLHeader();
	}

	Log::~Log()
	{
		writeHTMLTail();
		//mTextStream << "</Log>" << endl;
	}

	void Log::on_clearFilterButton_clicked()
	{
		filterLineEdit->setText("");
	}

	void Log::on_showDebugButton_clicked()
	{
		computeVisibleMessageTypes();
		filterMessages();
	}

	void Log::on_showInformationButton_clicked()
	{
		computeVisibleMessageTypes();
		filterMessages();
	}

	void Log::on_showWarningsButton_clicked()
	{
		computeVisibleMessageTypes();
		filterMessages();
	}

	void Log::on_showErrorsButton_clicked()
	{
		computeVisibleMessageTypes();
		filterMessages();
	}

	void Log::on_filterLineEdit_textChanged(const QString& string)
	{
		clearFilterButton->setEnabled(!string.isEmpty());
		filterMessages();
	}

	void Log::logMessage(const QString& message, LogLevel logLevel)
	{
		QString currentTimeAsString = QTime::currentTime().toString("hh:mm:ss");
		QTableWidgetItem* iconItem = new QTableWidgetItem(logLevel);
		QTableWidgetItem* timeItem = new QTableWidgetItem(currentTimeAsString + " -");
		QTableWidgetItem* messageItem = new QTableWidgetItem(message);

		iconItem->setBackgroundColor(backgroundColor);
		timeItem->setBackgroundColor(backgroundColor);
		messageItem->setBackgroundColor(backgroundColor);

		QString logLevelAsString;
		messageItem->setForeground(QBrush(debugColor));
		switch(logLevel)
		{
		case LL_DEBUG:
			iconItem->setIcon(debugIcon);
			timeItem->setForeground(QBrush(debugColor));			
			messageItem->setForeground(QBrush(debugColor));
			logLevelAsString = "Debug"; //Used for the file written to disk
			break;
		case LL_INFO:
			iconItem->setIcon(infoIcon);
			timeItem->setForeground(QBrush(infoColor));			
			messageItem->setForeground(QBrush(infoColor));
			logLevelAsString = "Info"; //Used for the file written to disk
			break;
		case LL_WARNING:
			iconItem->setIcon(warningIcon);
			timeItem->setForeground(QBrush(warningColor));
			messageItem->setForeground(QBrush(warningColor));
			logLevelAsString = "Warning"; //Used for the file written to disk
			break;
		case LL_ERROR:
			iconItem->setIcon(errorIcon);
			timeItem->setForeground(QBrush(errorColor));
			messageItem->setForeground(QBrush(errorColor));
			logLevelAsString = "Error"; //Used for the file written to disk
			break;
		}

		//Create and insert the row
		//TODO - if this is slow we could insert 1000 rows for every 1000th log message...
		int row = logTable->rowCount();
		logTable->insertRow(row);	

		//Set the size of the row
		//logTable->setRowHeight(row, 20); is an alternative to the following
		logTable->verticalHeader()->setResizeMode(row, QHeaderView::Fixed);
		logTable->verticalHeader()->resizeSection(row, 20); 

		//Add the items
		logTable->setItem(row, 0, iconItem);
		logTable->setItem(row, 1, timeItem);
		logTable->setItem(row, 2, messageItem);

		//Set the visibility
		setRowVisibility(row);

		//Make the new message visible
		logTable->scrollToBottom();

		if(mForceProcessEvents)
		{
			qApp->processEvents();
		}

		//Log to the file as well
		//mTextStream << "  <Entry LogLevel=\"" << logLevelAsString << "\" Time=\"" << currentTimeAsString << "\" Message=\"" << message << "\"/>" << endl;
		writeMessageToHTML(message, currentTimeAsString, logLevel);
	}

	void Log::filterMessages(void)
	{
		//TODO: This function can probably be sped up a lot by being intelligent
		//about what has changed and only changing visibility if necessary.

		//Iterate over each row in the table.
		for(int i = 0; i < logTable->rowCount(); ++i)
		{
			setRowVisibility(i);
		}
	}

	void Log::setRowVisibility(int row)
	{
		int type = logTable->item(row, 0)->type();
		const QString& message = logTable->item(row, 2)->text();

		if((type & mVisibleMessageTypes) && (message.contains(filterLineEdit->text(), Qt::CaseInsensitive)))
		{
			logTable->showRow(row);
		}
		else
		{
			logTable->hideRow(row);
		}
	}

	void Log::computeVisibleMessageTypes(void)
	{
		mVisibleMessageTypes = 0;
		if(showDebugButton->isChecked()) { mVisibleMessageTypes |= LL_DEBUG; }
		if(showInformationButton->isChecked()) { mVisibleMessageTypes |= LL_INFO; }
		if(showWarningsButton->isChecked()) { mVisibleMessageTypes |= LL_WARNING; }
		if(showErrorsButton->isChecked()) { mVisibleMessageTypes |= LL_ERROR; }
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

	void Log::writeMessageToHTML(const QString& message, const QString& timeStampAsString, LogLevel logLevel)
	{
		QString colour;
		QString icon;
		switch(logLevel)
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
			<< "<td style=\"width: 75px;\"><span style=\"color: " << colour << ";\">"
			<< timeStampAsString << " - "
			<< "</span></td>"
			<< "<td><span style=\"color: " << colour << ";\">"
			<< message
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
}