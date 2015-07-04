#ifndef MAININTERFACE_H
#define MAININTERFACE_H

#include <QtWidgets>

class MainInterface: public QWidget
{
    Q_OBJECT
public:
    MainInterface(QWidget *parent = 0);
private slots:
    void settings();
private:
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
};

#endif // MAININTERFACE_H
