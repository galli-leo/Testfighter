#include "uploadmanager.h"

UploadManager::UploadManager(QObject *parent) :
    QObject(parent)
{


}
void UploadManager::addItem(QString file)
{
    this->itemsToUpload.append(file);
}
void UploadManager::addPath(QString path)
{
    this->paths.append(path);
}

void UploadManager::start()
{
    this->currentUploadIndex = 0;
    this->totalSize = 0;
    this->UlSize = 0;
    this->lastUlSize = 0;
    this->times = 0;
    lastUlNice = 0;
    //Get all headers to get content length
    qDebug() << "first: " << itemsToUpload.at(0);
    foreach(QString url, this->itemsToUpload)
    {
       QFile file(url);
       file.open(QIODevice::ReadOnly);
       qDebug() << "lol";
       qDebug() << url;
       this->totalSize += file.size()+360; //360 is the size of the post request without any file so we have to add it
       file.close();
    }
    qDebug() << "Total size to upload: " << this->totalSize;
    this->networkManager = new QNetworkAccessManager(this);
    this->startUpload(0);
    this->time = (double)QDateTime::currentMSecsSinceEpoch()/1000;
    lastTimeNice = (double)QDateTime::currentMSecsSinceEpoch()/1000;
    lastTime = 0;
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(5000);

}
void UploadManager::update()
{
    lastUlNice = 0;
    lastTimeNice = (double)QDateTime::currentMSecsSinceEpoch()/1000;
}
QString UploadManager::readableTime(int secconds)
{
    //Break into sec min hours days
    int secs = secconds%60;
    secconds/=60;
    int mins = secconds%60;
    secconds/= 60;
    int hours = secconds%60;
    secconds/= 60;
    int days = secconds%60;

    QMap<QString, int> numbers;
    numbers.insert("day", days);
    numbers.insert("hour", hours);
    numbers.insert("minute", mins);
    numbers.insert("seccond", secs);
    foreach(QString key, numbers.keys())
    {
       if(numbers.find(key).value() != 0)
       {
           int value = numbers.find(key).value();
           int keyIdx = numbers.keys().indexOf(key);

           if(numbers.values().count()> keyIdx+1)
           {
                QString secKey = numbers.keys().at(keyIdx+1);

                if(numbers.values().at(keyIdx+1) != 0)
                {
                    return QString::number(value) + plural(value, " "+key) + ", " + QString::number(numbers.values().at(keyIdx+1)) + plural(numbers.values().at(keyIdx+1), " "+secKey);
                }
                else
                {
                    return QString::number(value) + plural(value, " "+key);
                }
           }
           else
           {
               return QString::number(value) + plural(value, " "+key);
           }
       }
    }
    return "";


}
QString UploadManager::plural(int times, QString base)
{
    if(times == 1)
    {
        return base;
    }
    else
    {
        return base + "s";
    }
}

QString UploadManager::niceSpeed(qint64 speed)
{
    int bytes = speed;
    float kbytes = (float)speed/1000;
    float mbytes = (float)speed/1000000;
    if(mbytes >= 1)
    {
        return QString::number(round(mbytes, 2)) + " MB/s";
    }
    else if(kbytes >= 1)
    {
        return QString::number(round(kbytes, 2)) + " KB/s";
    }
    else
    {
        return QString::number(bytes) + " B/s";
    }
}

/* round number n to d decimal points */
double UploadManager::round(double n, unsigned d)
{
      return floor(n * pow(10., d) + .5) / pow(10., d);
}

void UploadManager::startUpload(int index)
{
    lastUlSize = 0;
    QFile *file =  new QFile(this->itemsToUpload.at(index));
    QUrl url("http://leonardogalli.ch/beta/upload_single.php");
    QNetworkRequest request(url);
    this->currentFile = file;
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart loginPart;
    /* password */
    loginPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"password\""));
    loginPart.setBody("testfighter2015");
    multiPart->append(loginPart);
    loginPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"path\""));
    loginPart.setBody(this->paths.at(index).toLocal8Bit());
    qDebug() << this->paths.at(index).toLocal8Bit();
    multiPart->append(loginPart);
    QHttpPart filePart;
    /* important that the files[] variable have the brackets, for PHP to interpret correctly */
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"uploaded\"; filename=\""+ file->fileName() + "\""));
    qDebug() << "form-data; name=\"file1\"; filename=\""+ file->fileName() + "\"";
    file->open(QIODevice::ReadOnly | QIODevice::Unbuffered);

    //qDebug() << file->readAll();
    filePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
    multiPart->append(filePart);



    QNetworkReply* reply = networkManager->post(request, multiPart);
    this->currentReply = reply;
    multiPart->setParent(reply); // delete the multiPart with the reply
    QObject::connect(reply,SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgress(qint64,qint64)));
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(finished()));
    QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
}
void UploadManager::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    if(bytesSent!=0 && bytesTotal != 0)
    {
    qDebug() << "Uploaded:" << bytesSent << " bytes of total: " << bytesTotal << "diff to total: " << bytesTotal-this->totalSize;
    qint64 bytesDiff = bytesSent - this->lastUlSize;
    this->UlSize += bytesDiff;
    this->lastUlSize = bytesSent;
    double currentTime = (double)QDateTime::currentMSecsSinceEpoch()/1000;
    double timeDiff = currentTime-this->lastTimeNice;
    lastUlNice += bytesDiff;

    qDebug() << timeDiff;
    qDebug() << currentTime;
    qDebug() << lastTimeNice;
    if(timeDiff != 0.0){
        qDebug() << "Uploadspeed: " << ((bytesDiff/timeDiff));

        qint64 remainingUl = this->totalSize - this->UlSize;
        qint64 speed = ((lastUlNice/timeDiff)); //Download Speed in B/s
        int remainingTimeSec = remainingUl/speed;
        qDebug() << "Remaining time: " <<  this->readableTime(remainingTimeSec) << " percentage: " << (this->UlSize/this->totalSize)*100 << " speed: " << this->niceSpeed(speed);
        float percentage = ((float)this->UlSize/(float)this->totalSize)*100;
        emit uploadProg(this->niceSpeed(speed), this->readableTime(remainingTimeSec), percentage);
    }
    }
    qDebug() << "Hello there";

    lastTime = (double)QDateTime::currentMSecsSinceEpoch()/1000;

}
void UploadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << "Bytes received: " << bytesReceived << " of total: " << bytesTotal;
    qDebug() << "Time since last upload: " << (double)QDateTime::currentMSecsSinceEpoch()/1000-lastTime;
    totalTime += ((double)QDateTime::currentMSecsSinceEpoch()/1000-lastTime);
}

void UploadManager::finished()
{
    this->currentUploadIndex++;
    QByteArray data = currentReply->readAll();
    qDebug() << data;
    QString dataString = QString(data);
    if(!dataString.contains("has been uploaded"))
    {
        emit uploadFinished();
        qDebug() << "There has been an error while uploading: " << data;

        QMessageBox::critical(0,"Error while uploading", "There has been an error while uploading: " + data);
    }
    else
    {
    currentReply->deleteLater();
    if(this->currentUploadIndex >= this->itemsToUpload.count())
    {
        qDebug() << "Total time lost: " << totalTime;
        emit uploadFinished();

    }
    else
    {
        this->startUpload(this->currentUploadIndex);
    }
    qDebug() << this->currentUploadIndex;
    }
}
