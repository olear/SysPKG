#ifndef PKGUI_H
#define PKGUI_H

#include <QMainWindow>
#include <pkg.h>

namespace Ui {
class PKGui;
}

class PKGui : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PKGui(QWidget *parent = 0);
    ~PKGui();
    
private:
    Ui::PKGui *ui;
    PKG pkg;
};

#endif // PKGUI_H
