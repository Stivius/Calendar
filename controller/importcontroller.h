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

private:
    ImportView* _importView;
    EventsSqlModel* _eventsSqlModel;

};

#endif // IMPORTCONTROLLER_H
