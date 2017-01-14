#include "settingsview.h"
#include "ui_settingsview.h"

#include "model/settingssqlmodel.h"

//====================================================================================

SettingsView::SettingsView(QWidget *parent):
    QDialog(parent),
    ui(new Ui::SettingsView)
{
    ui->setupUi(this);

    connect(ui->choosePathBtn, &QPushButton::clicked, this, &SettingsView::choosePathBtnClicked);
    connect(ui->submitBtn, &QPushButton::clicked, this, &SettingsView::submitBtnClicked);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &SettingsView::cancelBtnClicked);

}

//====================================================================================

SettingsView::~SettingsView()
{
    delete ui;
}

//====================================================================================

QLineEdit *SettingsView::pathEdit()
{
    return ui->pathEdit;
}

//====================================================================================

QSlider *SettingsView::qualitySlider()
{
    return ui->qualitySlider;
}

//====================================================================================

QRadioButton *SettingsView::anniversaryBtn()
{
    return ui->anniversaryBtn;
}

//====================================================================================

void SettingsView::setPath(const QString &path)
{
    ui->pathEdit->setText(path);
}

//====================================================================================
