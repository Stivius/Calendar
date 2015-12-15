#include "manageupdate.h"

ManageUpdate::ManageUpdate()
{
    QNetworkAccessManager* network = new QNetworkAccessManager;
    QUrl url("ftp://ftp.tech-century.com/Calendar.exe");
    url.setPassword("123456");
    url.setUserName("stivius@tech-century.com");
    QNetworkRequest req(url);
    rep = network->get(req);
    connect(rep,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(erorCode(QNetworkReply::NetworkError)));
    connect(rep,SIGNAL(finished()),this,SLOT(downloadUpdate()));
    connect(rep,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(showProgress(qint64,qint64)));
}

void ManageUpdate::erorCode(QNetworkReply::NetworkError error)
{
    qDebug() << "Error code: " <<  error;
}


void ManageUpdate::showProgress(qint64 received, qint64 total)
{
    total += 0;
    qDebug() << "Received bytes: " << received << " " << "Total bytes: " << rep->size();
}

void ManageUpdate::downloadUpdate()
{
    if(rep->bytesAvailable() != 0)
    {
        QFile file(QApplication::applicationDirPath() + "/Calendar.exe");
        file.remove();
        file.link(QApplication::applicationDirPath() + "/Calendar.exe");
        file.open(QIODevice::ReadWrite);
        file.write(rep->readAll());
        file.close();
        QProcess upd;
        QString string = QApplication::applicationDirPath() + "/Calendar.exe";
        upd.startDetached(string,QStringList());
        QApplication::quit();
    }
}
