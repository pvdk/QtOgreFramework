CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(QtOgre)

SET(QtOgre_VERSION_MAJOR "0")
SET(QtOgre_VERSION_MINOR "1")
SET(QtOgre_VERSION_PATCH "0")
SET(QtOgre_VERSION "${QtOgre_VERSION_MAJOR}.${QtOgre_VERSION_MINOR}.${QtOgre_VERSION_PATCH}")

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

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include ${QtOgre_BINARY_DIR}/include ${OGRE_INCLUDE_DIRS})
LINK_DIRECTORIES(${OGRE_LIBRARY_DIRS})

ADD_LIBRARY(QtOgre SHARED ${SRC_FILES} ${MOC_SRCS} ${UI_SRCS} ${RESOURCE_SRCS})
TARGET_LINK_LIBRARIES(QtOgre ${QT_LIBRARIES} ${OGRE_LIBRARIES})
SET_TARGET_PROPERTIES(QtOgre PROPERTIES VERSION ${QtOgre_VERSION} SOVERSION ${QtOgre_VERSION_MAJOR})

ADD_LIBRARY(QtOgreStatic ${SRC_FILES} ${MOC_SRCS} ${UI_SRCS} ${RESOURCE_SRCS})
TARGET_LINK_LIBRARIES(QtOgreStatic ${QT_LIBRARIES} ${OGRE_LIBRARIES})
SET_TARGET_PROPERTIES(QtOgreStatic PROPERTIES OUTPUT_NAME "QtOgre")

install(TARGETS QtOgre QtOgreStatic
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

install(FILES ${INC_FILES}
	DESTINATION include
)

#Set up OGREConfig.cmake
if(WIN32)
	set(CONFIG_FILE_DIR "CMake")
else(WIN32)
	set(CONFIG_FILE_DIR "lib/QtOgre/CMake")
endif(WIN32)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QtOgreConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/QtOgreConfig.cmake)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QtOgreConfigVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/QtOgreConfigVersion.cmake)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QtOgreConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/QtOgreConfigVersion.cmake DESTINATION ${CONFIG_FILE_DIR})