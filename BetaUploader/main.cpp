#include "mainwindow.h"
#include "uploadwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //CMD options: [dir] [os]
    QStringList args = a.arguments();
    //qDebug() << "args" << args[2];
    if(args.count() > 2){
        //We are launching from cmd
        UploadWindow* uw = new UploadWindow();
        uw->os = args[2];
        uw->dir = args[1];
        uw->show();
        uw->startUpload();
    }else{
        MainWindow w;
        w.show();
    }

    return a.exec();
}
