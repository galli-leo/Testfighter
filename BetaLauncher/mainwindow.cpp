#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    appDir = AppData::Instance()->docsDirectory;

    QUrl listUrl(AppData::Instance()->settings["url"].toString() + AppData::Instance()->osName+"_list.json");
    listDownloader = new FileDownloader(listUrl, this);
    connect(listDownloader, SIGNAL(downloaded()), SLOT(listDownloaded()));

    ui->setupUi(this);
    ui->comboBox->addItem("Loading Items");

    feedDownloader = new FileDownloader(QUrl(AppData::Instance()->settings["url"].toString() + "feed/index.php?num=200"), this);
    connect(feedDownloader, SIGNAL(downloaded()), SLOT(feedDownloaded()));

    ui->progressBar->setHidden(true);

    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(handleButton()));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectedChange(QString)));

	this->shouldAutoStart = false;

}
void MainWindow::listDownloaded()
{
    QByteArray m_DownloadedData = listDownloader->downloadedData();
    QJsonDocument loadDoc = QJsonDocument::fromJson(m_DownloadedData);
    QStringList Items = loadDoc.object().keys();

    ui->comboBox->removeItem(0);
    ui->comboBox->addItems(Items);

    QFile loadFile("list.json");

    loadFile.open(QIODevice::ReadWrite);

    QByteArray list = loadFile.readAll();
    QJsonDocument loadDoc2(QJsonDocument::fromJson(list));
    QJsonObject newList;
    foreach(QString item, Items)
    {
        QJsonObject itemJ;
        if(loadDoc2.object().keys().contains(item))
        {
            //All ready parsed
            //printf("\nparsed: %s", loadDoc.object()[item].toObject()["time"]);
            //printf("\nloaded: %s", itemJ["time"]);

            itemJ = loadDoc2.object()[item].toObject();
            if(itemJ["time"] != loadDoc.object()[item].toObject()["time"])
            {
                AppData::Instance()->setDictItem(itemJ, "bNeedsUpdate", "true");
                this->needsUpdateList.append(item);
                qDebug() << "TIME: " <<  loadDoc.object()[item] << loadDoc.object()[item].toObject()["time"].toString();
                this->times.insert(item, QString::number(loadDoc.object()[item].toObject()["time"].toInt()));
            }
            foreach(QString key, loadDoc.object()[item].toObject().keys())
            {
                if(!(key == "time" && this->needsUpdateList.contains(item)))
                {
                    itemJ[key] = loadDoc.object()[item].toObject()[key];
                }
            }
        }else{
            foreach(QString key, loadDoc.object()[item].toObject().keys())
            {
                itemJ[key] = loadDoc.object()[item].toObject()[key];
            }
			itemJ["isInstalled"] = "false";
        }
        newList[item] = itemJ;
    }
    QJsonDocument writeDoc(newList);
    loadFile.resize(0);
    loadFile.write(writeDoc.toJson());
    loadFile.close();
    this->list = newList;
    QJsonObject item = this->list[ui->comboBox->currentText()].toObject();
	this->selectedChange(ui->comboBox->currentText());
	/*
    if(!this->isInstalled((ui->comboBox->currentText())))
    {
        ui->pushButton->setText("Install");
        this->setDescription(this->list[ui->comboBox->currentText()].toObject()["description"].toString());



    }
    else if(this->needsUpdateList.contains(ui->comboBox->currentText()))
    {
        ui->pushButton->setText("Update");
        this->setDescription("Changelog:\n" + this->list[ui->comboBox->currentText()].toObject()["changelog"].toString());

    }
    else
    {
        ui->pushButton->setEnabled(true);
    }*/
}
void MainWindow::feedDownloaded()
{
    ui->listWidget->clear();
    QByteArray m_DownloadedData = feedDownloader->downloadedData();
    qDebug() << "data: \n" << feedDownloader->downloadedData().data();
    QJsonDocument loadDoc = QJsonDocument::fromJson(m_DownloadedData);
    QJsonObject obj = loadDoc.object();
    foreach(QJsonValue item, loadDoc.array())
    {
        QDateTime converted_date(QDateTime::fromString(item.toObject()["date"].toString(), "dd-MM-yyyy hh:mm:ss"));
        qDebug() << converted_date;
        QString title = converted_date.toString("dd.MM.yy") + ": " + item.toObject()["title"].toString() + "\n\n";
        QString whole = title + item.toObject()["text"].toString() + "\n\n";
        ui->listWidget->addItem(whole);
    }
}

void MainWindow::handleButton()
{
    QJsonObject item = this->list[ui->comboBox->currentText()].toObject();
    if(this->isInstalled(ui->comboBox->currentText())&&!this->needsUpdateList.contains(ui->comboBox->currentText()))
    {
        launch();
    }else{

        if(this->needsUpdateList.contains(ui->comboBox->currentText()) && !(this->ui->pushButton->text()=="Update"))
        {
            if (QMessageBox::No == QMessageBox(QMessageBox::Information, "Update available", "Do you want to update " + ui->comboBox->currentText() + "?", QMessageBox::Yes|QMessageBox::No).exec())
            {
                launch();
                return;
            }
        }
        install(this->needsUpdateList.contains(ui->comboBox->currentText()), ui->comboBox->currentText());
    }
}
void MainWindow::install(bool updating, QString item)
{

    ui->progressBar->setHidden(false);
    QString rootPath;
    rootPath = appDir;

    this->hashDownloader = new FileDownloader(QUrl(AppData::Instance()->settings["url"].toString() + "builds/hashes/"+AppData::Instance()->osName +"/"+ item + ".json"));
    qDebug() << AppData::Instance()->settings["url"].toString() + "builds/hashes/"+AppData::Instance()->osName +"/"+ item + ".json";
    connect(this->hashDownloader, SIGNAL(downloaded()), this, SLOT(hashDownloaded()));
    ui->pushButton->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->label->setText("Inizializing");
    this->setDescription("");

    printf("Installing");

}
void MainWindow::downloadProgCalc(QString remainingTime, QString Speed, int percentage)
{
    ui->progressBar->setValue(percentage);
    ui->label->setText("Downloading with "+ Speed + ", " + remainingTime + " remaining");
}
void MainWindow::selectedChange(QString item)
{
    QJsonObject itemJ = this->list[item].toObject();
    if(!this->isInstalled((item)))
    {
        ui->pushButton->setText("Install");
        this->setDescription(this->list[ui->comboBox->currentText()].toObject()["description"].toString());
        ui->autoLaunch->setVisible(true);
        ui->pushButton->setEnabled(true);

    }
    else if(this->needsUpdateList.contains(item))
    {
        ui->pushButton->setText("Update");
        this->setDescription("Changelog: " + this->list[ui->comboBox->currentText()].toObject()["changelog"].toString());
        ui->autoLaunch->setVisible(true);
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setText("Launch");
        this->setDescription("");
        ui->autoLaunch->setVisible(false);
        ui->pushButton->setEnabled(true);
    }

}

void MainWindow::setDescription(QString text){
    ui->desc_label->setText(text);
}

void MainWindow::hashDownloaded()
{
    QByteArray m_DownloadedData = hashDownloader->downloadedData();
    this->dlListData = hashDownloader->downloadedData();

    QJsonDocument loadDoc = QJsonDocument::fromJson(m_DownloadedData);
    QJsonObject hashes = loadDoc.object();
    QStringList toDownload;
    QString item = ui->comboBox->currentText();

    qDebug() << hashes;

    QDirIterator it(appDir+item, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString fileL = it.next();

        QString relativeFile = fileL.replace(appDir, "");
        QString hash = this->hash(appDir + relativeFile);
        if(!hashes.contains(relativeFile))
        {
            qDebug() << "Remove file: " << relativeFile;
            QFile oldFile(appDir + relativeFile);
            oldFile.remove();
        }
        else
        {
            qDebug() << "Hash of file: " << fileL << ", " << hash << " online says: " << hashes[relativeFile].toString();
            if(hash == hashes[relativeFile].toString())
            {
                hashes.remove(relativeFile);
            }

        }
    }
    QString rootPath = appDir;
    DownloadManager* manager = new DownloadManager(this);
    int count = 0;
    foreach(QString file, hashes.keys())
    {
        count++;
        qDebug() << "Adding file: %s" << "http://leonardogalli.ch/beta/builds/" << file;
        manager->addItem(QUrl(AppData::Instance()->settings["url"].toString() + "builds/"+AppData::Instance()->osName+"/"+ file));
        manager->addPath(rootPath + "/" + file);

    }


    if(count==0)
    {
        this->downloadManagerFinished();
    }
    connect(manager, SIGNAL(progress(QString,QString,int)), SLOT(downloadProgCalc(QString,QString,int)));
    connect(manager, SIGNAL(finished()), SLOT(downloadManagerFinished()));
    manager->start();
}
QString MainWindow::hash(QString file)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QFile in(file);
    qDebug() << "Hashing file: " << file;
    if (in.open(QIODevice::ReadOnly)) {
        char buf[2048];
        int bytesRead;
        qint64 overallBytesRead = 0;
        if(in.size()<2048)
        {
            hash.addData(in.readAll());
        }
        else
        {
            while ((bytesRead = in.read(buf, 2048)) > 0) {
                overallBytesRead += bytesRead;
                hash.addData(buf, bytesRead);
            }
        }
        in.close();

    }
    else
    {

    }
    return hash.result().toHex();
}
void MainWindow::downloadManagerFinished()
{
/*
#ifdef Q_OS_MAC
    QString exePath = AppData::Instance()->executablePath( AppData::Instance()->docsDirectory + ui->comboBox->currentText());
    QFile file(exePath);
    file.setPermissions(QFile::ExeUser | QFile::ExeGroup | QFile::ExeOther | QFile::ExeOwner);
    qDebug() << file.permissions();
    qDebug() << exePath;
#endif*/
    ui->progressBar->setHidden(true);
    ui->label->setText("");
    ui->progressBar->setValue(0);



    ui->pushButton->setEnabled(true);
    ui->comboBox->setEnabled(true);
    this->needsUpdateList.removeAt(this->needsUpdateList.indexOf(ui->comboBox->currentText()));



    QJsonObject item = this->list[ui->comboBox->currentText()].toObject();
    AppData::Instance()->setDictItem(item, "time", this->times[ui->comboBox->currentText()].toInt());
	AppData::Instance()->setDictItem(item, "isInstalled", "true");
	AppData::Instance()->setDictItem(item, "bNeedsUpdate", "false");
    AppData::Instance()->setDictItem(this->list, ui->comboBox->currentText(), item);
	
    this->selectedChange(ui->comboBox->currentText());

    qDebug() << this->list << this->list[ui->comboBox->currentText()] << this->list[ui->comboBox->currentText()].toObject()["times"] << this->times[ui->comboBox->currentText()];
    QFile loadFile("list.json");

    loadFile.open(QIODevice::ReadWrite);
    QJsonDocument writeDoc(this->list);
    loadFile.resize(0);
    loadFile.write(writeDoc.toJson());
    loadFile.close();
	
    if(this->ui->autoLaunch->isChecked()){
		this->launch();
	}

}
bool MainWindow::isInstalled(QString item)
{
    qDebug() << (QDir(AppData::Instance()->docsDirectory + item).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() != 0? "Installed: true" : "Installed: false");
    //return QDir(AppData::Instance()->docsDirectory + item).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() != 0;
	QJsonObject curItem = this->list[item].toObject();
	return curItem["isInstalled"].toString() == "true" ? true : false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::afterInstall(QString item)
{
    //Fetch dependency and install
    //TODO: MAKE THIS WORK
    QString depend = this->list[item].toObject()["prereq"].toString();
    qDebug() << depend;
    QDirIterator it("", QDir::NoFilter, QDirIterator::Subdirectories);

    while(it.hasNext())
    {
        QString filePath = it.next();
        QFile file(filePath);
        QFileInfo info(file);
        if(info.fileName()==AppData::Instance()->appExtension){

        }
    }
}

void MainWindow::launch()
{
    //Find exe and launch, also ensure that steam is running or else launch it

    //gives the filter (e.g. yourapp.exe)
    const QStringList nameFilter( ui->comboBox->currentText()+AppData::Instance()->appExtension);
    qDebug() << AppData::Instance()->docsDirectory + ui->comboBox->currentText() << " " << ui->comboBox->currentText();
    //The iterator searching for the file
    QDirIterator dirIt(AppData::Instance()->docsDirectory + ui->comboBox->currentText(),nameFilter, QDir::NoFilter, QDirIterator::Subdirectories);
    while (dirIt.hasNext()) {
        dirIt.next();

        qDebug()<< "Path" << dirIt.filePath();
        //Real path for mac apps, windows it is just the exe file, mac it is yourapp.app/Contents/MacOs/yourapp
        QString realPath = dirIt.filePath();
        if(dirIt.filePath().contains(".app")){
            realPath += "/Contents/MacOS/" + ui->comboBox->currentText();
            //realPath = QString("\"") + realPath+ QString("\"");
            QFile file(realPath);
            //Set the permissions so we are sure we can launch it, dling it usually results in not the right permissions
            file.setPermissions(QFile::ExeGroup | QFile::ExeOther | QFile::ExeOwner | QFile::ExeUser | QFile::ReadGroup | QFile::ReadOwner | QFile::ReadOther | QFile::ReadUser);
        }

        launchProc = new QProcess(this);
        QStringList arguments;
        arguments << realPath;
        launchProc->start(realPath, arguments );
        //QProcess::startDetached(realPath);
        qDebug() << realPath;
        //ui->centralWidget->hide();
        //Show it minimized, so it does not confront app
        ui->centralWidget->showMinimized();
        this->showMinimized();
        this->clearFocus();
        connect(launchProc, SIGNAL(finished(int , QProcess::ExitStatus )), this, SLOT(launchFinished(int,QProcess::ExitStatus)));

        break;


    }
}
void MainWindow::launchFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Launch finished with exit code: " << exitCode << exitStatus;
    qDebug() <<  launchProc->errorString();
    launchProc->dumpObjectInfo();
    qDebug() << launchProc->readAllStandardOutput();
    qDebug() << launchProc->readAllStandardError();
    //dont now which one of these really works but it works
    this->showNormal();
    this->show();
    this->activateWindow();
    this->raise();
    this->ui->centralWidget->showNormal();
    this->ui->centralWidget->raise();
    this->ui->centralWidget->activateWindow();
}
