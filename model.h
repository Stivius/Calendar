#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QApplication>
#include <QDebug>
#include <QFile>

#include "database.h"

class Model: public QObject
{
    Q_OBJECT
public:
    Model();
    ~Model();
    void insertEvent(QVector<QString>& data);
    void updateEvent(int index, QVector<QString>& data);
    void removeEvent(int index);
    void updateFont(int _font);
    void updateHeaders(const QVector<int>& headers);
    void updateSettings(const QString& path, int _quality, int _anniver);
    QString getPath();
    int getQuality();
    int getFont();
    int getHeader(int number);

    int count();
    int imagesCount(int index);
    int getYear(int index);
    int getDay(int index);
    int getMonth(int index);
    int getMonth(const QString& monthName);
    QString getMonthName(int index);
    QString getExtra(int index);
    QString getSDescrpition(int index);
    QString getLDescrpition(int index);
    QString getDate(int index);
    QString getTheme(int index);
    QString getPlace(int index);
    QString getSource(int index);
    QString getImages(int index);
    QString formatDate(int day, int month, int year);

    bool startMultiInsertion();
    bool finishMultiInsertion();
private:
    QString formatDay(int day);
    QString formatMonth(int month);
    QString formatYear(int year);

    Database* db;
    int quality;
    QString path;
    int anniver;
    int font;
    QString headers;

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
    QVector<QString> images;
    int size;
};

#endif // MODEL_H
