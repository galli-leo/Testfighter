#ifndef APPDATA_H
#define APPDATA_H

#include <QObject>

class AppData : public QObject
{
    Q_OBJECT
public:
    static AppData* Instance();
    QString appPath(QString path);
    QString appExtension;

signals:

public slots:

private:
    explicit AppData(QObject *parent = 0);
    ~AppData();
};

#endif // APPDATA_H
