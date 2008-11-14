#include "LogManager.h"

namespace QtOgre
{
	LogManager::LogManager(QWidget *parent)
		:QDialog(parent,Qt::Tool)
	{
		mGridLayout = new QGridLayout;
		mLogTabs = new QTabWidget;
		mGridLayout->addWidget(mLogTabs);
		setLayout(mGridLayout);

		setModal(false);
	}

	Log* LogManager::createLog(const QString& name)
	{
		Log* mLog = new Log;
		mLogs.insert(name, mLog);
		mLogTabs->addTab(mLog, name);
		return mLog;
	}

	Log* LogManager::getLogByName(const QString& name)
	{
		//We do an explicit check, as otherwise the operator[] will insert an absent key.
		if(mLogs.contains(name))
		{
			return mLogs[name];
		}
		else
		{
			return 0;
		}
	}

	void LogManager::setForceProcessEvents(bool forceProcessEvents)
	{
		foreach (Log* value, mLogs)
			value->setForceProcessEvents(forceProcessEvents);
	}

	void LogManager::setVisibleLog(Log* log)
	{
		mLogTabs->setCurrentWidget(log);
	}
}