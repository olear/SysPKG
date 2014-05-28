#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <pkg.h>
#include <QTimer>
#include <QDebug>
#include <backend.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void scanning_status(int status);
private:
    Ui::MainWindow *ui;
    PKG libpkg;
    Backend work;
};

#endif // MAINWINDOW_H
