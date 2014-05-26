#include <QApplication>
#include "include/pkgui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PKGui w;
    w.show();
    
    return a.exec();
}
