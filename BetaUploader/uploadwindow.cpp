#include "uploadwindow.h"
#include "ui_uploadwindow.h"

UploadWindow::UploadWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UploadWindow)
{
    cli = false;
    ui->setupUi(this);
    QNetworkAccessManager *networkMgr = new QNetworkAccessManager(this);
    QNetworkReply *reply = networkMgr->get( QNetworkRequest( QUrl( AppData::Instance()->settings["url"].toString() + "options.json" ) ) );
    qDebug() << AppData::Instance()->settings["url"].toString();
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();

    // Lets print the HTTP GET response.
    //qDebug( reply->readAll());

    QByteArray optionsData = reply->readAll();
    QJsonDocument loadDoc = QJsonDocument::fromJson(optionsData);
    qDebug() << optionsData;
    this->options = loadDoc.object();
    //setupFields();

}

UploadWindow::~UploadWindow()
{
    delete ui;
}
void UploadWindow::startUpload()
{
    //Get the list for the os we are uploading
    QNetworkAccessManager *networkMgr = new QNetworkAccessManager(this);
    QNetworkReply *replyList = networkMgr->get( QNetworkRequest( QUrl( AppData::Instance()->settings["url"].toString() + this->os+"_list.json" ) ) );
    qDebug() << os << AppData::Instance()->settings["url"].toString() + this->os+"_list.json";
    QEventLoop loopList;
    QObject::connect(replyList, SIGNAL(finished()), &loopList, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loopList.exec();

    QJsonDocument loadDocList = QJsonDocument::fromJson(replyList->readAll());
    this->list = loadDocList.object();

    setupFields();
    initUpload();

}
void UploadWindow::setupFields()
{
    //Check if item exists
    QDir directory = QDir(dir);
    directory.setPath(dir);
    int start = dir.lastIndexOf("/");
    int len = dir.length()-start;

    QString dirName = dir.split("/").last();
    if(dirName == "")
    {
        dirName = dir.split("/").at(dir.split("/").count()-2);

    }

    qDebug() << this->list;
    qDebug() << dir;
    qDebug() << dirName;
    this->isInit = !this->list.contains(dirName);
    qDebug() << this->isInit;
    //This is responsible for storing all options according to their type
    QMap<QString, QList<QJsonObject> > optionsSorted;
    //This is responsible for looking up the options in the right order according to their type
    QList<QString> lookUpList;
    QList<QJsonObject> emptyList;
    lookUpList.append("yes/no");
    lookUpList.append("textfield");
    lookUpList.append("text");
    optionsSorted.insert("text", emptyList);
    optionsSorted.insert("textfield", emptyList);
    optionsSorted.insert("yes/no", emptyList);
    qDebug() << this->options;
    foreach(QString key, this->options.keys())
    {
        QJsonObject option = this->options.value(key).toObject();
        QList<QJsonObject> list = optionsSorted.value(option["Type"].toString());
        list.append(option);
        optionsSorted.insert(option["Type"].toString(), list);
    }
    qDebug() << optionsSorted.value("text").at(0);
    int column = 0;
    int row = 0;
    foreach(QString key, lookUpList)
    {
         QList<QJsonObject> list = optionsSorted.value(key);
         foreach(QJsonObject json, list)
         {
            addField(json["Type"].toString(), json, row, column);
         }
    }


}

void UploadWindow::addField(QString type, QJsonObject field, int &row, int &column)
{
    if((this->isInit && field["When"].toString()!= "init") || (!this->isInit && field["When"].toString()== "init"))
    {
        return;
    }
    QLabel* label = new QLabel(field["Name"].toString()+":", this);
    if(type == "yes/no")
    {
        QCheckBox* check = new QCheckBox(this);

        //QFormLayout *formLayout = new QFormLayout();
        //formLayout->addRow(label, check);
        this->ui->formLayout->addRow(label, check);
        if(column == 1)
        {
            row++;
            column = 0;

        } else
        column = 1;
        QWidget* widget = (QWidget*)check;
        edits.insert(keyForName(field["Name"].toString()), widget);
    } else if(type == "text")
    {
        QTextEdit *textEdit = new QTextEdit(this);
        this->ui->formLayout->addRow(label, textEdit);
        edits.insert(keyForName(field["Name"].toString()), textEdit);
    } else if(type == "textfield")
    {
        QLineEdit *lineEdit = new QLineEdit(this);
        this->ui->formLayout->addRow(label, lineEdit);
        edits.insert(keyForName(field["Name"].toString()), lineEdit);
    }
}

void UploadWindow::initUpload()
{
    QNetworkAccessManager * networkMgr = new QNetworkAccessManager(this);
    //QNetworkRequest request(QUrl("http://leonardogalli.ch/beta/upload_single.php"));

    UploadManager *uploadManager = new UploadManager(this);
    connect(uploadManager, SIGNAL(uploadProg(QString,QString,float)), this, SLOT(uploadProgress(QString,QString,float)));
    connect(uploadManager, SIGNAL(uploadFinished()), this, SLOT(uploadFin()));

    QString dirName = dir.split("/").last();
    if(dirName == "") // This accounts for urls like /bla/bla/test/
    {
        dirName = dir.split("/").at(dir.split("/").count()-2);

    }

    QDirIterator it(this->dir, QDir::Files, QDirIterator::Subdirectories);

    QNetworkReply *replyHash = networkMgr->get( QNetworkRequest( QUrl( AppData::Instance()->settings["url"].toString() + "builds/hash"+ dirName + ".json" ) ) );

    QEventLoop loopHash;
    QObject::connect(replyHash, SIGNAL(finished()), &loopHash, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loopHash.exec();
    QByteArray ret = replyHash->readAll();
    qDebug() << ret;
    QJsonDocument docHash = QJsonDocument::fromJson(ret);
    hashes = docHash.object();
    while (it.hasNext()) {
        QString fileL = it.next();

        QFile file(fileL);
        QFileInfo info(file);
        QString folderName = info.absolutePath();
        QString fileT = folderName.replace(dirName, "|||");
        QString path = dirName + fileT.split("|||").last() + "/";
        qDebug() << "local file: " << fileL;
        qDebug() <<  "hash of local file: " << hash(fileL);
        qDebug() << "path + filename" << path+info.fileName();
        //qDebug() << "hashs" <<hashes.keys().at(0);
        qDebug() << "hashes at filename"<<hashes[path+info.fileName()].toString();
        if(hash(fileL) != hashes[path+info.fileName()].toString()){
            qDebug() << "ASDFASDFASDF";
            uploadManager->addItem(fileL);
            uploadManager->addPath(path);
        }
        hashes.remove(path+info.fileName());
    }
    QString filesToRemove = "";
    foreach(QString key, hashes.keys())
    {

        filesToRemove + key + ";";

    }
    qDebug() << filesToRemove;
    if(filesToRemove != "")
    {
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QHttpPart loginPart;
        /* password */
        loginPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"password\""));
        loginPart.setBody("testfighter2015");
        multiPart->append(loginPart);
        loginPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"files\""));
        loginPart.setBody(filesToRemove.toLocal8Bit());
        QNetworkReply *replyDel = networkMgr->post( QNetworkRequest( QUrl( "http://leonardogalli.ch/beta/builds/remove_files.php" ) ), multiPart );
        multiPart->setParent(replyDel);
        QEventLoop loopDel;
        QObject::connect(replyDel, SIGNAL(finished()), &loopDel, SLOT(quit()));
        loopDel.exec();
        qDebug() << replyDel->readAll();
    }

    uploadManager->start();
}
void UploadWindow::uploadFin()
{
    this->ui->progressBar->setValue(100);
    this->ui->label->setText("");
    this->ui->pushButton->setEnabled(true);
    if(cli)
    {
        this->buildSubmit();
    }
}
void UploadWindow::uploadProgress(QString ulSpeed, QString timeRemaining, float percentage)
{
    this->ui->label->setText("Uploading with " + ulSpeed + ", " + timeRemaining + " remaining");
    this->ui->progressBar->setValue(percentage);
    this->ui->pushButton->setEnabled(false);
}
void UploadWindow::buildSubmit()
{
    QUrl url(AppData::Instance()->settings["url"].toString() + "build_submit.php");
    QNetworkRequest request(url);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    foreach(QString key, edits.keys())
    {
        QWidget* edit = edits[key];

        QHttpPart loginPart;

        loginPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + key +"\""));
        if(!strcmp( edit->metaObject()->className(), "QTextEdit"))
        {

            QTextEdit* textEdit = (QTextEdit*)edit;
            loginPart.setBody(textEdit->toPlainText().toLocal8Bit());
            qDebug() << "Key: " << key << " text: " << textEdit->toPlainText().toLocal8Bit();
        }
        if(!strcmp( edit->metaObject()->className(), "QLineEdit"))
        {

            QLineEdit* textEdit = (QLineEdit*)edit;
            loginPart.setBody(textEdit->text().toLocal8Bit());
            qDebug() << "Key: " << key << " text: " << textEdit->text();
        }
        if(!strcmp( edit->metaObject()->className(), "QCheckBox"))
        {

            QCheckBox* textEdit = (QCheckBox*)edit;
            int check = textEdit->checkState();
            if(check == 0)loginPart.setBody("false");
            if(check == 2)loginPart.setBody("true");
            qDebug() << "Key: " << key << " text: " << check;
        }
        qDebug() << key;
        qDebug() << edit->metaObject()->className();
        multiPart->append(loginPart);
    }
     QHttpPart fileName;
     fileName.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"filename\""));

     QString dirName = dir.split("/").last();
     if(dirName == "") // This accounts for urls like /bla/bla/test/
     {
         dirName = dir.split("/").at(dir.split("/").count()-2);

     }

     qDebug() << dirName.toLocal8Bit();
     fileName.setBody(dirName.toLocal8Bit());
     multiPart->append(fileName);
     QHttpPart osName;
     osName.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"os\""));
     osName.setBody(this->os.toLocal8Bit());
     multiPart->append(osName);

    qDebug() << multiPart;
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    QNetworkReply* reply = networkManager->post(request, multiPart);
    multiPart->setParent(reply);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();
    qDebug() << reply->readAll();

}
QString UploadWindow::keyForName(QString name)
{
    foreach(QString key, this->options.keys())
    {
        QJsonObject option = this->options[key].toObject();
        if(option["Name"] == name)
        {
            return key;
        }
    }
}

void UploadWindow::on_pushButton_clicked()
{
    buildSubmit();
}
QString UploadWindow::hash(QString file)
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

void UploadWindow::setField(QString key, QString value)
{
    QWidget* edit = edits[key];


    if(!strcmp( edit->metaObject()->className(), "QTextEdit"))
    {

        QTextEdit* textEdit = (QTextEdit*)edit;
        textEdit->setText(value);
    }
    if(!strcmp( edit->metaObject()->className(), "QLineEdit"))
    {

        QLineEdit* textEdit = (QLineEdit*)edit;
        textEdit->setText(value);
    }
    if(!strcmp( edit->metaObject()->className(), "QCheckBox"))
    {

        QCheckBox* textEdit = (QCheckBox*)edit;
        int check = textEdit->checkState();
        textEdit->setChecked(false);
        if (value == "true")
        {
             textEdit->setChecked(true);
        }
    }
}
