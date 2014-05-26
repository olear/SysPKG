/*
# libPKG
# Copyright (c) 2014 Ole Andre Rodlie <olear@dracolinux.org>. All rights reserved.
# libPKG is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 2.1.
*/
#ifndef PKG_H
#define PKG_H

#include "libPKG_global.h"
#include <QStringList>
#include <QProcess>
#include <QProcessEnvironment>
#include <QFile>
#include <QDir>

class LIBPKGSHARED_EXPORT PKG
{
public:
    PKG();
public slots:
    QString _tmp();
    QString _home();
    QString _db();
    QString _tools();
    QString _make();
    QStringList _installed();
    QString _info(QString pkg, QString cat);
};

#endif // PKG_H
