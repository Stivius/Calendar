#ifndef IMPORTCONTROLLER_H
#define IMPORTCONTROLLER_H

#include <QObject>

class ImportView;
class EventsSqlModel;

class ImportController : public QObject
{
    Q_OBJECT
signals:
    void finished();

public:
    explicit ImportController(ImportView *importView,
                              EventsSqlModel *eventsSqlModel,
                              QObject *parent);
    ~ImportController();

private slots:
    void importEvents();
    void openExcelTemplate();
    void choosePath();
    void setPath(const QString& path);

private:
    ImportView* _importView = nullptr;
    EventsSqlModel* _eventsSqlModel = nullptr;
    QString _path;

};

#endif // IMPORTCONTROLLER_H
