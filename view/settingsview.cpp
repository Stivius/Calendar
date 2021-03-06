#include "settingsview.h"
#include "ui_settingsview.h"

#include "model/settingssqlmodel.h"
#include "controller/settingscontroller.h"

//====================================================================================

SettingsView::SettingsView(QWidget *parent):
    QDialog(parent),
    ui(new Ui::SettingsView)
{
    ui->setupUi(this);

    connect(ui->choosePathBtn, &QPushButton::clicked, this, &SettingsView::choosePathBtnClicked);
    connect(ui->submitBtn, &QPushButton::clicked, this, [=](){
        _widgetMapper.submit();
        emit submitBtnClicked();
    });
    connect(ui->cancelBtn, &QPushButton::clicked, this, [=](){
        _widgetMapper.revert();
        emit cancelBtnClicked();
    });
}

//====================================================================================

SettingsView::~SettingsView()
{
    _widgetMapper.revert();
    delete ui;
}

//====================================================================================

void SettingsView::setMapperModel(SettingsSqlModel* settingsSqlModel)
{
    _widgetMapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    _widgetMapper.setModel(settingsSqlModel);

    _widgetMapper.addMapping(ui->pathEdit, settingsSqlModel->column(ImagesFolder));
    _widgetMapper.addMapping(ui->qualitySlider, settingsSqlModel->column(Quality));
    _widgetMapper.addMapping(ui->anniversaryBtn, settingsSqlModel->column(AnniversaryDates));
    _widgetMapper.setCurrentIndex(0);
}

//====================================================================================

void SettingsView::setController(std::shared_ptr<SettingsController> &settingsController)
{
    _settingsController = settingsController;
}

//====================================================================================

void SettingsView::setPath(const QString &path)
{
    ui->pathEdit->setText(path + "/");
}

//====================================================================================

QString SettingsView::path()
{
    return ui->pathEdit->text();
}

//====================================================================================
