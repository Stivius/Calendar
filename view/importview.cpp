#include "importview.h"
#include "ui_importview.h"

//====================================================================================

ImportView::ImportView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportView)
{
    ui->setupUi(this);

    connect(ui->choosePathBtn, &QPushButton::clicked, this, &ImportView::choosePathBtnClicked);
    connect(ui->importBtn, &QPushButton::clicked, this, &ImportView::importBtnClicked);
    connect(ui->excelTemplateBtn, &QPushButton::clicked, this, &ImportView::excelTemplateBtnClicked);
}

//====================================================================================

ImportView::~ImportView()
{
    delete ui;
}

//====================================================================================

void ImportView::setPath(const QString &path)
{
    ui->pathEdit->setText(path);
}

//====================================================================================
