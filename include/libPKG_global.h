#ifndef LIBPKG_GLOBAL_H
#define LIBPKG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBPKG_LIBRARY)
#  define LIBPKGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBPKGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBPKG_GLOBAL_H
