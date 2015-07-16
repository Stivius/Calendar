#include "newevent.h"
#include "maininterface.h"

NewEvent::NewEvent(EventsModel *model, MainInterface *in, QTableWidgetItem *it, QWidget *parent):QWidget(parent)
{
    db = model;
    db->getdata();
    inter = in;
    item = it;
    now = 0;
    mlayout = new QVBoxLayout;
    for(int i = 0; i != 6; i++)
    {
        vlay[i] = new QVBoxLayout;
        hlay[i] = new QHBoxLayout;
    }
    //
    wgt = new QWidget;
    lay = new QVBoxLayout;
    view = new QTreeView;
    ok = new QPushButton("Ok");
    connect(ok,SIGNAL(clicked()),SLOT(upload()));
    dir = new QDirModel;
    dir->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    view->setModel(dir);
    view->hideColumn(1);
    view->hideColumn(2);
    view->hideColumn(3);
    lay->addWidget(view);
    lay->addWidget(ok);
    wgt->setLayout(lay);
    //
    lbl[0] = new QLabel("Дата события");
    day = new QSpinBox;
    day->setRange(1,31); //
    QStringList lst;
    lst << "Январь" << "Февраль" << "Март" << "Апрель" << "Май" << "Июнь" << "Июль"
        << "Август" << "Сентябрь" << "Октябрь" << "Ноябрь" << "Декабрь";
    month = new QComboBox;
    month->addItems(lst); //
    month->setEditable(true);
    year = new QSpinBox;
    year->setRange(0,9999);
    year->setValue(2015); //
    lbl[1] = new QLabel("г.");
    hlay[0]->addWidget(day);
    hlay[0]->addWidget(month);
    hlay[0]->addWidget(year);
    hlay[0]->addWidget(lbl[1]);
    vlay[0]->addWidget(lbl[0]);
    vlay[0]->addLayout(hlay[0]);
    lbl[2] = new QLabel("Тематика");
    theme = new QComboBox;
    theme->addItem("");
    for(int i = 0; i != db->count(); i++)
    {
        theme->addItem(db->theme[i]);
    }
    theme->setEditable(true);
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
    place->addItem("");
    for(int i = 0; i != db->count(); i++)
    {
        place->addItem(db->place[i]);
    }
    place->setEditable(true);
    lbl[6] = new QLabel("Источник");
    source = new QComboBox;
    source->addItem("");
    for(int i = 0; i != db->count(); i++)
    {
        source->addItem(db->source[i]);
    }
    source->setEditable(true);
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
    pix = new QPixmap(150,150);
    pix->fill(Qt::transparent);
    lbl[9]->setPixmap(*pix);
    btn[0] = new QPushButton("+");
    btn[0]->setFixedSize(25,25);
    connect(btn[0],SIGNAL(clicked()),SLOT(uploadphoto()));
    btn[1] = new QPushButton("-");
    btn[1]->setFixedSize(25,25);
    connect(btn[1],SIGNAL(clicked()),SLOT(removephoto()));
    btn[2] = new QPushButton("->");
    btn[2]->setFixedSize(25,25);
    connect(btn[2],SIGNAL(clicked()),SLOT(next()));
    btn[3] = new QPushButton("<-");
    btn[3]->setFixedSize(25,25);
    connect(btn[3],SIGNAL(clicked()),SLOT(prev()));
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
    btn2[0] = new QPushButton("Ок");
    connect(btn2[0],SIGNAL(clicked()),SLOT(save()));
    btn2[1] = new QPushButton("Отмена");
    connect(btn2[1],SIGNAL(clicked()),SLOT(close()));
    hlay[5]->addWidget(btn2[0]);
    hlay[5]->addWidget(btn2[1]);
    //
    cimg = db->imgcount();
    if(item != 0)
    {
        for(int i = 0; i != db->images[inter->table->currentRow()].size(); i++)
        {
            pix->load(db->images[inter->table->currentRow()][i]);
            vec.push_back(pix->scaled(150,150,Qt::KeepAspectRatio));
            QString str = db->images[inter->table->currentRow()][i] + "\n";
            img += str;
        }
        if(vec.size() > 0)
        {
            now = vec.size() - 1;
            lbl[9]->setPixmap(vec[now]);
        }
        cimg = db->imgcount();
        int n;
        if(db->month[item->row()] == "Январь")
            n = 1;
        else if(db->month[item->row()] == "Февраль")
            n = 2;
        else if(db->month[item->row()] == "Март")
            n = 3;
        else if(db->month[item->row()] == "Апрель")
            n = 4;
        else if(db->month[item->row()] == "Май")
            n = 5;
        else if(db->month[item->row()] == "Июнь")
            n = 6;
        else if(db->month[item->row()] == "Июль")
            n = 7;
        else if(db->month[item->row()] == "Август")
            n = 8;
        else if(db->month[item->row()] == "Сентябрь")
            n = 9;
        else if(db->month[item->row()] == "Октябрь")
            n = 10;
        else if(db->month[item->row()] == "Ноябрь")
            n = 11;
        else if(db->month[item->row()] == "Декабрь")
            n = 12;
        day->setValue(db->day[item->row()]);
        month->setCurrentIndex(n-1);
        year->setValue(db->year[item->row()]);
        theme->addItem(db->theme[item->row()]);
        sdesc->setText(db->sdesc[item->row()]);
        ldesc->setText(db->ldesc[item->row()]);
        place->addItem(db->place[item->row()]);
        source->addItem(db->source[item->row()]);
        extra->setText(db->extra[item->row()]);
        sdesc2 = db->sdesc[item->row()];
    }
    //
    mlayout->addLayout(hlay[1]);
    mlayout->addLayout(vlay[2]);
    mlayout->addLayout(hlay[4]);
    mlayout->addLayout(hlay[5]);
    setLayout(mlayout);
}

void NewEvent::uploadphoto()
{
    wgt->show();
}

void NewEvent::removephoto()
{
    if(now >= 0 && now < vec.size()-1)
    {
        vec.remove(now);
        if(now <= db->images[inter->table->currentRow()].size()-1)
        {
            img.remove(db->images[inter->table->currentRow()][now] + "\n");
            removed.push_back(db->images[inter->table->currentRow()][now]);
        }
        else
        {
            int n = now-db->images[inter->table->currentRow()].size();
            img.remove(uploaded[n] + "\n");
            uploadedtemp.remove(n);
            uploaded.remove(n);
        }
        lbl[9]->setPixmap(vec[now]);
    }
    else if(now == vec.size()-1 && vec.size() > 1)
    {
        vec.remove(now);
        if(now <= db->images[inter->table->currentRow()].size()-1)
        {
            img.remove(db->images[inter->table->currentRow()][now] + "\n");
            removed.push_back(db->images[inter->table->currentRow()][now]);
        }
        else
        {
            int n = now-db->images[inter->table->currentRow()].size();
            img.remove(uploaded[n] + "\n");
            uploadedtemp.remove(n);
            uploaded.remove(n);
        }
        now--;
        lbl[9]->setPixmap(vec[now]);
    }
    else if(vec.size() == 1)
    {
        vec.remove(now);
        if(now <= db->images[inter->table->currentRow()].size()-1)
        {
            img.remove(db->images[inter->table->currentRow()][now] + "\n");
            removed.push_back(db->images[inter->table->currentRow()][now]);
        }
        else
        {
            int n = now-db->images[inter->table->currentRow()].size();
            img.remove(uploaded[n] + "\n");
            uploadedtemp.remove(n);
            uploaded.remove(n);
        }
        pix = new QPixmap(150,150);
        pix->fill(Qt::transparent);
        lbl[9]->setPixmap(*pix);
    }
}

void NewEvent::next()
{
    if(now < vec.size()-1)
    {
        now++;
        lbl[9]->setPixmap(vec[now]);
    }
}

void NewEvent::prev()
{
    if(now > 0)
    {
        now--;
        lbl[9]->setPixmap(vec[now]);
    }
}

void NewEvent::upload()
{
    QString str = dir->fileName(view->selectionModel()->currentIndex());
    if(str.indexOf(".png") == -1 && str.indexOf(".jpg") == -1)
    {
        qDebug() << "Not found";
    }
    else
    {
        pix->load(dir->filePath(view->selectionModel()->currentIndex()));
        lbl[9]->setPixmap(pix->scaled(150,150,Qt::KeepAspectRatio));
        vec.push_back(pix->scaled(150,150,Qt::KeepAspectRatio));
        now = vec.size()-1;
        QFile file;
        int n;
        for(int i = 0; i != 100; i++)
        {
            QString str("image" + QString::number(i) + ".png");
            if(!file.exists(str))
            {
                int c = 0;
                for(int j = 0; j != uploaded.size(); j++)
                {
                    if(str == uploaded[j])
                    {
                        c++;
                    }
                }
                if(c == 0)
                {
                    n = i;
                    break;
                }
            }
        }
        QString str("image" + QString::number(n) + ".png");
        n++;
        uploaded.push_back(str);
        uploadedtemp.push_back(*pix);
        QString strF(str + "\n");
        img += strF;
        wgt->hide();
        cimg++;
    }
}

void NewEvent::save()
{
    if(item != 0)
    {
        db->update(day->value(),month->currentText(),year->value(),theme->currentText(),sdesc->text(),ldesc->toPlainText(),place->currentText(),source->currentText(),extra->text(),img,sdesc2);
        inter->up(day->value(),month->currentText(),year->value(),sdesc->text(),place->currentText(),source->currentText());
    }
    else
    {
        db->save(day->value(),month->currentText(),year->value(),theme->currentText(),sdesc->text(),ldesc->toPlainText(),place->currentText(),source->currentText(),extra->text(),img);
        inter->set(day->value(),month->currentText(),year->value(),sdesc->text(),place->currentText(),source->currentText());
    }
    for(int i = 0; i != uploaded.size(); i++)
    {
        uploadedtemp[i].save(uploaded[i]);
    }
    for(int i = 0; i != removed.size(); i++)
    {
        QFile file(removed[i]);
        file.remove();
    }
    this->close();
}
