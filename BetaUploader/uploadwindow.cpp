#include "uploadwindow.h"
#include "ui_uploadwindow.h"

UploadWindow::UploadWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UploadWindow)
{
    ui->setupUi(this);
    QNetworkAccessManager *networkMgr = new QNetworkAccessManager(this);
    QNetworkReply *reply = networkMgr->get( QNetworkRequest( QUrl( "http://leonardogalli.ch/beta/options.json" ) ) );

    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loop.exec();

    // Lets print the HTTP GET response.
    //qDebug( reply->readAll());

    QNetworkReply *replyList = networkMgr->get( QNetworkRequest( QUrl( "http://leonardogalli.ch/beta/list.json" ) ) );

    QEventLoop loopList;
    QObject::connect(replyList, SIGNAL(finished()), &loopList, SLOT(quit()));

    // Execute the event loop here, now we will wait here until readyRead() signal is emitted
    // which in turn will trigger event loop quit.
    loopList.exec();
    QJsonDocument loadDocList = QJsonDocument::fromJson(replyList->readAll());
    QJsonDocument loadDoc = QJsonDocument::fromJson(reply->readAll());
    this->list = loadDocList.object();
    this->options = loadDoc.object();
    //setupFields();

}

UploadWindow::~UploadWindow()
{
    delete ui;
}
void UploadWindow::startUpload()
{

    setupFields();
     //initUpload();
    buildSubmit();
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

void UploadWindow::addField(QString type, QJsonObject field, int &row, int &column, QString key)
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
    } else if(type == "text")
    {
        QTextEdit *textEdit = new QTextEdit(this);
        this->ui->formLayout->addRow(label, textEdit);
        textEdits.insert(key, textEdit);
    } else if(type == "textfield")
    {
        QLineEdit *lineEdit = new QLineEdit(this);
        this->ui->formLayout->addRow(label, lineEdit);
    }
}

void UploadWindow::initUpload()
{
    //QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    //QNetworkRequest request(QUrl("http://leonardogalli.ch/beta/upload_single.php"));

    UploadManager *uploadManager = new UploadManager(this);
    connect(uploadManager, SIGNAL(uploadProg(QString,QString,float)), this, SLOT(uploadProgress(QString,QString,float)));
    connect(uploadManager, SIGNAL(uploadFinished()), this, SLOT(uploadFin()));
    QDirIterator it(this->dir, QDir::Files, QDirIterator::Subdirectories);
    QString dirName = dir.split("/").last();
    if(dirName == "") // This accounts for urls like /bla/bla/test/
    {
        dirName = dir.split("/").at(dir.split("/").count()-2);

    }
    while (it.hasNext()) {
        QString fileL = it.next();

        QFile file(fileL);
        QFileInfo info(file);
        QString folderName = info.absolutePath();
        QString fileT = folderName.replace(dirName, "|||");
        QString path = dirName + fileT.split("|||").last() + "/";
        qDebug() << fileL;
        qDebug() << path;
        uploadManager->addItem(fileL);
        uploadManager->addPath(path);
    }
    uploadManager->start();
}
void UploadWindow::uploadFin()
{
    this->ui->progressBar->setValue(100);
    this->ui->label->setText("");
    this->ui->pushButton->setEnabled(true);
}
void UploadWindow::uploadProgress(QString ulSpeed, QString timeRemaining, float percentage)
{
    this->ui->label->setText("Uploading with " + ulSpeed + ", " + timeRemaining + " remaining");
    this->ui->progressBar->setValue(percentage);
    this->ui->pushButton->setEnabled(false);
}
void UploadWindow::buildSubmit()
{
    QUrl url("http://leonardogalli.ch/beta/build_submit.php");
    QNetworkRequest request(url);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    for(int i = 0; i< this->ui->formLayout->rowCount(); i++)
    {
        QLabel *l = (QLabel*)this->ui->formLayoutWidget->childAt(1,i);
        qDebug() << l->text();

    }
    QHttpPart loginPart;

    loginPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"password\""));
    loginPart.setBody("pass");
    multiPart->append(loginPart);
    loginPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"path\""));
    //loginPart.setBody(this->paths.at(index).toLocal8Bit());
    //qDebug() << this->paths.at(index).toLocal8Bit();
    multiPart->append(loginPart);
    //QNetworkReply* reply = networkManager->post(request, multiPart);
    //this->currentReply = reply;
    //multiPart->setParent(reply);
}
