#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QSysInfo>
#include "downloadmanager.h"
#include "filedownloader.h"

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
    void fileDownloaded();
    void handleButton();
    void downloadProgCalc(QString remainingTime, QString Speed, int percentage);
    void selectedChange(QString item);
private:
    Ui::MainWindow *ui;
    FileDownloader* fileDownloader;
    QNetworkAccessManager m_WebCtrl;
    QJsonObject list;
    void install(bool updating, QString item);
};

#endif // MAINWINDOW_H
