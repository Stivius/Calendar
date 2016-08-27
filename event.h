#ifndef EVENT_H
#define EVENT_H

#include <QDialog>
#include <QFileDialog>
#include <QUrl>
#include <QDebug>
#include <QPixmap>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QMessageBox>

#include "model.h"

namespace Ui {
class Event;
}

struct Image {
    QPixmap image;
    QString path;
    QString tempPath;
};

class Event : public QDialog
{
    Q_OBJECT

public:
    Event(Model* _model, QWidget *parent = 0, int _row = -1);
    ~Event();
signals:
    void addEvent(const QString& date, const QString& shortDescr, const QString& place, const QString& source, const QString& photos);
    void updateEvent(int id, const QString& date, const QString& shortDescr, const QString& place, const QString& source, const QString& photos);
private slots:
    void on_uploadButton_clicked();
    void uploadedPhoto(QString& filePath);
    void on_removeImage_clicked();
    void on_nextImage_clicked();
    void on_previousImage_clicked();
    void on_saveButton_clicked();
    void on_cancelButton_clicked();
    void on_currentPhoto_clicked();
private:
    void loadImages(const QStringList& imagesList);
    QString getSavedImages();
    void removeImages();
    Ui::Event *ui;
    Model* model;
    int row;
    QVector<Image> images;
    QVector<QString> removed;
    int currentImage;
};

#endif // EVENT_H
