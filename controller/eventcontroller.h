#ifndef EVENT_H
#define EVENT_H

#include <QObject>
#include <QPixmap>

class EventsSqlModel;
class SettingsSqlModel;
class EventView;

struct Image {
   QPixmap _pixmap;
   QString _imageName;
   QString _tempPath;
};

class EventController : public QObject
{
    Q_OBJECT
signals:
    void finished();

public:
    EventController(EventView* evenView,
                    EventsSqlModel* eventsModel,
                    SettingsSqlModel* settingsModel,
                    int currentRow,
                    QObject* parent);
    ~EventController();

private slots:
    void saveEvent();
    void cancelSaving();

private:
    void uploadImage(const QString& filePath);
    void openCurrentImage();
    void openFileDialog();
    void removeImage();
    void nextImage();
    void previousImage();

    void loadImages();
    void saveImages();
    void removeTemporaryImages();
    QStringList getImagesNames();

private:
    EventView* _eventView = nullptr;
    EventsSqlModel* _eventsModel = nullptr;
    SettingsSqlModel* _settingsModel = nullptr;
    int _currentRow;

    QVector<Image> _images;
    QStringList _imagesToRemove;
    int _currentImageIndex;  

};

#endif // EVENT_H
