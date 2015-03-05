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
   /* QNetworkAccessManager *networkMgr = new QNetworkAccessManager(this);
    QNetworkReply *reply = networkMgr->head( QNetworkRequest( QUrl( "http://leonardogalli.ch/beta/download_update?file=" + fileToDownload + "&os="+ osName() ) ) );

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();
    this->dlSize = reply->header(QNetworkRequest::ContentLengthHeader).toInt();
    qDebug() << "Raw headers: " << reply->rawHeaderList();
    qDebug() << "DLSIZE:       " << this->dlSize;
    qDebug() << "Content Type: " << reply->header(QNetworkRequest::ContentTypeHeader);*/
    //manager = QNetworkAccessManager(this);
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
                SLOT(fileDownloaded(QNetworkReply*)));
    writeToFile = new QFile(pathToDownload + fileToDownload);
    writeToFile->open(QIODevice::ReadWrite);
    QNetworkRequest request(AppData::Instance()->settings["url"].toString() + "download_update.php?file=" + fileToDownload + "&os="+ osName());
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
    this->ui->label->setText("Unpacking");



    this->ui->label->setText("Done!");
    this->ui->progressBar->setValue(120);
    QString execPath;
    if(osName()=="osx")
    {
        QProcess::execute(QString("chmod +x %1").arg(pathToDownload + fileToDownload.replace(".zip", "")+ + ".app/Contents/MacOS/" + fileToDownload.replace(".zip", "")));
        QProcess::execute(QString("chmod +x %1").arg(pathToDownload + fileToDownload.replace(".zip", "")+ ".app"));
        execPath = QString("open %1").arg(pathToDownload + fileToDownload.replace(".zip", "")+ ".app");//"open " + pathToDownload + fileToDownload.replace(".zip", "")+ ".app";// + ".app/Contents/MacOS/" + fileToDownload.replace(".zip", "");
    }else{

    }
    QProcess::startDetached(execPath);
    qDebug() << pathToDownload + fileToDownload.replace(".zip", "") + ".app/Contents/MacOS/" + fileToDownload.replace(".zip", "");
    //qDebug() << process->errorString();
    this->close();

}


void UpdatingWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qlonglong test = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    qDebug() << reply->header(QNetworkRequest::ContentLengthHeader);
    qDebug() << test;
    float percent = (float)bytesReceived/test;
    this->ui->progressBar->setValue(percent*100);
    QString string = "Downloading (" + QString::number(bytesReceived/1000) + "/" + QString::number(test/1000) + "kB)";
    qDebug() << "Downloading (" + QString::number(bytesReceived/1000) + "/" + QString::number(test/1000) + "kB)";
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
       return QLatin1String("windows");
   #elif defined(Q_OS_LINUX)
       return QLatin1String("linux");
   #elif defined(Q_OS_UNIX)
       return QLatin1String("unix");
   #else
       return QLatin1String("unknown");
   #endif
}
