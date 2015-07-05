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
#include <QJsonValueRef>
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
    void listDownloaded();
    void handleButton();
    void downloadProgCalc(QString remainingTime, QString Speed, int percentage);
    void selectedChange(QString item);
    void hashDownloaded();
    void downloadManagerFinished();
    void launchFinished(int exitCode, QProcess::ExitStatus exitStatus);
    QString hash(QString file);
    void feedDownloaded();
private:
    Ui::MainWindow *ui;
    FileDownloader* listDownloader;
    FileDownloader* hashDownloader;
    FileDownloader* feedDownloader;
    QNetworkAccessManager m_WebCtrl;
    QJsonObject list;
    bool isInstalled(QString item);
    QStringList needsUpdateList;
    QHash<QString,QString> times;
    QByteArray dlListData;
    void install(bool updating, QString item);
    void afterInstall(QString item);
    void launch();
    QString appDir;
    QProcess* launchProc;
};

#endif // MAINWINDOW_H
