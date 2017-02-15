#ifndef EXPORT_H
#define EXPORT_H

#include <QDialog>
#include <memory>

class ExportController;

namespace Ui {
class ExportView;
}

class ExportView : public QDialog
{
    Q_OBJECT
signals:
    void choosePathBtnClicked();
    void submitBtnClicked();
    void browserBtnClicked();
    void pdfBtnClicked();
    void memoryDatesBtnClicked();
    void usualEventsBtnClicked();

public:
    ExportView(QWidget *parent = 0);
    ~ExportView();
    void setPath(const QString& path);
    void setController(std::shared_ptr<ExportController> &exportController);

private:
    Ui::ExportView *ui;
    std::shared_ptr<ExportController> _exportController;

};

#endif // EXPORT_H
