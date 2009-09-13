# This QMake file is not part of the official build system, and may
# not work. The official build system is based on CMake (not QMake). 
# Please see www.cmake.org and the CMakeLists.txt file.

# I can't support using the QMake system, but if you make any
# changes to improve it then I will be happy to include them.

exists(../settings.pro):include(../settings.pro)
else:message("I can't find settings file")

QT += gui xml
config += share link_prl
TEMPLATE = app
TARGET = Demo
DESTDIR = $$ROOT/Demo

CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
  LIBS *= -lOgreMain_d -lQtOgred
}
CONFIG(release, debug|release) {
  LIBS *= -lOgreMain -lQtOgre
}

HEADERS += include/ChooseMeshWidget.h include/DemoGameLogic.h include/MainMenu.h
FORMS += ui/ChooseMeshWidget.ui ui/MainMenu.ui
SOURCES += source/ChooseMeshWidget.cpp \
           source/DemoGameLogic.cpp \
           source/main.cpp \
           source/MainMenu.cpp
RESOURCES += resources/DemoResources.qrc
