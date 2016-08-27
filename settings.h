#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QFileDialog>

#include "model.h"

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(Model* _model, QWidget *parent = 0);
    ~Settings();
private slots:
    void on_cancelButton_clicked();
    void on_pathButton_clicked();
    void on_applyButton_clicked();
private:
    Ui::Settings *ui;
    Model* model;
};

#endif // SETTINGS_H
