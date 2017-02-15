#ifndef IMPORT_H
#define IMPORT_H

#include <QDialog>
#include <memory>

class ImportController;

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
    void setController(std::shared_ptr<ImportController> &importController);

private:
    Ui::ImportView *ui;
    std::shared_ptr<ImportController> _importController;

};

#endif // IMPORT_H
