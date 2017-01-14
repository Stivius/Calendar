#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QLineEdit>
#include <QSlider>
#include <QRadioButton>

namespace Ui {
class SettingsView;
}

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

    QLineEdit* pathEdit();
    QSlider* qualitySlider();
    QRadioButton* anniversaryBtn();

public slots:
    void setPath(const QString& path);

private:
    Ui::SettingsView *ui;

};

#endif // SETTINGS_H
