#ifndef MODEL_H
#define MODEL_H

#include <QObject>

#include "database.h"

class Model: public QObject
{
    Q_OBJECT
public:
    Model();
    ~Model();
    void insertEvent(QVector<QString>);
    void updateEvent(int, QVector<QString>);
    void removeEvent(int);
    void updateFont(int);
    void updateSettings(QString, int, int);
    QString getPath();
    int getQuality();
    int getFont();
    int count();
    int getYear(int);
    int getDay(int);
    int getMonth(int);
    int getMonth(QString);
    QString getMonthName(int);
    QString getExtra(int);
    QString getSDescrpition(int);
    QString getLDescrpition(int);
    QString getDate(int);
    QString getTheme(int);
    QString getPlace(int);
    QString getSource(int);
    QString formatDate(int, int, int);
private:
    QString formatDay(int);
    QString formatMonth(int);
    QString formatYear(int);

    Database* db;
    int quality;
    QString path;
    int anniver;
    int font;
    QVector<int> id;
    QVector<int> days;
    QVector<int> months;
    QVector<int> years;
    QVector<QString> themes;
    QVector<QString> sDescriptions;
    QVector<QString> lDescriptions;
    QVector<QString> places;
    QVector<QString> sources;
    QVector<QString> extra;
    int size;
};

#endif // MODEL_H
