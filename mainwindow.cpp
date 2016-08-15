#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "event.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new Model;
    filter = new Filter(model, this);
    QFont fnt(ui->tableWidget->font());
    fnt.setPointSize(model->getFont());
    ui->tableWidget->setFont(fnt);
    for(int i = 0; i != 5; i++)
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    ui->detailBox->hide();
    ui->listWidget->hide();
    if(model->count() > 0)
        fillTable();
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), SLOT(showMenu(const QPoint&)));
}

MainWindow::~MainWindow()
{
    delete model;
    delete ui;
}

void MainWindow::updateRows(const QSet<int>& rows)
{
    for(int i = 0; i != model->count(); i++)
    {
        if(rows.contains(i))
            ui->tableWidget->hideRow(i);
        else
            ui->tableWidget->showRow(i);
    }
}

void MainWindow::fillTable()
{
    for(int i = 0; i != model->count(); i++)
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(model->getDate(i)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(model->getSDescrpition(i)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(model->imagesCount(i) > 0 ? "Есть" : "Нет"));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(model->getPlace(i)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(model->getSource(i)));
    }
}

void MainWindow::on_detailAction_triggered()
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
}

void MainWindow::on_newEventAction_triggered()
{
    Event* window = new Event(model, this);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void MainWindow::on_settingsAction_triggered()
{
    Settings* window = new Settings(model);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void MainWindow::on_exportAction_triggered()
{
    Export* window = new Export(model);
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void MainWindow::on_exitAction_triggered()
{
    qApp->quit();
}

void MainWindow::on_themeAction_triggered()
{
    ui->listWidget->clear();
    ui->listWidget->addItem("тематика");
    QStringList themes;
    for(int i = 0; i != model->count(); i++)
        themes.push_back(model->getTheme(i));
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
            filter->removeFilter(PLACE_FILTER);
            ui->placeAction->setChecked(false);
            ui->themeAction->setChecked(true);
        }
        else
        {
            filter->removeFilter(THEME_FILTER);
            ui->listWidget->hide();
            ui->themeAction->setChecked(false);
        }
    }
}

void MainWindow::on_placeAction_triggered()
{
    ui->listWidget->clear();
    ui->listWidget->addItem("место");
    QStringList places;
    for(int i = 0; i != model->count(); i++)
        places.push_back(model->getPlace(i));
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
            filter->removeFilter(THEME_FILTER);
            ui->placeAction->setChecked(true);
            ui->themeAction->setChecked(false);
        }
        else
        {
            filter->removeFilter(PLACE_FILTER);
            ui->listWidget->hide();
            ui->placeAction->setChecked(false);
        }
    }
}

// change
void MainWindow::addEvent(QString date, QString shortDescr, QString place, QString source, QString photos)
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(date));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(shortDescr));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem(photos));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3, new QTableWidgetItem(place));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 4, new QTableWidgetItem(source));
}

void MainWindow::updateEvent(int row, QString date, QString shortDescr, QString place, QString source, QString photos)
{
    ui->tableWidget->item(row, 0)->setText(date);
    ui->tableWidget->item(row, 1)->setText(shortDescr);
    ui->tableWidget->item(row, 2)->setText(photos);
    ui->tableWidget->item(row, 3)->setText(place);
    ui->tableWidget->item(row, 4)->setText(source);
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    if(!ui->detailBox->isHidden())
    {
        ui->extraEdit->setText(model->getExtra(row));
        ui->fullEdit->setPlainText(model->getLDescrpition(row));
        ui->dateEdit->setText(model->getDate(row));
        ui->themeEdit->setText(model->getTheme(row));
        ui->placeEdit->setText(model->getPlace(row));
    }
}

void MainWindow::showMenu(const QPoint& pos)
{
    QPoint globalPos = ui->tableWidget->mapToGlobal(pos);
    QMenu* contextmenu = new QMenu;
    contextmenu->installEventFilter(this);
    contextmenu->setAttribute(Qt::WA_DeleteOnClose);
    contextmenu->addAction(ui->cardAction);
    contextmenu->addAction(ui->fullList);
    contextmenu->addAction(ui->removeEvent);
    contextmenu->exec(globalPos);
}

void MainWindow::on_removeEvent_triggered()
{
    QModelIndexList selection = ui->tableWidget->selectionModel()->selectedRows();
    if(selection.count() == 0)
        return ;
    model->removeEvent(ui->tableWidget->currentRow());
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void MainWindow::on_fullList_triggered()
{
    qDebug() << "FullList";
}

void MainWindow::on_cardAction_triggered()
{
    QModelIndexList selection = ui->tableWidget->selectionModel()->selectedRows();
    if(selection.count() == 0)
        return ;
    Event* window = new Event(model, this, ui->tableWidget->currentRow());
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    Event* window = new Event(model, this, item->row());
    window->setWindowModality(Qt::ApplicationModal);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

void MainWindow::on_increaseFont_triggered()
{
    QFont fnt;
    model->updateFont(model->getFont()+1);
    fnt.setPointSize(model->getFont());
    ui->tableWidget->setFont(fnt);
}

void MainWindow::on_decreaseFont_triggered()
{
    if(model->getFont() > 7)
    {
        QFont fnt;
        model->updateFont(model->getFont()-1);
        fnt.setPointSize(model->getFont());
        ui->tableWidget->setFont(fnt);
    }
}

void MainWindow::on_dayBox_activated(int index)
{
    if(index == 0)
        filter->removeFilter(DAY_FILTER);
    else
        filter->setFilter(DAY_FILTER, ui->dayBox->currentText());
}

void MainWindow::on_monthBox_activated(int index)
{
    if(index == 0)
        filter->removeFilter(MONTH_FILTER);
    else
        filter->setFilter(MONTH_FILTER, QString::number(model->getMonth(ui->monthBox->currentText())));
}

void MainWindow::on_yearEdit_textChanged(const QString &year)
{
    if(year == "")
        filter->removeFilter(YEAR_FILTER);
    else
        filter->setFilter(YEAR_FILTER, ui->yearEdit->text());
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow != -1)
    {
        if(ui->themeAction->isChecked())
        {
            if(currentRow == 0)
                filter->removeFilter(THEME_FILTER);
            else
                filter->setFilter(THEME_FILTER, ui->listWidget->currentItem()->text());
        }
        else
        {
            if(currentRow == 0)
                filter->removeFilter(PLACE_FILTER);
            else
                filter->setFilter(PLACE_FILTER, ui->listWidget->currentItem()->text());
        }
    }
}

void MainWindow::on_searchEdit_textChanged(const QString &text)
{
    if(text == "")
        filter->removeFilter(TEXT_FILTER);
    else
        filter->setFilter(TEXT_FILTER, ui->searchEdit->text());
}

void MainWindow::on_anniverBtn_clicked()
{
    if(!ui->anniverBtn->isChecked())
        filter->removeFilter(ANNIVER_FILTER);
    else
        filter->setFilter(ANNIVER_FILTER, QString::number(QDate::currentDate().year()));
}

void MainWindow::on_helpAction_triggered()
{
    QMessageBox::information(this, "Справка", "Справка будет добавлена позже", QMessageBox::Ok);
}

void MainWindow::on_photosAction_triggered()
{
    if(!ui->photosAction->isChecked())
        filter->removeFilter(IMAGES_FILTER);
    else
        filter->setFilter(IMAGES_FILTER);
}
