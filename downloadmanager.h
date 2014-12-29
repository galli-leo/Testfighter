#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
//#include <QMessageBox>
#include "filedownloader.h"
#include "mainwindow.h"
class DownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = 0);
    void addItem(QUrl);
    void addPath(QString path);
    void start();
signals:
    void progress(QString remainingTime, QString Speed, int percentage);
    void finished();
public slots:

private slots:
    void downloadProg(qint64 bytesReceived, qint64 bytesTotal);
    void headersFinished(QNetworkReply* reply);
    void downloadFinished();
private:
    QList<QUrl> downloadingItems;
    int currentDownloadIndex;
    qint64 totalSize;
    qint64 dlSize;
    int time;
    void startDownloading();
    int currentHeaderIndex;
    const char* fromQString(QString string);
    qint64 lastDlSize;
    QString readableTime(int secconds);
    QString plural(int times, QString base);
    int times;
    QString niceSpeed(qint64 speed);
    double round(double n, unsigned d);
    QStringList downloadingPaths;

};

#endif // DOWNLOADMANAGER_H
