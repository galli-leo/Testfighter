#ifndef UPDATINGWINDOW_H
#define UPDATINGWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QProcess>

namespace Ui {
class UpdatingWindow;
}

class UpdatingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UpdatingWindow(QWidget *parent = 0);
    ~UpdatingWindow();
    QString pathToDownload;
    QString fileToDownload;
    void startDownload();
    QFile* writeToFile;
    QNetworkReply* reply;
    QNetworkAccessManager manager;
    QString osName();
    int dlSize;
private slots:

    void fileDownloaded(QNetworkReply* pReply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void readyRead();
private:
    Ui::UpdatingWindow *ui;
};

#endif // UPDATINGWINDOW_H
