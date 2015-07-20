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
    dir.mkdir("images");
    dir.mkdir("temporary");
    // ----------------------
    // создание таблицы
    table = new QTableWidget(0,5);
    connect(this,SIGNAL(senditem(QTableWidgetItem*)),SLOT(edititem(QTableWidgetItem*)));
    connect(table,SIGNAL(itemClicked(QTableWidgetItem*)),SLOT(changedetails(QTableWidgetItem*)));
    connect(table,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),SLOT(card(QTableWidgetItem*)));
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
    // ----------------------
    // горизонтальное меню
    bar = new QMenuBar;
    asettings = new QAction("Настройки", 0);
    connect(asettings,SIGNAL(triggered()),SLOT(settings()));
    areference = new QAction("Справка", 0);
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
    aphotos = new QAction("С фотографиями", 0);
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
    selected = new QCheckBox("В выбранном");
    anniversary = new QCheckBox("Юбилейные");
    day = new QComboBox;
    month = new QComboBox;
    year = new QComboBox;
    daylbl = new QLabel("День");
    monthlbl = new QLabel("Месяц");
    yearlbl = new QLabel("Год");
    searchinput = new QLineEdit;
    searchlay->addWidget(searchinput);
    searchlay->addWidget(selected);
    search->setLayout(searchlay);
    datesearchlay->addWidget(anniversary);
    datesearchlay->addWidget(daylbl);
    datesearchlay->addWidget(day);
    datesearchlay->addWidget(monthlbl);
    datesearchlay->addWidget(month);
    datesearchlay->addWidget(yearlbl);
    datesearchlay->addWidget(month);
    datesearch->setLayout(datesearchlay);
    msearchlay->addWidget(search);
    msearchlay->addWidget(datesearch);
    // ----------------------
    // фильтр тематики + места
    showlist = 0;
    list = new QListWidget;
    list->setFixedWidth(150);
    list->hide();
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
    importgroup = new QGroupBox("Тип импорта");
    textfile = new QRadioButton("Из текстового файла");
    xlsxfile = new QRadioButton("Из таблицы Excel");
    xlsxfile->setChecked(true);
    importlayout[0] = new QVBoxLayout;
    importlayout[1] = new QVBoxLayout;
    excel = new QPushButton("Загрузить шаблон таблицы Excel");
    connect(excel, SIGNAL(clicked()), SLOT(exceltemplate()));
    importbtn = new QPushButton("Импорт");
    connect(importbtn, SIGNAL(clicked()), SLOT(submitimport()));
    importlayout[0]->addWidget(textfile);
    importlayout[0]->addWidget(xlsxfile);
    importgroup->setLayout(importlayout[0]);
    importlayout[1]->addWidget(importgroup);
    importlayout[1]->addWidget(excel);
    importlayout[1]->addWidget(importbtn);
    importwgt->setLayout(importlayout[1]);
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
        int n = db->getmonth(i);
        table->setItem(i, 0, new QTableWidgetItem(QString::number(db->day[i]) + "." + QString::number(n) + "." + QString::number(db->year[i])));
        table->setItem(i, 1, new QTableWidgetItem(db->sdesc[i]));
        table->setItem(i, 3, new QTableWidgetItem(db->place[i]));
        table->setItem(i, 4, new QTableWidgetItem(db->source[i]));
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

// изменить подробное описание
void MainInterface::changedetails(QTableWidgetItem *item)
{
    if(!eventgroup->isHidden())
    {
        if(item != 0)
        {
            db->getdata();
            extra->setText(db->extra[item->row()]);
            ldesc->setText(db->ldesc[item->row()]);
            int n = db->getmonth(item->row());
            QString str(QString::number(db->day[item->row()]) + "." + QString::number(n) + "." + QString::number(db->year[item->row()]));
            date->setText(str);
            theme->setText(db->theme[item->row()]);
            place->setText(db->place[item->row()]);
        }
    }
}

// обновить настройки
void MainInterface::upsettings()
{
    int flag;
    if(textfile->isChecked())
    {
        flag = 1;
    }
    else
    {
        flag = 0;
    }
    db->upsettings(settingspath->text(), quality->value(), flag);
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
            extra->setText(db->extra[table->currentRow()]);
            ldesc->setText(db->ldesc[table->currentRow()]);
            int n = db->getmonth(table->currentRow());
            QString str(QString::number(db->day[table->currentRow()]) + "." + QString::number(n) + "." + QString::number(db->year[table->currentRow()]));
            date->setText(str);
            theme->setText(db->theme[table->currentRow()]);
            place->setText(db->place[table->currentRow()]);
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
    exportwgt->show();
}

// создание таблицы в Excel
void MainInterface::exceltemplate()
{
    QXlsx::Document xlsx("Import.xlsx");
    xlsx.write("A1","Дата");
    xlsx.write("B1","Событие");
    xlsx.save();
    QDesktopServices process;
    process.openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath()+ "/Import.xlsx"));
}

// подтвердить импорт
void MainInterface::submitimport()
{
    // модифицировать
    QXlsx::Document xlsx("Import.xlsx");
    if(textfile->isChecked()) // текстовый файл
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
        wgtpath->show();
    }
    else // из Excel
    {
        bool loop = true;
        int num = 2;
        while(loop)
        {
            QString date = xlsx.read("A" + QString::number(num)).toString();
            if(date != "")
            {
                QRegularExpression re("^([0-9]{1,2})\\.([0-9]{1,2})\\.([0-9]{2,4})$");
                QRegularExpressionMatch match = re.match(date);
                if(match.hasMatch())
                {
                    QString event = xlsx.read("B" + QString::number(num)).toString();
                    table->insertRow(table->rowCount());
                    table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(date));
                    table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(event));
                    //db->update(day,month,year,"",event,"","","","","",id); через captured
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
        QFile file("Import.xlsx");
        file.remove();
        importwgt->hide();
    }
}

// виджет импорта
void MainInterface::importtable()
{
    importwgt->show();
}

// показать все темы
void MainInterface::themes()
{
    if(list->isHidden() || showlist == 2)
    {
        showlist = 1;
        list->clear();
        for(int i = 0; i != db->count(); i++)
        {
            list->addItem(db->theme[i]);
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
    }

}

// показать все места
void MainInterface::places()
{
    if(list->isHidden() || showlist == 1)
    {
        showlist = 2;
        list->clear();
        for(int i = 0; i != db->count(); i++)
        {
            list->addItem(db->place[i]);
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
    }
}

// увеличить шрифт
void MainInterface::fbig()
{
    QFont fnt;
    fnt.setPointSize(table->font().pointSize()+1);
    table->setFont(fnt);
}

// уменьшить шрифт
void MainInterface::fsmall()
{
    QFont fnt;
    fnt.setPointSize(table->font().pointSize()-1);
    table->setFont(fnt);
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
    NewEvent *e = new NewEvent(db,this,item);
    e->show();
}

// настройки
void MainInterface::settings()
{
    //wgt->setWindowModality(Qt::ApplicationModal);
    db->getsettings();
    quality->setValue(db->quality);
    settingspath->setText(db->path);
    settingswgt->show();
}

// удалить событие
void MainInterface::del()
{
    if(table->currentItem() != 0)
    {
        db->del(table->currentRow(),table->item(table->currentRow(),1)->text());
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
    e->show();
}

// редактировать событие
void MainInterface::edititem(QTableWidgetItem *item)
{
    if(item != 0)
    {
        NewEvent *e = new NewEvent(db,this, item);
        e->show();
    }
}

// вставить новое событие в таблицу
void MainInterface::set(int day, QString month, int year,QString sdesc, QString place, QString source)
{
    table->insertRow(table->rowCount());
    int n = db->getmonth(month);
    table->setItem(table->rowCount()-1, 0, new QTableWidgetItem(QString::number(day) + "." + QString::number(n) + "." + QString::number(year)));
    table->setItem(table->rowCount()-1, 1, new QTableWidgetItem(sdesc));
    table->setItem(table->rowCount()-1, 3, new QTableWidgetItem(place));
    table->setItem(table->rowCount()-1, 4, new QTableWidgetItem(source));
}

// обновить событие в таблице
void MainInterface::up(int day, QString month, int year,QString sdesc, QString place, QString source)
{
    int n = db->getmonth(month);
    table->item(table->currentRow(),0)->setText(QString::number(day) + "." + QString::number(n) + "." + QString::number(year));
    table->item(table->currentRow(), 1)->setText(sdesc);
    table->item(table->currentRow(), 3)->setText(place);
    table->item(table->currentRow(), 4)->setText(source);
}
