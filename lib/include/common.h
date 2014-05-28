#ifndef COMMON_H
#define COMMON_H

#include <QObject>
#include <QStringList>
#include <QProcess>
#include <QProcessEnvironment>
#include <QFile>
#include <QDir>
#include <QTextStream>

class Common : public QObject
{
    Q_OBJECT
public:
    explicit Common(QObject *parent = 0);
    QString _scan_category;
signals:
    void _scanned_local_package(QString result);
    void _scan_local_packages_done();
public slots:
    QString _tmp();
    QString _home();
    QString _db();
    QString _tools();
    QString _make();
    QStringList _info(QString pkg, QString cat);
    QStringList _installed_packages();
    QStringList _available_local_packages(QString cat, QString search);
    QStringList _available_local_categories();
    void _scan_local_packages();
};

#endif // COMMON_H
