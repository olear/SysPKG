# libPKG
# Copyright (c) 2014 Ole Andre Rodlie <olear@dracolinux.org>. All rights reserved.
# libPKG is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 2.1.

isEmpty(PREFIX) {
    PREFIX                                   = /usr/local
}

isEmpty(LIBDIR) {
    LIBDIR                                   = $${PREFIX}/lib$${LIBSUFFIX}
}

isEmpty(INCLUDEDIR) {
    INCLUDEDIR                               = $${PREFIX}/include
}

isEmpty(DOCDIR) {
    DOCDIR                                   = $$PREFIX/share/doc
}

QT                                          += network
QT                                          -= gui
VERSION                                      = 1.0.0
TARGET                                       = PKG
TEMPLATE                                     = lib
DEFINES                                     += LIBPKG_LIBRARY
SOURCES                                     += source/pkg.cpp
HEADERS                                     += include/pkg.h include/libPKG_global.h
DESTDIR                                      = build
OBJECTS_DIR                                  = $${DESTDIR}/.obj
MOC_DIR                                      = $${DESTDIR}/.moc

target.path                                  = $${LIBDIR}
target_docs.path                             = $${DOCDIR}/$${TEMPLATE}$${TARGET}-$${VERSION}
target_docs.files                            = docs/README docs/COPYING.LIB
target_include.path                          = $${INCLUDEDIR}
target_include.files                         = $${HEADERS}

INSTALLS                                    += target target_docs target_include
QMAKE_CLEAN                                 += -r $${DESTDIR} Makefile
