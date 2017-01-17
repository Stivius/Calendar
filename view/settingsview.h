#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QDataWidgetMapper>

namespace Ui {
class SettingsView;
}

class SettingsSqlModel;

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
    QString path();

public slots:
    void setPath(const QString& path);

private:
    Ui::SettingsView *ui;
    QDataWidgetMapper _widgetMapper;

};

#endif // SETTINGS_H
