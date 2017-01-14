#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class Events;
class EventsSqlModel;
class EventsProxyModel;
class SettingsSqlModel;

namespace Ui {
class EventsMainWindow;
}

class EventsMainWindow : public QMainWindow
{
    Q_OBJECT
signals:


public:
    EventsMainWindow(QWidget *parent = 0);
    ~EventsMainWindow();
    void hideColumns();

private slots:
    void on_detailAction_triggered();
    void on_newEventAction_triggered();
    void on_settingsAction_triggered();
    void on_exitAction_triggered();
    void on_exportAction_triggered();
    void on_themeAction_triggered();
    void on_placeAction_triggered();
    void showMenu(const QPoint&);
    void on_removeEvent_triggered();
    void on_fullList_triggered();
    void on_cardAction_triggered();
    void on_increaseFont_triggered();
    void on_decreaseFont_triggered();
    void on_dayBox_activated(int index);
    void on_monthBox_activated(int index);
    void on_yearEdit_textChanged(const QString &year);
    void on_listWidget_currentRowChanged(int currentRow);
    void on_searchEdit_textChanged(const QString &text);
    void on_anniverBtn_clicked();
    void on_helpAction_triggered();
    void on_imagesAction_triggered();
    void on_importAction_triggered();
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::EventsMainWindow *ui;
    QSqlDatabase _database;
    EventsSqlModel* _eventsSqlModel = nullptr;
    EventsProxyModel* _eventsProxyModel = nullptr;
    SettingsSqlModel* _settingsSqlModel = nullptr;
    QDataWidgetMapper _widgetMapper;

};

#endif // MAINWINDOW_H
