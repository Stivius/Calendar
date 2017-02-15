#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QObject>
#include <memory>

class SettingsView;
class SettingsSqlModel;

class SettingsController : public QObject
{
    Q_OBJECT
public:    
    explicit SettingsController(SettingsView *settingsView,
                                std::shared_ptr<SettingsSqlModel>& settingsSqlModel,
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
    std::shared_ptr<SettingsSqlModel> _settingsSqlModel;
    QString _path;

};

#endif // SETTINGSCONTROLLER_H
