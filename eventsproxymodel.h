#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QSortFilterProxyModel>

#include "eventssqlmodel.h"

class EventsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit EventsProxyModel(EventsSqlModel* model, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    int column(int role) const;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    QString date(int row) const;
    QString shortDescription(int row) const;
    QString theme(int row) const;
    QString place(int row) const;
    QString source(int row) const;
    int day(int row) const;
    int month(int row) const;
    int year(int row) const;
    void setDate(int row, int day, int month, int year);
private:
    EventsSqlModel* _model;

};

#endif // TABLEMODEL_H
