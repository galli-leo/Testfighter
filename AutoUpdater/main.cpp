#include "updatingwindow.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QByteArray dyldPath = qgetenv("DYLD_LIBRARY_PATH");
    if(!dyldPath.isEmpty())
        dyldPath.append(":");
    dyldPath.append(QCoreApplication::applicationDirPath());
    qputenv("DYLD_LIBRARY_PATH", dyldPath);
    qDebug() << dyldPath;
    QApplication a(argc, argv);
    QStringList cmdline_args = QCoreApplication::arguments();
    int i = 0;
    UpdatingWindow w;
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
