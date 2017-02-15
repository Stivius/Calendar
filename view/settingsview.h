#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <memory>

namespace Ui {
class SettingsView;
}

class SettingsSqlModel;
class SettingsController;

class SettingsView : public QDialog
{
    Q_OBJECT
signals:
    void choosePathBtnClicked();
    void submitBtnClicked();
    void cancelBtnClicked();

public:
    explicit SettingsView(QWidget *parent = 0);
    ~SettingsView();
    void setMapperModel(SettingsSqlModel *settingsSqlModel);
    void setController(std::shared_ptr<SettingsController> &settingsController);
    QString path();

public slots:
    void setPath(const QString& path);

private:
    Ui::SettingsView *ui;
    QDataWidgetMapper _widgetMapper;
    std::shared_ptr<SettingsController> _settingsController;

};

#endif // SETTINGS_H
