#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)), this,
                    SLOT(fileDownloaded(QNetworkReply*)));
sleep(5);
QString target = "http://www.leonardogalli.ch/beta/list.json";
        QNetworkRequest request(QUrl::fromEncoded(target.toLocal8Bit()));
        m_WebCtrl.get(request);
    ui->setupUi(this);
    ui->comboBox->addItem("KnigthsOfSound");
    ui->listWidget->addItem("23. March 2014:\nUpdated KnightsOfSound\n");
    ui->listWidget->addItem("23. March 2014:\nUpdated KnightsOfSound\n");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileDownloaded(QNetworkReply* pReply)
{
    QByteArray m_DownloadedData = pReply->readAll();
    printf("data: %s", pReply->readAll().data());
    QJsonDocument loadDoc = QJsonDocument::fromJson(m_DownloadedData);
    printf(loadDoc.object()["test2"].toObject()["version"].toString().toLocal8Bit().data());

}
