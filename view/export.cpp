#include "export.h"
#include "ui_export.h"

#include <QtPrintSupport/QPrinter>
#include <QDesktopServices>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextStream>

#include "model/eventsproxymodel.h"
#include "model/eventssqlmodel.h"

Export::Export(EventsProxyModel* eventsProxyModel, QWidget *parent) :
    QDialog(parent),
    _eventsProxyModel(eventsProxyModel),
    ui(new Ui::Export)
{
    ui->setupUi(this);
    ui->pathEdit->setText(QApplication::applicationDirPath());
}

Export::~Export()
{
    delete ui;
}

void Export::on_pathButton_clicked()
{
    QFileDialog* import = new QFileDialog(this);
    //import->setOption(QFileDialog::DontUseNativeDialog, true);
    connect(import, &QFileDialog::fileSelected, ui->pathEdit, &QLineEdit::setText);
    import->setFileMode(QFileDialog::Directory);
    import->setWindowModality(Qt::ApplicationModal);
    import->setAttribute(Qt::WA_DeleteOnClose);
    import->show();
}

void Export::on_submitExport_clicked()
{
    EventsSqlModel* sqlModel = static_cast<EventsSqlModel*>(_eventsProxyModel->sourceModel());
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QString str = QString();
    str += "<html>"
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
        QString strF =
               "<tr align='center'>"
               "<td width='%1' height='%2'><img src='%3' width='%1'></td>"
               "<td width='%1' height='%2'>%4</td>"
               "<td width='%1' height='%2'>%5</td>"
               "</tr>";
        QModelIndex sourceIndex = _eventsProxyModel->mapToSource(_eventsProxyModel->index(i, 0));
        int row = sourceIndex.row();
        QStringList images = sqlModel->imagesList(row);
        QString imagePath = QString();
        if(!images.empty())
            imagePath = QApplication::applicationDirPath() + "/images/" + images.at(0);
        str += strF.arg("100").arg("100").arg(imagePath).arg(sqlModel->date(row)).arg(sqlModel->shortDescription(row));
    }
    str += "</table>"
           "</html>";
    QDesktopServices process;
    if(ui->browserBtn->isChecked()) // HTML
    {
        QFile file(ui->pathEdit->text() + "/export.html");
        if(file.exists())
            file.remove();
        file.open(QIODevice::WriteOnly);
        QTextStream stream(&file);
        stream << str;
        process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/export.html"));
    }
    else // PDF
    {
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(ui->pathEdit->text() + "/export.pdf");
        QTextDocument *doc = new QTextDocument;
        doc->setHtml(str);
        doc->print(&printer);
        process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/export.pdf"));
    }
    this->close();
}
