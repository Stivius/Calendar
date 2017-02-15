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
                             "`language`	INTEGER DEFAULT 0,"
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
        database.exec(INSERT_DATA.arg(QApplication::applicationDirPath()+ "/"));

    setEditStrategy(QSqlTableModel::OnManualSubmit);
    setTable("settings");
    select();
}

//====================================================================================

SettingsSqlModel::~SettingsSqlModel()
{
    qDebug() << "settingssql model deleted";
}

//====================================================================================

QHash<int, QByteArray> SettingsSqlModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ImagesFolder] = "path";
    roles[Quality] = "quality";
    roles[Font] = "font";
    roles[AnniversaryDates] = "anniversary";
    roles[CurrentLanguage] = "currentLanguage";
    roles[HeadersSizes] = "headersSize";
    return roles;
}

//====================================================================================

int SettingsSqlModel::column(int role)
{
    return role - Qt::UserRole;
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
    return data(index(0, column(Font))).toInt();
}

//====================================================================================

void SettingsSqlModel::setHeadersSizes(QStringList sizes)
{
    setData(index(0, column(HeadersSizes)), sizes.join(SECTIONS_SLITTER));
    submitAll();
}

//====================================================================================

QStringList SettingsSqlModel::headersSizes()
{
    return data(index(0, column(HeadersSizes))).toString().split(SECTIONS_SLITTER);
}

//====================================================================================

bool SettingsSqlModel::anniversaryDates()
{
    return data(index(0, column(AnniversaryDates))).toBool();
}

//====================================================================================

QString SettingsSqlModel::imagesFolder()
{
    return data(index(0, column(ImagesFolder))).toString();
}

//====================================================================================

void SettingsSqlModel::setLanguage(Language language)
{
    setData(index(0, column(CurrentLanguage)), language);
    submitAll();
}

//====================================================================================

Language SettingsSqlModel::language()
{
    return static_cast<Language>(data(index(0, column(CurrentLanguage))).toInt());
}

//====================================================================================
