#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include <QObject>
#include <QtSql>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QProgressBar>

class EventsModel: public QObject
{
    Q_OBJECT
public:
    friend class NewEvent;
    friend class MainInterface;
    EventsModel();
    void save(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void update(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,int);
    void getdata();
    void upsettings(QString, int, int);
    void upfont(int);
    void getsettings();
    void del(int);
    int count();
    int imgcount();
    int getmonth(int);
    int getmonth(QString);
    QString getmonthname(int);
private slots:
    void erorCode(QNetworkReply::NetworkError);
    void downloadUpgrader();
    void checkVersion();
    void showProgress(qint64,qint64);
private:
    QNetworkAccessManager* network;
    QNetworkReply* rep;
    QNetworkReply* rep2;
    QProgressBar *bar;
    int quality;
    QString version;
    QString path;
    int img;
    int anniver;
    int font;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlRecord rec;
    QVector<int> id;
    QVector<int> tempid;
    QMap<int,int> day;
    QMap<int,QString> month;
    QMap<int,int> year;
    QMap<int,QString> theme;
    QMap<int,QString> sdesc;
    QMap<int,QString> ldesc;
    QMap<int,QString> place;
    QMap<int,QString> source;
    QMap<int,QString> extra;
    QMap<int,QVector<QString> > images;
    int size;
};

#endif // EVENTSMODEL_H
