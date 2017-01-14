#ifndef EXPORTCONTROLLER_H
#define EXPORTCONTROLLER_H

#include <QObject>

class ExportView;
class EventsProxyModel;

enum class ExportType {
    Browser,
    Pdf
};

class ExportController : public QObject
{
    Q_OBJECT
signals:
    void finished();

public:
    explicit ExportController(ExportView *exportView,
                              EventsProxyModel *eventsModel,
                              QObject *parent);
    ~ExportController();

private slots:
    void choosePath();
    void submitExport();
    void setPath(const QString& path);

private:
    void setExportType(ExportType exportType);

private:
    ExportView* _exportView;
    EventsProxyModel* _eventsProxyModel;

    QString _path;
    ExportType _exportType = ExportType::Pdf;

};

#endif // EXPORTCONTROLLER_H
