#include "exportcontroller.h"

#include <QApplication>
#include <QtPrintSupport/QPrinter>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextStream>
#include <QDebug>
#include <QDate>
#include <functional>

#include "model/eventssqlmodel.h"
#include "model/eventsproxymodel.h"
#include "model/settingssqlmodel.h"
#include "view/exportview.h"

//====================================================================================

const int INVALID_MONTH = -1;

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

    sortData();
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
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString html = formatHtml();

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
    _exportView->close();
}

//====================================================================================

void ExportController::setPath(const QString &path)
{
    _path = path + "/";
    _exportView->setPath(path);
}

//====================================================================================

void ExportController::setExportType(ExportType exportType)
{
    _exportType = exportType;
}

//====================================================================================

void ExportController::sortData()
{
    EventsSqlModel* _eventsSqlModel = static_cast<EventsSqlModel*>(_eventsProxyModel->sourceModel());

    for(int i = 0; i != _eventsProxyModel->rowCount(); i++)
    {
        QModelIndex sourceIndex = _eventsProxyModel->mapToSource(_eventsProxyModel->index(i, 0));
        int row = sourceIndex.row();
        int day = _eventsSqlModel->day(row);
        int month = _eventsSqlModel->month(row);
        int year = _eventsSqlModel->year(row);
        QString fullDescription = _eventsSqlModel->fullDescription(row);
        _exportedEvents.push_back({day, month, year, fullDescription});
    }

    auto comparator = std::bind(&ExportController::lessThanMonth, this, std::placeholders::_1,  std::placeholders::_2);
    std::sort(_exportedEvents.begin(), _exportedEvents.end(), comparator);
}

//====================================================================================

bool ExportController::lessThanMonth(const Event& left, const Event& right)
{
    if(left._month == right._month)
        return left._day < right._day;
    return left._month < right._month;
}

//====================================================================================

QString ExportController::formatHtml()
{
    QString html = "<html>"
                   "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'/>"
                   "<table width=600 border='1' align='center'>"
                   "<caption>Таблица из календаря</caption>";

    int currentMonth = INVALID_MONTH, previousMonth = INVALID_MONTH;
    QLocale locale(QLocale::Russian);
    for(Event event: _exportedEvents)
    {
        currentMonth = event._month;
        if(currentMonth != previousMonth)
        {
            QString htmlHeader = "<tr>"
                                 "<th colspan=2>%1</th>" // месяц
                                 "</tr>";
            QString monthName;
            if(event._month)
                monthName = locale.standaloneMonthName(event._month);
            else
                monthName = "Месяц неизвестен";
            monthName[0] = monthName[0].toUpper();
            html += htmlHeader.arg(monthName);
        }

        QString htmlRow = "<tr>"
                          "<td align=center>%1</td>"
                          "<td><b>%2</b><br>%3</td>"
                          "</tr>";
        QString formattedYears = countYears(event._year);
        html += htmlRow.arg(event._day).arg(formattedYears).arg(event._fullDescription);
        previousMonth = currentMonth;
    }
    html += "</table>"
            "</html>";

    return html;
}

//====================================================================================

QString ExportController::countYears(int year)
{
    QString result;

    if(year == 0)
        return QString("Год неизвестен");

    int currentYear = QDate::currentDate().year();
    QString yearsDifference = QString::number(abs(currentYear - year));
    char lastDigit = yearsDifference.at(yearsDifference.size()-1).toLatin1();
    switch(lastDigit)
    {
    case '1':
        result += yearsDifference + " год";
        break;
    case '2':
    case '3':
    case '4':
        result += yearsDifference + " года";
        break;
    default:
        result += yearsDifference + " лет";
        break;
    }

    if(currentYear - year >= 0)
        result += " прошло";
    else
        result += " осталось";

    return result;
}
