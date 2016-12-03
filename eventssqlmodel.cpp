#include "eventssqlmodel.h"

#include <QDebug>

EventsSqlModel::EventsSqlModel(QSqlDatabase database, QObject* parent) :
    QSqlTableModel(parent, database)
{
    setTable("events");
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

QString EventsSqlModel::theme(int row)
{
    return data(index(row, column(Theme)), Qt::DisplayRole).toString();
}

QString EventsSqlModel::place(int row)
{
    return data(index(row, column(Place)), Qt::DisplayRole).toString();
}

int EventsSqlModel::column(int role)
{
    return role - (Qt::UserRole + 1);
}
