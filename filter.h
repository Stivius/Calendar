#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QSet>
#include <QDate>
#include <QDebug>
#include <bitset>

#include "model.h"


#define DAY_FILTER 0x01 // hex for 0000 0001
#define MONTH_FILTER 0x02 // hex for 0000 0010
#define YEAR_FILTER 0x04 // hex for 0000 0100
#define THEME_FILTER 0x08 // hex for 0000 1000
#define PLACE_FILTER 0x10 // hex for 0001 0000
#define TEXT_FILTER 0x20 // hex for 0010 0000
#define ANNIVER_FILTER 0x40 // hex for 0100 0000
#define IMAGES_FILTER 0x80 // hex for 1000 0000

class Filter : public QObject
{
    Q_OBJECT
public:
    Filter(Model* _model, QObject *parent = 0);
    void setFilter(unsigned short, QString value = "");
    void removeFilter(unsigned short);
    unsigned short getFilter();
    QSet<int> getHiddenRows();
signals:
    void updateRows(const QSet<int>&);
private:
    void changeDayFilter(size_t, int);
    void changeMonthFilter(size_t, int);
    void changeYearFilter(size_t, int);
    void changeThemeFilter(size_t, QString);
    void changePlaceFilter(size_t, QString);
    void changeTextFilter(size_t, QString);
    void changeAnniverFilter(size_t, int);
    void changeImageFilter(size_t);
    Model* model;
    std::bitset<8> flags;
    QSet<int> hiddenRows[8];
    QSet<int> unitedSet;
};

#endif // FILTER_H
