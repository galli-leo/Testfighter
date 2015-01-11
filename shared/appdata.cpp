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
    file.open(QIODevice::ReadWrite);
    QJsonDocument loadDoc = QJsonDocument::fromJson(file.readAll());
    settings = loadDoc.object();

#ifdef Q_OS_MAC
    appExtension = ".app";
    appDirectory = QStandardPaths::writableLocation( QStandardPaths::GenericDataLocation )+ "/Testfighter/Apps/";


#elif Q_OS_WIN32
    appExtension = ".exe";
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

#elif Q_OS_WIN32
    apppath = path+".exe";
#endif
    return apppath;
}

AppData::~AppData()
{

}

