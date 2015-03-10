#ifndef APPDATA_H
#define APPDATA_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>
#include <QDesktopServices>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class AppData : public QObject
{
    Q_OBJECT
public:
    static AppData* Instance();
    QString appPath(QString path);
    QString appExtension;
    QJsonObject settings;
    QString appDirectory;
    QString osName;
    bool checkForUpdate(QString appName);
    QJsonObject setItem(QJsonObject dict, QString key, QJsonValue value);

signals:

public slots:

private:
    explicit AppData(QObject *parent = 0);
    static AppData* m_pInstance;
    ~AppData();
};

#endif // APPDATA_H
