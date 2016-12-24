#ifndef EVENT_H
#define EVENT_H

#include <QDialog>
#include <QDataWidgetMapper>

class EventsProxyModel;

namespace Ui {
class Event;
}

struct Image {
    QPixmap _pixmap;
    QString _path;
    QString _tempPath;
};

class Event : public QDialog
{
    Q_OBJECT
public:
    Event(EventsProxyModel* eventsModel, int currentRow = -1, QWidget* parent = 0);
    ~Event();
private slots:
//    void on_uploadButton_clicked();
//    void uploadPhoto(QString filePath);
//    void on_removeImage_clicked();
//    void on_nextImage_clicked();
//    void on_previousImage_clicked();
    void on_saveButton_clicked();
    void on_cancelButton_clicked();
//    void on_currentPhoto_clicked();

private:
//    void loadImages();
//    void saveImages();
//    void removeImages();

private:
    Ui::Event* ui;
    EventsProxyModel* _eventsModel;
    QDataWidgetMapper* _widgetMapper;
    int _currentRow;
    int _currentImageIndex;
    QVector<Image> images;
    QVector<QString> imagesToRemove;
};

#endif // EVENT_H
