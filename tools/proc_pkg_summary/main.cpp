#include <QCoreApplication>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile pkg_summary_file(QDir::homePath()+"/.pkg_summary");
    if (pkg_summary_file.exists()) {
        if (pkg_summary_file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&pkg_summary_file);
            QString txt = stream.readAll();
            pkg_summary_file.close();
            QStringList list = txt.split("\n\n",QString::SkipEmptyParts);
            for (int i = 0; i < list.size(); ++i) {
                QStringList pkg_info = list.at(i).split("\n",QString::SkipEmptyParts);
                for (int y = 0; y < pkg_info.size(); ++y) {
                    if (pkg_info.at(y).contains("PKGPATH=")) {
                        QString output = pkg_info.at(y);
                        qDebug()<<output.replace("PKGPATH=","").simplified();
                    }
                }
            }
        }
    }

    return 0;
}
