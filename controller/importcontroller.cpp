#include "importcontroller.h"

#include <QDesktopServices>
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDate>
#include <QUrl>
#include <QFileDialog>

#include "xlsxdocument.h"
#include "model/eventssqlmodel.h"
#include "view/importview.h"

//====================================================================================

const int INITIAL_CELL_NUMBER = 2;
const QString EXCEL_DATE_FORMAT = "yyyy-MM-dd";

//====================================================================================

ImportController::ImportController(ImportView* importView,
                                   std::shared_ptr<EventsSqlModel> &eventsSqlModel,
                                   QObject *parent) :
    QObject(parent),
    _importView(importView),
    _eventsSqlModel(eventsSqlModel)
{
    setPath(QApplication::applicationDirPath());

    connect(_importView, &ImportView::choosePathBtnClicked, this, &ImportController::choosePath);
    connect(_importView, &ImportView::importBtnClicked, this, &ImportController::importEvents);
    connect(_importView, &ImportView::excelTemplateBtnClicked, this, &ImportController::openExcelTemplate);
}

//====================================================================================

ImportController::~ImportController()
{
    qDebug() << "importcontroller deleted";
}

//====================================================================================

void ImportController::importEvents()
{
    QXlsx::Document xlsx(_path + "/import.xlsx");
    int cellNumber = INITIAL_CELL_NUMBER;
    _eventsSqlModel->database().transaction();

    QString date = xlsx.read("A2").toString();
    QString eventsDescription = xlsx.read("B2").toString();
    while(date != QString() && eventsDescription != QString())
    {
        if(QDate::fromString(date, EXCEL_DATE_FORMAT).isValid())
        {
            date = date.replace('-', '/');
            _eventsSqlModel->insertEvent(date, eventsDescription);
        }
        else
            qDebug() << "Ошибка в ячейке: " << cellNumber;

        cellNumber++;
        date = xlsx.read("A" + QString::number(cellNumber)).toString();
        eventsDescription = xlsx.read("B" + QString::number(cellNumber)).toString();
    }

    _eventsSqlModel->database().commit();
    _importView->close();
}

//====================================================================================

void ImportController::openExcelTemplate()
{
    QFile file(_path + "/import.xlsx");
    if(file.exists())
        file.remove();
    QXlsx::Document xlsx(_path + "/import.xlsx");
    xlsx.write("A1","Дата");
    xlsx.write("B1","Событие");
    xlsx.save();
    QDesktopServices process;
    process.openUrl(QUrl::fromLocalFile(_path + "/import.xlsx"));
}

//====================================================================================

void ImportController::choosePath()
{
    QFileDialog* importDialog = new QFileDialog(_importView);
    //importDialog->setOption(QFileDialog::DontUseNativeDialog, true);
    connect(importDialog, &QFileDialog::fileSelected, this, &ImportController::setPath);
    importDialog->setFileMode(QFileDialog::Directory);
    importDialog->setWindowModality(Qt::ApplicationModal);
    importDialog->setAttribute(Qt::WA_DeleteOnClose);
    importDialog->show();
}

//====================================================================================

void ImportController::setPath(const QString &path)
{
    _path = path;
    _importView->setPath(path + "/");
}

//====================================================================================
