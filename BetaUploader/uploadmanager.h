#ifndef UPLOADMANAGER_H
#define UPLOADMANAGER_H
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QSysInfo>
#include <QDirIterator>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <math.h>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
class UploadManager : public QObject
{
    Q_OBJECT
signals:
    void uploadProg(QString ulSpeed, QString timeRemaining, float percentage);
    void uploadFinished();

public:
    explicit UploadManager(QObject *parent = 0);
    void addItem(QString file);
    void addPath(QString path);
    void start();
private:
    QStringList itemsToUpload;
    QStringList paths;
    int startTime;
    qint64 totalSize;
    qint64 UlSize;
    QString readableTime(int secconds);
    QString plural(int times, QString base);
    int times;
    QString niceSpeed(qint64 speed);
    double round(double n, unsigned d);
    int currentUploadIndex;
    qint64 lastUlSize;
    QFile* currentFile;
    void startUpload(int index);
    QNetworkReply* currentReply;
    double time;
    QNetworkAccessManager *networkManager;
    double lastTime;
    double totalTime;
    qint64 lastUlNice;
    double lastTimeNice;
private slots:
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void finished();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void update();
};

#endif // UPLOADMANAGER_H
