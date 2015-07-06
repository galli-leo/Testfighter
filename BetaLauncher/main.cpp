#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(AppData::Instance()->checkForUpdate("Testfighter"))
    {
        return 0;
    }

        MainWindow w;
        w.show();

    qDebug() << "no update available";

    return a.exec();
}
