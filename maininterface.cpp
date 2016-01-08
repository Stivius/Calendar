#include "maininterface.h"
#include "newevent.h"
#include "xlsxdocument.h"
#include <QtPrintSupport>

MainInterface::MainInterface(QWidget *parent):QWidget(parent)
{
    db = new EventsModel;
    mlayout = new QVBoxLayout;
    // папки по умолчанию
    QDir dir;
    dir.mkdir(QApplication::applicationDirPath() + "/images");
    dir.mkdir(QApplication::applicationDirPath() + "/temporary");
    // ----------------------
    // создание таблицы
    db->getsettings();
    table = new QTableWidget(0,5);
    QFont fnt(table->font());
    fnt.setPointSize(db->font);
    table->setFont(fnt);
    connect(this,SIGNAL(senditem(QTableWidgetItem*)),SLOT(edititem(QTableWidgetItem*)));
    connect(table,SIGNAL(itemClicked(QTableWidgetItem*)),SLOT(changedetails(QTableWidgetItem*)));
    connect(table,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),SLOT(card(QTableWidgetItem*)));
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i = 0; i != 5; i++)
        table->horizontalHeader()->resizeSection(i,db->headers[i]);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    table->setHorizontalHeaderItem(0, new QTableWidgetItem("Дата"));
    table->setHorizontalHeaderItem(1, new QTableWidgetItem("Событие"));
    table->setHorizontalHeaderItem(2, new QTableWidgetItem("Изображения"));
    table->setHorizontalHeaderItem(3, new QTableWidgetItem("Место"));
    table->setHorizontalHeaderItem(4, new QTableWidgetItem("Источник"));
    // ----------------------
    // горизонтальное меню
    bar = new QMenuBar;
    asettings = new QAction("Настройки", 0);
    connect(asettings,SIGNAL(triggered()),SLOT(settings()));
    areference = new QAction("Справка", 0);
    //connect(areference,SIGNAL(triggered()),SLOT(test()));
    menu = new QMenu("Даты");
    menu->installEventFilter(this);
    anewevent = new QAction("Новое событие", 0);
    connect(anewevent,SIGNAL(triggered()),SLOT(newevent()));
    aimport = new QAction("Импорт из файла", 0);
    connect(aimport,SIGNAL(triggered()),SLOT(importtable()));
    aexport = new QAction("Экспорт в файл", 0);
    connect(aexport,SIGNAL(triggered()),SLOT(exporttable()));
    athemes = new QAction("Тематика", 0);
    athemes->setCheckable(true);
    connect(athemes,SIGNAL(triggered()),SLOT(themes()));
    aplaces = new QAction("Места", 0);
    aplaces->setCheckable(true);
    connect(aplaces,SIGNAL(triggered()),SLOT(places()));
    aexit = new QAction("Выход", 0);
    connect(aexit,SIGNAL(triggered()),SLOT(close()));
    menu->addAction(anewevent);
    menu->addAction(aimport);
    menu->addAction(aexport);
    menu->addAction(athemes);
    menu->addAction(aplaces);
    menu->addAction(aexit);
    //
    menu2 = new QMenu("Вид");
    menu2->installEventFilter(this);
    abigfont = new QAction("Шрифт крупнее", 0);
    connect(abigfont,SIGNAL(triggered()),SLOT(fbig()));
    asmallfont = new QAction("Шрифт меньше", 0);
    connect(asmallfont,SIGNAL(triggered()),SLOT(fsmall()));
    aanniversary = new QAction("Юбилейные", 0);
    aanniversary->setCheckable(true);
    connect(aanniversary,SIGNAL(triggered()),SLOT(filteranniver()));
    aphotos = new QAction("С фотографиями", 0);
    aphotos->setCheckable(true);
    connect(aphotos,SIGNAL(triggered()),SLOT(filterphotos()));
    aindetail = new QAction("Показать подробнее", 0);
    connect(aindetail,SIGNAL(triggered()),SLOT(indetail()));
    menu2->addAction(abigfont);
    menu2->addAction(asmallfont);
    menu2->addAction(aanniversary);
    menu2->addAction(aphotos);
    menu2->addAction(aindetail);
    //
    bar->addMenu(menu);
    bar->addMenu(menu2);
    bar->addAction(asettings);
    bar->addAction(areference);
    // ----------------------
    // Поиск
    search = new QGroupBox("Поиск");
    datesearch = new QGroupBox("Дата");
    msearchlay = new QHBoxLayout;
    datesearchlay = new QHBoxLayout;
    searchlay = new QHBoxLayout;
    anniversary = new QCheckBox("Юбилейные");
    anniversary->setCheckable(true);
    connect(anniversary,SIGNAL(clicked()),SLOT(filteranniver()));
    day = new QComboBox;
    day->setEditable(true);
    day->addItem("любой");
    for(int i = 1; i <= 31; i++)
    {
        day->addItem(QString::number(i));
    }
    connect(day,SIGNAL(activated(int)),this,SLOT(filterday(int)));
    month = new QComboBox;
    month->setEditable(true);
    QStringList lst;
    lst << "любой" << "Январь" << "Февраль" << "Март" << "Апрель" << "Май" << "Июнь" << "Июль"
        << "Август" << "Сентябрь" << "Октябрь" << "Ноябрь" << "Декабрь";
    month->addItems(lst); //
    connect(month,SIGNAL(activated(int)),this,SLOT(filtermonth(int)));
    year = new QComboBox;
    year->setEditable(true);
    year->addItem("любой");
    for(int i = 1000; i <= 2050; i++)
    {
        year->addItem(QString::number(i));
    }
    connect(year,SIGNAL(activated(int)),this,SLOT(filteryear(int)));
    daylbl = new QLabel("День");
    monthlbl = new QLabel("Месяц");
    yearlbl = new QLabel("Год");
    searchinput = new QLineEdit;
    connect(searchinput,SIGNAL(textEdited(QString)),this,SLOT(filtersearch(QString)));
    searchlay->addWidget(searchinput);
    search->setLayout(searchlay);
    datesearchlay->addWidget(anniversary);
    datesearchlay->addWidget(daylbl);
    datesearchlay->addWidget(day);
    datesearchlay->addWidget(monthlbl);
    datesearchlay->addWidget(month);
    datesearchlay->addWidget(yearlbl);
    datesearchlay->addWidget(year);
    datesearch->setLayout(datesearchlay);
    msearchlay->addWidget(search);
    msearchlay->addWidget(datesearch);
    // ----------------------
    // фильтр тематики + места
    showlist = 0;
    for(int i = 0; i != 7; i++)
    {
        filter[i] = 0;
    }
    list = new QListWidget;
    list->setFixedWidth(150);
    list->hide();
    connect(list,SIGNAL(currentRowChanged(int)),this,SLOT(filtertheme(int)));
    connect(list,SIGNAL(currentRowChanged(int)),this,SLOT(filterplace(int)));
    laylist = new QHBoxLayout;
    laylist->addWidget(list);
    laylist->addWidget(table);
    // ----------------------
    // экспорт
    exportwgt = new QWidget;
    texportbox = new QGroupBox("Тип экспорта");
    print = new QRadioButton("На печать");
    browser = new QRadioButton("Формат веб-браузера");
    browser->setChecked(true);
    texportlay = new QVBoxLayout;
    exportlay = new QVBoxLayout;
    exportpathlay = new QHBoxLayout;
    path = new QLineEdit(QApplication::applicationDirPath());
    choose = new QPushButton("Выбрать");
    connect(choose, SIGNAL(clicked()), SLOT(choosepath()));
    submit = new QPushButton("Экспорт");
    connect(submit, SIGNAL(clicked()), SLOT(submitexport()));
    exportpathlay->addWidget(path);
    exportpathlay->addWidget(choose);
    texportlay->addWidget(print);
    texportlay->addWidget(browser);
    texportbox->setLayout(texportlay);
    exportlay->addLayout(exportpathlay);
    exportlay->addWidget(texportbox);
    exportlay->addWidget(submit);
    exportwgt->setLayout(exportlay);
    // ----------------------
    // импорт
    importwgt = new QWidget;
    importlayout = new QVBoxLayout;
    excel = new QPushButton("Загрузить шаблон таблицы Excel");
    connect(excel, SIGNAL(clicked()), SLOT(exceltemplate()));
    importbtn = new QPushButton("Импорт");
    connect(importbtn, SIGNAL(clicked()), SLOT(submitimport()));
    importlayout->addWidget(excel);
    importlayout->addWidget(importbtn);
    importwgt->setLayout(importlayout);
    // ----------------------
    // подробное описание
    datelay = new QVBoxLayout;
    themelay = new QVBoxLayout;
    placelay = new QVBoxLayout;
    extralay = new QVBoxLayout;
    ldesclay = new QVBoxLayout;
    hlay[0] = new QHBoxLayout;
    hlay[1] = new QHBoxLayout;
    vlay = new QVBoxLayout;
    eventgroup = new QGroupBox("Событие");
    datelbl = new QLabel("Дата");
    date = new QLineEdit;
    date->setReadOnly(true);
    datelay->addWidget(datelbl);
    datelay->addWidget(date);
    themelbl = new QLabel("Тематика");
    theme = new QLineEdit;
    theme->setReadOnly(true);
    themelay->addWidget(themelbl);
    themelay->addWidget(theme);
    placelbl = new QLabel("Место");
    place = new QLineEdit;
    place->setReadOnly(true);
    placelay->addWidget(placelbl);
    placelay->addWidget(place);
    extralbl = new QLabel("Дополнительно");
    extra = new QLineEdit;
    extra->setReadOnly(true);
    extralay->addWidget(extralbl);
    extralay->addWidget(extra);
    ldesclbl = new QLabel("Подробное описание");
    ldesc = new QTextEdit;
    ldesc->setReadOnly(true);
    ldesclay->addWidget(ldesclbl);
    ldesclay->addWidget(ldesc);
    hlay[0]->addLayout(datelay);
    hlay[0]->addLayout(themelay);
    vlay->addLayout(hlay[0]);
    vlay->addLayout(placelay);
    vlay->addLayout(extralay);
    hlay[1]->addLayout(vlay);
    hlay[1]->addLayout(ldesclay);
    eventgroup->setLayout(hlay[1]);
    eventgroup->setFixedHeight(200);
    eventgroup->hide();
    // ----------------------
    // настройки
    settingswgt = new QWidget;
    launchlay = new QVBoxLayout;
    setlay = new QVBoxLayout;
    btnlay = new QHBoxLayout;
    pathlay = new QHBoxLayout;
    choosep = new QPushButton("Выбрать");
    connect(choosep,SIGNAL(clicked()),SLOT(choosepath()));
    settingspath = new QLineEdit;
    pathlay->addWidget(settingspath);
    pathlay->addWidget(choosep);
    quality = new QSlider(Qt::Horizontal);
    quality->setRange(0,100);
    ok = new QPushButton("Ок");
    connect(ok,SIGNAL(clicked()),SLOT(upsettings()));
    cancel = new QPushButton("Отмена");
    connect(cancel,SIGNAL(clicked()),SLOT(closeset()));
    lbl[0] = new QLabel("Папка для хранения фотографий");
    lbl[1] = new QLabel("Качество сжатия в JPEG");
    launchbox = new QGroupBox("При запуске");
    alldates = new QRadioButton("показать все даты");
    alldates->setChecked(true);
    anniversarytoday = new QRadioButton("юбилейные на сегодня");
    launchlay->addWidget(alldates);
    launchlay->addWidget(anniversarytoday);
    launchbox->setLayout(launchlay);
    btnlay->addWidget(ok);
    btnlay->addWidget(cancel);
    setlay->addWidget(lbl[0]);
    setlay->addLayout(pathlay);
    setlay->addWidget(lbl[1]);
    setlay->addWidget(quality);
    setlay->addWidget(launchbox);
    setlay->addLayout(btnlay);
    settingswgt->setLayout(setlay);
    // ----------------------
    // заполнение таблицы данными из базы данных
    db->getdata();
    for(int i = 0; i != db->count(); i++)
    {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(((db->day[db->id[i]]<10)?("0" + QString::number(db->day[db->id[i]])):QString::number(db->day[db->id[i]])) + "." + ((db->month[db->id[i]].toInt()<10)?("0" + db->month[db->id[i]]):db->month[db->id[i]]) + "." + ((db->year[db->id[i]]<1000)?("000" + QString::number(db->year[db->id[i]])):QString::number(db->year[db->id[i]]))));
        table->setItem(i, 1, new QTableWidgetItem(db->sdesc[db->id[i]]));
        table->setItem(i, 2, new QTableWidgetItem((db->images[db->id[i]].size() > 0)?"Есть":"Нет"));
        table->setItem(i, 3, new QTableWidgetItem(db->place[db->id[i]]));
        table->setItem(i, 4, new QTableWidgetItem(db->source[db->id[i]]));
    }
    if(db->anniver == 1)
    {
        anniversary->setChecked(true);
        aanniversary->setChecked(true);
        QDate time;
        filter[3] = 1;
        fanniver = time.currentDate().year();
        setfilter();
    }
    // ----------------------
    table->setContextMenuPolicy(Qt::CustomContextMenu); // контекстное меню
    connect(table, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(menushow(const QPoint&)));
    mlayout->addWidget(bar);
    mlayout->addLayout(msearchlay);
    mlayout->addLayout(laylist);
    mlayout->addWidget(eventgroup);
    setLayout(mlayout);
}

MainInterface::~MainInterface()
{
    db->query->exec("CREATE TABLE events_sorted"
            "("
            "ID INTEGER PRIMARY KEY,"
            "day INTEGER,"
            "month VARCHAR(64),"
            "year INTEGER,"
            "theme VARCHAR(500),"
            "sdesc VARCHAR(500),"
            "ldesc VARCHAR(500),"
            "place VARCHAR(500),"
            "source VARCHAR(500),"
            "extra VARCHAR(500),"
            "images VARCHAR(500)"
            ");");
    QString str = "INSERT INTO events_sorted (day,month,year,theme,sdesc,ldesc,place,source,extra,images) SELECT day,month,year,theme,sdesc,ldesc,place,source,extra,images FROM events ORDER BY year,month,day;";
    db->query->exec(str);
    db->query->finish();
    db->query->exec("DROP TABLE events;");
    db->query->exec("ALTER TABLE events_sorted RENAME TO events;");
    QVector<int> vec;
    for(int i = 0; i != 5; i++)
    {
        vec.push_back(table->horizontalHeader()->sectionSize(i));
    }
    db->upheaders(vec[0],vec[1],vec[2],vec[3],vec[4]);
}

void MainInterface::filterday(int index)
{
    if(index == 0)
    {
        filter[0] = 0;
    }
    else
    {
        filter[0] = 1;
        fday = day->itemText(index).toInt();
    }
    setfilter();
}

void MainInterface::filtermonth(int index)
{
    if(index == 0)
    {
        filter[1] = 0;
    }
    else
    {
        filter[1] = 1;
        fmonth = month->itemText(index);
    }
    setfilter();
}

void MainInterface::filteryear(int index)
{
    if(index == 0)
    {
        filter[2] = 0;
    }
    else
    {
        filter[2] = 1;
        fyear = year->itemText(index).toInt();
    }
    setfilter();
}

void MainInterface::filteranniver()
{
    if(filter[3] == 1)
    {
        anniversary->setChecked(false);
        aanniversary->setChecked(false);
        filter[3] = 0;
    }
    else
    {
        anniversary->setChecked(true);
        aanniversary->setChecked(true);
        QDate time;
        filter[3] = 1;
        fanniver = time.currentDate().year();
    }
    setfilter();
}

void MainInterface::filtertheme(int index)
{
    if(showlist == 1)
    {
        if(index == 0)
        {
            filter[4] = 0;
        }
        else
        {
            filter[4] = 1;
            if(index != -1)
            {
                ftheme = list->item(index)->text();
            }
        }
        setfilter();
    }
}

void MainInterface::filterplace(int index)
{
    if(showlist == 2)
    {
        if(index == 0)
        {
            filter[4] = 0;
        }
        else
        {
            filter[4] = 2;
            if(index != -1)
            {
                fplace = list->item(index)->text();
            }
        }
        setfilter();
    }
}

// показать все темы
void MainInterface::themes()
{
    db->getdata();
    QList<QString> lst;
    if(list->isHidden() || showlist == 2)
    {
        showlist = 1;
        list->clear();
        list->addItem("любая");
        list->setCurrentRow(0);
        if(!isFilter())
        {
            for(int i = 0; i != db->count(); i++)
            {
                lst.insert(lst.end(),db->theme[db->id[i]]);
            }
        }
        else
        {
            for(int i = 0; i != db->count(); i++)
            {
                lst.insert(lst.end(),db->theme[db->tempid[i]]);
            }
        }
        QSet<QString> set = lst.toSet();
        lst = set.toList();
        qSort(lst);
        QListIterator<QString> i(lst);
        while(i.hasNext())
        {
            list->addItem(i.next());
        }
        athemes->setChecked(true);
        aplaces->setChecked(false);
        list->show();
    }
    else
    {
        showlist = 0;
        athemes->setChecked(false);
        list->hide();
        filter[4] = 0;
        setfilter();
    }

}

// показать все места
void MainInterface::places()
{
    db->getdata();
    QList<QString> lst;
    if(list->isHidden() || showlist == 1)
    {
        showlist = 2;
        list->clear();
        list->addItem("любое");
        list->setCurrentRow(0);
        if(!isFilter())
        {
            for(int i = 0; i != db->count(); i++)
            {
                lst.insert(lst.end(),db->place[db->id[i]]);
            }
        }
        else
        {
            for(int i = 0; i != db->count(); i++)
            {
                lst.insert(lst.end(),db->place[db->tempid[i]]);
            }
        }
        QSet<QString> set = lst.toSet();
        lst = set.toList();
        qSort(lst);
        QListIterator<QString> i(lst);
        while(i.hasNext())
        {
            list->addItem(i.next());
        }
        athemes->setChecked(false);
        aplaces->setChecked(true);
        list->show();
    }
    else
    {
        showlist = 0;
        aplaces->setChecked(false);
        list->hide();
        filter[4] = 0;
        setfilter();
    }
}

// с фотографиями
void MainInterface::filterphotos()
{
    if(filter[5] == 1)
    {
        filter[5] = 0;
        aphotos->setChecked(false);
    }
    else
    {
        filter[5] = 1;
        aphotos->setChecked(true);
    }
    setfilter();
}

void MainInterface::filtersearch(QString str)
{
    if(str == "")
    {
        filter[6] = 0;
    }
    else
    {
        filter[6] = 1;
    }
    setfilter();
}

bool MainInterface::isFilter()
{
    for(int i = 0; i != 7; i++)
    {
        if(filter[i] == 1)
        {
            return true;
        }
    }
    return false;
}

void MainInterface::setfilter()
{
    db->getdata();
    db->tempid.erase(db->tempid.begin(),db->tempid.end());
    while(table->rowCount() > 0)
    {
        table->removeRow(0);
    }
    bool allowed;
    for(int i = 0, j = 0; i != db->count(); i++)
    {
        allowed = true;
        if(filter[0] == 1)
        {
            if(db->day[db->id[i]] == fday)
            {
                allowed = true;
            }
            else
            {
                allowed = false;
            }
        }
        if(filter[1] == 1 && allowed == true)
        {
            if(db->month[db->id[i]] == fmonth)
            {
                allowed = true;
            }
            else
            {
                allowed = false;
            }
        }
        if(filter[2] == 1 && allowed == true)
        {
            if(db->year[db->id[i]] == fyear)
            {
                allowed = true;
            }
            else
            {
                allowed = false;
            }
        }
        if(filter[3] == 1 && allowed == true)
        {
            int d = db->year[db->id[i]] - fanniver;
            if(d%5 == 0 && d != 0)
            {
                allowed = true;
            }
            else
            {
                allowed = false;
            }
        }
        if(filter[4] == 1 && allowed == true)
        {
            if(db->theme[db->id[i]] == ftheme)
            {
                allowed = true;
            }
            else
            {
                allowed = false;
            }
        }
        if(filter[4] == 2 && allowed == true)
        {
            if(db->place[db->id[i]] == fplace)
            {
                allowed = true;
            }
            else
            {
                allowed = false;
            }
        }
        if(filter[5] == 1 && allowed == true)
        {
            if(db->images[db->id[i]].size() > 0)
            {
                allowed = true;
            }
            else
            {
                allowed = false;
            }
        }
        if(filter[6] == 1 && allowed == true)
        {
            QString str = searchinput->text();
            str = str.toLower();
            QString src = db->sdesc[db->id[i]];
            src = src.toLower();
            bool found = src.contains(str);
            if(found)
            {
                allowed = true;
            }
            else
            {
                allowed = false;
            }
        }
        if(allowed)
        {
            table->insertRow(j);
            table->setItem(j, 0, new QTableWidgetItem(((db->day[db->id[i]]<10)?("0" + QString::number(db->day[db->id[i]])):QString::number(db->day[db->id[i]])) + "." + ((db->month[db->id[i]].toInt()<10)?("0" + db->month[db->id[i]]):db->month[db->id[i]]) + "." + ((db->year[db->id[i]]<1000)?("000" + QString::number(db->year[db->id[i]])):QString::number(db->year[db->id[i]]))));
            table->setItem(j, 1, new QTableWidgetItem(db->sdesc[db->id[i]]));
            table->setItem(j, 2, new QTableWidgetItem((db->images[db->id[i]].size() > 0)?"Есть":"Нет"));
            table->setItem(j, 3, new QTableWidgetItem(db->place[db->id[i]]));
            table->setItem(j, 4, new QTableWidgetItem(db->source[db->id[i]]));
            j++;
            db->tempid.push_back(db->id[i]);
        }
    }
}

// изменить подробное описание
void MainInterface::changedetails(QTableWidgetItem *item)
{
    if(!eventgroup->isHidden())
    {
        if(item != 0)
        {
            db->getdata();
            int id;
            if(!isFilter())
            {
                id = db->id[table->currentRow()];
            }
            else
            {
                id = db->tempid[table->currentRow()];
            }
            extra->setText(db->extra[id]);
            ldesc->setText(db->ldesc[id]);
            QString str(((db->day[id]<10)?("0" + QString::number(db->day[id])):QString::number(db->day[id])) + '.' + ((db->month[id].toInt()<10)?("0" + db->month[id]):db->month[id]) + '.' + ((db->year[id]<1000)?("000" + QString::number(db->year[id])):QString::number(db->year[id])));
            date->setText(str);
            theme->setText(db->theme[id]);
            place->setText(db->place[id]);
        }
    }
}

// обновить настройки
void MainInterface::upsettings()
{
    int anniv;
    if(alldates->isChecked())
    {
        anniv = 0;
    }
    else
    {
        anniv = 1;
    }
    db->upsettings(settingspath->text(), quality->value(), anniv);
    settingswgt->hide();
}

// подробное описание
void MainInterface::indetail()
{
    if(eventgroup->isHidden())
    {
        if(table->currentItem() != 0)
        {
            db->getdata();
            int id;
            if(!isFilter())
            {
                id = db->id[table->currentRow()];
            }
            else
            {
                id = db->tempid[table->currentRow()];
            }
            extra->setText(db->extra[id]);
            ldesc->setText(db->ldesc[id]);
            QString str(((db->day[id]<10)?("0" + QString::number(db->day[id])):QString::number(db->day[id])) + "." + ((db->month[id].toInt()<10)?("0" + db->month[id]):db->month[id]) + "." + ((db->year[id]<1000)?("000" + QString::number(db->year[id])):QString::number(db->year[id])));
            date->setText(str);
            theme->setText(db->theme[id]);
            place->setText(db->place[id]);
        }
        eventgroup->show();
    }
    else
    {
        extra->setText("");
        ldesc->setText("");
        date->setText("");
        theme->setText("");
        place->setText("");
        eventgroup->hide();
    }
}

// установить путь к директории
void MainInterface::setpath()
{
    // модифицировать
    QString p = dirpath->filePath(viewpath->selectionModel()->currentIndex());
    path->setText(p);
    if(!importwgt->isHidden())
    {
        if(p.indexOf(".txt") != -1)
        {
            QFile file(p);
            file.open(QIODevice::ReadWrite);
            QTextStream stream(&file);
            while(!stream.atEnd())
            {
                QString str = stream.readLine();
                qDebug() << str;
                /*QRegularExpression re("([0-9]{1,2})\s([0-9]{1,2})|(апрель)\s([0-9]{2,4})");
                QRegularExpressionMatch match = re.match(str);
                if(match.hasMatch())
                {
                    qDebug() << str;
                }
                else
                {
                    qDebug() << "Не совпадает";
                }*/
            }
            importwgt->hide();
        }
    }
    else if(!settingswgt->isHidden())
    {
        settingspath->setText(p);
        db->path = p;
    }
    wgtpath->hide();
}

// выбор пути
void MainInterface::choosepath()
{
    wgtpath = new QWidget;
    laypath = new QVBoxLayout;
    viewpath = new QTreeView;
    okpath = new QPushButton("Ok");
    connect(okpath,SIGNAL(clicked()),SLOT(setpath()));
    dirpath = new QDirModel;
    dirpath->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    viewpath->setModel(dirpath);
    viewpath->hideColumn(1);
    viewpath->hideColumn(2);
    viewpath->hideColumn(3);
    laypath->addWidget(viewpath);
    laypath->addWidget(okpath);
    wgtpath->setLayout(laypath);
    wgtpath->setWindowModality(Qt::ApplicationModal);
    wgtpath->show();
}

// подтвердить экспорт
void MainInterface::submitexport()
{
    db->getdata();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(("windows-1251")));
    QString str;
    str += "<html>"
           "<meta http-equiv='Content-Type' content='text/html; charset=windows-1251' />"
           "<table border='1' align='center'>"
           "<caption>Таблица из календаря (by stivius)</caption>"
           "<tr>"
           "<th>Изображение</th>"
           "<th>Дата</th>"
           "<th>Событие</th>"
           "</tr>";
    for(int i = 0; i != table->rowCount(); i++)
    {
        QString strF =
               "<tr>"
               "<td><img src='%1'></td>"
               "<td>%2</td>"
               "<td>%3</td>"
               "</tr>";
        if(db->images[i].size() > 0)
        {
            QPixmap pix(QApplication::applicationDirPath() + "/" + db->images[i][db->images[i].size()-1]);
            pix = pix.scaled(150,150,Qt::KeepAspectRatio);
            pix.save(QApplication::applicationDirPath() + "/temporary/temporary" + QString::number(i) + ".png");
        }
        str += strF.arg(QApplication::applicationDirPath() + "/temporary/temporary" + QString::number(i) + ".png").arg(table->item(i,0)->text()).arg(table->item(i,1)->text());
    }
    str += "</table>"
           "</html>";
    QDesktopServices process;
    if(browser->isChecked()) // HTML
    {
        QFile file(path->text() + "/export.htm");
        if(file.exists())
        {
            file.remove();
        }
        file.open(QIODevice::ReadWrite);
        QTextStream stream(&file);
        stream << str;
        process.openUrl(QUrl::fromLocalFile(path->text() + "/export.htm"));
    }
    else // PDF
    {
        QPrinter printer(QPrinter::PrinterResolution);
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(path->text() + "/export.pdf");
        QTextDocument *doc = new QTextDocument;
        doc->setHtml(str);
        doc->print(&printer);
        process.openUrl(QUrl::fromLocalFile(path->text() + "/export.pdf"));
    }
    exportwgt->hide();
}

// показ вииджета экспорт
void MainInterface::exporttable()
{
    exportwgt->setWindowModality(Qt::ApplicationModal);
    exportwgt->show();
}

// создание таблицы в Excel
void MainInterface::exceltemplate()
{
    QXlsx::Document xlsx(QApplication::applicationDirPath()+ "/Import.xlsx");
    xlsx.write("A1","Дата");
    xlsx.write("B1","Событие");
    xlsx.save();
    QDesktopServices process;
    QUrl url(QApplication::applicationDirPath()+ "/Import.xlsx");
    process.openUrl(url);
}

// подтвердить импорт
void MainInterface::submitimport()
{
    QXlsx::Document xlsx(QApplication::applicationDirPath()+ "/Import.xlsx");
    bool loop = true;
    int num = 2;
    db->db.transaction();
    while(loop)
    {
        QString date = xlsx.read("A" + QString::number(num)).toString();
        if(date != "")
        {
            QRegularExpression re("^([0-9]{2,4})\\-([0-9]{1,2})\\-([0-9]{1,2})$");
            QRegularExpression re2("^([0-9]{1,2})\\.([0-9]{1,2})\\.([0-9]{1,4})$");
            QRegularExpressionMatch match = re.match(date);
            QRegularExpressionMatch match2 = re2.match(date);
            if(match.hasMatch() || match2.hasMatch())
            {
                int ind, year, month, day;
                ind = date.indexOf('-');
                year = date.mid(0,ind).toInt();
                date.remove(0,ind+1);
                ind = date.indexOf('-');
                month = date.mid(0,ind).toInt();
                date.remove(0,ind+1);
                ind = date.indexOf('-');
                day = date.mid(0,ind).toInt();
                QString event = xlsx.read("B" + QString::number(num)).toString();
                table->insertRow(table->rowCount());
                table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(((day<10)?("0" + QString::number(day)):QString::number(day)) + '.' + ((month<10)?("0" + QString::number(month)):QString::number(month)) + '.' + ((year<1000)?("000" + QString::number(year)):QString::number(year))));
                table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(event));
                table->setItem(table->rowCount()-1, 2, new QTableWidgetItem("Нет"));
                table->setItem(table->rowCount()-1, 3, new QTableWidgetItem(""));
                table->setItem(table->rowCount()-1, 4, new QTableWidgetItem(""));
                QString string = "INSERT INTO events (day,month,year,theme,sdesc,ldesc,place,source,extra,images) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10')";
                QString query = string.arg(day).arg(QString::number(month)).arg(year).arg("").arg(event).arg("").arg("").arg("").arg("").arg("");
                db->db.exec(query);
            }
            else
            {
                qDebug() << "Неверный формат даты";
            }
        }
        else
        {
            loop = false;
        }
        num++;
    }
    db->db.commit();
    QFile file(QApplication::applicationDirPath()+ "/Import.xlsx");
    file.remove();
    importwgt->hide();
}

// виджет импорта
void MainInterface::importtable()
{
    importwgt->setWindowModality(Qt::ApplicationModal);
    importwgt->show();
}

// увеличить шрифт
void MainInterface::fbig()
{
    QFont fnt;
    db->font += 1;
    fnt.setPointSize(db->font);
    table->setFont(fnt);
    db->upfont(db->font);
}

// уменьшить шрифт
void MainInterface::fsmall()
{
    QFont fnt;
    db->font -= 1;
    fnt.setPointSize(db->font);
    table->setFont(fnt);
    db->upfont(db->font);
}

// запретить нажатие ПКМ в меню
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

// контекстное меню
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

// событие (через меню)
void MainInterface::card()
{
    emit senditem(table->currentItem());
}

// событие (двойной клик)
void MainInterface::card(QTableWidgetItem *item)
{
    NewEvent *e = new NewEvent(db,this,item,table->currentRow());
    e->setWindowModality(Qt::ApplicationModal);
    e->show();
}

// настройки
void MainInterface::settings()
{
    db->getsettings();
    quality->setValue(db->quality);
    settingspath->setText(db->path);
    if(db->anniver == 1)
    {
        alldates->setChecked(false);
        anniversarytoday->setChecked(true);
    }
    else
    {
        alldates->setChecked(true);
        anniversarytoday->setChecked(false);
    }
    settingswgt->setWindowModality(Qt::ApplicationModal);
    settingswgt->show();
}

// удалить событие
void MainInterface::del()
{
    db->getdata();
    if(table->currentItem() != 0)
    {
        int id;
        if(!isFilter())
        {
            id = db->id[table->currentRow()];
        }
        else
        {
            id = db->tempid[table->currentRow()];
        }
        db->del(id);
        table->removeRow(table->currentRow());
    }
}

// закрыть настройки
void MainInterface::closeset()
{
    settingswgt->close();
}

// новое событие
void MainInterface::newevent()
{
    NewEvent *e = new NewEvent(db,this);
    e->setWindowModality(Qt::ApplicationModal);
    e->show();
}

// редактировать событие
void MainInterface::edititem(QTableWidgetItem *item)
{
    if(item != 0)
    {
        NewEvent *e = new NewEvent(db,this, item, table->currentRow());
        e->setWindowModality(Qt::ApplicationModal);
        e->show();
    }
}

// вставить новое событие в таблицу
void MainInterface::set(QString day, QString month, QString year,QString sdesc, QString place, QString source, QString photos)
{
    if(day == "Неизвестно")
    {
        day = "0";
    }
    if(year == "Неизвестно")
    {
        year = "0";
    }
    db->getdata();
    table->insertRow(table->rowCount());
    int n = db->getmonth(month);
    table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(((day.toInt()<10)?("0" + day):day) + "." + ((n<10)?("0" + QString::number(n)):QString::number(n)) + "." + ((year.toInt()<1000)?("000" + year):year)));
    table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(sdesc));
    table->setItem(table->rowCount()-1, 2, new QTableWidgetItem(photos));
    table->setItem(table->rowCount()-1, 3, new QTableWidgetItem(place));
    table->setItem(table->rowCount()-1, 4, new QTableWidgetItem(source));
}

// обновить событие в таблице
void MainInterface::up(QString day, QString month, QString year,QString sdesc, QString place, QString source, QString photos)
{
    if(day == "Неизвестно")
    {
        day = "0";
    }
    if(year == "Неизвестно")
    {
        year = "0";
    }
    int n = db->getmonth(month);
    table->item(table->currentRow(), 0)->setText(((day.toInt()<10)?("0" + day):day) + "." + ((n<10)?("0" + QString::number(n)):QString::number(n)) + "." + ((year.toInt()<1000)?("000" + year):year));
    table->item(table->currentRow(), 1)->setText(sdesc);
    table->item(table->currentRow(), 2)->setText(photos);
    table->item(table->currentRow(), 3)->setText(place);
    table->item(table->currentRow(), 4)->setText(source);
}
