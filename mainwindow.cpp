#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    QUrl imageUrl("http://leonardogalli.ch/beta/list.json");
    fileDownloader = new FileDownloader(imageUrl, this);

    connect(fileDownloader, SIGNAL(downloaded()), SLOT(fileDownloaded()));
    ui->setupUi(this);
    ui->comboBox->addItem("KnigthsOfSound");
    ui->listWidget->addItem("15 of December 2014:\nRelease of Beta Launcher");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fileDownloaded()
{
    QByteArray m_DownloadedData = fileDownloader->downloadedData();
    printf("data: %s",fileDownloader->downloadedData().data());
    QJsonDocument loadDoc = QJsonDocument::fromJson(m_DownloadedData);
    printf(loadDoc.object()["test2"].toObject()["version"].toString().toLocal8Bit().data());

}
