#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(work.count());
    connect(&work,SIGNAL(scanning_status(int)),this,SLOT(scanning_status(int)));
    work.scan();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::scanning_status(int status)
{
    ui->progressBar->setValue(status);
}

