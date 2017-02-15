#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <memory>

class EventsSqlModel;

enum FilterType {
    DayFilter,
    MonthFilter,
    YearFilter,
    ThemeFilter,
    PlaceFilter,
    TextFilter,
    AnniversaryFilter,
    ImagesFilter
};

class EventsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
signals:
    void filterUpdated();

public:
    explicit EventsProxyModel(std::shared_ptr<EventsSqlModel>& model, QObject *parent = 0);
    ~EventsProxyModel();
    QVariant data(const QModelIndex &index, int role) const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    void setFilter(FilterType filterType, QVariant value);
    void removeFilter(FilterType filterType);

private:
    std::shared_ptr<EventsSqlModel> _model;
    QVector<QVariant> _filters;

};

#endif // TABLEMODEL_H
