#include "eventsmainwindow.h"
#include "ui_eventsmainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QDate>

#include "model/eventsproxymodel.h"
#include "model/eventssqlmodel.h"
#include "model/settingssqlmodel.h"
#include "view/export.h"
#include "view/import.h"
#include "view/settings.h"
#include "view/eventview.h"
#include "controller/eventcontroller.h"

//====================================================================================

const int INVALID_INDEX = -1;
const int MIN_FONT_SIZE = 7;

//====================================================================================

EventsMainWindow::EventsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EventsMainWindow)
{
    ui->setupUi(this);
    ui->detailBox->hide();
    ui->listWidget->hide();

    _database = QSqlDatabase::addDatabase("QSQLITE");
    _database.setDatabaseName("db");
    _database.open();

    _eventsSqlModel = new EventsSqlModel(_database);
    _eventsProxyModel = new EventsProxyModel(_eventsSqlModel);
    _settingsModel = new SettingsSqlModel(_database);

    ui->tableView->setModel(_eventsProxyModel);
    QFont fnt;
    fnt.setPointSize(_settingsModel->font());
    ui->tableView->setFont(fnt);

    hideColumns();
    connect(_eventsProxyModel, &EventsProxyModel::filterUpdated, this, [=](){
        hideColumns();
    });

    _widgetMapper.setModel(_eventsProxyModel);

    _widgetMapper.addMapping(ui->extraEdit, _eventsSqlModel->column(ExtraDescription));
    _widgetMapper.addMapping(ui->fullEdit, _eventsSqlModel->column(LongDescription));
    _widgetMapper.addMapping(ui->dateEdit, _eventsSqlModel->column(Date));
    _widgetMapper.addMapping(ui->themeEdit, _eventsSqlModel->column(Theme));
    _widgetMapper.addMapping(ui->placeEdit, _eventsSqlModel->column(Place));

    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &EventsMainWindow::showMenu);
}

//====================================================================================

EventsMainWindow::~EventsMainWindow()
{
    delete ui;
}

//====================================================================================

void EventsMainWindow::on_detailAction_triggered()
{
    if(ui->detailBox->isHidden())
    {
        ui->detailBox->show();
        ui->detailAction->setChecked(true);
    }
    else
    {
        ui->detailBox->hide();
        ui->detailAction->setChecked(false);
    }
    _widgetMapper.setCurrentIndex(ui->tableView->currentIndex().row());
}

//====================================================================================

void EventsMainWindow::on_newEventAction_triggered()
{
    EventView* eventView = new EventView;
    eventView->setWindowModality(Qt::ApplicationModal);
    eventView->setAttribute(Qt::WA_DeleteOnClose);

    EventController* eventController = new EventController(eventView,
                                                           _eventsSqlModel,
                                                           INVALID_INDEX,
                                                           this);
    connect(eventController, &EventController::finished, [=](){
        delete eventController;
    });

    eventView->show();
}

//====================================================================================

void EventsMainWindow::on_settingsAction_triggered()
{
    Settings* window = new Settings(_settingsModel);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

//====================================================================================

void EventsMainWindow::on_exportAction_triggered()
{
    Export* window = new Export(_eventsProxyModel);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

//====================================================================================

void EventsMainWindow::on_importAction_triggered()
{
    Import* window = new Import(_eventsProxyModel, this);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

//====================================================================================

void EventsMainWindow::on_exitAction_triggered()
{
    qApp->quit();
}

//====================================================================================

void EventsMainWindow::on_helpAction_triggered()
{
    QMessageBox::information(this, "Справка", "Справка будет добавлена позже", QMessageBox::Ok);
}

//====================================================================================

void EventsMainWindow::on_themeAction_triggered()
{
    ui->listWidget->clear();
    ui->listWidget->addItem("тематика");
    QStringList themes;
    for(int i = 0; i != _eventsSqlModel->rowCount(); i++)
        themes.push_back(_eventsSqlModel->theme(i));
    themes.removeDuplicates();
    ui->listWidget->addItems(themes);
    ui->listWidget->setCurrentRow(0);

    if(ui->listWidget->isHidden())
    {
        ui->listWidget->show();
        ui->themeAction->setChecked(true);
    }
    else
    {
        if(ui->placeAction->isChecked())
        {
            ui->placeAction->setChecked(false);
            ui->themeAction->setChecked(true);
        }
        else
        {
            ui->listWidget->hide();
            ui->themeAction->setChecked(false);
        }
    }
}

//====================================================================================

void EventsMainWindow::on_placeAction_triggered()
{
    ui->listWidget->clear();
    ui->listWidget->addItem("место");
    QStringList places;
    for(int i = 0; i != _eventsSqlModel->rowCount(); i++)
        places.push_back(_eventsSqlModel->place(i));
    places.removeDuplicates();
    ui->listWidget->addItems(places);
    ui->listWidget->setCurrentRow(0);

    if(ui->listWidget->isHidden())
    {
        ui->listWidget->show();
        ui->placeAction->setChecked(true);
    }
    else
    {
        if(ui->themeAction->isChecked())
        {
            ui->placeAction->setChecked(true);
            ui->themeAction->setChecked(false);
        }
        else
        {
            ui->listWidget->hide();
            ui->placeAction->setChecked(false);
        }
    }
}

//====================================================================================

void EventsMainWindow::on_removeEvent_triggered()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if(selection.empty())
        return;
    QModelIndex sourceIndex = _eventsProxyModel->mapToSource(ui->tableView->currentIndex());
    _eventsSqlModel->removeRow(sourceIndex.row());
    _eventsSqlModel->submitAll();
}

//====================================================================================

void EventsMainWindow::on_fullList_triggered()
{
    qDebug() << "FullList";
}

//====================================================================================

void EventsMainWindow::on_cardAction_triggered()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if(selection.empty())
        return;

    EventView* eventView = new EventView;
    eventView->setWindowModality(Qt::ApplicationModal);

    QModelIndex sourceIndex = _eventsProxyModel->mapToSource(ui->tableView->currentIndex());
    EventController* eventController = new EventController(eventView,
                                                           _eventsSqlModel,
                                                           sourceIndex.row(),
                                                           this);
    connect(eventController, &EventController::finished, [=](){
        delete eventController;
    });

    eventView->show();
}

//====================================================================================

void EventsMainWindow::showMenu(const QPoint& pos)
{
    QPoint globalPos = ui->tableView->mapToGlobal(pos);
    QMenu* contextmenu = new QMenu;
    contextmenu->installEventFilter(this);
    contextmenu->setAttribute(Qt::WA_DeleteOnClose);
    contextmenu->addAction(ui->cardAction);
    contextmenu->addAction(ui->fullList);
    contextmenu->addAction(ui->removeEvent);
    contextmenu->exec(globalPos);
}

//====================================================================================

void EventsMainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    EventView* eventView = new EventView;
    eventView->setWindowModality(Qt::ApplicationModal);
    eventView->setAttribute(Qt::WA_DeleteOnClose);

    QModelIndex sourceIndex = _eventsProxyModel->mapToSource(index);
    EventController* eventController = new EventController(eventView,
                                                           _eventsSqlModel,
                                                           sourceIndex.row(),
                                                           this);
    connect(eventController, &EventController::finished, [=](){
        delete eventController;
    });

    eventView->show();
}

//====================================================================================

void EventsMainWindow::on_tableView_clicked(const QModelIndex &index)
{
    _widgetMapper.setCurrentIndex(index.row());
}

//====================================================================================

void EventsMainWindow::on_increaseFont_triggered()
{
    QFont fnt;
    fnt.setPointSize(_settingsModel->font() + 1);
    _settingsModel->setFont(fnt.pointSize());
    ui->tableView->setFont(fnt);
}

//====================================================================================

void EventsMainWindow::on_decreaseFont_triggered()
{
    if(_settingsModel->font() > MIN_FONT_SIZE)
    {
        QFont fnt;
        fnt.setPointSize(_settingsModel->font() - 1);
        _settingsModel->setFont(fnt.pointSize());
        ui->tableView->setFont(fnt);
    }
}

//====================================================================================

void EventsMainWindow::on_dayBox_activated(int index)
{
    if(index == 0)
        _eventsProxyModel->removeFilter(DayFilter);
    else
        _eventsProxyModel->setFilter(DayFilter, ui->dayBox->currentIndex());
}

//====================================================================================

void EventsMainWindow::on_monthBox_activated(int index)
{
    if(index == 0)
        _eventsProxyModel->removeFilter(MonthFilter);
    else
        _eventsProxyModel->setFilter(MonthFilter, ui->monthBox->currentIndex());
}

//====================================================================================

void EventsMainWindow::on_yearEdit_textChanged(const QString &year)
{
    if(year == QString())
        _eventsProxyModel->removeFilter(YearFilter);
    else
        _eventsProxyModel->setFilter(YearFilter, ui->yearEdit->text());
}

//====================================================================================

void EventsMainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow != INVALID_INDEX)
    {
        if(ui->themeAction->isChecked())
        {
            if(currentRow == 0)
                _eventsProxyModel->removeFilter(ThemeFilter);
            else
                _eventsProxyModel->setFilter(ThemeFilter, ui->listWidget->currentItem()->text());
        }
        else
        {
            if(currentRow == 0)
                _eventsProxyModel->removeFilter(PlaceFilter);
            else
                _eventsProxyModel->setFilter(PlaceFilter, ui->listWidget->currentItem()->text());
        }
    }
}

//====================================================================================

void EventsMainWindow::on_searchEdit_textChanged(const QString &text)
{
    if(text == QString())
        _eventsProxyModel->removeFilter(TextFilter);
    else
        _eventsProxyModel->setFilter(TextFilter, ui->searchEdit->text());
}

//====================================================================================

void EventsMainWindow::on_anniverBtn_clicked()
{
    if(!ui->anniverBtn->isChecked())
        _eventsProxyModel->removeFilter(AnniversaryFilter);
    else
        _eventsProxyModel->setFilter(AnniversaryFilter, QDate::currentDate().year());
}

//====================================================================================

void EventsMainWindow::on_imagesAction_triggered()
{
    if(!ui->imagesAction->isChecked())
        _eventsProxyModel->removeFilter(ImagesFilter);
    else
        _eventsProxyModel->setFilter(ImagesFilter, 0);
}

//====================================================================================

void EventsMainWindow::hideColumns()
{
    ui->tableView->setColumnHidden(_eventsSqlModel->column(Id), true);
    ui->tableView->setColumnHidden(_eventsSqlModel->column(Theme), true);
    ui->tableView->setColumnHidden(_eventsSqlModel->column(LongDescription), true);
    ui->tableView->setColumnHidden(_eventsSqlModel->column(ExtraDescription), true);
}

//====================================================================================
