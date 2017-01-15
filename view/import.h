#ifndef IMPORT_H
#define IMPORT_H

#include <QDialog>

namespace Ui {
class ImportView;
}

class ImportView : public QDialog
{
    Q_OBJECT
signals:
    void importBtnClicked();
    void excelTemplateBtnClicked();

public:
    explicit ImportView(QWidget *parent = 0);
    ~ImportView();

private:
    Ui::ImportView *ui;

};

#endif // IMPORT_H
