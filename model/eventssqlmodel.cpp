#include "eventssqlmodel.h"

#include <QDebug>
#include <QDate>

EventsSqlModel::EventsSqlModel(QSqlDatabase database, QObject* parent) :
    QSqlTableModel(parent, database)
{
    setTable("events");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
}

QVariant EventsSqlModel::data(const QModelIndex& index, int role) const
{
    return QSqlTableModel::data(index, role);
}

QHash<int, QByteArray> EventsSqlModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Id] = "eventId";
    roles[Date] = "date";
    roles[ShortDescription] = "shortDescription";
    roles[Images] = "images";
    roles[Place] = "place";
    roles[Source] = "source";
    roles[Theme] = "theme";
    roles[LongDescription] = "longDescription";
    roles[ExtraDescription] = "extraDescription";
    return roles;
}

int EventsSqlModel::column(int role) const
{
    return role - (Qt::UserRole + 1);
}

QString EventsSqlModel::theme(int row) const
{
    return data(index(row, column(Theme)), Qt::DisplayRole).toString();
}

QString EventsSqlModel::place(int row) const
{
    return data(index(row, column(Place)), Qt::DisplayRole).toString();
}

QString EventsSqlModel::date(int row) const
{
    return data(index(row, column(Date)), Qt::DisplayRole).toString();
}

QString EventsSqlModel::shortDescription(int row) const
{
    return data(index(row, column(ShortDescription)), Qt::DisplayRole).toString();
}

QString EventsSqlModel::source(int row) const
{
    return data(index(row, column(Source)), Qt::DisplayRole).toString();
}

int EventsSqlModel::day(int row) const
{
    QDate d = QDate::fromString(date(row), "dd/MM/yyyy");
    return d.day();
}

int EventsSqlModel::month(int row) const
{
    QDate d = QDate::fromString(date(row), "dd/MM/yyyy");
    return d.month();
}

int EventsSqlModel::year(int row) const
{
    QDate d = QDate::fromString(date(row), "dd/MM/yyyy");
    return d.year();
}
