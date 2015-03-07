#include "appdata.h"
AppData* AppData::m_pInstance = NULL;

AppData* AppData::Instance()
{
    if(!m_pInstance)
    {
        m_pInstance = new AppData;
    }

    return m_pInstance;
}

AppData::AppData(QObject *parent) : QObject(parent)
{
    //Load options.json
    QFile file("settings.json");
    if(!file.open(QIODevice::ReadWrite)){
        qDebug() << "Failed to read settings.json!";
    }
    //qDebug() << file.readAll();
    QByteArray arr = file.readAll();
    qDebug() << "arr: " << QString(arr);
    QJsonDocument loadDoc = QJsonDocument::fromJson(arr);
    settings = loadDoc.object();

#ifdef Q_OS_MAC
    appExtension = ".app";
    osName = "osx";
    appDirectory = QStandardPaths::writableLocation( QStandardPaths::GenericDataLocation )+ "/Testfighter/Apps/";


#elif defined(Q_OS_WIN32)
    appExtension = ".exe";
    osName = "win";
    appDirectory = QCoreApplication::applicationFilePath() + "Apps/";


#elif defined(Q_OS_LINUX)
    osName = "linux";
    appDirectory = QCoreApplication::applicationFilePath() + "Apps/";
#endif

    QDir dir(appDirectory);
    dir.mkpath(appDirectory);
}

QString AppData::appPath(QString path)
{
    QString apppath = "";
    if(path.at(path.length()-1)=='/')
    {
        path.resize(path.length()-1);
    }

    QString appName = path.split("/").last();

#ifdef Q_OS_MAC
    apppath = path+".app/Contents/MacOS/" + appName;

#elif defined(Q_OS_WIN32)
    apppath = path+".exe";
#endif
    return apppath;
}

AppData::~AppData()
{

}

