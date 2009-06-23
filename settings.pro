ROOT = $$PWD

Debug {
  DEFINES += _DEBUG
  OBJECTS_DIR =  ./build/debug/.obj
  MOC_DIR = ./build/debug/.moc
  RCC_DIR = ./build/debug/.rcc
  UI_DIR = ./build/debug/.ui
} else {
  OBJECTS_DIR =  ./build/debug/.obj
  MOC_DIR = ./build/release/.moc
  RCC_DIR = ./build/release/.rcc
  UI_DIR = ./build/release/.ui
}

CONFIG += debug_and_release
INCLUDEPATH += $$ROOT/QtOgre/include $$ROOT/Demo/include $$ROOT/QtOgre/$$UI_DIR
LIBS += -L$$ROOT/lib

unix {
    # You may need to change this include directory
    INCLUDEPATH += /usr/include/OGRE
    CONFIG += link_pkgconfig
    PKGCONFIG += OGRE
}

win32 {
    # You may need to change this include directory
    OGREHOME = C:\OgreSDK
    Release:LIBS += -L$$OGREHOME\bin\release
    Debug:LIBS += -L$$OGREHOME\bin\debug
    INCLUDEPATH += $$OGREHOME\include
}
