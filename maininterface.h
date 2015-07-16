#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QtWidgets>
#include "eventsmodel.h"

class MainInterface: public QWidget
{
    Q_OBJECT
public:
    friend class NewEvent;
    MainInterface(QWidget *parent = 0);
    void set(int,QString,int,QString,QString,QString);
    void up(int,QString,int,QString,QString,QString);
private slots:
    void settings();
    void closeset();
    void newevent();
    void edititem(QTableWidgetItem*);
    void del();
    void menushow(const QPoint&);
    void fbig();
    void fsmall();
    void themes();
    void places();
    void card();
    void indetail();
signals:
    void senditem(QTableWidgetItem*);
private:
    //
    QLineEdit *line;
    QTextEdit *tedit;
    QLabel *label[5];
    QComboBox *combobox[3];
    QVBoxLayout *lay[5];
    QHBoxLayout *hlay[2];
    QVBoxLayout *vlay;
    QGroupBox *group;
    //
    QMenu *contextmenu;
    QAction *cmm[3];
    int showlist;
    bool eventFilter(QObject*, QEvent*);
    QListWidget *list;
    QHBoxLayout *laylist;
    QMenuBar *bar;
    QAction *mm[2];
    QAction *mm2[6];
    QAction *mm3[5];
    QMenu *menu;
    QMenu *menu2;
    QGroupBox *box[2];
    QHBoxLayout *boxlay[3];
    QVBoxLayout *mlayout;
    QComboBox *combo[3];
    QLabel *combolbl[3];
    QCheckBox *check[2];
    QLineEdit *edit;
    QTableWidget *table;
    // settings
    QWidget *wgt;
    QLabel *lbl[2];
    QGroupBox *box2;
    QRadioButton *radio[2];
    QLineEdit *edit2;
    QSlider *slider;
    QPushButton *btn[2];
    QVBoxLayout *box2lay;
    QVBoxLayout *setlay;
    QHBoxLayout *btnlay;
    //
    EventsModel *db;
};

#endif // MAININTERFACE_H
