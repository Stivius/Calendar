#include "eventsmainwindow.h"
#include "ui_eventsmainwindow.h"

#include <QMessageBox>
#include <QDebug>
#include <QDate>

#include "model/eventsproxymodel.h"
#include "model/eventssqlmodel.h"
#include "model/settingssqlmodel.h"
#include "view/exportview.h"
#include "view/importview.h"
#include "view/settingsview.h"
#include "view/eventview.h"
#include "controller/eventcontroller.h"
#include "controller/exportcontroller.h"
#include "controller/settingscontroller.h"
#include "controller/importcontroller.h"

//====================================================================================

const int INVALID_INDEX = -1;
const int MIN_FONT_SIZE = 7;
const int COLUMNS_COUNT = 5;

//====================================================================================

EventsMainWindow::EventsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EventsMainWindow)
{
    ui->setupUi(this);
    ui->detailBox->hide();
    ui->listWidget->hide();

    connectToDatabase();

    _eventsSqlModel = std::unique_ptr<EventsSqlModel>(new EventsSqlModel(_database));
    _eventsProxyModel = std::unique_ptr<EventsProxyModel>(new EventsProxyModel(_eventsSqlModel.get()));
    _settingsSqlModel = std::unique_ptr<SettingsSqlModel>(new SettingsSqlModel(_database));



    ui->tableView->setModel(_eventsProxyModel.get());
    QFont fnt;
    fnt.setPointSize(_settingsSqlModel->font());
    ui->tableView->setFont(fnt);

    QStringList sizes = _settingsSqlModel->headersSizes();
    for(int i = 1; i <= COLUMNS_COUNT; i++)
        ui->tableView->horizontalHeader()->resizeSection(i, sizes[i-1].toInt());

    hideColumns();
    connect(_eventsProxyModel.get(), &EventsProxyModel::filterUpdated, this, [=](){
        hideColumns();
    });

    if(_settingsSqlModel->anniversaryDates())
    {
        ui->anniverBtn->setChecked(true);
        _eventsProxyModel->setFilter(AnniversaryFilter, QDate::currentDate().year());
    }

    _widgetMapper.setModel(_eventsProxyModel.get());

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
    calculateColumnsSize();

    delete ui;
}

//====================================================================================

void EventsMainWindow::connectToDatabase()
{
    _database = QSqlDatabase::addDatabase("QSQLITE");
    _database.setDatabaseName("userdata.sqlite");
    _database.open();
}

//====================================================================================

void EventsMainWindow::calculateColumnsSize()
{
    QStringList sizes;
    for(int i = 1; i <= COLUMNS_COUNT; i++)
        sizes.push_back(QString::number(ui->tableView->horizontalHeader()->sectionSize(i)));
    _settingsSqlModel->setHeadersSizes(sizes);
}

//====================================================================================

void EventsMainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    openEventView(_eventsProxyModel->mapToSource(index).row());
}

//====================================================================================

void EventsMainWindow::on_newEventAction_triggered()
{
    openEventView(INVALID_INDEX);
}

//====================================================================================

void EventsMainWindow::on_cardAction_triggered()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if(selection.empty())
        return;

    QModelIndex sourceIndex = _eventsProxyModel->mapToSource(ui->tableView->currentIndex());
    openEventView(sourceIndex.row());
}

//====================================================================================

void EventsMainWindow::openEventView(int row)
{
    EventView* eventView = new EventView;
    eventView->setWindowModality(Qt::ApplicationModal);
    eventView->setAttribute(Qt::WA_DeleteOnClose);

    EventController* eventController = new EventController(eventView,
                                                           _eventsSqlModel.get(),
                                                           _settingsSqlModel.get(),
                                                           row,
                                                           this);
    connect(eventController, &EventController::finished, [=](){
        delete eventController;
    });

    eventView->show();
}

//====================================================================================

void EventsMainWindow::on_settingsAction_triggered()
{
    SettingsView* settingsView = new SettingsView(this);
    settingsView->setWindowModality(Qt::ApplicationModal);
    settingsView->setAttribute(Qt::WA_DeleteOnClose);

    SettingsController* settingsController = new SettingsController(settingsView,
                                                                    _settingsSqlModel.get(),
                                                                    this);
    connect(settingsController, &SettingsController::finished, [=](){
        delete settingsController;
    });

    settingsView->show();
}

//====================================================================================

void EventsMainWindow::on_exportAction_triggered()
{
    ExportView* exportView = new ExportView(this);
    exportView->setWindowModality(Qt::ApplicationModal);
    exportView->setAttribute(Qt::WA_DeleteOnClose);

    ExportController* exportController = new ExportController(exportView,
                                                              _eventsProxyModel.get(),
                                                              _settingsSqlModel.get(),
                                                              this);
    connect(exportController, &ExportController::finished, [=](){
        delete exportController;
    });

    exportView->show();
}

//====================================================================================

void EventsMainWindow::on_importAction_triggered()
{
    ImportView* importView = new ImportView(this);
    importView->setWindowModality(Qt::ApplicationModal);
    importView->setAttribute(Qt::WA_DeleteOnClose);

    ImportController* importController = new ImportController(importView,
                                                              _eventsSqlModel.get(),
                                                              this);
    connect(importController, &ImportController::finished, [=](){
        delete importController;
    });

    importView->show();
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

void EventsMainWindow::on_tableView_clicked(const QModelIndex &index)
{
    _widgetMapper.setCurrentIndex(index.row());
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

void EventsMainWindow::on_fullList_triggered()
{
    qDebug() << "FullList";
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

void EventsMainWindow::on_increaseFont_triggered()
{
    QFont fnt;
    fnt.setPointSize(_settingsSqlModel->font() + 1);
    _settingsSqlModel->setFont(fnt.pointSize());
    ui->tableView->setFont(fnt);
}

//====================================================================================

void EventsMainWindow::on_decreaseFont_triggered()
{
    if(_settingsSqlModel->font() > MIN_FONT_SIZE)
    {
        QFont fnt;
        fnt.setPointSize(_settingsSqlModel->font() - 1);
        _settingsSqlModel->setFont(fnt.pointSize());
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
