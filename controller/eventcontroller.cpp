#include "eventcontroller.h"
#include "ui_eventview.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>

#include "model/eventssqlmodel.h"
#include "view/eventview.h"

EventController::EventController(EventView* eventView,
                                 EventsSqlModel* eventsModel,
                                 int currentRow,
                                 QObject* parent) :
    QObject(parent),
    _eventView(eventView),
    _eventsModel(eventsModel),
    _currentRow(currentRow)
{
    connect(_eventView, &EventView::saved, this, &EventController::saveEvent);
    connect(_eventView, &EventView::canceled, this, &EventController::cancelSaving);

    _widgetMapper = new QDataWidgetMapper;
    _widgetMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    _widgetMapper->setModel(_eventsModel);

    QStringList themesList = {""}, placesList = {""}, sourcesList = {""};
    for(int i = 0; i != _eventsModel->rowCount(); i++)
    {
        themesList.push_back(_eventsModel->theme(i));
        placesList.push_back(_eventsModel->place(i));
        sourcesList.push_back(_eventsModel->source(i));
    }
    themesList.removeDuplicates();
    placesList.removeDuplicates();
    sourcesList.removeDuplicates();
    _eventView->addThemes(themesList);
    _eventView->addPlaces(placesList);
    _eventView->addSources(sourcesList);

    if(currentRow != -1)
    {
        _eventView->setDay(_eventsModel->day(currentRow));
        _eventView->setMonth(_eventsModel->month(currentRow));
        _eventView->setYear(QString::number(_eventsModel->year(currentRow)));

        _eventView->setCurrentTheme(_eventsModel->theme(_currentRow));
        _eventView->setCurrentPlace(_eventsModel->place(_currentRow));
        _eventView->setCurrentSource(_eventsModel->source(_currentRow));
    }
    else
    {
        _eventsModel->insertRow(_eventsModel->rowCount());
        _currentRow = _eventsModel->rowCount()-1;
    }

    _widgetMapper->addMapping(_eventView->shortEdit(), _eventsModel->column(ShortDescription));
    _widgetMapper->addMapping(_eventView->fullEdit(), _eventsModel->column(LongDescription));
    _widgetMapper->addMapping(_eventView->extraEdit(), _eventsModel->column(ExtraDescription));
    _widgetMapper->addMapping(_eventView->themeBox(), _eventsModel->column(Theme));
    _widgetMapper->addMapping(_eventView->placeBox(), _eventsModel->column(Place));
    _widgetMapper->addMapping(_eventView->sourceBox(), _eventsModel->column(Source));
    _widgetMapper->setCurrentIndex(_currentRow);
}

EventController::~EventController()
{
    _widgetMapper->revert();
    _eventsModel->revertAll();
}

void EventController::saveEvent()
{
    _eventsModel->setDate(_currentRow,
                          _eventView->selectedDay(),
                          _eventView->selectedMonth(),
                          _eventView->selectedYear());
    _widgetMapper->submit();
    _eventsModel->submitAll();
    _eventView->close();
}

void EventController::cancelSaving()
{
    _widgetMapper->revert();
    _eventsModel->revertAll();
    _eventView->close();
}
