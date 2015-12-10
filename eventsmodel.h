#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include <QObject>
#include <QtSql>

class EventsModel: public QObject
{
    Q_OBJECT
public:
    friend class NewEvent;
    friend class MainInterface;
    EventsModel();
    void save(int,QString,int,QString,QString,QString,QString,QString,QString,QString);
    void update(int,QString,int,QString,QString,QString,QString,QString,QString,QString,int);
    void getdata();
    void upsettings(QString, int, int);
    void getsettings();
    void del(int);
    int count();
    int imgcount();
    int getmonth(int);
    int getmonth(QString);
private:
    int quality;
    int show;
    QString path;
    int img;
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
