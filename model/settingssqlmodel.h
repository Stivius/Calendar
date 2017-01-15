#ifndef SETTINGSSQLMODEL_H
#define SETTINGSSQLMODEL_H

#include <QSqlTableModel>

enum SettingsModelRoles {
    Path = Qt::UserRole,
    Quality,
    Font,
    Anniversary,
    HeadersSizes
};

class SettingsSqlModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit SettingsSqlModel(QSqlDatabase database, QObject *parent = 0);
    ~SettingsSqlModel();
    QHash<int, QByteArray> roleNames() const;
    int column(int role);
    QStringList sectionSizes();
    void setSectionsSizes(QStringList sizes);
    bool anniversaryDates();
    void setFont(int fontSize);
    int font();
};

#endif // SETTINGSSQLMODEL_H
