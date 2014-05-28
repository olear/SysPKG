#include "backend.h"
#include <QDebug>

Backend::Backend(QObject *parent) :
    QObject(parent)
{
    counter = 0;
    moveToThread(&t);
    t.start();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::homePath()+"/.syspkgdb");
    if (db.open()) {
        QSqlQuery query(db);
        if (!query.exec("CREATE  TABLE  IF NOT EXISTS \"main\".\"pkg\" (\"id\" INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , \"name\" TEXT NOT NULL , \"version\" TEXT, \"category\" TEXT, \"comment\" TEXT, \"path\" TEXT UNIQUE , \"depends\" TEXT NOT NULL , \"options\" TEXT NOT NULL , \"updated\" DATETIME DEFAULT CURRENT_TIMESTAMP, \"installed\" INTEGER DEFAULT 0)"))
            qDebug()<<query.lastError();
    }
    else
        qDebug()<<db.lastError();
}

Backend::~Backend()
{
    t.quit();
    t.wait();
    db.close();
}

void Backend::proc_scanned_package(QString input)
{
    counter = counter+1;
    emit scanning_status(counter);
    QStringList list = input.split("|",QString::SkipEmptyParts);
    QTreeWidgetItem *item = new QTreeWidgetItem;
    for (int i = 0; i < list.size(); ++i) {
        item->setText(i,list.at(i));
    }
    add_package_to_database(item->text(0),item->text(1),item->text(2),item->text(3),item->text(4),item->text(5),item->text(6));
}

int Backend::count()
{
    int packages = 0;
    QStringList cats = libpkg.common._available_local_categories();
    for (int i = 0; i < cats.size(); ++i) {
        QStringList pkgs = libpkg.common._available_local_packages(cats.at(i),"");
        for (int y = 0; y < pkgs.size(); ++y)
            packages++;
    }
    return packages;
}

void Backend::scan()
{
    QMetaObject::invokeMethod(this, "doscan");
}

void Backend::doscan()
{
    QStringList cats = libpkg.common._available_local_categories();
    for (int i = 0; i < cats.size(); ++i) {
        QThread* thread = new QThread;
        Common* worker = new Common();
        worker->_scan_category = cats.at(i);
        worker->moveToThread(thread);
        connect(worker,SIGNAL(_scanned_local_package(QString)),this,SLOT(proc_scanned_package(QString)));
        connect(thread,SIGNAL(started()),worker,SLOT(_scan_local_packages()));
        connect(worker,SIGNAL(_scan_local_packages_done()),thread,SLOT(quit()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
}

bool Backend::add_package_to_database(QString name, QString version, QString category, QString path, QString description, QString depends, QString options)
{
    bool status = false;
    if (db.isOpen()) {
        QSqlQuery query(db);
        if (query.exec("INSERT into \"pkg\" (name,version,category,comment,path,depends,options) VALUES (\""+name+"\",\""+version+"\", \""+category+"\", \""+description.replace("\"","")+"\", \""+path+"\", \""+depends+"\", \""+options+"\")")) {
            status = true;
        }
        else {
            qDebug()<<query.lastQuery();
            qDebug()<<query.lastError();
        }
    }
    return status;
}
