#ifndef SETTINGSSQLMODEL_H
#define SETTINGSSQLMODEL_H

#include <QSqlTableModel>

enum SettingsModelRoles {
    Path = Qt::UserRole,
    Quality,
    Font,
    Anniversary
};

class SettingsSqlModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit SettingsSqlModel(QSqlDatabase database, QObject *parent = 0);
    QHash<int, QByteArray> roleNames() const;
    int column(int role);
    void setFont(int fontSize);
    int font();
};

#endif // SETTINGSSQLMODEL_H
