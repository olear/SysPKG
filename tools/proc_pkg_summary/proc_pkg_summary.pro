#-------------------------------------------------
#
# Project created by QtCreator 2014-05-28T20:36:14
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = proc_pkg_summary
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

DESTDIR                                      = build
OBJECTS_DIR                                  = $${DESTDIR}/.obj
MOC_DIR                                      = $${DESTDIR}/.moc
QMAKE_CLEAN                                 += -r $${DESTDIR} Makefile
