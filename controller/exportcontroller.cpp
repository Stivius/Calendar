#include "exportcontroller.h"

#include <QApplication>
#include <QtPrintSupport/QPrinter>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextStream>
#include <QDebug>

#include "model/eventssqlmodel.h"
#include "model/eventsproxymodel.h"
#include "model/settingssqlmodel.h"
#include "view/exportview.h"

//====================================================================================

ExportController::ExportController(ExportView* exportView,
                                   EventsProxyModel* eventsProxyModel,
                                   SettingsSqlModel* settingsSqlModel,
                                   QObject* parent) :
    QObject(parent),
    _exportView(exportView),
    _eventsProxyModel(eventsProxyModel),
    _settingsSqlModel(settingsSqlModel)
{
    setPath(QApplication::applicationDirPath());

    connect(_exportView, &ExportView::destroyed, this, &ExportController::finished);

    connect(_exportView, &ExportView::choosePathBtnClicked, this, &ExportController::choosePath);
    connect(_exportView, &ExportView::submitBtnClicked, this, &ExportController::submitExport);

    connect(_exportView, &ExportView::browserBtnClicked, this, [=](){
        setExportType(ExportType::Browser);
    });

    connect(_exportView, &ExportView::pdfBtnClicked, this, [=](){
        setExportType(ExportType::Pdf);
    });
}

//====================================================================================

ExportController::~ExportController()
{
    //qDebug() << "export controller deleted";
}

//====================================================================================

void ExportController::choosePath()
{
    QFileDialog* exportDialog = new QFileDialog(_exportView);
    //exportDialog->setOption(QFileDialog::DontUseNativeDialog, true);
    connect(exportDialog, &QFileDialog::fileSelected, this, &ExportController::setPath);
    exportDialog->setFileMode(QFileDialog::Directory);
    exportDialog->setWindowModality(Qt::ApplicationModal);
    exportDialog->setAttribute(Qt::WA_DeleteOnClose);
    exportDialog->show();
}

//====================================================================================

void ExportController::submitExport()
{
    EventsSqlModel* eventsSqlModel = static_cast<EventsSqlModel*>(_eventsProxyModel->sourceModel());
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString html;
    html += "<html>"
           "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8' />"
           "<table border='1' align='center'>"
           "<caption>Таблица из календаря (by stivius)</caption>"
           "<tr>"
           "<th>Изображение</th>"
           "<th>Дата</th>"
           "<th>Событие</th>"
           "</tr>";
    for(int i = 0; i != _eventsProxyModel->rowCount(); i++)
    {
        QString htmlRowTable =
               "<tr align='center'>"
               "<td width='%1' height='%2'><img src='%3' width='%1'></td>"
               "<td width='%1' height='%2'>%4</td>"
               "<td width='%1' height='%2'>%5</td>"
               "</tr>";
        QModelIndex sourceIndex = _eventsProxyModel->mapToSource(_eventsProxyModel->index(i, 0));
        int row = sourceIndex.row();
        QStringList images = eventsSqlModel->imagesList(row);
        QString imagePath = QString();
        if(!images.empty())
            imagePath = _settingsSqlModel->imagesFolder() + images.at(0);
        html += htmlRowTable.arg("100").arg("100").arg(imagePath).arg(eventsSqlModel->date(row)).arg(eventsSqlModel->shortDescription(row));
    }
    html += "</table>"
           "</html>";
    QDesktopServices process;
    if(_exportType == ExportType::Browser)
    {
        QFile file(_path + "export.html");
        if(file.exists())
            file.remove();
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);
        stream << html;
        process.openUrl(QUrl::fromLocalFile(_path + "export.html"));
    }
    else // PDF
    {
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(_path + "export.pdf");
        QTextDocument *doc = new QTextDocument;
        doc->setHtml(html);
        doc->print(&printer);
        process.openUrl(QUrl::fromLocalFile(_path + "export.pdf"));
    }
    delete _exportView;
}

//====================================================================================

void ExportController::setPath(const QString &path)
{
    _path = path;
    _exportView->setPath(path + "/");
}

//====================================================================================

void ExportController::setExportType(ExportType exportType)
{
    _exportType = exportType;
}

//====================================================================================
