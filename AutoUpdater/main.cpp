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
    w.fileToDownload = "BetaLauncher.zip";
    w.pathToDownload = "/Users/leonardogalli/beta/";
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
