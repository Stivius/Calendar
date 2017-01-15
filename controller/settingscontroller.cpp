#include "settingscontroller.h"

#include <QApplication>
#include <QFileDialog>
#include <QDebug>

#include "model/settingssqlmodel.h"
#include "view/settingsview.h"

//====================================================================================

SettingsController::SettingsController(SettingsView* settingsView,
                                       SettingsSqlModel* settingsSqlModel,
                                       QObject* parent) :
    QObject(parent),
    _settingsView(settingsView),
    _settingsSqlModel(settingsSqlModel)
{
    connect(_settingsView, &SettingsView::destroyed, this, &SettingsController::finished);

    connect(_settingsView, &SettingsView::choosePathBtnClicked, this, &SettingsController::choosePath);
    connect(_settingsView, &SettingsView::submitBtnClicked, this, &SettingsController::submit);
    connect(_settingsView, &SettingsView::cancelBtnClicked, this, &SettingsController::cancel);

    _settingsView->setMapperModel(_settingsSqlModel);
}

//====================================================================================

SettingsController::~SettingsController()
{
    qDebug() << "settings controller deleted";
}

//====================================================================================

void SettingsController::choosePath()
{
    QFileDialog* import = new QFileDialog(_settingsView);
    //import->setOption(QFileDialog::DontUseNativeDialog, true);
    connect(import, &QFileDialog::fileSelected, _settingsView, &SettingsView::setPath);
    import->setWindowModality(Qt::ApplicationModal);
    import->setFileMode(QFileDialog::Directory);
    import->setAttribute(Qt::WA_DeleteOnClose);
    import->show();
}

//====================================================================================

void SettingsController::submit()
{
    _settingsSqlModel->submitAll();
    _settingsView->close();
}

//====================================================================================

void SettingsController::cancel()
{
    _settingsView->close();
}

//====================================================================================
