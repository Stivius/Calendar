#ifndef EXPORT_H
#define EXPORT_H

#include <QDialog>
#include <QFileDialog>

#include "model.h"

namespace Ui {
class Export;
}

class Export : public QDialog
{
    Q_OBJECT

public:
    Export(Model*, QWidget *parent = 0);
    ~Export();
private slots:
    void on_pathButton_clicked();    
    void on_submitExport_clicked();

private:
    Ui::Export *ui;
    Model* model;
};

#endif // EXPORT_H
