#include "export.h"
#include "ui_export.h"
#include <QtPrintSupport/QPrinter>
#include <QDesktopServices>
#include <QTextCodec>
#include <QTextDocument>
#include <QTextStream>

Export::Export(Model* _model, Filter* _filter, QWidget *parent) :
    QDialog(parent),
    model(_model),
    filter(_filter),
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
    /*
     * This line is commented below because there is some issue on Linux desktop
     * fileSelected signal is emitted twice
     * So this line possible will resolve this strange issue
     */
    //import->setOption(QFileDialog::DontUseNativeDialog, true);
    //connect(import,SIGNAL(fileSelected(QString)),SLOT(submitImport(QString)));
    import->setWindowModality(Qt::ApplicationModal);
    import->setAttribute(Qt::WA_DeleteOnClose);
    import->show();
}
void Export::on_submitExport_clicked()
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(("windows-1251")));
    QString str;
    str += "<html>"
           "<meta http-equiv='Content-Type' content='text/html; charset=windows-1251' />"
           "<table border='1' align='center'>"
           "<caption>Таблица из календаря (by stivius)</caption>"
           "<tr>"
           "<th>Изображение</th>"
           "<th>Дата</th>"
           "<th>Событие</th>"
           "</tr>";
    QSet<int> hiddenRows = filter->getHiddenRows();
    for(int i = 0; i != model->count(); i++)
    {
        if(!hiddenRows.contains(i))
        {
            QString strF =
                   "<tr>"
                   "<td><img src='%1' width='150'></td>"
                   "<td>%2</td>"
                   "<td>%3</td>"
                   "</tr>";
            QString imagePath = "";
            if(model->getImages(i) > 0)
                imagePath = QApplication::applicationDirPath() + "/images/" + model->getImages(i).split(QChar('\n'),QString::SkipEmptyParts).at(0);
            str += strF.arg(imagePath).arg(model->getDate(i)).arg(model->getSDescrpition(i));
        }
    }
    str += "</table>"
           "</html>";
    QDesktopServices process;
    if(ui->browserBtn->isChecked()) // HTML
    {
        QFile file(QApplication::applicationDirPath() + "/export.html");
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
        printer.setOutputFileName(QApplication::applicationDirPath() + "/export.pdf");
        QTextDocument *doc = new QTextDocument;
        doc->setHtml(str);
        doc->print(&printer);
        process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/export.pdf"));
    }
}
