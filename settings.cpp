#include "settings.h"
#include "ui_settings.h"

Settings::Settings(Model *_model, QWidget *parent):
    QDialog(parent),
    ui(new Ui::Settings),
    model(_model)
{
    ui->setupUi(this);
    ui->qualitySlider->setValue(model->getQuality());
    ui->pathEdit->setText(QApplication::applicationDirPath() + "/images/");
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_cancelButton_clicked()
{
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
    model->updateSettings(QApplication::applicationDirPath() + "/images/", ui->qualitySlider->value(), 0);
    this->close();
}
