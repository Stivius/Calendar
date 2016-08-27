#include "filter.h"

Filter::Filter(Model* _model, QObject *parent) : QObject(parent), model(_model)
{
    connect(this, SIGNAL(updateRows(const QSet<int>&)), parent, SLOT(updateRows(const QSet<int>&)));
}

void Filter::changeDayFilter(size_t index, int value)
{
    for(int i = 0; i != model->count(); i++)
        if(model->getDay(i) != value)
            hiddenRows[index].insert(i);
}

void Filter::changeMonthFilter(size_t index, int value)
{
    for(int i = 0; i != model->count(); i++)
        if(model->getMonth(i) != value)
            hiddenRows[index].insert(i);
}

void Filter::changeYearFilter(size_t index, int value)
{
    for(int i = 0; i != model->count(); i++)
        if(model->getYear(i) != value)
            hiddenRows[index].insert(i);
}

void Filter::changeThemeFilter(size_t index, const QString& value)
{
    for(int i = 0; i != model->count(); i++)
        if(model->getTheme(i) != value)
            hiddenRows[index].insert(i);
}

void Filter::changePlaceFilter(size_t index, const QString& value)
{
    for(int i = 0; i != model->count(); i++)
        if(model->getPlace(i) != value)
            hiddenRows[index].insert(i);
}

void Filter::changeTextFilter(size_t index, const QString& value)
{
    for(int i = 0; i != model->count(); i++)
        if(!model->getSDescrpition(i).contains(value,Qt::CaseInsensitive))
            hiddenRows[index].insert(i);
}

void Filter::changeAnniverFilter(size_t index, int value)
{
    for(int i = 0; i != model->count(); i++)
        if(abs(value-model->getYear(i)) % 5 != 0)
            hiddenRows[index].insert(i);
}

void Filter::changeImageFilter(size_t index)
{
    for(int i = 0; i != model->count(); i++)
        if(model->imagesCount(i) == 0)
            hiddenRows[index].insert(i);
}

void Filter::setFilter(unsigned short flag, QString value)
{
    std::bitset<8> temp(flags);
    temp |= flag;
    if(flags == temp)
        removeFilter(flag);
    std::bitset<8> oldFlags(flags);
    flags |= flag;
    for(size_t i = 0; i != flags.size(); i++)
    {
        if(flags.test(i) != oldFlags.test(i))
        {
            switch(i)
            {
                case 0: changeDayFilter(i, value.toInt()); break;
                case 1: changeMonthFilter(i, value.toInt()); break;
                case 2: changeYearFilter(i, value.toInt()); break;
                case 3: changeThemeFilter(i, value); break;
                case 4: changePlaceFilter(i, value); break;
                case 5: changeTextFilter(i, value); break;
                case 6: changeAnniverFilter(i, value.toInt()); break;
                case 7: changeImageFilter(i); break;
            }
            for(int i = 0; i != 8; i++)
                unitedSet += hiddenRows[i];
            emit updateRows(unitedSet);
        }
    }
}

void Filter::removeFilter(unsigned short flag)
{
    std::bitset<8> oldFlags(flags);
    flags &= ~flag;
    for(size_t i = 0; i != flags.size(); i++)
    {
        if(flags.test(i) != oldFlags.test(i) && flags.test(i) == 0)
        {
            hiddenRows[i].clear();
            unitedSet.clear();
            for(int i = 0; i != 8; i++)
                unitedSet += hiddenRows[i];
            emit updateRows(unitedSet);
        }
    }
}

unsigned short Filter::getFilter()
{
    return flags.to_ulong();
}

QSet<int> Filter::getHiddenRows()
{
    return unitedSet;
}
