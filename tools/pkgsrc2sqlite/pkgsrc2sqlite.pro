#-------------------------------------------------
#
# Project created by QtCreator 2014-05-28T02:58:43
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pkgsrc2sqlite
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    backend.cpp \

HEADERS  += mainwindow.h \
    backend.h \

FORMS    += mainwindow.ui

DESTDIR                                      = build
OBJECTS_DIR                                  = $${DESTDIR}/.obj
MOC_DIR                                      = $${DESTDIR}/.moc
QMAKE_CLEAN                                 += -r $${DESTDIR} Makefile

INCLUDEPATH                         += "../../lib/include"
LIBS                                += -L"../../lib/build"
LIBS+="-lPKG"
