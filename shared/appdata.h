#ifndef APPDATA_H
#define APPDATA_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>
#include <QDesktopServices>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class AppData : public QObject
{
    Q_OBJECT
public:
    static AppData* Instance();

    /*
     * Gets the executable for a certian application under the specific path
     * e.g.: /Applications/DiskUtility -> /Applications/DiskUtility.app/Contents/MacOS/DiskUtility
     */
    QString executablePath(QString path);

    /*
     * The extension of an application on the current operating system.
     * e.g. on MacOS .app
     */
    QString appExtension;

    /*
     * A json object of the settings.json file stored in the docsDirectory.
     *
     */
    QJsonObject settings;

    /*
     * This is the writable documents directory for example storing config files.
     *
     */
    QString docsDirectory;

    /*
     * The name of the current os.
     *
     */
    QString osName;

    /*
     * Checks if the given application needs an update.
     *
     */
    bool checkForUpdate(QString appName);

    /*
     * Helper function for setting an item in a QJsonObject. (Currently QJsonObjects cannot be written to.)
     *
     */
    QJsonObject setItem(QJsonObject dict, QString key, QJsonValue value);

    /*
     * Gets a string response from endpoint.
     * Runs synchronously.
     */
    static QString getStringResponse(QString endPoint);

    /*
     * Gets a json response from endPoint (e.g. x.php?y=z) and returns a correct json dict.
     * Runs synchronously, thus only use sparsly.
     */
    static QJsonObject getJsonResponse(QString endPoint);

    /*
     * Gets a string response from endPoint.
     * Runs asynchronously.
     */
    static QNetworkReply getStringResponseAsync(QString endPoint);

signals:
    void gotStringResponse();

public slots:

private:
    explicit AppData(QObject *parent = 0);
    static AppData* m_pInstance;
    ~AppData();
};

#endif // APPDATA_H
