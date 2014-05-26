#-------------------------------------------------
#
# Project created by QtCreator 2014-05-27T00:28:08
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = libPKG
TEMPLATE = lib

DEFINES += LIBPKG_LIBRARY

SOURCES += pkg.cpp

HEADERS += pkg.h\
        libPKG_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE7584C3F
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = libPKG.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
