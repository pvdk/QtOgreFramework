#ifndef QTOGRE_LOGLEVEL_H_
#define QTOGRE_LOGLEVEL_H_

namespace QtOgre
{
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
}

#endif