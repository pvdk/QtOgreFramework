# This QMake file is not part of the official build system, and may
# not work. The official build system is based on CMake (not QMake). 
# Please see www.cmake.org and the CMakeLists.txt file.

# I can't support using the QMake system, but if you make any
# changes to improve it then I will be happy to include them.


exists(../settings.pro):include(../settings.pro)
else:message("I can't find settings file")

QT += gui xml

CONFIG += shared create_prl

VERSION = 0.1.0
TARGET = qtogre
TEMPLATE = lib
DESTDIR = $$ROOT/lib
DLLDESTDIR = $$ROOT/Demo

CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
  LIBS *= -lOgreMain_d
}

CONFIG(release, debug|release) {
  LIBS *= -lOgreMain
}

SOURCES += 	source/AbstractSettingsWidget.cpp \
	source/Application.cpp \
	source/DotSceneHandler.cpp \
	source/EventHandlingOgreWidget.cpp \
	source/FPSDialog.cpp \
	source/GameLogic.cpp \
	source/GraphicsSettingsWidget.cpp \
	source/Log.cpp \
	source/LogEntry.cpp \
	source/LogManager.cpp \
	source/LogModel.cpp \
	source/LogModelSortFilterProxy.cpp \
	source/OgreWidget.cpp \
	source/SettingsDialog.cpp 

HEADERS += 	include/AbstractSettingsWidget.h \
	include/Application.h \
	include/DotSceneHandler.h \
	include/EventHandler.h \
	include/EventHandlingOgreWidget.h \
	include/FPSDialog.h \
	include/GameLogic.h \
	include/GraphicsSettingsWidget.h \
	include/Log.h \
	include/LogEntry.h \
	include/LogLevel.h \
	include/LogManager.h \
	include/LogModel.h \
	include/LogModelSortFilterProxy.h \
	include/OgreWidget.h \
	include/SettingsDialog.h

FORMS += 	ui/FPSDialog.ui \
	ui/GraphicsSettingsWidget.ui \
	ui/Log.ui \
	ui/SettingsDialog.ui

RESOURCES += resources/resources.qrc
