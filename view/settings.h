#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QFileDialog>
#include <QDataWidgetMapper>

class SettingsSqlModel;

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(SettingsSqlModel* settingsModel, QWidget *parent = 0);
    ~Settings();
private slots:
    void on_cancelButton_clicked();
    void on_pathButton_clicked();
    void on_applyButton_clicked();
private:
    Ui::Settings *ui;
    SettingsSqlModel* _settingsModel;
    QDataWidgetMapper _widgetMapper;
};

#endif // SETTINGS_H
