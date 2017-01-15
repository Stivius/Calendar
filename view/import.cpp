#include "import.h"
#include "ui_importview.h"

//====================================================================================

ImportView::ImportView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportView)
{
    ui->setupUi(this);

    connect(ui->importBtn, &QPushButton::clicked, this, &ImportView::importBtnClicked);
    connect(ui->excelTemplateBtn, &QPushButton::clicked, this, &ImportView::excelTemplateBtnClicked);
}

//====================================================================================

ImportView::~ImportView()
{
    delete ui;
}

//====================================================================================
