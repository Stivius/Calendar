#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QApplication>
#include <QDebug>

class Database : public QObject
{
    Q_OBJECT
public:
    Database(QObject *parent = 0);
    ~Database();
    void getSettings(QString&, int&, int&, int&);
    int getData(QVector<int>&, QVector<int>&, QVector<int>&, QVector<int>&, QVector<QString>&, QVector<QString>&,
                QVector<QString>&, QVector<QString>&, QVector<QString>&, QVector<QString>&, QVector<QString>&);
    int insertEvent(const QVector<QString>&);
    void updateEvent(int, const QVector<QString>&);
    void removeEvent(int);
    void updateSettings(QString, int, int);
    void updateFont(int);
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
