#include "settings.h"
#include "ui_settings.h"

#include "model/settingssqlmodel.h"

Settings::Settings(SettingsSqlModel* settingsModel, QWidget *parent):
    QDialog(parent),
    ui(new Ui::Settings),
    _settingsModel(settingsModel)
{
    ui->setupUi(this);

    _widgetMapper = new QDataWidgetMapper;
    _widgetMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    _widgetMapper->setModel(_settingsModel);
    _widgetMapper->addMapping(ui->pathEdit, _settingsModel->column(Path));
    _widgetMapper->addMapping(ui->qualitySlider, _settingsModel->column(Quality));
    _widgetMapper->addMapping(ui->radioButton_2, _settingsModel->column(Anniversary));
    _widgetMapper->setCurrentIndex(0);
}

Settings::~Settings()
{
    _widgetMapper->revert();
    delete ui;
}

void Settings::on_cancelButton_clicked()
{
    _widgetMapper->revert();
    this->close();
}

void Settings::on_pathButton_clicked()
{
    QFileDialog* import = new QFileDialog(this);
    /*
     * This line is commented below because there is some issue on Linux desktop
     * fileSelected signal is emitted twice
     * So this line possible will resolve this strange issue
     */
    //import->setOption(QFileDialog::DontUseNativeDialog, true);
    //connect(import,SIGNAL(fileSelected(QString)),SLOT(submitImport(QString)));
    import->setWindowModality(Qt::ApplicationModal);
    import->setAttribute(Qt::WA_DeleteOnClose);
    import->show();
}

void Settings::on_applyButton_clicked()
{
    _widgetMapper->submit();
    _settingsModel->submitAll();
    this->close();
}
