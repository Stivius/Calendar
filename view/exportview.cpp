#include "exportview.h"
#include "ui_exportview.h"

//====================================================================================

ExportView::ExportView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportView)
{
    ui->setupUi(this);

    connect(ui->choosePathBtn, &QPushButton::clicked, this, &ExportView::choosePathBtnClicked);
    connect(ui->submitExportBtn, &QPushButton::clicked, this, &ExportView::submitBtnClicked);
    connect(ui->browserBtn, &QPushButton::clicked, this, &ExportView::browserBtnClicked);
    connect(ui->pdfBtn, &QPushButton::clicked, this, &ExportView::pdfBtnClicked);
}

//====================================================================================

ExportView::~ExportView()
{
    delete ui;
}

//====================================================================================

void ExportView::setPath(const QString &path)
{
    ui->pathEdit->setText(path);
}

//====================================================================================
