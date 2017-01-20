#include "eventssqlmodel.h"

#include <QDebug>
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>

//====================================================================================

const QString FORMATTED_DATE = "%1/%2/%3";
const QString UNFORMATTED_DATE = "%3/%2/%1";
const QString UNKNOWN_DAY = "00";
const QString UNKNOWN_MONTH = "00";
const QString UNKNOWN_YEAR = "0000";
const QString KNOWN_DAY = "dd";
const QString KNOWN_MONTH = "MM";
const QString KNOWN_YEAR = "yyyy";

const QChar IMAGES_SPLITTER = '\n';
const QChar DATE_SPLITTER = '/';

const int DAY_INDEX = 0;
const int MONTH_INDEX = 1;
const int YEAR_INDEX = 2;

const int DEFAULT_DAY = 1;
const int DEFAULT_MONTH = 1;
const int DEFAULT_YEAR = 1;

const QString CREATE_EVENTS_TABLE = "CREATE TABLE IF NOT EXISTS `events` ("
                                    "`id`	INTEGER,"
                                    "`date`	TEXT,"
                                    "`theme`	TEXT,"
                                    "`shortDescription`	TEXT,"
                                    "`fullDescription`	TEXT,"
                                    "`place`	TEXT,"
                                    "`source`	TEXT,"
                                    "`extraDescription`	TEXT,"
                                    "`images`	TEXT,"
                                    "PRIMARY KEY(id)"
                                    ");";

//====================================================================================

EventsSqlModel::EventsSqlModel(QSqlDatabase database, QObject* parent) :
    QSqlTableModel(parent, database)
{
    database.exec(CREATE_EVENTS_TABLE);

    setTable("events");
    setSort(column(Date), Qt::AscendingOrder);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();

    updateHeadersData();
}

//====================================================================================

EventsSqlModel::~EventsSqlModel()
{
    //qDebug() << "eventssql model deleted";
}

//====================================================================================

QVariant EventsSqlModel::data(const QModelIndex& index, int role) const
{
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(index.column() == column(Images))
        {
            if(imagesList(index.row()).size() > 0)
                return QString(tr("Есть"));
            else
                return QString(tr("Нет"));
        }
        else if(index.column() == column(Date))
        {
            return date(index.row());
        }

    }
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
    roles[FullDescription] = "fullDescription";
    roles[ExtraDescription] = "extraDescription";
    return roles;
}

//====================================================================================

void EventsSqlModel::insertEvent(const QString &date, const QString &eventDescription)
{
    insertRow(rowCount());
    setData(index(rowCount()-1, column(Date)), date);
    setData(index(rowCount()-1, column(ShortDescription)), eventDescription);
    submitAll();
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

QString EventsSqlModel::fullDescription(int row) const
{
    return data(index(row, column(FullDescription)), Qt::DisplayRole).toString();
}

//====================================================================================

QStringList EventsSqlModel::imagesList(int row) const
{
    return QSqlTableModel::data(QSqlTableModel::index(row, column(Images)), Qt::DisplayRole).toString().split(IMAGES_SPLITTER, QString::SkipEmptyParts);
}

//====================================================================================

void EventsSqlModel::setImagesList(int row, QStringList imagesList)
{
    setData(index(row, column(Images)), imagesList.join(IMAGES_SPLITTER));
}

//====================================================================================

int EventsSqlModel::day(int row) const
{
    QString s = date(row);

    QStringList splittedDate = s.split(QChar(DATE_SPLITTER), QString::SkipEmptyParts);
    if(splittedDate.isEmpty())
        return 0;

    return splittedDate[DAY_INDEX].toInt();;
}

//====================================================================================

int EventsSqlModel::month(int row) const
{
    QString s = date(row);

    QStringList splittedDate = s.split(QChar(DATE_SPLITTER), QString::SkipEmptyParts);
    if(splittedDate.isEmpty())
        return 0;

    return splittedDate[MONTH_INDEX].toInt();
}

//====================================================================================

int EventsSqlModel::year(int row) const
{
    QString s = date(row);

    QStringList splittedDate = s.split(QChar(DATE_SPLITTER), QString::SkipEmptyParts);
    if(splittedDate.isEmpty())
        return 0;

    return splittedDate[YEAR_INDEX].toInt();
}

//====================================================================================

QString EventsSqlModel::date(int row) const
{
    QString s = QSqlTableModel::data(QSqlTableModel::index(row, column(Date)), Qt::DisplayRole).toString();
    QStringList splittedDate = s.split(QChar(DATE_SPLITTER), QString::SkipEmptyParts);
    if(splittedDate.isEmpty())
        return QString();

    std::reverse(splittedDate.begin(), splittedDate.end());

    int day = splittedDate[DAY_INDEX].toInt();
    int month = splittedDate[MONTH_INDEX].toInt();
    int year = splittedDate[YEAR_INDEX].toInt();

    QString finalUnformattedFormat = getFormat(day, month, year, UNFORMATTED_DATE);
    QString finalFormattedFormat = getFormat(day, month, year, FORMATTED_DATE);

    QDate d = QDate::fromString(s, finalUnformattedFormat);
    return d.toString(finalFormattedFormat);
}

//====================================================================================

bool EventsSqlModel::setDate(int row, int day, int month, int year)
{
    QString finalFormat = getFormat(day, month, year, UNFORMATTED_DATE);
    day = (day == 0) ? DEFAULT_DAY : day;
    month = (month == 0) ? DEFAULT_MONTH : month;
    year = (year == 0) ? DEFAULT_YEAR : year;
    if(!QDate(year, month, day).isValid())
        return false;
    setData(index(row, column(Date)), QDate(year, month, day).toString(finalFormat));
    return true;
}

//====================================================================================

QString EventsSqlModel::getFormat(int day, int month, int year, const QString& format) const
{
    QString finalDay = (day == 0) ? UNKNOWN_DAY : KNOWN_DAY;
    QString finalMonth = (month == 0) ? UNKNOWN_MONTH : KNOWN_MONTH;
    QString finalYear = (year == 0) ? UNKNOWN_YEAR : KNOWN_YEAR;
    QString finalFormat = format.arg(finalDay).arg(finalMonth).arg(finalYear);
    return finalFormat;
}

//====================================================================================

void EventsSqlModel::updateHeadersData()
{
    setHeaderData(column(Date), Qt::Horizontal, QString(tr("Дата")));
    setHeaderData(column(ShortDescription), Qt::Horizontal, QString(tr("Событие")));
    setHeaderData(column(Images), Qt::Horizontal, QString(tr("Фото")));
    setHeaderData(column(Place), Qt::Horizontal, QString(tr("Место")));
    setHeaderData(column(Source), Qt::Horizontal, QString(tr("Источник")));
}

//====================================================================================

