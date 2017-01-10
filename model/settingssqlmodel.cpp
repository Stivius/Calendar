#include "settingssqlmodel.h"

#include <QSqlError>
#include <QDebug>

const QString UPDATE_FONT = "UPDATE settings SET font='%1'";

//====================================================================================

SettingsSqlModel::SettingsSqlModel(QSqlDatabase database, QObject* parent) :
    QSqlTableModel(parent, database)
{
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    setTable("settings");
    select();
}

//====================================================================================

QHash<int, QByteArray> SettingsSqlModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Path] = "path";
    roles[Quality] = "quality";
    roles[Font] = "font";
    roles[Anniversary] = "anniversary";
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
    return role - (Qt::UserRole + 1);
}

//====================================================================================
