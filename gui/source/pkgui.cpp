#include "include/pkgui.h"
#include "ui_pkgui.h"
#include <QDebug>

PKGui::PKGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PKGui)
{
    ui->setupUi(this);
    qDebug() << pkg._db();
    qDebug() << pkg._home();
    qDebug() << pkg._make();
    qDebug() << pkg._tmp();
    qDebug() << pkg._tools();
    qDebug() << pkg._installed();
    qDebug() << pkg._info("gimp","");
    qDebug() << pkg._info("joe","editors");
}

PKGui::~PKGui()
{
    delete ui;
}
