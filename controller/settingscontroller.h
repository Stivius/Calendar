#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>

class SettingsView;
class SettingsSqlModel;

class SettingsController : public QObject
{
    Q_OBJECT
signals:
    void finished();

public:    
    explicit SettingsController(SettingsView *settingsView,
                                SettingsSqlModel *settingsSqlModel,
                                QObject *parent);
    ~SettingsController();

private slots:
    void choosePath();
    void submit();
    void cancel();

private:
    void checkFilesForMoving();

private:
    SettingsView* _settingsView = nullptr;
    SettingsSqlModel* _settingsSqlModel = nullptr;
    QString _path;

};

#endif // SETTINGSCONTROLLER_H
