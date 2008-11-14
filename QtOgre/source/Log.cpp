#include "Log.h"

#include <QHeaderView>
#include <QTime>

namespace QtOgre
{
	Log::Log(QWidget* parent)
	:QWidget(parent)
	,mForceProcessEvents(false)
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

		backgroundColor.setRgb(0, 0, 0);
		debugColor.setRgb(255, 255, 255);
		infoColor.setRgb(64, 64, 255);
		warningColor.setRgb(255, 255, 64);
		errorColor.setRgb(255,64,64);

		QPalette palette = logTable->palette();
		palette.setColor(QPalette::Active, QPalette::Base, backgroundColor);
		palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
		logTable->setPalette(palette);

		computeVisibleMessageTypes();

		QAbstractItemModel* model = logTable->model();
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
		QTableWidgetItem* iconItem = new QTableWidgetItem(logLevel);
		QTableWidgetItem* timeItem = new QTableWidgetItem(QTime::currentTime().toString("hh:mm:ss") + " -");
		QTableWidgetItem *messageItem = new QTableWidgetItem(message);

		iconItem->setBackgroundColor(backgroundColor);
		timeItem->setBackgroundColor(backgroundColor);
		messageItem->setBackgroundColor(backgroundColor);

		messageItem->setForeground(QBrush(debugColor));
		switch(logLevel)
		{
		case LL_DEBUG:
			iconItem->setIcon(debugIcon);
			timeItem->setForeground(QBrush(debugColor));			
			messageItem->setForeground(QBrush(debugColor));
			break;
		case LL_INFO:
			iconItem->setIcon(infoIcon);
			timeItem->setForeground(QBrush(infoColor));			
			messageItem->setForeground(QBrush(infoColor));
			break;
		case LL_WARNING:
			iconItem->setIcon(warningIcon);
			timeItem->setForeground(QBrush(warningColor));
			messageItem->setForeground(QBrush(warningColor));
			break;
		case LL_ERROR:
			iconItem->setIcon(errorIcon);
			timeItem->setForeground(QBrush(errorColor));
			messageItem->setForeground(QBrush(errorColor));
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
}