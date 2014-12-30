#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QDir>


class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QUrl imageUrl, QObject *parent = 0, QString filePath = "");

    virtual ~FileDownloader();

    QByteArray downloadedData() const;

signals:
        void downloaded();
        void downloadProg(qint64 bytesReceived, qint64 bytesTotal);

private slots:

    void fileDownloaded(QNetworkReply* pReply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void readyRead();
private:

    QNetworkAccessManager m_WebCtrl;
    QFile* writeToFile;
    QByteArray m_DownloadedData;
    QNetworkReply* reply;
    bool shouldSave;

};

#endif // FILEDOWNLOADER_H
