#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDataWidgetMapper>
#include <QTranslator>
#include <QActionGroup>
#include <memory>

#include "model/translationmodel.h"

class EventsSqlModel;
class EventsProxyModel;
class SettingsSqlModel;
class EventController;
class SettingsController;
class ExportController;
class ImportController;

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

    void connectToDatabase();
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
    void on_russianAction_triggered();
    void on_ukrainianAction_triggered();
    void on_englishAction_triggered();

private:
    void calculateColumnsSize();
    void openEventView(int row);

private:
    Ui::EventsMainWindow *ui;
    QSqlDatabase _database;

    std::shared_ptr<EventsSqlModel> _eventsSqlModel;
    std::shared_ptr<EventsProxyModel> _eventsProxyModel;
    std::shared_ptr<SettingsSqlModel> _settingsSqlModel;
    std::unique_ptr<QActionGroup> _languageGroup;

    TranslationModel _translationModel;
    QDataWidgetMapper _widgetMapper;

};

#endif // MAINWINDOW_H
