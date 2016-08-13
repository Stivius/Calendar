#ifndef EVENT_H
#define EVENT_H

#include <QWidget>
#include <QFileDialog>
#include "mainwindow.h"
#include "model.h"

namespace Ui {
class Event;
}

class MainWindow;

class Event : public QWidget
{
    Q_OBJECT

public:
    Event(MainWindow* _window, Model* _model, int _row = -1, QWidget *parent = 0);
    ~Event();
private slots:
    void on_uploadButton_clicked();
    void uploadedPhoto(QString);
    void on_removeImage_clicked();
    void on_nextImage_clicked();
    void on_previousImage_clicked();
    void on_saveButton_clicked();
    void on_cancelButton_clicked();
    void on_currentPhoto_clicked();
private:
    Ui::Event *ui;
    MainWindow* window;
    Model* model;
    int row;

    QVector<QPixmap> images;
    QVector<QPixmap> uploadedImages;
    QVector<QString> uploadedPath;
    int currentImage;
};

#endif // EVENT_H
