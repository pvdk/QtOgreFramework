CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(QtOgreFramework)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
FIND_PACKAGE(OGRE 1.6 REQUIRED) #Ogre is needed by almost everything

ADD_SUBDIRECTORY(QtOgre)
ADD_SUBDIRECTORY(Demo)
ADD_SUBDIRECTORY(EngineTest)
