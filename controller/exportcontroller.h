#ifndef EXPORTCONTROLLER_H
#define EXPORTCONTROLLER_H

#include <QObject>
#include <functional>

class ExportView;
class EventsProxyModel;
class SettingsSqlModel;

enum class ExportType {
    Browser,
    Pdf
};

enum class ExportFormat {
    MemoryDates,
    UsualEvents
};

struct Event {
    int _day;
    int _month;
    int _year;
    QString _fullDescription;
};

using LessThanComparator = std::function<bool(const Event&, const Event&)>;

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
    void initializeData();

    bool lessThanMonth(const Event &left, const Event &right);
    bool lessThanYear(const Event &left, const Event &right);

    QString formatMonthName(int month);
    QString formatEvent(const Event &event);
    QString formatMemoryDates();
    QString formatMonths(int year = -1);
    QString formatUsualEvents();

    QString countYears(int year);

private:
    ExportView* _exportView = nullptr;
    EventsProxyModel* _eventsProxyModel = nullptr;
    SettingsSqlModel* _settingsSqlModel = nullptr;

    QString _path;
    ExportType _exportType = ExportType::Pdf;
    ExportFormat _exportFormat = ExportFormat::MemoryDates;
    std::vector<Event> _exportedEvents;

};

#endif // EXPORTCONTROLLER_H
