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
    ui->comboBox->addItem("Loading Items");
    ui->listWidget->addItem("15 of December 2014:\nRelease of Beta Launcher");
    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(handleButton()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedChange(QString)));


}
void MainWindow::fileDownloaded()
{
    QByteArray m_DownloadedData = fileDownloader->downloadedData();
    printf("data: %s\n",fileDownloader->downloadedData().data());
    QJsonDocument loadDoc = QJsonDocument::fromJson(m_DownloadedData);
    printf(loadDoc.object()["test2"].toObject()["version"].toString().toLocal8Bit().data());
    QStringList Items = loadDoc.object().keys();
    //Items.removeAt(0);
    ui->comboBox->removeItem(0);
    ui->comboBox->addItems(Items);
    //printf("\n %s", Items.at(0));
    QFile loadFile("list.json");


        printf_s("File does not exist");
        loadFile.open(QIODevice::ReadWrite);



     printf_s("File does not exist");
    //QFile loadFile("list.json");
    QByteArray list = loadFile.readAll();
    QJsonDocument loadDoc2(QJsonDocument::fromJson(list));
    QJsonObject root;
    foreach(QString item, Items)
    {
        QJsonObject itemJ;
        if(loadDoc2.object().keys().contains(item))
        {
            //All ready parsed
            printf("\nparsed: %s", loadDoc.object()[item].toObject()["time"]);
            printf("\nloaded: %s", itemJ["time"]);

            itemJ = loadDoc2.object()[item].toObject();
            if(itemJ["time"] != loadDoc.object()[item].toObject()["time"])
            {
                itemJ["bNeedsUpdate"] = true;
            }
            foreach(QString key, loadDoc.object()[item].toObject().keys())
            {
                itemJ[key] = loadDoc.object()[item].toObject()[key];
            }
        }else{
            foreach(QString key, loadDoc.object()[item].toObject().keys())
            {
                itemJ[key] = loadDoc.object()[item].toObject()[key];
                printf("\nkey: %s", key);
            }
            itemJ["installed"] = false;
        }
        root[item] = itemJ;
    }
    QJsonDocument writeDoc(root);
    loadFile.resize(0);
    loadFile.write(writeDoc.toJson());
    this->list = root;
    QJsonObject item = this->list[ui->comboBox->currentText()].toObject();
    if(item["installed"]!= true)
    {
        ui->pushButton->setText("Install");
    }
    else if(item["bNeedsUpdate"]==true)
    {
        ui->pushButton->setText("Update");
    }

}
void MainWindow::handleButton()
{
    QJsonObject item = this->list[ui->comboBox->currentText()].toObject();
    if(item["installed"]==true)
    {
        //Find exe and launch, also ensure that steam is running or else launch it
    }else{
        install(false, ui->comboBox->currentText());
    }
}
void MainWindow::install(bool updating, QString item)
{
    QString rootPath;
    rootPath = "Apps";
    if(!updating)
    {

#ifdef Q_WS_MACX
        rootPath = QDesktopServices::storageLocation( QDesktopServices::DataLocation )+"BetaLauncher";
#endif
        QDir path = QDir("");
       path.mkpath(rootPath + "/" + item);

    }
    ui->label->setText("Inizializing");
    printf("Installing");
    DownloadManager* manager = new DownloadManager(this);
    manager->addItem(QUrl("http://leonardogalli.ch/beta/builds/SlashAnimationTest.zip"));
    manager->addPath(rootPath + "/" + item + "/test.zip");
    connect(manager, SIGNAL(progress(QString,QString,int)), SLOT(downloadProgCalc(QString,QString,int)));
    manager->start();
}
void MainWindow::downloadProgCalc(QString remainingTime, QString Speed, int percentage)
{
    ui->progressBar->setValue(percentage);
    ui->label->setText("Downloading with "+ Speed + ", " + remainingTime + " remaining");
}
void MainWindow::selectedChange(QString item)
{
    QJsonObject itemJ = this->list[item].toObject();
    if(itemJ["installed"]!= true)
    {
        ui->pushButton->setText("Install");
    }
    else if(itemJ["bNeedsUpdate"]==true)
    {
        ui->pushButton->setText("Update");
    }
    else
    {
        ui->pushButton->setText("Launch");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

