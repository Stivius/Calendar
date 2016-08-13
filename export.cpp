#include "export.h"
#include "ui_export.h"

Export::Export(QWidget *parent) :
    QDialog(parent),
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
