#ifndef EXPORTCONTROLLER_H
#define EXPORTCONTROLLER_H

#include <QObject>

class ExportView;
class EventsProxyModel;
class SettingsSqlModel;

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
                              SettingsSqlModel* settingsSqlModel,
                              QObject *parent);
    ~ExportController();

private slots:
    void choosePath();
    void submitExport();
    void setPath(const QString& path);

private:
    void setExportType(ExportType exportType);

private:
    ExportView* _exportView = nullptr;
    EventsProxyModel* _eventsProxyModel = nullptr;
    SettingsSqlModel* _settingsSqlModel = nullptr;

    QString _path;
    ExportType _exportType = ExportType::Pdf;

};

#endif // EXPORTCONTROLLER_H
