#include "eventsproxymodel.h"

#include <QDate>
#include <QDebug>

EventsProxyModel::EventsProxyModel(EventsSqlModel* model, QObject *parent) :
    QSortFilterProxyModel(parent),
    _model(model),
    _filters(8, QVariant())
{
    setSourceModel(_model);
    setHeaderData(this->column(Date), Qt::Horizontal, QString("Дата"));
    setHeaderData(this->column(ShortDescription), Qt::Horizontal, QString("Событие"));
    setHeaderData(this->column(Images), Qt::Horizontal, QString("Фото"));
    setHeaderData(this->column(Place), Qt::Horizontal, QString("Место"));
    setHeaderData(this->column(Source), Qt::Horizontal, QString("Источник"));
}

QVariant EventsProxyModel::data(const QModelIndex &index, int role) const
{
    return QSortFilterProxyModel::data(index, role);
}

bool EventsProxyModel::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    switch(source_column)
    {
    case 0:
    case 5:
    case 6:
    case 7:
        return false;
    }
    return true;
}

bool EventsProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    if(_filters[DayFilter].isValid())
    {
        if(_model->day(source_row) != _filters[DayFilter])
            return false;
    }
    if(_filters[MonthFilter].isValid())
    {
        if(_model->month(source_row) != _filters[MonthFilter])
            return false;
    }
    if(_filters[YearFilter].isValid())
    {
        if(_model->year(source_row) != _filters[YearFilter])
            return false;
    }
    if(_filters[ThemeFilter].isValid())
    {
        if(_model->theme(source_row) != _filters[ThemeFilter])
            return false;
    }
    if(_filters[PlaceFilter].isValid())
    {
        if(_model->place(source_row) != _filters[PlaceFilter])
            return false;
    }
    if(_filters[TextFilter].isValid())
    {
        if(!_model->shortDescription(source_row).contains(_filters[TextFilter].toString(),Qt::CaseInsensitive))
            return false;
    }
    if(_filters[AnniversaryFilter].isValid())
    {
        if(abs(_filters[AnniversaryFilter].toInt() - _model->year(source_row)) % 5 != 0)
            return false;
    }
    return true;
}

void EventsProxyModel::setFilter(FilterType filterType, QVariant value)
{
    _filters[filterType] = value;
    invalidate();
}

void EventsProxyModel::removeFilter(FilterType filterType)
{
    _filters[filterType] = QVariant();
    invalidate();
}

QString EventsProxyModel::theme(int row) const
{
    return data(index(row, column(Theme)), Qt::DisplayRole).toString();
}

QString EventsProxyModel::place(int row) const
{
    return data(index(row, column(Place)), Qt::DisplayRole).toString();
}

QString EventsProxyModel::source(int row) const
{
    return data(index(row, column(Source)), Qt::DisplayRole).toString();
}

int EventsProxyModel::day(int row) const
{
    QDate d = QDate::fromString(date(row), "dd/MM/yyyy");
    return d.day();
}

int EventsProxyModel::month(int row) const
{
    QDate d = QDate::fromString(date(row), "dd/MM/yyyy");
    return d.month();
}

int EventsProxyModel::year(int row) const
{
    QDate d = QDate::fromString(date(row), "dd/MM/yyyy");
    return d.year();
}

QString EventsProxyModel::date(int row) const
{
    return data(index(row, column(Date)), Qt::DisplayRole).toString();
}

void EventsProxyModel::setDate(int row, int day, int month, int year)
{
    setData(index(row, column(Date)), QDate(year, month, day).toString("dd/MM/yyyy"));
}

bool EventsProxyModel::submitAll()
{
    return _model->submitAll();
}

void EventsProxyModel::revertAll()
{
    _model->revertAll();
}

QString EventsProxyModel::shortDescription(int row) const
{
    return data(index(row, column(ShortDescription)), Qt::DisplayRole).toString();
}

int EventsProxyModel::column(int role) const
{
    return role - (Qt::UserRole + 2);
}
