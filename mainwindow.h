#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDebug>

#include "event.h"
#include "settings.h"
#include "export.h"
#include "model.h"
#include "filter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void updateRows(const QSet<int>&);
    void addEvent(QString, QString, QString, QString, QString);
    void updateEvent(int, QString, QString, QString, QString, QString);
private slots:
    void on_detailAction_triggered();
    void on_newEventAction_triggered();
    void on_settingsAction_triggered();
    void on_exitAction_triggered();
    void on_exportAction_triggered();
    void on_themeAction_triggered();
    void on_placeAction_triggered();
    void on_tableWidget_clicked(const QModelIndex &index);   
    void showMenu(const QPoint&);
    void on_removeEvent_triggered();
    void on_fullList_triggered();
    void on_cardAction_triggered();
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);
    void on_increaseFont_triggered();
    void on_decreaseFont_triggered();
    void on_dayBox_activated(int index);
    void on_monthBox_activated(int index);
    void on_yearEdit_textChanged(const QString &year);
    void on_listWidget_currentRowChanged(int currentRow);
    void on_searchEdit_textChanged(const QString &text);
    void on_anniverBtn_clicked();
    void on_helpAction_triggered();
    void on_photosAction_triggered();

private:
    void fillTable();
    Ui::MainWindow *ui;
    Model* model;
    Filter* filter;
};

#endif // MAINWINDOW_H
