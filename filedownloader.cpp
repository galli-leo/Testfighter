#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl imageUrl, QObject *parent, QString filePath) :
    QObject(parent)
{
    shouldSave = false;
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
                SLOT(fileDownloaded(QNetworkReply*)));

    if(filePath != "")
    {

        if (QFile::exists(filePath)) {
                QFile::remove(filePath);

        }
        else
        {
            QFileInfo fileInf(filePath);
            QDir dir("");
            dir.mkpath(fileInf.absolutePath());
        }

        this->writeToFile = new QFile(filePath);
        this->writeToFile->open(QIODevice::ReadWrite);
        this->shouldSave = true;
    }

    QNetworkRequest request(imageUrl);
   this->reply =  m_WebCtrl.get(request);
    connect(this->reply, SIGNAL(downloadProgress(qint64,qint64)),
                SLOT(downloadProgress(qint64,qint64)));
    connect(this->reply, SIGNAL(readyRead()),
                this, SLOT(readyRead()));
}

FileDownloader::~FileDownloader()
{

}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    if(this->shouldSave)
    {
        this->writeToFile->close();
    }
    //emit a signal
    pReply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const
{
    return m_DownloadedData;
}

void FileDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit downloadProg(bytesReceived, bytesTotal);
}

void FileDownloader::readyRead()
{
    QByteArray data = reply->readAll();
    m_DownloadedData += data;
    if (this->shouldSave)
    {
        qDebug() << "Got data: " << data;
        this->writeToFile->write(data);

    }

}
