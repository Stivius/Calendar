#include "eventview.h"
#include "ui_eventview.h"

//====================================================================================

EventView::EventView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventView)
{
    ui->setupUi(this);

    connect(ui->cancelButton, &QPushButton::clicked, this, &EventView::canceled);
    connect(ui->saveButton, &QPushButton::clicked, this, &EventView::saved);

    connect(ui->uploadImage, &QPushButton::clicked, this, &EventView::uploadBtnClicked);
    connect(ui->removeImage, &QPushButton::clicked, this, &EventView::removeBtnClicked);
    connect(ui->nextImage, &QPushButton::clicked, this, &EventView::nextBtnClicked);
    connect(ui->previousImage, &QPushButton::clicked, this, &EventView::previousBtnClicked);
    connect(ui->currentImage, &QPushButton::clicked, this, &EventView::currentImageClicked);
}

//====================================================================================

EventView::~EventView()
{
    delete ui;
}

//====================================================================================

void EventView::setDay(int index)
{
    ui->dayBox->setCurrentIndex(index);
}

//====================================================================================

void EventView::setMonth(int index)
{
    ui->monthBox->setCurrentIndex(index);
}

//====================================================================================

void EventView::setYear(const QString& year)
{
    ui->yearEdit->setText(year);
}

//====================================================================================

int EventView::selectedDay()
{
    return ui->dayBox->currentIndex();
}

//====================================================================================

int EventView::selectedMonth()
{
    return ui->monthBox->currentIndex();
}

//====================================================================================

int EventView::selectedYear()
{
    return ui->yearEdit->text().toInt();
}

//====================================================================================

void EventView::addThemes(const QStringList& themes)
{
    ui->themeBox->addItems(themes);
}

//====================================================================================

void EventView::addPlaces(const QStringList& places)
{
    ui->placeBox->addItems(places);
}

//====================================================================================

void EventView::addSources(const QStringList& sources)
{
    ui->sourceBox->addItems(sources);
}

//====================================================================================

void EventView::setCurrentTheme(const QString& theme)
{
    int index = ui->themeBox->findText(theme);
    ui->themeBox->setCurrentIndex(index);
}

//====================================================================================

void EventView::setCurrentPlace(const QString& place)
{
    int index = ui->placeBox->findText(place);
    ui->placeBox->setCurrentIndex(index);
}

//====================================================================================

void EventView::setCurrentSource(const QString& source)
{
    int index = ui->sourceBox->findText(source);
    ui->sourceBox->setCurrentIndex(index);
}

//====================================================================================

QComboBox* EventView::dayBox()
{
    return ui->dayBox;
}

//====================================================================================

QComboBox* EventView::monthBox()
{
    return ui->monthBox;
}

//====================================================================================

QLineEdit* EventView::yearEdit()
{
    return ui->yearEdit;
}

//====================================================================================

QLineEdit* EventView::shortEdit()
{
    return ui->shortEdit;
}

//====================================================================================

QPlainTextEdit* EventView::fullEdit()
{
    return ui->fullEdit;
}

//====================================================================================

QLineEdit* EventView::extraEdit()
{
    return ui->extraEdit;
}

//====================================================================================

QComboBox* EventView::themeBox()
{
    return ui->themeBox;
}

//====================================================================================

QComboBox* EventView::placeBox()
{
    return ui->placeBox;
}

//====================================================================================

QComboBox* EventView::sourceBox()
{
    return ui->sourceBox;
}

//====================================================================================

QPushButton* EventView::currentImage()
{
    return ui->currentImage;
}

//====================================================================================
