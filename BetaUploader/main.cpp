#include "mainwindow.h"
#include "uploadwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //CMD options: [dir] [os] [metadata e.g: dexcription=This is a test&bUsesSteam=true]
    QStringList args = a.arguments();
    //qDebug() << "args" << args[2];
    if(args.count() > 3){
        //We are just on cmd
        QString os = args[2];
        QString dir = args[1];
        QStringList meta = args[3].split("&");
        QHash<QString,QString> metadata;
        foreach(QString m, meta){
            QStringList tmp = m.split("=");
            metadata.insert(tmp.at(0), tmp.at(1));
        }
        qDebug() << metadata;
        UploadWindow* uw = new UploadWindow();
        uw->os = args[2];
        uw->dir = args[1];
        uw->cli = true;
        uw->show();
        uw->startUpload();
        foreach(QString key, metadata.keys())
        {
            QString value = metadata[key];
            uw->setField(key, value);
        }
        //uw->buildSubmit();


    }else if(args.count() > 2){
        //We are launching from cmd but will show ui (for yosemite)
        UploadWindow* uw = new UploadWindow();
        uw->os = args[2];
        uw->dir = args[1];
        uw->show();
        uw->startUpload();
    }else{

        //w.setParent(a);
        w.show();
    }


    return a.exec();
}

