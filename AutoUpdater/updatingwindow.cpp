#include "updatingwindow.h"
#include "ui_updatingwindow.h"
#include "appdata.h"


UpdatingWindow::UpdatingWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UpdatingWindow)
{
    ui->setupUi(this);
}

UpdatingWindow::~UpdatingWindow()
{
    delete ui;
}

void UpdatingWindow::startDownload()
{

    QDir dir;
    QStringList path = pathToDownload.split("/");
    path.removeLast();
    QString oldPath = path.join("/")+"_old/";

    QString resourcesPath = pathToDownload;
    if(osName() == "win")
    {
        resourcesPath = AppData::Instance()->executablePath(pathToDownload).replace(".exe", "")+ "/";
    }
    QDir old(oldPath);
    dir.mkpath(oldPath);
    if(!dir.rename(resourcesPath+"settings.json", oldPath+"settings.json"))
    {
        qDebug() << "Move failed!" << resourcesPath;
    }
    if(!dir.rename(resourcesPath+"list.json", oldPath+"list.json"))
    {
        qDebug() << "Move failed!" << resourcesPath+"list.json" << oldPath+"list.json";
    }

    connect(&manager, SIGNAL(finished(QNetworkReply*)),
                SLOT(fileDownloaded(QNetworkReply*)));
    writeToFile = new QFile(pathToDownload + fileToDownload);
    writeToFile->open(QIODevice::ReadWrite | QIODevice::Truncate);
    //writeToFile->resize(0);
    QNetworkRequest request(AppData::Instance()->settings["url"].toString() + "download_update.php?file=" + fileToDownload + "&os="+ osName());
    qDebug() << AppData::Instance()->settings["url"].toString() + "download_update.php?file=" + fileToDownload + "&os="+ osName();
   this->reply =  manager.get(request);
    connect(this->reply, SIGNAL(downloadProgress(qint64,qint64)),
                SLOT(downloadProgress(qint64,qint64)));
    connect(this->reply, SIGNAL(readyRead()),
                this, SLOT(readyRead()));
}

void UpdatingWindow::fileDownloaded(QNetworkReply* pReply)
{

    //emit a signal
    pReply->deleteLater();
    writeToFile->close();
    this->ui->label->setText("Unpacking");

    QDir dir;
    QStringList path = pathToDownload.split("/");
    path.removeLast();
    QString oldPath = path.join("/")+"_old/";
    QString exeName = fileToDownload.replace(".zip", "");
    qDebug() << exeName << AppData::Instance()->executablePath(pathToDownload+exeName) << AppData::Instance()->executablePath(pathToDownload+exeName).replace(exeName, "").replace(".app", exeName+".app");

    QString resourcesPath = pathToDownload;
    if(osName() == "win")
    {
        resourcesPath = AppData::Instance()->executablePath(pathToDownload).replace(".exe", "")+ "/";
    }


    QString program;
    QStringList arguments;
    if(osName() == "osx" || osName()=="linux")
    {
        program = "unzip";
        arguments << "-o" << pathToDownload + fileToDownload << "-d " << pathToDownload;
        QProcess::execute("unzip -o " + pathToDownload + fileToDownload + " -d " + pathToDownload);
    }
    else if(osName()== "win")
    {
        program = "7za.exe";
        QString test = QString("-o")+pathToDownload;
        arguments << "x" << pathToDownload + fileToDownload << "-aoa" <<test;
    }




    QProcess *myProcess = new QProcess();
    qDebug() << program;
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(program, arguments);
    //sleep(10);
    myProcess->waitForFinished(-1);
    //qDebug() << myProcess->program() << myProcess->arguments().at(3);
    qDebug() << myProcess->readAll();
    if (QFile::exists(resourcesPath+"settings.json"))
    {
        if(QFile::remove(resourcesPath+"settings.json")){
            qDebug() << "removed settings.json";
        }

    }
    if (QFile::exists(resourcesPath+"list.json"))
    {
        QFile::remove(resourcesPath+"list.json");
    }

    qDebug() << resourcesPath;
    qDebug() << oldPath;
    QFile file(oldPath+"settings.json");
    if(!file.rename(resourcesPath+"settings.json")){
        qWarning() << "Settings.json move failed!!!";
    }
    QFile file2(oldPath+"list.json");
    file2.rename(resourcesPath+"list.json");
    QDir old(oldPath);
    old.removeRecursively();

    if(writeToFile->remove()){
        qDebug() << "Removed Zip File";
        QDir macJunkDir(pathToDownload + "__MACOSX");
        macJunkDir.removeRecursively();
        writeToFile->close();
    } else {
        qDebug() << "Failed to remove zip file";
    }

    this->ui->label->setText("Done!");
    this->ui->progressBar->setValue(120);
    QString execPath;
    if(osName()=="osx")
    {
        QProcess::execute(QString("chmod +x %1").arg(pathToDownload + fileToDownload.replace(".zip", "")+ + ".app/Contents/MacOS/" + fileToDownload.replace(".zip", "")));
        QProcess::execute(QString("chmod +x %1").arg(pathToDownload + fileToDownload.replace(".zip", "")+ ".app"));
        execPath = QString("open %1").arg(pathToDownload + fileToDownload.replace(".zip", "")+ ".app");
        system(execPath.toLatin1());
    }else if(osName()=="win"){
        execPath = AppData::Instance()->executablePath("start " +pathToDownload+"BetaUploader");
        system(execPath.toLatin1());
    }
    QProcess::startDetached(execPath);
    qDebug() << execPath;
    //qDebug() << process->errorString();
    this->close();

}


void UpdatingWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qlonglong contentLength = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    float percent = (float)bytesReceived/contentLength;
    this->ui->progressBar->setValue(percent*100);
    QString string = "Downloading (" + QString::number(bytesReceived/1000) + "/" + QString::number(contentLength/1000) + "kB)";
    qDebug() << "Downloading (" + QString::number(bytesReceived/1000) + "/" + QString::number(contentLength/1000) + "kB)";
    this->ui->label->setText(string);
}

void UpdatingWindow::readyRead()
{
    QByteArray data = reply->readAll();
    this->writeToFile->write(data);

    //qDebug() << data;
}

QString UpdatingWindow::osName()
{
   #if defined(Q_OS_ANDROID)
       return QLatin1String("android");
   #elif defined(Q_OS_BLACKBERRY)
       return QLatin1String("blackberry");
   #elif defined(Q_OS_IOS)
       return QLatin1String("ios");
   #elif defined(Q_OS_MAC)
       return QLatin1String("osx");
   #elif defined(Q_OS_WINCE)
       return QLatin1String("wince");
   #elif defined(Q_OS_WIN)
       return QLatin1String("win");
   #elif defined(Q_OS_LINUX)
       return QLatin1String("linux");
   #elif defined(Q_OS_UNIX)
       return QLatin1String("unix");
   #else
       return QLatin1String("unknown");
   #endif
}
