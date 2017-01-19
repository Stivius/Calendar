#ifndef SETTINGSSQLMODEL_H
#define SETTINGSSQLMODEL_H

#include <QSqlTableModel>

#include "model/translationmodel.h"

enum SettingsModelRoles {
    ImagesFolder = Qt::UserRole,
    Quality,
    Font,
    AnniversaryDates,
    CurrentLanguage,
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
    void setFont(int fontSize);
    int font();
    void setHeadersSizes(QStringList sizes);
    QStringList headersSizes();
    bool anniversaryDates();
    QString imagesFolder();
    void setLanguage(Language language);
    Language language();

};

#endif // SETTINGSSQLMODEL_H
