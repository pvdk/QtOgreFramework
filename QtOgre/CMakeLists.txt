CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(QtOgre)

SET(SRC_FILES
	source/AbstractSettingsWidget.cpp
	source/Application.cpp
	source/EventHandlingOgreWidget.cpp
	source/FPSDialog.cpp
	source/GameLogic.cpp
	source/GraphicsSettingsWidget.cpp
	source/LogManager.cpp
	source/Log.cpp
	source/OgreWidget.cpp
	source/SettingsDialog.cpp
)

SET(INC_FILES
	include/AbstractSettingsWidget.h
	include/Application.h
	include/EventHandler.h
	include/EventHandlingOgreWidget.h
	include/FPSDialog.h
	include/GameLogic.h
	include/GraphicsSettingsWidget.h
	include/LogManager.h
	include/Log.h
	include/OgreWidget.h
	include/SettingsDialog.h
)

SET(UI_FILES
	ui/FPSDialog.ui
	ui/GraphicsSettingsWidget.ui
	ui/Log.ui
	ui/SettingsDialog.ui
)

SET(RESOURCE_FILES
	resources/resources.qrc
)

#"Sources" and "Headers" are the group names in Visual Studio.
SOURCE_GROUP("Sources" FILES ${SRC_FILES})
SOURCE_GROUP("Headers" FILES ${INC_FILES})

FIND_PACKAGE(Qt4)
#SET(QT_USE_QTXML 1)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_CPP(MOC_SRCS ${INC_FILES})
QT4_WRAP_UI(UI_SRCS ${UI_FILES})
QT4_ADD_RESOURCES(RESOURCE_SRCS ${RESOURCE_FILES})

SET(CMAKE_DEBUG_POSTFIX "_d")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include ${OGRE_INCLUDE_DIR})
LINK_DIRECTORIES(${OGRE_LIB_DIR})
ADD_LIBRARY(QtOgre ${SRC_FILES} ${INC_FILES} ${MOC_SRCS} ${UI_SRCS} ${RESOURCE_SRCS})
TARGET_LINK_LIBRARIES(QtOgre ${QT_LIBRARIES} ${OGRE_LIBRARIES})