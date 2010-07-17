CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(EngineTest)

SET(SRC_FILES
	source/CameraWrapper.cpp
	source/Globals.cpp
	source/Light.cpp
	source/main.cpp
	source/Mouse.cpp
	source/EngineTestGameLogic.cpp
	source/Keyboard.cpp
	source/ObjectStore.cpp
	source/ScriptEditorWidget.cpp
)

SET(INC_FILES
	include/CameraWrapper.h
	include/EngineTestGameLogic.h
	include/Globals.h
	include/Keyboard.h
	include/Light.h
	include/Mouse.h
	include/ObjectStore.h
	include/ScriptEditorWidget.h
)

SET(UI_FILES
	ui/ScriptEditorWidget.ui
)

SET(RESOURCE_FILES
	resources/EngineTestResources.qrc
)

#"Sources" and "Headers" are the group names in Visual Studio.
SOURCE_GROUP("Sources" FILES ${SRC_FILES})
SOURCE_GROUP("Headers" FILES ${INC_FILES})

FIND_PACKAGE(Qt4)
SET(QT_USE_QTGUI 1)
SET(QT_USE_QTSCRIPT 1)
SET(QT_USE_QTSCRIPTTOOLS 1)
SET(QT_USE_QTXML 1)
INCLUDE(${QT_USE_FILE})

QT4_WRAP_CPP(MOC_SRCS ${INC_FILES})
QT4_WRAP_UI(UI_SRCS ${UI_FILES})
QT4_ADD_RESOURCES(RESOURCE_SRCS ${RESOURCE_FILES})

SET(CMAKE_DEBUG_POSTFIX "_d")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/../QtOgre/include ${EngineTest_BINARY_DIR} ${EngineTest_BINARY_DIR}/include ${QtOgre_BINARY_DIR} ${OGRE_INCLUDE_DIRS})
LINK_DIRECTORIES(${OGRE_LIBRARY_DIRS} ${QtOgre_BINARY_DIR})
ADD_EXECUTABLE(EngineTest ${SRC_FILES} ${MOC_SRCS} ${UI_SRCS} ${RESOURCE_SRCS})
TARGET_LINK_LIBRARIES(EngineTest ${QT_LIBRARIES} ${OGRE_LIBRARIES} QtOgreStatic)

ADD_CUSTOM_TARGET(copyenginetestmedia ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/media ${CMAKE_CURRENT_BINARY_DIR}/media COMMENT "Copying media files to build directory" VERBATIM)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/settings.ini ${CMAKE_CURRENT_BINARY_DIR}/settings.ini COPYONLY)
IF(WIN32)
	SET(OGRE_RELEASE_PLUGIN_FOLDER ${OGRESDK}/bin/release)
	SET(OGRE_DEBUG_PLUGIN_FOLDER ${OGRESDK}/bin/debug)
ELSE()
	SET(OGRE_RELEASE_PLUGIN_FOLDER ${OGRE_LIBRARY_DIRS}/OGRE)
	SET(OGRE_DEBUG_PLUGIN_FOLDER ${OGRE_LIBRARY_DIRS}/OGRE)
ENDIF()
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/plugins.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/plugins_d.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/plugins_d.cfg)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/resources.cfg ${CMAKE_CURRENT_BINARY_DIR}/resources.cfg COPYONLY)

IF (WIN32) #Windows
#CONFIGURE_FILE(${OGRE_LIBRARY_DIRS}/OgreMain_d.dll ${CMAKE_CURRENT_BINARY_DIR}/debug/OgreMain_d.dll COPYONLY)
#CONFIGURE_FILE(${OGRE_LIBRARY_DIRS}/RenderSystem_GL_d.dll ${CMAKE_CURRENT_BINARY_DIR}/debug/RenderSystem_GL_d.dll COPYONLY)
#CONFIGURE_FILE(${OGRE_LIBRARY_DIRS}/RenderSystem_Direct3D9_d.dll ${CMAKE_CURRENT_BINARY_DIR}/debug/RenderSystem_Direct3D9_d.dll COPYONLY)

#CONFIGURE_FILE(${OGRE_LIBRARY_DIRS}/OgreMain.dll ${CMAKE_CURRENT_BINARY_DIR}/release/OgreMain.dll COPYONLY)
#CONFIGURE_FILE(${OGRE_LIBRARY_DIRS}/RenderSystem_GL.dll ${CMAKE_CURRENT_BINARY_DIR}/release/RenderSystem_GL.dll COPYONLY)
#CONFIGURE_FILE(${OGRE_LIBRARY_DIRS}/RenderSystem_Direct3D9.dll ${CMAKE_CURRENT_BINARY_DIR}/release/RenderSystem_Direct3D9.dll COPYONLY)
ENDIF (WIN32)


