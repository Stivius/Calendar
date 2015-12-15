#ifndef MANAGEUPDATE_H
#define MANAGEUPDATE_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QApplication>

class ManageUpdate: public QObject
{
    Q_OBJECT
public:
    ManageUpdate();
private slots:
    void erorCode(QNetworkReply::NetworkError);
    void downloadUpdate();
    void showProgress(qint64,qint64);
private:
    QNetworkReply* rep;
};

#endif // MANAGEUPDATE_H
