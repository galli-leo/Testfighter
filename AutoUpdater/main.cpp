#include "updatingwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QStringList cmdline_args = QCoreApplication::arguments();
    int i = 0;
    UpdatingWindow w;
    //CMD options: [fileToDownload] [pathToDowload]
    w.fileToDownload = "Testfighter.zip";
    w.pathToDownload = "L:/Github/Testfighter/build-BetaLauncher-Desktop_Qt_5_4_1_MSVC2013_64bit-Debug/debug/";
    foreach(QString str, cmdline_args)
    {
        qDebug() << str;
        if(i==1)
        {
            w.fileToDownload = str;
        }
        if(i==2)
        {
            w.pathToDownload = str;
        }
        i++;
    }
    w.startDownload();
    w.show();

    return a.exec();
}
