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
    void getSettings(QString& path, int& quality, int& anniver, int& font, QString& headers);
    int getData(QVector<int>& id, QVector<int>& days, QVector<int>& months, QVector<int>& years,
                QVector<QString>& themes, QVector<QString>& sDescriptions, QVector<QString>& lDescriptions,
                QVector<QString>& places, QVector<QString>& sources, QVector<QString>& extra, QVector<QString>& images);
    int insertEvent(const QVector<QString>& data);
    void updateEvent(int id, const QVector<QString>& data);
    void removeEvent(int id);
    void updateSettings(const QString& path, int quality, int anniver);
    void updateFont(int font);
    void updateHeaders(const QString& headers);

    bool startTransaction();
    bool finishTransaction();
private:
    QSqlDatabase db;
};

#endif // DATABASE_H
