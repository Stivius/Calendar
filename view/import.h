#ifndef IMPORT_H
#define IMPORT_H

#include <QDialog>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>

#include "model/eventsproxymodel.h"
#include "xlsxdocument.h"

namespace Ui {
class Import;
}

class Import : public QDialog
{
    Q_OBJECT

public:
    explicit Import(EventsProxyModel* eventsModel, QWidget *parent = 0);
    ~Import();
signals:
    void addEvent(const QString& date, const QString& shortDescr, const QString& place, const QString& source, const QString& photos);
private slots:
    void on_importBtn_clicked();
    void on_excelTemplateBtn_clicked();
private:
    EventsProxyModel* _eventsModel;
    Ui::Import *ui;
};

#endif // IMPORT_H