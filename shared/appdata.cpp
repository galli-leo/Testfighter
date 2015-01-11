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

#ifdef Q_OS_MAC
    appExtension = ".app";
#elif Q_OS_WIN32
    appExtension = ".exe";
#endif
}

QString AppData::appPath(QString path)
{
    QString apppath = "";
    if(apppath.at(apppath.length()-1)=="/")
    {
        apppath.reserve(apppath.length()-1);
    }
    QString appName = path.split("/").last;

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

