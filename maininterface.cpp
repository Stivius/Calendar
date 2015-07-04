#include "maininterface.h"

MainInterface::MainInterface(QWidget *parent):QWidget(parent)
{
    table = new QTableWidget(10,4);
    table->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );
    table->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Stretch );
    table->horizontalHeader()->setSectionResizeMode( 2, QHeaderView::Stretch );
    table->horizontalHeader()->setSectionResizeMode( 3, QHeaderView::Stretch );
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Дата"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Событие"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Место"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Источник"));
    bar = new QMenuBar;
    mm[0] = new QAction("Настройка", 0);
    connect(mm[0],SIGNAL(triggered()),SLOT(settings()));
    mm[1] = new QAction("Справка", 0);
    menu = new QMenu("Даты");
    mm2[0] = new QAction("Новое событие", 0);
    mm2[1] = new QAction("Импорт из файла", 0);
    mm2[2] = new QAction("Экспорт в файл", 0);
    mm2[3] = new QAction("Тематика", 0);
    mm2[4] = new QAction("Места", 0);
    mm2[5] = new QAction("Выход", 0);
    for(int i = 0; i != 6; i++)
    {
        menu->addAction(mm2[i]);
    }
    menu2 = new QMenu("Вид");
    mm3[0] = new QAction("Шрифт крупнее", 0);
    mm3[1] = new QAction("Шрифт меньше", 0);
    mm3[2] = new QAction("Юбилейные", 0);
    mm3[3] = new QAction("С фотографиями", 0);
    mm3[4] = new QAction("Показать подробнее", 0);
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
    mlayout->addWidget(bar);
    mlayout->addLayout(boxlay[2]);
    mlayout->addWidget(table);
    // settings
    wgt = new QWidget;
    box2lay = new QVBoxLayout;
    setlay = new QVBoxLayout;
    btnlay = new QHBoxLayout;
    edit2 = new QLineEdit;
    slider = new QSlider(Qt::Horizontal);
    btn[0] = new QPushButton("Ок");
    btn[1] = new QPushButton("Отмена");
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
    setLayout(mlayout);
}

void MainInterface::settings()
{
    wgt->setWindowModality(Qt::ApplicationModal);
    wgt->show();
}
