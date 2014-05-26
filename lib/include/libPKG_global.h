/*
# libPKG
# Copyright (c) 2014 Ole Andre Rodlie <olear@dracolinux.org>. All rights reserved.
# libPKG is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 2.1.
*/

#ifndef LIBPKG_GLOBAL_H
#define LIBPKG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBPKG_LIBRARY)
#  define LIBPKGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBPKGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBPKG_GLOBAL_H
