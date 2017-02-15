#ifndef EVENTVIEW_H
#define EVENTVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>
#include <memory>

namespace Ui {
class EventView;
}

class EventsSqlModel;
class EventController;

class EventView : public QWidget
{
    Q_OBJECT
signals:
    void saveBtnClicked();
    void cancelBtnClicked();
    void uploadBtnClicked();
    void removeBtnClicked();
    void nextBtnClicked();
    void previousBtnClicked();
    void currentImageClicked();

public:
    explicit EventView(QWidget *parent = 0);
    ~EventView();

    void setDay(int index);
    void setMonth(int index);
    void setYear(const QString &year);
    int selectedDay();
    int selectedMonth();
    int selectedYear();
    void addThemes(const QStringList &themes);
    void addPlaces(const QStringList &places);
    void addSources(const QStringList &sources);
    void setCurrentTheme(const QString &theme);
    void setCurrentPlace(const QString &place);
    void setCurrentSource(const QString &source);
    void setMapperModel(EventsSqlModel* eventsModel);
    void setMapperIndex(int index);
    void setCurrentImage(const QPixmap& image);
    void setController(std::shared_ptr<EventController>& eventController);

private:
    Ui::EventView *ui;
    QDataWidgetMapper _widgetMapper;
    std::shared_ptr<EventController> _eventController;

};

#endif // EVENTVIEW_H
