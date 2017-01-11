#include "eventsproxymodel.h"
#include "model/eventssqlmodel.h"

#include <QDate>
#include <QDebug>

//====================================================================================

const int NUMBER_OF_FILTERS = 8;

//====================================================================================

EventsProxyModel::EventsProxyModel(EventsSqlModel* model, QObject *parent) :
    QSortFilterProxyModel(parent),
    _model(model),
    _filters(NUMBER_OF_FILTERS, QVariant())
{
    setSourceModel(_model);
}

//====================================================================================

QVariant EventsProxyModel::data(const QModelIndex &index, int role) const
{
    return QSortFilterProxyModel::data(index, role);
}

//====================================================================================

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
    if(_filters[ImagesFilter].isValid())
    {
        if(_model->imagesList(source_row).size() == _filters[ImagesFilter].toInt())
            return false;
    }
    return true;
}

//====================================================================================

void EventsProxyModel::setFilter(FilterType filterType, QVariant value)
{
    _filters[filterType] = value;
    invalidate();
    emit filterUpdated();
}

//====================================================================================

void EventsProxyModel::removeFilter(FilterType filterType)
{
    _filters[filterType] = QVariant();
    invalidate();
    emit filterUpdated();
}

//====================================================================================
