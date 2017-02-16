#include "settingscontroller.h"

#include <QApplication>
#include <QFileDialog>
#include <QDebug>

#include "model/settingssqlmodel.h"
#include "view/settingsview.h"

//====================================================================================

SettingsController::SettingsController(SettingsView* settingsView,
                                       std::shared_ptr<SettingsSqlModel>& settingsSqlModel,
                                       QObject* parent) :
    QObject(parent),
    _settingsView(settingsView),
    _settingsSqlModel(settingsSqlModel)
{
    connect(_settingsView, &SettingsView::choosePathBtnClicked, this, &SettingsController::choosePath);
    connect(_settingsView, &SettingsView::submitBtnClicked, this, &SettingsController::submit);
    connect(_settingsView, &SettingsView::cancelBtnClicked, this, &SettingsController::cancel);

    _settingsView->setMapperModel(_settingsSqlModel.get());
    _path = _settingsView->path();
}

//====================================================================================

SettingsController::~SettingsController()
{
    qDebug() << "settings controller deleted";
}

//====================================================================================

void SettingsController::choosePath()
{
    auto dialog = new QFileDialog(_settingsView);
    //import->setOption(QFileDialog::DontUseNativeDialog, true);
    connect(dialog, &QFileDialog::fileSelected, _settingsView, &SettingsView::setPath);
    dialog->setWindowModality(Qt::ApplicationModal);
    dialog->setFileMode(QFileDialog::Directory);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

//====================================================================================

void SettingsController::submit()
{
    checkFilesForMoving();

    _settingsSqlModel->submitAll();
    _settingsView->close();
}

//====================================================================================

void SettingsController::cancel()
{
    _settingsView->close();
}

//====================================================================================

void SettingsController::checkFilesForMoving()
{
    QString currentPath = _settingsView->path();
    if(_path != currentPath)
    {
        QDir currentDirectory(_path);
        QFileInfoList files = currentDirectory.entryInfoList({"*.png", "*.jpg", "*.jpeg"}, QDir::Files);
        for(QFileInfo fileInfo: files)
        {
            QFile file(fileInfo.absoluteFilePath());
            file.rename(currentPath + fileInfo.fileName());
        }
    }
}

//====================================================================================

