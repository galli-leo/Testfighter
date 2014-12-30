#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) :
    QObject(parent)
{


}
void DownloadManager::addItem(QUrl url)
{
    this->downloadingItems.append(url);
}
void DownloadManager::start()
{
    this->currentDownloadIndex = 0;
    this->currentHeaderIndex = 0;
    this->totalSize = 0;
    this->dlSize = 0;
    this->lastDlSize = 0;
    this->times = 0;
    //Get all headers to get content length
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(headersFinished(QNetworkReply*)));
    foreach(QUrl url, this->downloadingItems)
    {
       manager->head(QNetworkRequest(url));
       printf("\nGetting Headers for url: %s", url.toString().toStdString().c_str());
    }




}


void DownloadManager::headersFinished(QNetworkReply * reply){
    if (reply->operation() == QNetworkAccessManager::HeadOperation){

        int content_length = reply->header(QNetworkRequest::ContentLengthHeader).toInt();

        this->totalSize += content_length;
        this->currentHeaderIndex++;
        if(this->currentHeaderIndex== this->downloadingItems.count())
        {
            printf("\nHeaders gathering complete, total size to download: %s bytes", QString::number(this->totalSize).toStdString().c_str());
            this->startDownloading();
        }
    }
    reply->deleteLater();

}
const char* DownloadManager::fromQString(QString string)
{
    return string.toStdString().c_str();
}
void DownloadManager::startDownloading()
{
    this->time = QDateTime::currentMSecsSinceEpoch()/1000;
    FileDownloader* downloader = new FileDownloader(this->downloadingItems.at(this->currentDownloadIndex), this, this->downloadingPaths.at(this->currentDownloadIndex));

    connect(downloader, SIGNAL(downloadProg(qint64,qint64)), SLOT(downloadProg(qint64,qint64)));
    connect(downloader, SIGNAL(downloaded()), SLOT(downloadFinished()));

}
void DownloadManager::downloadProg(qint64 bytesReceived, qint64 bytesTotal)
{
    this->times++;
    qint64 bytesDiff = bytesReceived - this->lastDlSize;
    this->dlSize += bytesDiff;
    this->lastDlSize = bytesReceived;
    int timeDiff = QDateTime::currentMSecsSinceEpoch()/1000-this->time;
    if(timeDiff!= 0)
    {
        printf("\nDownloading Speed: %s, dlSize: %s, bytesDiff: %s", QString::number((this->dlSize/timeDiff)).toStdString().c_str(), QString::number(this->dlSize).toStdString().c_str(), QString::number(bytesDiff).toStdString().c_str());

        qint64 remainingDl = this->totalSize - this->dlSize;
        qint64 speed = ((this->dlSize/timeDiff)); //Download Speed in B/s
        int remainingTimeSec = remainingDl/speed;
        printf("\nRemaining time: %s, times: %i, percentage: %i", readableTime(remainingTimeSec).toStdString().c_str(), this->times, (this->dlSize/this->totalSize)*100);
        //We only want every 10th time to update ui
        if(this->times == 10)
        {

            emit progress(readableTime(remainingTimeSec), niceSpeed(speed), (float((float)this->dlSize/this->totalSize))*100);
            this->times = 0;
        }
    }

}
void DownloadManager::downloadFinished()
{
    this->lastDlSize = 0;
    this->currentDownloadIndex++;
    if(this->currentDownloadIndex == this->downloadingItems.count())
    {
        //WE are done
        printf("\nFinished downloading all items total dl: %s", QString::number(this->dlSize).toStdString().c_str());
        emit finished();

    }
    else
    {
        FileDownloader* downloader = new FileDownloader(this->downloadingItems.at(this->currentDownloadIndex), this, this->downloadingPaths.at(this->currentDownloadIndex));
        connect(downloader, SIGNAL(downloadProg(qint64,qint64)), SLOT(downloadProg(qint64,qint64)));
        connect(downloader, SIGNAL(downloaded()), SLOT(downloadFinished()));
    }
}
QString DownloadManager::readableTime(int secconds)
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
QString DownloadManager::plural(int times, QString base)
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

QString DownloadManager::niceSpeed(qint64 speed)
{
    int bytes = speed;
    float kbytes = (float)speed/1000;
    float mbytes = (float)speed/1000000;
    if(mbytes != 0)
    {
        return QString::number(round(mbytes, 1)) + " MB/s";
    }
    else if(kbytes != 0)
    {
        return QString::number(round(kbytes, 1)) + " KB/s";
    }
    else
    {
        return QString::number(bytes) + " B/s";
    }
}

/* round number n to d decimal points */
double DownloadManager::round(double n, unsigned d)
{
      return qFloor(n * qPow(10., d) + .5) / qPow(10., d);
}

void DownloadManager::addPath(QString path)
{
    this->downloadingPaths.append(path);
}
