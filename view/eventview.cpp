#include "eventview.h"
#include "ui_eventview.h"

#include <QDebug>

#include "model/eventssqlmodel.h"
#include "controller/eventcontroller.h"

//====================================================================================

EventView::EventView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventView)
{
    ui->setupUi(this);

    connect(ui->cancelButton, &QPushButton::clicked, this, [=](){
        _widgetMapper.revert();
        emit cancelBtnClicked();
    });
    connect(ui->saveButton, &QPushButton::clicked, this, [=](){
        _widgetMapper.submit();
        emit saveBtnClicked();
    });

    connect(ui->uploadImage, &QPushButton::clicked, this, &EventView::uploadBtnClicked);
    connect(ui->removeImage, &QPushButton::clicked, this, &EventView::removeBtnClicked);
    connect(ui->nextImage, &QPushButton::clicked, this, &EventView::nextBtnClicked);
    connect(ui->previousImage, &QPushButton::clicked, this, &EventView::previousBtnClicked);
    connect(ui->currentImage, &QPushButton::clicked, this, &EventView::currentImageClicked);
}

//====================================================================================

EventView::~EventView()
{
    qDebug() << "eventview deleted";
    _widgetMapper.revert();
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

void EventView::setMapperIndex(int index)
{
    _widgetMapper.setCurrentIndex(index);
}

//====================================================================================

void EventView::setCurrentImage(const QPixmap &image)
{
    ui->currentImage->setIcon(QIcon(image));
}

//====================================================================================

void EventView::setController(std::shared_ptr<EventController>& eventController)
{
    _eventController = eventController;
}

//====================================================================================

void EventView::setMapperModel(EventsSqlModel* eventsModel)
{
    _widgetMapper.setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    _widgetMapper.setModel(eventsModel);
    _widgetMapper.addMapping(ui->shortEdit, eventsModel->column(ShortDescription));
    _widgetMapper.addMapping(ui->fullEdit, eventsModel->column(FullDescription));
    _widgetMapper.addMapping(ui->extraEdit, eventsModel->column(ExtraDescription));
    _widgetMapper.addMapping(ui->themeBox, eventsModel->column(Theme));
    _widgetMapper.addMapping(ui->placeBox, eventsModel->column(Place));
    _widgetMapper.addMapping(ui->sourceBox, eventsModel->column(Source));
}

//====================================================================================
