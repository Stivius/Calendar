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
}

//====================================================================================

SettingsController::~SettingsController()
{
    //qDebug() << "settings controller deleted";
}

//====================================================================================

void SettingsController::choosePath()
{
    QFileDialog* import = new QFileDialog(_settingsView);
    /*
     * This line is commented below because there is some issue on Linux desktop
     * fileSelected signal is emitted twice
     * So this line possible will resolve this strange issue
     */
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
    _widgetMapper.submit();
    _settingsSqlModel->submitAll();
    delete _settingsView;
}

//====================================================================================

void SettingsController::cancel()
{
    _widgetMapper.revert();
    delete _settingsView;
}

//====================================================================================
