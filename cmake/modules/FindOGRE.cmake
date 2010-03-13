# Find OGRE includes and library
#
# This module defines
#  OGRE_INCLUDE_DIRS
#  OGRE_LIBRARIES, the libraries to link against to use OGRE.
#  OGRE_LIBRARY_DIRS, the location of the libraries
#  OGRE_FOUND, If false, do not try to use OGRE
#
# Copyright © 2008, Matt Williams
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

IF (OGRE_LIBRARIES AND OGRE_INCLUDE_DIRS)
	SET(OGRE_FIND_QUIETLY TRUE) # Already in cache, be silent
ENDIF (OGRE_LIBRARIES AND OGRE_INCLUDE_DIRS)

IF (WIN32) #Windows
	MESSAGE(STATUS "Looking for OGRE")
	SET(OGRESDK $ENV{OGRE_HOME})
	SET(OGRESOURCE $ENV{OGRE_SRC})
	IF (OGRESDK)
		MESSAGE(STATUS "Using OGRE SDK")
		STRING(REGEX REPLACE "[\\]" "/" OGRESDK "${OGRESDK}")
		SET(OGRE_INCLUDE_DIRS ${OGRESDK}/include/Ogre)
		SET(OGRE_LIBRARY_DIRS ${OGRESDK}/lib)
		SET(OGRE_LIBRARIES debug OgreMain_d optimized OgreMain)
	ENDIF (OGRESDK)
	IF (OGRESOURCE)
		MESSAGE(STATUS "Using OGRE built from source")
		SET(OGRE_INCLUDE_DIRS $ENV{OGRE_SRC}/OgreMain/include)
		SET(OGRE_LIBRARY_DIRS $ENV{OGRE_SRC}/lib)
		SET(OGRE_LIBRARIES debug OgreMain_d optimized OgreMain)
	ENDIF (OGRESOURCE)
ELSEIF (APPLE) # Apple, requires environment variable OGRE_HOME pointing to the OgreSDK Folder
    MESSAGE(STATUS "Looking for OGRE")
    SET(OGRESDK $ENV{OGRE_HOME})
    IF(DEFINED OGRESDK AND IS_DIRECTORY ${OGRESDK})
        MESSAGE(STATUS "Found OgreSDK at ${OGRESDK}")
        SET(Ogre_FRAMEWORKS ${OGRESDK}/Dependencies/Ogre.framework)
    ELSE(DEFINED OGRESDK AND IS_DIRECTORY ${OGRESDK})
        MESSAGE(FATAL_ERROR "OGRE_HOME environment variable not set")
    ENDIF(DEFINED OGRESDK AND IS_DIRECTORY ${OGRESDK})
    
    IF (Ogre_FRAMEWORKS)
        SET(OGRE_FOUND TRUE)

        FOREACH (dir ${Ogre_FRAMEWORKS})
            SET(Ogre_FRAMEWORK_INCLUDES ${Ogre_FRAMEWORK_INCLUDES} ${dir}/Headers)
        ENDFOREACH (dir)

        SET (OGRE_INCLUDE_DIRS
            ${OGRE_INCLUDE_DIRS}
            ${Ogre_FRAMEWORK_INCLUDES}
        )
        SET (OGRE_LIBRARY_DIRS
            ${OGRE_LIBRARY_DIRS}
        )
        
        SET(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Ogre_FRAMEWORKS})
    ENDIF (Ogre_FRAMEWORKS)
ELSE (WIN32) #Unix
	CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR) #Needed for VERSION stuff
	FIND_PACKAGE(PkgConfig)
	
	PKG_SEARCH_MODULE(OGRE OGRE) #OGRE_VERSION is set here
	SET(OGRE_INCLUDE_DIRS ${OGRE_INCLUDE_DIRS})
	SET(OGRE_LIBRARY_DIRS ${OGRE_LIBDIR})
	SET(OGRE_LIBRARIES ${OGRE_LIBRARIES} CACHE STRING "")
ENDIF (WIN32)




#Version checking
IF(DEFINED OGRE_FIND_VERSION AND DEFINED OGRE_VERSION)
	IF(${OGRE_VERSION} VERSION_EQUAL ${OGRE_FIND_VERSION})
		#Exact match
		SET(OGRE_VERSION_EXACT TRUE)
		SET(OGRE_VERSION_COMPATIBLE TRUE)
	ELSEIF(${OGRE_VERSION} VERSION_GREATER ${OGRE_FIND_VERSION})
		#Installed version is greater than requested
		SET(OGRE_VERSION_EXACT FALSE)
		SET(OGRE_VERSION_COMPATIBLE TRUE)
	ELSE(${OGRE_VERSION} VERSION_EQUAL ${OGRE_FIND_VERSION})
		#Installed version isn't a high enough version
		SET(OGRE_VERSION_EXACT FALSE)
		SET(OGRE_VERSION_COMPATIBLE FALSE)
	ENDIF(${OGRE_VERSION} VERSION_EQUAL ${OGRE_FIND_VERSION})
ELSE(DEFINED OGRE_FIND_VERSION AND DEFINED OGRE_VERSION)
	#It may well be that we fail to determine what version we have (this is particularly hard on Windows)
	#This is not an error, so we allow the build to continue. Alternatively, the user may not have specified
	#which version of Ogre they require. This is also fine (or at least it's their problem!).
	SET(OGRE_VERSION_COMPATIBLE TRUE)
ENDIF(DEFINED OGRE_FIND_VERSION AND DEFINED OGRE_VERSION)

#Do some preparation
SEPARATE_ARGUMENTS(OGRE_INCLUDE_DIRS)
SEPARATE_ARGUMENTS(OGRE_LIBRARIES)

SET(OGRE_INCLUDE_DIRS ${OGRE_INCLUDE_DIRS} CACHE PATH "")
SET(OGRE_LIBRARIES ${OGRE_LIBRARIES} CACHE STRING "")
SET(OGRE_LIBRARY_DIRS ${OGRE_LIBRARY_DIRS} CACHE PATH "")

IF (OGRE_INCLUDE_DIRS AND OGRE_LIBRARIES)
	SET(OGRE_FOUND TRUE)
ELSE(OGRE_INCLUDE_DIRS AND OGRE_LIBRARIES)
	SET(OGRE_FOUND FALSE)
ENDIF (OGRE_INCLUDE_DIRS AND OGRE_LIBRARIES)

IF(NOT OGRE_VERSION_COMPATIBLE)
	MESSAGE(STATUS "  Could not find a compatible version of OGRE. Needs ${OGRE_FIND_VERSION} but found ${OGRE_VERSION}")
	SET(OGRE_FOUND FALSE)
ENDIF(NOT OGRE_VERSION_COMPATIBLE)

IF (OGRE_FOUND)
	IF (NOT OGRE_FIND_QUIETLY)
		MESSAGE(STATUS "  libraries : ${OGRE_LIBRARIES} from ${OGRE_LIBRARY_DIRS}")
		MESSAGE(STATUS "  includes  : ${OGRE_INCLUDE_DIRS}")
	ENDIF (NOT OGRE_FIND_QUIETLY)
ELSE (OGRE_FOUND)
	IF (OGRE_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find OGRE")
	ENDIF (OGRE_FIND_REQUIRED)
ENDIF (OGRE_FOUND)
