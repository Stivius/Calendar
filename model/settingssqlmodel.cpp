#include "settingssqlmodel.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QApplication>

const QChar SECTIONS_SLITTER = '-';
const QString CREATE_TABLE = "CREATE TABLE IF NOT EXISTS `settings` ("
                             "`path`	TEXT,"
                             "`quality`	INTEGER DEFAULT 100,"
                             "`font`	INTEGER DEFAULT 14,"
                             "`anniversaryDates`	INTEGER DEFAULT 0,"
                             "`headersSizes`	TEXT DEFAULT '270-270-270-270-270'"
                             ");";
const QString FETCH_DATA = "SELECT * FROM settings;";
const QString INSERT_DATA = "INSERT INTO settings (path) VALUES ('%1');";

//====================================================================================

SettingsSqlModel::SettingsSqlModel(QSqlDatabase database, QObject* parent) :
    QSqlTableModel(parent, database)
{
    database.exec(CREATE_TABLE);
    if(!database.exec(FETCH_DATA).first())
        database.exec(INSERT_DATA.arg(QApplication::applicationDirPath()));

    setEditStrategy(QSqlTableModel::OnManualSubmit);
    setTable("settings");
    select();
}

//====================================================================================

SettingsSqlModel::~SettingsSqlModel()
{
    //qDebug() << "settingssql model deleted";
}

//====================================================================================

QHash<int, QByteArray> SettingsSqlModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Path] = "path";
    roles[Quality] = "quality";
    roles[Font] = "font";
    roles[Anniversary] = "anniversary";
    roles[HeadersSizes] = "headersSize";
    return roles;
}

//====================================================================================

void SettingsSqlModel::setFont(int fontSize)
{
    setData(index(0, column(Font)), fontSize);
    submitAll();
}

//====================================================================================

int SettingsSqlModel::font()
{
    return QSqlTableModel::data(index(0, column(Font))).toInt();
}

//====================================================================================

int SettingsSqlModel::column(int role)
{
    return role - Qt::UserRole;
}

//====================================================================================

QStringList SettingsSqlModel::sectionSizes()
{
    return data(index(0, column(HeadersSizes))).toString().split(SECTIONS_SLITTER);
}

//====================================================================================

void SettingsSqlModel::setSectionsSizes(QStringList sizes)
{
    setData(index(0, column(HeadersSizes)), sizes.join(SECTIONS_SLITTER));
    submitAll();
}

//====================================================================================

bool SettingsSqlModel::anniversaryDates()
{
    return QSqlTableModel::data(index(0, column(Anniversary))).toBool();
}

//====================================================================================
