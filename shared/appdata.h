#ifndef APPDATA_H
#define APPDATA_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

class AppData : public QObject
{
    Q_OBJECT
public:
    static AppData* Instance();
    QString appPath(QString path);
    QString appExtension;
    QJsonObject settings;

signals:

public slots:

private:
    explicit AppData(QObject *parent = 0);
    static AppData* m_pInstance;
    ~AppData();
};

#endif // APPDATA_H
