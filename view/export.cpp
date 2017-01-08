#include "export.h"
#include "ui_export.h"

#include <QtPrintSupport/QPrinter>
#include <QDesktopServices>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextStream>

#include "model/eventsproxymodel.h"

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
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName(("windows-1251")));
//    QString str;
//    str += "<html>"
//           "<meta http-equiv='Content-Type' content='text/html; charset=windows-1251' />"
//           "<table border='1' align='center'>"
//           "<caption>Таблица из календаря (by stivius)</caption>"
//           "<tr>"
//           "<th>Изображение</th>"
//           "<th>Дата</th>"
//           "<th>Событие</th>"
//           "</tr>";
//    for(int i = 0; i != _eventsProxyModel->rowCount(); i++)
//    {
//        QString strF =
//               "<tr>"
//               "<td><img src='%1' width='%2'></td>"
//               "<td>%3</td>"
//               "<td>%4</td>"
//               "</tr>";
//        QString imagePath = "";
//        str += strF.arg(imagePath).arg((imagePath != QString()) ? "150" : "0").arg(_eventsProxyModel->date(i)).arg(_eventsProxyModel->shortDescription(i));
//    }
//    str += "</table>"
//           "</html>";
//    QDesktopServices process;
//    if(ui->browserBtn->isChecked()) // HTML
//    {
//        QFile file(ui->pathEdit->text() + "/export.html");
//        if(file.exists())
//            file.remove();
//        file.open(QIODevice::WriteOnly);
//        QTextStream stream(&file);
//        stream << str;
//        process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/export.html"));
//    }
//    else // PDF
//    {
//        QPrinter printer(QPrinter::PrinterResolution);
//        printer.setPageSize(QPrinter::A4);
//        printer.setOutputFormat(QPrinter::PdfFormat);
//        printer.setOutputFileName(ui->pathEdit->text() + "/export.pdf");
//        QTextDocument *doc = new QTextDocument;
//        doc->setHtml(str);
//        doc->print(&printer);
//        process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/export.pdf"));
//    }
}
