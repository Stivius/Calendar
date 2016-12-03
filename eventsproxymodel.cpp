#include "eventsproxymodel.h"

#include <QDate>
#include <QDebug>

EventsProxyModel::EventsProxyModel(EventsSqlModel* model, QObject *parent) :
    QSortFilterProxyModel(parent),
    _model(model)
{
    setSourceModel(_model);
    setHeaderData(_model->column(Date), Qt::Horizontal, QString("Дата"));
    setHeaderData(_model->column(ShortDescription), Qt::Horizontal, QString("Событие"));
    setHeaderData(_model->column(Images), Qt::Horizontal, QString("Фото"));
    setHeaderData(_model->column(Place), Qt::Horizontal, QString("Место"));
    setHeaderData(_model->column(Source), Qt::Horizontal, QString("Источник"));
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
        return false;
    }
    return true;
}

bool EventsProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(source_row == 0)
        return false;
    return true;
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

QString EventsProxyModel::shortDescription(int row) const
{
    return data(index(row, column(ShortDescription)), Qt::DisplayRole).toString();
}

int EventsProxyModel::column(int role) const
{
    return role - (Qt::UserRole + 2);
}
