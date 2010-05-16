CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(QtOgre)

SET(QtOgre_VERSION_MAJOR "0")
SET(QtOgre_VERSION_MINOR "1")
SET(QtOgre_VERSION_PATCH "0")
SET(QtOgre_VERSION "${QtOgre_VERSION_MAJOR}.${QtOgre_VERSION_MINOR}.${QtOgre_VERSION_PATCH}")

SET(SRC_FILES
	source/AbstractSettingsWidget.cpp
	source/Application.cpp
	source/DotSceneHandler.cpp
	source/EventHandlingOgreWidget.cpp
	source/FPSDialog.cpp
	source/GameLogic.cpp
	source/GraphicsSettingsWidget.cpp
	source/Log.cpp
	source/LogEntry.cpp
	source/LogManager.cpp
	source/LogModel.cpp
	source/LogModelSortFilterProxy.cpp
	source/OgreWidget.cpp
	source/SettingsDialog.cpp
	source/Scripting/OgreVector3Class.cpp
	source/Scripting/OgreVector3Prototype.cpp
)

SET(INC_FILES
	include/AbstractSettingsWidget.h
	include/Application.h
	include/DotSceneHandler.h
	include/EventHandler.h
	include/EventHandlingOgreWidget.h
	include/FPSDialog.h
	include/GameLogic.h
	include/GraphicsSettingsWidget.h
	include/Log.h
	include/LogEntry.h
	include/LogLevel.h
	include/LogManager.h
	include/LogModel.h
	include/LogModelSortFilterProxy.h
	include/OgreWidget.h
	include/QtOgreForwardDeclarations.h
	include/SettingsDialog.h
	include/Scripting/OgreVector3Class.h
	include/Scripting/OgreVector3Prototype.h
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
SET(QT_USE_QTGUI 1)
SET(QT_USE_QTSCRIPT 1)
SET(QT_USE_QTXML 1)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_CPP(MOC_SRCS ${INC_FILES})
QT4_WRAP_UI(UI_SRCS ${UI_FILES})
QT4_ADD_RESOURCES(RESOURCE_SRCS ${RESOURCE_FILES})

SET(CMAKE_DEBUG_POSTFIX "_d")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include ${QtOgre_BINARY_DIR} ${OGRE_INCLUDE_DIRS})
LINK_DIRECTORIES(${OGRE_LIBRARY_DIRS})

#NOTE - The shared version of the library has been giving some problems (at least in windows). Sometimes you have
#to build twice before everything works. Or maybe it was that you had to build the static project on it's own, before
#you could build the shared. I forget, but something didn't seem right. Anyway, the nature of the QtOgre framework 
#seems a lot more like something which people will want to include directly into their executable, rather than
#providing Qt and Ogre *and* QtOgre shared libraries. So, for now at least, I'm removing the shared option.
#ADDITIONAL - Actually, maybe there should be a switch to CMake shich specifies whether shared or static should be built?
#ADD_LIBRARY(QtOgre SHARED ${SRC_FILES} ${MOC_SRCS} ${UI_SRCS} ${RESOURCE_SRCS})
#TARGET_LINK_LIBRARIES(QtOgre ${QT_LIBRARIES} ${OGRE_LIBRARIES})
#SET_TARGET_PROPERTIES(QtOgre PROPERTIES VERSION ${QtOgre_VERSION} SOVERSION ${QtOgre_VERSION_MAJOR})

ADD_LIBRARY(QtOgreStatic ${SRC_FILES} ${MOC_SRCS} ${UI_SRCS} ${RESOURCE_SRCS})
TARGET_LINK_LIBRARIES(QtOgreStatic ${QT_LIBRARIES} ${OGRE_LIBRARIES})
SET_TARGET_PROPERTIES(QtOgreStatic PROPERTIES OUTPUT_NAME "QtOgre")

install(TARGETS QtOgreStatic
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

if(WIN32)
	#On windows, we also install the debug information. It's unfortunate that we have to hard-code
	#the 'Debug' part of the path, but CMake doesn't seem to provide a way around this. The best I
	#found was: http://www.cmake.org/pipermail/cmake/2007-October/016924.html (and it is a bit ugly).
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Debug/QtOgre_d.pdb DESTINATION lib CONFIGURATIONS Debug)
endif(WIN32)

if(WIN32)
	set(QtOgre_INCLUDE_INSTALL_DIRS "include")
ELSE()
	set(QtOgre_INCLUDE_INSTALL_DIRS "include/QtOgre")
ENDIF()

install(FILES ${INC_FILES} ${UI_SRCS}
	DESTINATION ${QtOgre_INCLUDE_INSTALL_DIRS}
)

#Set up QtOgreConfig.cmake
if(WIN32)
	set(CONFIG_FILE_DIR "CMake")
else(WIN32)
	set(CONFIG_FILE_DIR "share/QtOgre/cmake")
endif(WIN32)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QtOgreConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/QtOgreConfig.cmake @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/QtOgreConfigVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/QtOgreConfigVersion.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/QtOgreConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/QtOgreConfigVersion.cmake DESTINATION ${CONFIG_FILE_DIR})

find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qt.tag ${CMAKE_CURRENT_BINARY_DIR}/qt.tag COPYONLY)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ogre.tag ${CMAKE_CURRENT_BINARY_DIR}/ogre.tag COPYONLY)
	add_custom_target(doc COMMAND ${DOXYGEN_EXECUTABLE} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} COMMENT "Building documentation" VERBATIM)
endif()
