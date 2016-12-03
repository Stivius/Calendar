#include "eventsmainwindow.h"
#include "ui_eventsmainwindow.h"

#include <QMessageBox>
#include <QDebug>

#include "event.h"
#include "eventssqlmodel.h"
#include "eventsproxymodel.h"
#include "settingssqlmodel.h"
#include "settings.h"
#include "export.h"

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
    ui->tableView->setColumnHidden(5, true);
    ui->tableView->setColumnHidden(6, true);
    ui->tableView->setColumnHidden(7, true);

    _widgetMapper = new QDataWidgetMapper;
    _widgetMapper->setModel(_eventsProxyModel);

    _widgetMapper->addMapping(ui->extraEdit, _eventsProxyModel->column(ExtraDescription));
    _widgetMapper->addMapping(ui->fullEdit, _eventsProxyModel->column(LongDescription));
    _widgetMapper->addMapping(ui->dateEdit, _eventsProxyModel->column(Date));
    _widgetMapper->addMapping(ui->themeEdit, _eventsProxyModel->column(Theme));
    _widgetMapper->addMapping(ui->placeEdit, _eventsProxyModel->column(Place));

    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &EventsMainWindow::showMenu);

}

EventsMainWindow::~EventsMainWindow()
{
    delete ui;
}

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
    _widgetMapper->setCurrentIndex(ui->tableView->currentIndex().row());
}

void EventsMainWindow::on_newEventAction_triggered()
{
    Event* window = new Event(_eventsProxyModel);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void EventsMainWindow::on_settingsAction_triggered()
{
    Settings* window = new Settings(_settingsModel);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void EventsMainWindow::on_exportAction_triggered()
{
    Export* window = new Export(_eventsProxyModel);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

//void MainWindow::on_importAction_triggered()
//{
//    Import* window = new Import(model, this);
//    window->setWindowModality(Qt::ApplicationModal);
//    window->setAttribute(Qt::WA_DeleteOnClose);
//    window->show();
//}

void EventsMainWindow::on_exitAction_triggered()
{
    qApp->quit();
}

void EventsMainWindow::on_helpAction_triggered()
{
    QMessageBox::information(this, "Справка", "Справка будет добавлена позже", QMessageBox::Ok);
}

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

void EventsMainWindow::on_removeEvent_triggered()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if(selection.count() == 0)
        return ;
    _eventsSqlModel->removeRow(ui->tableView->currentIndex().row());
    _eventsSqlModel->submitAll();
}

void EventsMainWindow::on_fullList_triggered()
{
    qDebug() << "FullList";
}

void EventsMainWindow::on_cardAction_triggered()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if(selection.count() == 0)
        return ;
    Event* window = new Event(_eventsProxyModel, ui->tableView->currentIndex().row(), this);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

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

void EventsMainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    Event* window = new Event(_eventsProxyModel, index.row(), this);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void EventsMainWindow::on_tableView_clicked(const QModelIndex &index)
{
    _widgetMapper->setCurrentIndex(index.row());
}

void EventsMainWindow::on_increaseFont_triggered()
{
    QFont fnt;
    fnt.setPointSize(_settingsModel->font() + 1);
    _settingsModel->setFont(fnt.pointSize());
    ui->tableView->setFont(fnt);
}

void EventsMainWindow::on_decreaseFont_triggered()
{
    if(_settingsModel->font() > 7)
    {
        QFont fnt;
        fnt.setPointSize(_settingsModel->font() - 1);
        _settingsModel->setFont(fnt.pointSize());
        ui->tableView->setFont(fnt);
    }
}

//void MainWindow::on_dayBox_activated(int index)
//{
//    if(index == 0)
//        filter->removeFilter(DAY_FILTER);
//    else
//        filter->setFilter(DAY_FILTER, ui->dayBox->currentText());
//}

//void MainWindow::on_monthBox_activated(int index)
//{
//    if(index == 0)
//        filter->removeFilter(MONTH_FILTER);
//    else
//        filter->setFilter(MONTH_FILTER, QString::number(model->getMonth(ui->monthBox->currentText())));
//}

//void MainWindow::on_yearEdit_textChanged(const QString &year)
//{
//    if(year == "")
//        filter->removeFilter(YEAR_FILTER);
//    else
//        filter->setFilter(YEAR_FILTER, ui->yearEdit->text());
//}

//void MainWindow::on_listWidget_currentRowChanged(int currentRow)
//{
//    if(currentRow != -1)
//    {
//        if(ui->themeAction->isChecked())
//        {
//            if(currentRow == 0)
//                filter->removeFilter(THEME_FILTER);
//            else
//                filter->setFilter(THEME_FILTER, ui->listWidget->currentItem()->text());
//        }
//        else
//        {
//            if(currentRow == 0)
//                filter->removeFilter(PLACE_FILTER);
//            else
//                filter->setFilter(PLACE_FILTER, ui->listWidget->currentItem()->text());
//        }
//    }
//}

//void MainWindow::on_searchEdit_textChanged(const QString &text)
//{
//    if(text == "")
//        filter->removeFilter(TEXT_FILTER);
//    else
//        filter->setFilter(TEXT_FILTER, ui->searchEdit->text());
//}

//void MainWindow::on_anniverBtn_clicked()
//{
//    if(!ui->anniverBtn->isChecked())
//        filter->removeFilter(ANNIVER_FILTER);
//    else
//        filter->setFilter(ANNIVER_FILTER, QString::number(QDate::currentDate().year()));
//}

//void MainWindow::on_photosAction_triggered()
//{
//    if(!ui->photosAction->isChecked())
//        filter->removeFilter(IMAGES_FILTER);
//    else
//        filter->setFilter(IMAGES_FILTER);
//}
