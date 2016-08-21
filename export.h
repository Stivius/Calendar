#ifndef EXPORT_H
#define EXPORT_H

#include <QDialog>
#include <QFileDialog>

#include "model.h"
#include "filter.h"

namespace Ui {
class Export;
}

class Export : public QDialog
{
    Q_OBJECT

public:
    Export(Model* _model, Filter* _filter, QWidget *parent = 0);
    ~Export();
private slots:
    void on_pathButton_clicked();    
    void on_submitExport_clicked();

private:
    Model* model;
    Filter* filter;
    Ui::Export *ui;
};

#endif // EXPORT_H
