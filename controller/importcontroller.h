#ifndef IMPORTCONTROLLER_H
#define IMPORTCONTROLLER_H

#include <QObject>
#include <memory>

class ImportView;
class EventsSqlModel;

class ImportController : public QObject
{
    Q_OBJECT
public:
    explicit ImportController(ImportView *importView,
                              std::shared_ptr<EventsSqlModel>& eventsSqlModel,
                              QObject *parent);
    ~ImportController();

private slots:
    void importEvents();
    void openExcelTemplate();
    void choosePath();
    void setPath(const QString& path);

private:
    ImportView* _importView = nullptr;
    std::shared_ptr<EventsSqlModel> _eventsSqlModel;
    QString _path;

};

#endif // IMPORTCONTROLLER_H
