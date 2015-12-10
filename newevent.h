#ifndef NEWEVENT_H
#define NEWEVENT_H

#include <QtWidgets>
#include "eventsmodel.h"
#include "maininterface.h"

class NewEvent: public QWidget
{
    Q_OBJECT
public:
    NewEvent(EventsModel *model = 0, MainInterface *in = 0, QTableWidgetItem *it = 0, int row = 0,  QWidget *parent = 0);
private slots:
    void save();
    void upload();
    void uploadphoto();
    void removephoto();
    void next();
    void prev();
private:
    QVector<QPixmap> uploadedtemp;
    QVector<QString> uploaded;
    QVector<QString> removed;
    QString img;
    int now;
    int num;
    int cimg;
    int row_;
    QVector<QPixmap> vec;
    QTreeView *view;
    QDirModel *dir;
    QPushButton *ok;
    QVBoxLayout *lay;
    QWidget *wgt;
    int edit;
    QString sdesc2;
    QTableWidgetItem *item;
    QSpinBox *day;
    QComboBox *month;
    QSpinBox *year;
    QComboBox *theme;
    QLineEdit *sdesc;
    QTextEdit *ldesc;
    QComboBox *place;
    QComboBox *source;
    QLineEdit *extra;
    QPushButton *btn[4];
    QPixmap *pix;
    QLabel *lbl[10];
    QVBoxLayout *vlay[6];
    QHBoxLayout *hlay[6];
    QVBoxLayout *mlayout;
    QPushButton *btn2[2];
    EventsModel *db;
    MainInterface *inter;
};

#endif // NEWEVENT_H
