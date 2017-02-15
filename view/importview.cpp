#include "importview.h"
#include "ui_importview.h"

#include "controller/importcontroller.h"

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

void ImportView::setController(std::shared_ptr<ImportController>& importController)
{
    _importController = importController;
}

//====================================================================================

void ImportView::setPath(const QString &path)
{
    ui->pathEdit->setText(path);
}

//====================================================================================
