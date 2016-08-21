#include "import.h"
#include "ui_import.h"
#include <QMessageBox>

Import::Import(Model* _model, QWidget *parent) :
    QDialog(parent),
    model(_model),
    ui(new Ui::Import)
{
    connect(this, SIGNAL(addEvent(QString,QString,QString,QString,QString)), parent, SLOT(addEvent(QString,QString,QString,QString,QString)));
    ui->setupUi(this);
}

Import::~Import()
{
    delete ui;
}

void Import::on_importBtn_clicked()
{
    QXlsx::Document xlsx(QApplication::applicationDirPath()+ "/Import.xlsx");
    bool loop = true;
    int num = 2;
    model->startMultiInsertion();
    while(loop)
    {
        QString date = xlsx.read("A" + QString::number(num)).toString();
        QString event = xlsx.read("B" + QString::number(num)).toString();
        if(date != "" && event != "")
        {
            QRegularExpression re("^([0-9]{2,4})\\-([0-9]{1,2})\\-([0-9]{1,2})$");
            QRegularExpressionMatch match = re.match(date);
            if(match.hasMatch())
            {
                QStringList fullDate = date.split(QChar('-'),QString::SkipEmptyParts);
                QString date = model->formatDate(fullDate[2].toInt(),fullDate[1].toInt(),fullDate[0].toInt());
                QVector<QString> data = {fullDate[2], fullDate[1], fullDate[0], "", event, "", "", "", "", ""};
                model->insertEvent(data);
                emit addEvent(date, event, "", "", "Нет");
            }
            else
                qDebug() << "Ошибка в записи: " << num-1;
        }
        else
            loop = false;
        num++;
    }
    model->finishMultiInsertion();
    QFile file(QApplication::applicationDirPath()+ "/Import.xlsx");
    file.remove();
    this->close();
}

void Import::on_excelTemplateBtn_clicked()
{
    QXlsx::Document xlsx(QApplication::applicationDirPath()+ "/Import.xlsx");
    xlsx.write("A1","Дата");
    xlsx.write("B1","Событие");
    xlsx.save();
    QDesktopServices process;
    process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath()+ "/Import.xlsx"));
}
