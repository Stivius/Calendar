#ifndef NEWEVENT_H
#define NEWEVENT_H

#include <QtWidgets>

class NewEvent: public QWidget
{
    Q_OBJECT
public:
    NewEvent(QWidget *parent = 0);
private:
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
};

#endif // NEWEVENT_H
