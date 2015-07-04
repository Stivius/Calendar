#include "newevent.h"

NewEvent::NewEvent(QWidget *parent):QWidget(parent)
{
    mlayout = new QVBoxLayout;
    for(int i = 0; i != 6; i++)
    {
        vlay[i] = new QVBoxLayout;
        hlay[i] = new QHBoxLayout;
    }
    //
    lbl[0] = new QLabel("Дата события");
    day = new QSpinBox;
    month = new QComboBox;
    year = new QSpinBox;
    lbl[1] = new QLabel("г.");
    hlay[0]->addWidget(day);
    hlay[0]->addWidget(month);
    hlay[0]->addWidget(year);
    hlay[0]->addWidget(lbl[1]);
    vlay[0]->addWidget(lbl[0]);
    vlay[0]->addLayout(hlay[0]);
    lbl[2] = new QLabel("Тематика");
    theme = new QComboBox;
    vlay[1]->addWidget(lbl[2]);
    vlay[1]->addWidget(theme);
    hlay[1]->addLayout(vlay[0]);
    hlay[1]->addLayout(vlay[1]);
    //
    lbl[3] = new QLabel("Краткое описание события");
    sdesc = new QLineEdit;
    lbl[4] = new QLabel("Полное описание события");
    ldesc = new QTextEdit;
    vlay[2]->addWidget(lbl[3]);
    vlay[2]->addWidget(sdesc);
    vlay[2]->addWidget(lbl[4]);
    vlay[2]->addWidget(ldesc);
    //
    lbl[5] = new QLabel("Место события");
    place = new QComboBox;
    lbl[6] = new QLabel("Источник");
    source = new QComboBox;
    lbl[7] = new QLabel("Дополнительно");
    extra = new QLineEdit;
    vlay[3]->addWidget(lbl[5]);
    vlay[3]->addWidget(place);
    vlay[3]->addWidget(lbl[6]);
    vlay[3]->addWidget(source);
    vlay[3]->addWidget(lbl[7]);
    vlay[3]->addWidget(extra);
    //
    lbl[8] = new QLabel("Фотография");
    lbl[9] = new QLabel;
    pix = new QPixmap(125,125);
    lbl[9]->setPixmap(*pix);
    btn[0] = new QPushButton("+");
    btn[0]->setFixedSize(25,25);
    btn[1] = new QPushButton("-");
    btn[1]->setFixedSize(25,25);
    btn[2] = new QPushButton("->");
    btn[2]->setFixedSize(25,25);
    btn[3] = new QPushButton("<-");
    btn[3]->setFixedSize(25,25);
    vlay[4]->addWidget(btn[0]);
    vlay[4]->addWidget(btn[1]);
    vlay[4]->addWidget(btn[2]);
    vlay[4]->addWidget(btn[3]);
    hlay[3]->addLayout(vlay[4]);
    hlay[3]->addWidget(lbl[9]);
    vlay[5]->addWidget(lbl[8]);
    vlay[5]->addLayout(hlay[3]);
    //
    hlay[4]->addLayout(vlay[3]);
    hlay[4]->addLayout(vlay[5]);
    //
    mlayout->addLayout(hlay[1]);
    mlayout->addLayout(vlay[2]);
    mlayout->addLayout(hlay[4]);
    setLayout(mlayout);
}

