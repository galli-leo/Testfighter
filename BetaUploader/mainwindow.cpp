#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    uw = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QString file;
    foreach (const QUrl &url, e->mimeData()->urls()) {
        const QString &fileName = url.toLocalFile();
        file = fileName;
        qDebug() << "Dropped file:" << fileName;
    }

    if (!uw) {
        uw = new UploadWindow(this);
        uw->show();
        for (int i = 0; i < this->ui->horizontalLayout->count();i++) {
            if (QWidgetItem *myItem = dynamic_cast <QWidgetItem*>(this->ui->horizontalLayout->itemAt(i))) {
                QRadioButton *btn = (QRadioButton*)myItem->widget();
                qDebug() << btn->text();
                if(btn->isChecked()){
                    if(btn->text() == "Mac"){
                        uw->os = "osx";
                    }else if(btn->text() == "Windows"){
                        uw->os = "win";
                    }else if(btn->text() == "Linux"){
                        uw->os = "linux";
                    }


                }
            }
        }
        uw->dir = file;
        hide();
        uw->startUpload();


    }

}
