#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QThread>
#include <pkg.h>
#include <QtSql>
#include <QTreeWidgetItem>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = 0);
    ~Backend();
    QThread t;
    PKG libpkg;
    int counter;
    QSqlDatabase db;
signals:
    void scanning_status(int package);
public slots:
    int count();
    void proc_scanned_package(QString input);
    void doscan();
    void scan();
    bool add_package_to_database(QString name, QString version, QString category, QString path, QString description, QString depends, QString options);
};

#endif // BACKEND_H
