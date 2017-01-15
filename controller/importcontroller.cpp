#include "importcontroller.h"

#include <QDesktopServices>
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDate>
#include <QUrl>

#include "xlsxdocument.h"
#include "model/eventssqlmodel.h"
#include "view/importview.h"

//====================================================================================

const int INITIAL_CELL_NUMBER = 2;
const QString EXCEL_DATE_FORMAT = "yyyy-MM-dd";

//====================================================================================

ImportController::ImportController(ImportView* importView,
                                   EventsSqlModel* eventsSqlModel,
                                   QObject *parent) :
    QObject(parent),
    _importView(importView),
    _eventsSqlModel(eventsSqlModel)
{
    connect(_importView, &ImportView::destroyed, this, &ImportController::finished);

    connect(_importView, &ImportView::importBtnClicked, this, &ImportController::importEvents);
    connect(_importView, &ImportView::excelTemplateBtnClicked, this, &ImportController::openExcelTemplate);
}

//====================================================================================

ImportController::~ImportController()
{

}

//====================================================================================

void ImportController::importEvents()
{
    QXlsx::Document xlsx(QApplication::applicationDirPath()+ "/import.xlsx");
    int num = INITIAL_CELL_NUMBER;
    _eventsSqlModel->database().transaction();

    QString date = xlsx.read("A2").toString();
    QString eventsDescriotion = xlsx.read("B2").toString();
    while(date != QString() && eventsDescriotion != QString())
    {
        if(QDate::fromString(date, EXCEL_DATE_FORMAT).isValid())
        {
            date = date.replace('-', '/');
            _eventsSqlModel->insertEvent(date, eventsDescriotion);
        }
        else
            qDebug() << "Ошибка в ячейке: " << num;

        num++;
        date = xlsx.read("A" + QString::number(num)).toString();
        eventsDescriotion = xlsx.read("B" + QString::number(num)).toString();
    }

    _eventsSqlModel->database().commit();
    _importView->close();
}

//====================================================================================

void ImportController::openExcelTemplate()
{
    QFile file(QApplication::applicationDirPath()+ "/import.xlsx");
    if(file.exists())
        file.remove();
    QXlsx::Document xlsx(QApplication::applicationDirPath()+ "/import.xlsx");
    xlsx.write("A1","Дата");
    xlsx.write("B1","Событие");
    xlsx.save();
    QDesktopServices process;
    process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath()+ "/import.xlsx"));
}

//====================================================================================