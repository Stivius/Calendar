#ifndef EXPORT_H
#define EXPORT_H

#include <QDialog>

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

public:
    ExportView(QWidget *parent = 0);
    ~ExportView();
    void setPath(const QString& path);

private:
    Ui::ExportView *ui;

};

#endif // EXPORT_H
