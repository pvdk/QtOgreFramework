exists(../settings.pro):include(../settings.pro)
else:message("I can't find settings file")

QT += gui xml
config += share link_prl
TEMPLATE = app
TARGET = Demo
DESTDIR = $$ROOT/Demo

CONFIG(debug, debug|release) {
  TARGET = $$join(TARGET,,,d)
  LIBS *= -lOgreMain_d -lqtogred
}
CONFIG(release, debug|release) {
  LIBS *= -lOgreMain -lqtogre
}

HEADERS += include/ChooseMeshWidget.h include/DemoGameLogic.h include/MainMenu.h
FORMS += ui/ChooseMeshWidget.ui ui/MainMenu.ui
SOURCES += source/ChooseMeshWidget.cpp \
           source/DemoGameLogic.cpp \
           source/main.cpp \
           source/MainMenu.cpp
RESOURCES += resources/DemoResources.qrc
