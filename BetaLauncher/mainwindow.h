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
#include <QDirIterator>
#include <QProcess>
#include <QDebug>
#include <QCryptographicHash>
#include <QMessageBox>
#include "downloadmanager.h"
#include "filedownloader.h"
#include "appdata.h"

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
    void hashDownloaded();
    void downloadManagerFinished();
    QString hash(QString file);
private:
    Ui::MainWindow *ui;
    FileDownloader* fileDownloader;
    FileDownloader* hashDownloader;
    QNetworkAccessManager m_WebCtrl;
    QJsonObject list;
    bool isInstalled(QString item);
    QStringList needsUpdateList;
    QStringList times;
    QByteArray dlListData;
    void install(bool updating, QString item);
    void afterInstall(QString item);
};

#endif // MAINWINDOW_H
