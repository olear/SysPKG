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
