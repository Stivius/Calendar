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

#include "model/eventssqlmodel.h"
#include "model/eventsproxymodel.h"
#include "model/settingssqlmodel.h"
#include "view/exportview.h"

//====================================================================================

const int INVALID_INDEX = -1;

const QString HTML_MONTH_NAME = "<tr>"
                                "<th colspan=2>%1</th>"
                                "</tr>";

const QString HTML_EVENT = "<tr>"
                           "<td width=30 align=center>%1</td>"
                           "<td><b>%2</b>%3</td>"
                           "</tr>";

const QString HTML_MEMORY_TABLE = "<html>"
                                  "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'/>"
                                  "<table width=600 border='1' align='center'>"
                                  "<caption>Памятные даты</caption>"
                                  "%1"
                                  "</table>"
                                  "</html>";

const QString HTML_YEAR_TABLE = "<html>"
                                "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8'/>"
                                "<table width=600 border='1' align='center'>"
                                "<caption>%1</caption>"
                                "%2"
                                "</table>"
                                "</html>"
                                "<br><br>";

//====================================================================================

ExportController::ExportController(ExportView* exportView,
                                   std::shared_ptr<EventsProxyModel> &eventsProxyModel,
                                   std::shared_ptr<SettingsSqlModel> &settingsSqlModel,
                                   QObject* parent) :
    QObject(parent),
    _exportView(exportView),
    _eventsProxyModel(eventsProxyModel),
    _settingsSqlModel(settingsSqlModel)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    setPath(QApplication::applicationDirPath());

    connect(_exportView, &ExportView::choosePathBtnClicked, this, &ExportController::choosePath);
    connect(_exportView, &ExportView::submitBtnClicked, this, &ExportController::submitExport);

    connect(_exportView, &ExportView::browserBtnClicked, this, [=](){
        _exportType = ExportType::Browser;
    });

    connect(_exportView, &ExportView::pdfBtnClicked, this, [=](){
        _exportType = ExportType::Pdf;
    });

    connect(_exportView, &ExportView::memoryDatesBtnClicked, this, [=](){
        _exportFormat = ExportFormat::MemoryDates;
    });

    connect(_exportView, &ExportView::usualEventsBtnClicked, this, [=](){
        _exportFormat = ExportFormat::UsualEvents;
    });

    initializeData();
}

//====================================================================================

ExportController::~ExportController()
{
    qDebug() << "export controller deleted";
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
    LessThanComparator comp;
    QString resultedText;

    if(_exportFormat == ExportFormat::MemoryDates)
    {
        comp = std::bind(&ExportController::lessThanMonth, this, std::placeholders::_1, std::placeholders::_2);
        std::sort(_exportedEvents.begin(), _exportedEvents.end(), comp);
        resultedText = formatMemoryDates();

    }
    else
    {
        comp = std::bind(&ExportController::lessThanYear, this, std::placeholders::_1, std::placeholders::_2);
        std::sort(_exportedEvents.begin(), _exportedEvents.end(), comp);
        resultedText = formatUsualEvents();
    }

    exportEvents(resultedText);
    _exportView->close();
}

//====================================================================================

void ExportController::exportEvents(const QString& text)
{
    QDesktopServices process;

    if(_exportType == ExportType::Browser)
    {
        QFile file(_path + "export.html");
        if(file.exists())
            file.remove();

        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);
        stream << text;
        process.openUrl(QUrl::fromLocalFile(_path + "export.html"));
    }
    else // PDF
    {
        QFile file(_path + "export.pdf");
        if(file.exists())
            file.remove();

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(_path + "export.pdf");
        QTextDocument* document = new QTextDocument;
        document->setHtml(text);
        document->print(&printer);
        process.openUrl(QUrl::fromLocalFile(_path + "export.pdf"));
    }
}

//====================================================================================

void ExportController::setPath(const QString &path)
{
    _path = path + "/";
    _exportView->setPath(path);
}

//====================================================================================

void ExportController::initializeData()
{
    auto eventsSqlModel = static_cast<EventsSqlModel*>(_eventsProxyModel->sourceModel());

    for(int i = 0; i != _eventsProxyModel->rowCount(); i++)
    {
        QModelIndex sourceIndex = _eventsProxyModel->mapToSource(_eventsProxyModel->index(i, 0));
        int row = sourceIndex.row();
        int day = eventsSqlModel->day(row);
        int month = eventsSqlModel->month(row);
        int year = eventsSqlModel->year(row);
        QString fullDescription = eventsSqlModel->fullDescription(row);
        _exportedEvents.push_back({day, month, year, fullDescription});
    }
}

//====================================================================================

bool ExportController::lessThanMonth(const Event& left, const Event& right)
{
    if(left._month == right._month)
        return left._day < right._day;

    return left._month < right._month;
}

//====================================================================================

bool ExportController::lessThanYear(const Event& left, const Event& right)
{
    if(left._year == right._year)
        return lessThanMonth(left, right);

    return left._year < right._year;
}

//====================================================================================

QString ExportController::formatMonths(int year)
{
    int currentMonth = INVALID_INDEX, previousMonth = INVALID_INDEX;
    QString formattedMonths;

    for(const Event& event: _exportedEvents)
    {
        if(event._year == year || year == INVALID_INDEX)
        {
            currentMonth = event._month;
            if(currentMonth != previousMonth)
            {
                formattedMonths += HTML_MONTH_NAME.arg(formatMonthName(event._month));
            }
            formattedMonths += formatEvent(event);
            previousMonth = currentMonth;
        }
    }
    return formattedMonths;
}

//====================================================================================

QString ExportController::formatUsualEvents()
{
    QString formattedYears;
    int currentYear = INVALID_INDEX, previousYear = INVALID_INDEX;

    for(const Event& event: _exportedEvents)
    {
        currentYear = event._year;

        if(currentYear != previousYear)
        {
            QString sYear;
            if(event._year == 0)
                sYear = "Год неизвестен";
            else
                sYear = QString::number(event._year);

            formattedYears += HTML_YEAR_TABLE.arg(sYear).arg(formatMonths(event._year));
        }
        previousYear = currentYear;
    }
    return formattedYears;
}

//====================================================================================

QString ExportController::formatMemoryDates()
{
    return HTML_MEMORY_TABLE.arg(formatMonths());
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

    return result + " назад";
}

//====================================================================================

QString ExportController::formatMonthName(int month)
{
    QLocale locale(QLocale::Russian);

    QString monthName;
    if(month)
        monthName = locale.standaloneMonthName(month);
    else
        monthName = "Месяц неизвестен";
    monthName[0] = monthName[0].toUpper();

    return monthName;
}

//====================================================================================

QString ExportController::formatEvent(const Event& event)
{
    QString formattedYears;
    if(_exportFormat == ExportFormat::MemoryDates)
        formattedYears = countYears(event._year) + "<br>";

    return HTML_EVENT.arg(event._day).arg(formattedYears).arg(event._fullDescription);
}

//====================================================================================
