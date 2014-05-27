#-------------------------------------------------
#
# Project created by QtCreator 2014-05-27T01:10:17
#
#-------------------------------------------------

isEmpty(PREFIX) {
    PREFIX                                   = /usr/local
}

isEmpty(BINDIR) {
    BINDIR                                   = $${PREFIX}/bin
}

isEmpty(DOCDIR) {
    DOCDIR                                   = $$PREFIX/share/doc
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PKGui
VERSION=1.0.0
TEMPLATE = app


SOURCES += source/main.cpp\
        source/pkgui.cpp

HEADERS  += include/pkgui.h

FORMS    += forms/pkgui.ui

DESTDIR                                      = build
OBJECTS_DIR                                  = $${DESTDIR}/.obj
MOC_DIR                                      = $${DESTDIR}/.moc

target.path                                  = $${BINDIR}
target_docs.path                             = $${DOCDIR}/$${TARGET}-$${VERSION}
target_docs.files                            = docs/README docs/COPYING

INSTALLS                                    += target target_docs
QMAKE_CLEAN                                 += -r $${DESTDIR} Makefile

INCLUDEPATH                         += "../lib/include"
LIBS                                += -L"../lib/build"
LIBS+="-lPKG"
