#include "settingssqlmodel.h"

#include <QSqlError>
#include <QDebug>

const QChar SECTIONS_SLITTER = '-';

//====================================================================================

SettingsSqlModel::SettingsSqlModel(QSqlDatabase database, QObject* parent) :
    QSqlTableModel(parent, database)
{
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
