#ifndef EXPORT_H
#define EXPORT_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class Export;
}

class Export : public QDialog
{
    Q_OBJECT

public:
    explicit Export(QWidget *parent = 0);
    ~Export();
private slots:
    void on_pathButton_clicked();
private:
    Ui::Export *ui;
};

#endif // EXPORT_H
