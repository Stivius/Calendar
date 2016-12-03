#ifndef EXPORT_H
#define EXPORT_H

#include <QDialog>
#include <QFileDialog>

class EventsProxyModel;

namespace Ui {
class Export;
}

class Export : public QDialog
{
    Q_OBJECT

public:
    Export(EventsProxyModel* eventsProxyModel, QWidget *parent = 0);
    ~Export();
private slots:
    void on_pathButton_clicked();
    void on_submitExport_clicked();
private:
    EventsProxyModel* _eventsProxyModel;
    Ui::Export *ui;
};

#endif // EXPORT_H
