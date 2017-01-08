#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QDataWidgetMapper>

class EventsSqlModel;
class EventView;

class EventController : public QObject
{
    Q_OBJECT
public:
    EventController(EventView* evenView,
                    EventsSqlModel* eventsModel,
                    int currentRow,
                    QObject* parent);
    ~EventController();

private slots:
    void saveEvent();
    void cancelSaving();

private:
    EventView* _eventView;
    EventsSqlModel* _eventsModel;
    QDataWidgetMapper* _widgetMapper;
    int _currentRow;

};

#endif // EVENT_H
