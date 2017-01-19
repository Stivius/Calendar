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
    void choosePathBtnClicked();

public:
    explicit ImportView(QWidget *parent = 0);
    ~ImportView();
    void setPath(const QString &path);

private:
    Ui::ImportView *ui;

};

#endif // IMPORT_H
