#include "maininterface.h"
#include "newevent.h"

MainInterface::MainInterface(QWidget *parent):QWidget(parent)
{
    db = new EventsModel;
    table = new QTableWidget(0,5);
    connect(this,SIGNAL(senditem(QTableWidgetItem*)),SLOT(edititem(QTableWidgetItem*)));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Дата"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Событие"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Изображения"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Место"));
    table->setHorizontalHeaderItem(4, new QTableWidgetItem("Источник"));
    bar = new QMenuBar;
    mm[0] = new QAction("Настройка", 0);
    connect(mm[0],SIGNAL(triggered()),SLOT(settings()));
    mm[1] = new QAction("Справка", 0);
    menu = new QMenu("Даты");
    menu->installEventFilter(this);
    mm2[0] = new QAction("Новое событие", 0);
    connect(mm2[0],SIGNAL(triggered()),SLOT(newevent()));
    mm2[1] = new QAction("Импорт из файла", 0);
    mm2[2] = new QAction("Экспорт в файл", 0);
    mm2[3] = new QAction("Тематика", 0);
    mm2[3]->setCheckable(true);
    connect(mm2[3],SIGNAL(triggered()),SLOT(themes()));
    mm2[4] = new QAction("Места", 0);
    mm2[4]->setCheckable(true);
    connect(mm2[4],SIGNAL(triggered()),SLOT(places()));
    mm2[5] = new QAction("Выход", 0);
    connect(mm2[5],SIGNAL(triggered()),SLOT(close()));
    for(int i = 0; i != 6; i++)
    {
        menu->addAction(mm2[i]);
    }
    menu2 = new QMenu("Вид");
    menu2->installEventFilter(this);
    mm3[0] = new QAction("Шрифт крупнее", 0);
    connect(mm3[0],SIGNAL(triggered()),SLOT(fbig()));
    mm3[1] = new QAction("Шрифт меньше", 0);
    connect(mm3[1],SIGNAL(triggered()),SLOT(fsmall()));
    mm3[2] = new QAction("Юбилейные", 0);
    mm3[3] = new QAction("С фотографиями", 0);
    mm3[4] = new QAction("Показать подробнее", 0);
    connect(mm3[4],SIGNAL(triggered()),SLOT(indetail()));
    for(int i = 0; i != 5; i++)
    {
        menu2->addAction(mm3[i]);
    }
    bar->addMenu(menu);
    bar->addMenu(menu2);
    bar->addAction(mm[0]);
    bar->addAction(mm[1]);
    box[0] = new QGroupBox("Поиск");
    box[1] = new QGroupBox("Дата");
    boxlay[0] = new QHBoxLayout;
    boxlay[1] = new QHBoxLayout;
    boxlay[2] = new QHBoxLayout;
    mlayout = new QVBoxLayout;
    check[0] = new QCheckBox("В выбранном");
    check[1] = new QCheckBox("Юбилейные");
    combo[0] = new QComboBox;
    combo[1] = new QComboBox;
    combo[2] = new QComboBox;
    combolbl[0] = new QLabel("День");
    combolbl[1] = new QLabel("Месяц");
    combolbl[2] = new QLabel("Год");
    edit = new QLineEdit;
    boxlay[0]->addWidget(edit);
    boxlay[0]->addWidget(check[0]);
    box[0]->setLayout(boxlay[0]);
    boxlay[1]->addWidget(check[1]);
    boxlay[1]->addWidget(combolbl[0]);
    boxlay[1]->addWidget(combo[0]);
    boxlay[1]->addWidget(combolbl[1]);
    boxlay[1]->addWidget(combo[1]);
    boxlay[1]->addWidget(combolbl[2]);
    boxlay[1]->addWidget(combo[2]);
    box[1]->setLayout(boxlay[1]);
    boxlay[2]->addWidget(box[0]);
    boxlay[2]->addWidget(box[1]);
    //
    showlist = 0;
    list = new QListWidget;
    list->setFixedWidth(150);
    list->hide();
    laylist = new QHBoxLayout;
    laylist->addWidget(list);
    laylist->addWidget(table);
    //
    for(int i = 0; i != 5; i++)
    {
        lay[i] = new QVBoxLayout;
    }
    hlay[0] = new QHBoxLayout;
    hlay[1] = new QHBoxLayout;
    vlay = new QVBoxLayout;
    group = new QGroupBox("Событие");
    label[0] = new QLabel("Дата");
    combobox[0] = new QComboBox;
    lay[0]->addWidget(label[0]);
    lay[0]->addWidget(combobox[0]);
    label[1] = new QLabel("Тематика");
    combobox[1] = new QComboBox;
    lay[1]->addWidget(label[1]);
    lay[1]->addWidget(combobox[1]);
    label[2] = new QLabel("Место");
    combobox[2] = new QComboBox;
    lay[2]->addWidget(label[2]);
    lay[2]->addWidget(combobox[2]);
    label[3] = new QLabel("Дополнительно");
    line = new QLineEdit;
    lay[3]->addWidget(label[3]);
    lay[3]->addWidget(line);
    label[4] = new QLabel("Подробное описание");
    tedit = new QTextEdit;
    lay[4]->addWidget(label[4]);
    lay[4]->addWidget(tedit);
    hlay[0]->addLayout(lay[0]);
    hlay[0]->addLayout(lay[1]);
    vlay->addLayout(hlay[0]);
    vlay->addLayout(lay[2]);
    vlay->addLayout(lay[3]);
    hlay[1]->addLayout(vlay);
    hlay[1]->addLayout(lay[4]);
    group->setLayout(hlay[1]);
    group->hide();
    //
    mlayout->addWidget(bar);
    mlayout->addLayout(boxlay[2]);
    mlayout->addLayout(laylist);
    mlayout->addWidget(group);
    // settings
    wgt = new QWidget;
    box2lay = new QVBoxLayout;
    setlay = new QVBoxLayout;
    btnlay = new QHBoxLayout;
    edit2 = new QLineEdit;
    slider = new QSlider(Qt::Horizontal);
    btn[0] = new QPushButton("Ок");
    btn[1] = new QPushButton("Отмена");
    connect(btn[1],SIGNAL(clicked()),SLOT(closeset()));
    lbl[0] = new QLabel("Папка для хранения фотографий");
    lbl[1] = new QLabel("Качество сжатия в JPEG");
    box2 = new QGroupBox("При запуске");
    radio[0] = new QRadioButton("показать все даты");
    radio[1] = new QRadioButton("юбилейные на сегодня");
    box2lay->addWidget(radio[0]);
    box2lay->addWidget(radio[1]);
    box2->setLayout(box2lay);
    btnlay->addWidget(btn[0]);
    btnlay->addWidget(btn[1]);
    setlay->addWidget(lbl[0]);
    setlay->addWidget(edit2);
    setlay->addWidget(lbl[1]);
    setlay->addWidget(slider);
    setlay->addWidget(box2);
    setlay->addLayout(btnlay);
    wgt->setLayout(setlay);
    //
    db->getdata();
    for(int i = 0; i != db->count(); i++)
    {
        table->insertRow(i);
        int n;
        if(db->month[i] == "Январь")
            n = 1;
        else if(db->month[i] == "Февраль")
            n = 2;
        else if(db->month[i] == "Март")
            n = 3;
        else if(db->month[i] == "Апрель")
            n = 4;
        else if(db->month[i] == "Май")
            n = 5;
        else if(db->month[i] == "Июнь")
            n = 6;
        else if(db->month[i] == "Июль")
            n = 7;
        else if(db->month[i] == "Август")
            n = 8;
        else if(db->month[i] == "Сентябрь")
            n = 9;
        else if(db->month[i] == "Октябрь")
            n = 10;
        else if(db->month[i] == "Ноябрь")
            n = 11;
        else if(db->month[i] == "Декабрь")
            n = 12;
        table->setItem(i, 0, new QTableWidgetItem(QString::number(db->day[i]) + "." + QString::number(n) + "." + QString::number(db->year[i])));
        table->setItem(i, 1, new QTableWidgetItem(db->sdesc[i]));
        table->setItem(i, 3, new QTableWidgetItem(db->place[i]));
        table->setItem(i, 4, new QTableWidgetItem(db->source[i]));
    }
    table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(menushow(const QPoint&)));
    setLayout(mlayout);
}

void MainInterface::indetail()
{
    if(group->isHidden())
    {
        if(table->currentItem() != 0)
        {
            line->setText(db->extra[table->currentRow()]);
            tedit->setText(db->ldesc[table->currentRow()]);
            combobox[0]->addItem("");
            combobox[1]->addItem("");
            combobox[2]->addItem("");
        }
        group->show();
    }
    else
    {
        group->hide();
    }
}

void MainInterface::themes()
{
    if(list->isHidden() || showlist == 2)
    {
        showlist = 1;
        list->clear();
        list->addItem("");
        for(int i = 0; i != db->count(); i++)
        {
            list->addItem(db->theme[i]);
        }
        mm2[3]->setChecked(true);
        mm2[4]->setChecked(false);
        list->show();
    }
    else
    {
        showlist = 0;
        mm2[3]->setChecked(false);
        list->hide();
    }

}

void MainInterface::places()
{
    if(list->isHidden() || showlist == 1)
    {
        showlist = 2;
        list->clear();
        list->addItem("");
        for(int i = 0; i != db->count(); i++)
        {
            list->addItem(db->place[i]);
        }
        mm2[3]->setChecked(false);
        mm2[4]->setChecked(true);
        list->show();
    }
    else
    {
        showlist = 0;
        mm2[4]->setChecked(false);
        list->hide();
    }
}

void MainInterface::fbig()
{
    QFont fnt;
    fnt.setPointSize(table->font().pointSize()+1);
    table->setFont(fnt);
}

void MainInterface::fsmall()
{
    QFont fnt;
    fnt.setPointSize(table->font().pointSize()-1);
    table->setFont(fnt);
}

bool MainInterface::eventFilter(QObject *obj, QEvent *event)
{
    bool val= QObject::eventFilter(obj, event);
    QMenu *menu = dynamic_cast<QMenu*>(obj);
    if(menu && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick || event->type() == QEvent::MouseButtonRelease))
    {
        QMouseEvent *ev = dynamic_cast<QMouseEvent*>(event);
        if(ev)
        {
            if(ev->button() == Qt::RightButton)
            {
                ev->ignore();
                return true; // yes we filter the event
            }
        }
    }
    return val;
}

void MainInterface::menushow(const QPoint& pos)
{
    QPoint globalPos = table->mapToGlobal(pos);
    contextmenu = new QMenu;
    contextmenu->installEventFilter(this);
    cmm[0] =  new QAction("Карточка", 0);
    connect(cmm[0],SIGNAL(triggered()),SLOT(card()));
    cmm[1] =  new QAction("Показать полный список", 0);
    cmm[2] =  new QAction("Удалить", 0);
    contextmenu->addAction(cmm[0]);
    contextmenu->addAction(cmm[1]);
    contextmenu->addAction(cmm[2]);
    connect(cmm[2],SIGNAL(triggered()),SLOT(del()));
    contextmenu->exec(globalPos);
}

void MainInterface::card()
{
    emit senditem(table->currentItem());
}

void MainInterface::settings()
{
    wgt->setWindowModality(Qt::ApplicationModal);
    wgt->show();
}

void MainInterface::del()
{
    db->del(table->currentRow(),table->item(table->currentRow(),1)->text());
    table->removeRow(table->currentRow());
}

void MainInterface::closeset()
{
    wgt->close();
}

void MainInterface::newevent()
{
    NewEvent *e = new NewEvent(db,this);
    e->show();
}

void MainInterface::edititem(QTableWidgetItem *item)
{
    NewEvent *e = new NewEvent(db,this, item);
    e->show();
}

void MainInterface::set(int day, QString month, int year,QString sdesc, QString place, QString source)
{
    table->insertRow(table->rowCount());
    int n;
    if(month == "Январь")
        n = 1;
    else if(month == "Февраль")
        n = 2;
    else if(month == "Март")
        n = 3;
    else if(month == "Апрель")
        n = 4;
    else if(month == "Май")
        n = 5;
    else if(month == "Июнь")
        n = 6;
    else if(month == "Июль")
        n = 7;
    else if(month == "Август")
        n = 8;
    else if(month == "Сентябрь")
        n = 9;
    else if(month == "Октябрь")
        n = 10;
    else if(month == "Ноябрь")
        n = 11;
    else if(month == "Декабрь")
        n = 12;
    table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(QString::number(day) + "." + QString::number(n) + "." + QString::number(year)));
    table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(sdesc));
    table->setItem(table->rowCount()-1, 3, new QTableWidgetItem(place));
    table->setItem(table->rowCount()-1, 4, new QTableWidgetItem(source));
}

void MainInterface::up(int day, QString month, int year,QString sdesc, QString place, QString source)
{
    int n;
    if(month == "Январь")
        n = 1;
    else if(month == "Февраль")
        n = 2;
    else if(month == "Март")
        n = 3;
    else if(month == "Апрель")
        n = 4;
    else if(month == "Май")
        n = 5;
    else if(month == "Июнь")
        n = 6;
    else if(month == "Июль")
        n = 7;
    else if(month == "Август")
        n = 8;
    else if(month == "Сентябрь")
        n = 9;
    else if(month == "Октябрь")
        n = 10;
    else if(month == "Ноябрь")
        n = 11;
    else if(month == "Декабрь")
        n = 12;
    table->item(table->currentRow(),0)->setText(QString::number(day) + "." + QString::number(n) + "." + QString::number(year));
    table->item(table->currentRow(), 1)->setText(sdesc);
    table->item(table->currentRow(), 3)->setText(place);
    table->item(table->currentRow(), 4)->setText(source);
}
