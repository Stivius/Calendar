#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

#include "model/eventssqlmodel.h"

enum FilterType {
    DayFilter,
    MonthFilter,
    YearFilter,
    ThemeFilter,
    PlaceFilter,
    TextFilter,
    AnniversaryFilter
};

class EventsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
signals:
    void filterUpdated();

public:
    explicit EventsProxyModel(EventsSqlModel* model, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    void setFilter(FilterType filterType, QVariant value);
    void removeFilter(FilterType filterType);

private:
    EventsSqlModel* _model;
    QVector<QVariant> _filters;

};

#endif // TABLEMODEL_H
