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
    docsDirectory = QStandardPaths::writableLocation( QStandardPaths::GenericDataLocation )+ "/Testfighter/Apps/";


#elif defined(Q_OS_WIN32)
    appExtension = ".exe";
    osName = "win";
    appDirectory = QCoreApplication::applicationFilePath() + "Apps/";


#elif defined(Q_OS_LINUX)
    osName = "linux";
    appDirectory = QCoreApplication::applicationFilePath() + "Apps/";
#endif

    QDir dir(docsDirectory);
    dir.mkpath(docsDirectory);
}


QString AppData::executablePath(QString path)
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


bool AppData::checkForUpdate(QString appName)
{
    QString localHash = AppData::Instance()->settings["hash"].toString();
    QString onlineHash = AppData::getStringResponse("hash.php?app=" + appName + "&os=" + this->osName);

    if(onlineHash != localHash)
    {
        QString programm = this->executablePath("AutoUpdater");
        QStringList arguments = QStringList();
        QDir dir = QDir();
        QString path = dir.absolutePath().replace(appName + ".app/Contents/MacOS", "");
        QChar car('/');
        if(path.at(path.length()-1) != car){
            path.append("/");
        }
        arguments << appName + ".zip" << path;
        qDebug() << arguments;
        this->settings = setItem(this->settings, "hash", onlineHash);
        QFile file("settings.json");
        if(!file.open(QIODevice::ReadWrite)){
            qDebug() << "Failed to read settings.json!";
        }
        QJsonDocument doc = QJsonDocument(this->settings);
        file.resize(0);
        file.write(doc.toJson());
        file.close();
        QProcess::startDetached(programm, arguments);
        return true;
    }
    return false;
}

QJsonObject AppData::setItem(QJsonObject dict, QString key, QJsonValue value)
{
    QJsonObject newDict;
    foreach(QString keyDict, dict.keys())
    {
        if(keyDict == key)
        {
            newDict[key] = value;
        }
        else
        {
            newDict[keyDict] = dict[keyDict];
        }
    }
    return newDict;
}

QString AppData::getStringResponse(QString endPoint){
    QNetworkAccessManager *networkMgr = new QNetworkAccessManager();
    QNetworkReply *reply = networkMgr->get( QNetworkRequest( QUrl( AppData::Instance()->settings["url"].toString() + endPoint ) ) );
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();

    QString response = QString(reply->readAll());
    return response;
}

QJsonObject AppData::getJsonResponse(QString endPoint){
    QString response = AppData::getStringResponse(endPoint);

    QJsonObject obj = QJsonDocument::fromVariant(response).object();
    return obj;
}

QNetworkReply AppData::getStringResponseAsync(QString endPoint){

}
