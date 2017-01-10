#include "eventssqlmodel.h"

#include <QDebug>
#include <QDate>

//====================================================================================

const QString DATE_PATTERN = "dd/MM/yyyy";
const QChar IMAGES_SPLITTER = '\n';

//====================================================================================

EventsSqlModel::EventsSqlModel(QSqlDatabase database, QObject* parent) :
    QSqlTableModel(parent, database)
{
    setTable("events");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();

    setHeaderData(column(Date), Qt::Horizontal, QString("Дата"));
    setHeaderData(column(ShortDescription), Qt::Horizontal, QString("Событие"));
    setHeaderData(column(Images), Qt::Horizontal, QString("Фото"));
    setHeaderData(column(Place), Qt::Horizontal, QString("Место"));
    setHeaderData(column(Source), Qt::Horizontal, QString("Источник"));
}

//====================================================================================

QVariant EventsSqlModel::data(const QModelIndex& index, int role) const
{
    return QSqlTableModel::data(index, role);
}

//====================================================================================

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

//====================================================================================

int EventsSqlModel::column(int role) const
{
    return role - Qt::UserRole;
}

//====================================================================================

QString EventsSqlModel::theme(int row) const
{
    return data(index(row, column(Theme)), Qt::DisplayRole).toString();
}

//====================================================================================

QString EventsSqlModel::place(int row) const
{
    return data(index(row, column(Place)), Qt::DisplayRole).toString();
}

//====================================================================================

QString EventsSqlModel::source(int row) const
{
    return data(index(row, column(Source)), Qt::DisplayRole).toString();
}

//====================================================================================

QString EventsSqlModel::shortDescription(int row) const
{
    return data(index(row, column(ShortDescription)), Qt::DisplayRole).toString();
}

//====================================================================================

QStringList EventsSqlModel::imagesList(int row) const
{
    return data(index(row, column(Images)), Qt::DisplayRole).toString().split(IMAGES_SPLITTER);
}

//====================================================================================

void EventsSqlModel::setImagesList(int row, QStringList imagesList)
{
    setData(index(row, column(Images)), imagesList.join(IMAGES_SPLITTER));
}

//====================================================================================

int EventsSqlModel::day(int row) const
{
    QDate d = QDate::fromString(date(row), DATE_PATTERN);
    return d.day();
}

//====================================================================================

int EventsSqlModel::month(int row) const
{
    QDate d = QDate::fromString(date(row), DATE_PATTERN);
    return d.month();
}

//====================================================================================

int EventsSqlModel::year(int row) const
{
    QDate d = QDate::fromString(date(row), DATE_PATTERN);
    return d.year();
}

//====================================================================================

QString EventsSqlModel::date(int row) const
{
    return data(index(row, column(Date)), Qt::DisplayRole).toString();
}

//====================================================================================

void EventsSqlModel::setDate(int row, int day, int month, int year)
{
    setData(index(row, column(Date)), QDate(year, month, day).toString(DATE_PATTERN));
}

//====================================================================================
