#include "newevent.h"
#include "maininterface.h"

NewEvent::NewEvent(EventsModel *model, MainInterface *in, QTableWidgetItem *it, int row, QWidget *parent):QWidget(parent)
{
    db = model;
    db->getsettings();
    db->getdata();
    inter = in;
    item = it;
    now = 0;
    row_ = row;
    mlayout = new QVBoxLayout;
    for(int i = 0; i != 6; i++)
    {
        vlay[i] = new QVBoxLayout;
        hlay[i] = new QHBoxLayout;
    }
    // выбор пути для фотографий
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
    // ----------------------
    // дата и тематика
    lbl[0] = new QLabel("Дата события");
    day = new QComboBox;
    day->setEditable(true);
    day->addItem("Неизвестно");
    for(int i = 1; i <= 31; i++)
    {
        day->addItem(QString::number(i));
    }
    QStringList lst;
    lst << "Неизвестно" << "Январь" << "Февраль" << "Март" << "Апрель" << "Май" << "Июнь" << "Июль"
        << "Август" << "Сентябрь" << "Октябрь" << "Ноябрь" << "Декабрь";
    month = new QComboBox;
    month->addItems(lst); //
    month->setEditable(true);
    year = new QComboBox;
    year->setEditable(true);
    year->addItem("Неизвестно");
    for(int i = 1000; i <= 2050; i++)
    {
        year->addItem(QString::number(i));
    }
    lbl[1] = new QLabel("г.");
    hlay[0]->addWidget(day);
    hlay[0]->addWidget(month);
    hlay[0]->addWidget(year);
    hlay[0]->addWidget(lbl[1]);
    vlay[0]->addWidget(lbl[0]);
    vlay[0]->addLayout(hlay[0]);
    lbl[2] = new QLabel("Тематика");
    theme = new QComboBox;
    theme->setEditable(true);
    vlay[1]->addWidget(lbl[2]);
    vlay[1]->addWidget(theme);
    hlay[1]->addLayout(vlay[0]);
    hlay[1]->addLayout(vlay[1]);
    // ----------------------
    // краткое и полное описание
    lbl[3] = new QLabel("Краткое описание события");
    sdesc = new QLineEdit;
    lbl[4] = new QLabel("Полное описание события");
    ldesc = new QTextEdit;
    vlay[2]->addWidget(lbl[3]);
    vlay[2]->addWidget(sdesc);
    vlay[2]->addWidget(lbl[4]);
    vlay[2]->addWidget(ldesc);
    // ----------------------
    // место для доп. полей
    lbl[5] = new QLabel("Место события");
    place = new QComboBox;
    place->setEditable(true);
    lbl[6] = new QLabel("Источник");
    source = new QComboBox;
    source->setEditable(true);
    lbl[7] = new QLabel("Дополнительно");
    extra = new QLineEdit;
    vlay[3]->addWidget(lbl[5]);
    vlay[3]->addWidget(place);
    vlay[3]->addWidget(lbl[6]);
    vlay[3]->addWidget(source);
    vlay[3]->addWidget(lbl[7]);
    vlay[3]->addWidget(extra);
    // ----------------------
    // место для изображений
    lbl[8] = new QLabel("Фотография");
    fullphoto = new QPushButton;
    fullphoto->setFlat(true);
    pix = new QPixmap(150,150);
    pix->fill(Qt::transparent);
    QIcon icn(*pix);
    fullphoto->setIconSize(QSize(150,150));
    fullphoto->setIcon(icn);
    connect(fullphoto,SIGNAL(clicked()),SLOT(openfull()));
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
    hlay[3]->addWidget(fullphoto);
    vlay[5]->addWidget(lbl[8]);
    vlay[5]->addLayout(hlay[3]);
    // ----------------------
    hlay[4]->addLayout(vlay[3]);
    hlay[4]->addLayout(vlay[5]);
    // закрытие окна
    btn2[0] = new QPushButton("Ок");
    connect(btn2[0],SIGNAL(clicked()),SLOT(save()));
    btn2[1] = new QPushButton("Отмена");
    connect(btn2[1],SIGNAL(clicked()),SLOT(close()));
    hlay[5]->addWidget(btn2[0]);
    hlay[5]->addWidget(btn2[1]);
    // ----------------------
    QList<QString> tlst,tlst2,tlst3;
    for(int i = 0; i != db->count(); i++)
    {
        tlst.insert(tlst.end(),db->theme[db->id[i]]);
        tlst2.insert(tlst2.end(),db->place[db->id[i]]);
        tlst3.insert(tlst3.end(),db->source[db->id[i]]);
    }
    QSet<QString> set = tlst.toSet();
    QSet<QString> set2 = tlst2.toSet();
    QSet<QString> set3 = tlst3.toSet();
    tlst = set.toList();
    tlst2 = set2.toList();
    tlst3 = set3.toList();
    qSort(tlst);
    qSort(tlst2);
    qSort(tlst3);
    // редактирование события
    cimg = db->imgcount();
    if(item != 0)
    {
        int idd;
        if(!inter->isFilter())
        {
            idd = db->id[inter->table->currentRow()];
        }
        else
        {
            idd = db->tempid[inter->table->currentRow()];
        }
        int c = 0, ind = 0, ind2 = 0, ind3 = 0;
        QListIterator<QString> i(tlst);
        while(i.hasNext())
        {
            QString str = i.next();
            if(str == db->theme[idd])
                ind = c;
            theme->addItem(str);
            c++;
        }
        c = 0;
        QListIterator<QString> i2(tlst2);
        while(i2.hasNext())
        {
            QString str = i2.next();
            if(str == db->place[idd])
                ind2 = c;
            place->addItem(str);
            c++;
        }
        c = 0;
        QListIterator<QString> i3(tlst3);
        while(i3.hasNext())
        {
            QString str = i3.next();
            if(str == db->source[idd])
                ind3 = c;
            source->addItem(str);
            c++;
        }
        for(int i = 0; i != db->images[idd].size(); i++)
        {
            pix->load(db->path + "/" + db->images[idd][i]);
            vec.push_back(pix->scaled(150,150,Qt::KeepAspectRatio));
            QString str = db->images[idd][i] + "\n";
            img += str;
        }
        if(vec.size() > 0)
        {
            now = vec.size() - 1;
            QIcon icn(vec[now]);
            fullphoto->setIconSize(QSize(150,150));
            fullphoto->setIcon(icn);
        }
        cimg = db->imgcount();
        month->setCurrentIndex(month->findText(db->getmonthname(db->month[idd].toInt())));
        if(db->day[idd] != 0)
        {
            day->setCurrentIndex(day->findText(QString::number(db->day[idd])));
        }
        else
        {
            day->setCurrentIndex(0);
        }
        if(db->year[idd] != 0)
        {
            year->setCurrentIndex(year->findText(QString::number(db->year[idd])));
        }
        else
        {
            year->setCurrentIndex(0);
        }
        theme->setCurrentIndex(ind);
        sdesc->setText(db->sdesc[idd]);
        ldesc->setText(db->ldesc[idd]);
        place->setCurrentIndex(ind2);
        source->setCurrentIndex(ind3);
        extra->setText(db->extra[idd]);
        sdesc2 = db->sdesc[idd];
    }
    else
    {
        QListIterator<QString> i(tlst);
        theme->addItem("");
        while(i.hasNext())
        {
            QString str = i.next();
            theme->addItem(str);
        }
        QListIterator<QString> i2(tlst2);
        place->addItem("");
        while(i2.hasNext())
        {
            QString str = i2.next();
            place->addItem(str);
        }
        QListIterator<QString> i3(tlst3);
        source->addItem("");
        while(i3.hasNext())
        {
            QString str = i3.next();
            source->addItem(str);
        }
    }
    // ----------------------
    mlayout->addLayout(hlay[1]);
    mlayout->addLayout(vlay[2]);
    mlayout->addLayout(hlay[4]);
    mlayout->addLayout(hlay[5]);
    setLayout(mlayout);
    this->setFixedSize(650,525);
}

void NewEvent::uploadphoto()
{
    wgt->setWindowModality(Qt::ApplicationModal);
    wgt->show();
}

// удаление фотографий
void NewEvent::removephoto()
{
    int idd;
    if(!inter->isFilter())
    {
        idd = db->id[inter->table->currentRow()];
    }
    else
    {
        idd = db->tempid[inter->table->currentRow()];
    }
    if(now >= 0 && now < vec.size()-1)
    {
        vec.remove(now);
        if(now <= db->images[idd].size()-1) // с начала или с середины
        {
            img.remove(db->images[idd][now] + "\n");
            removed.push_back(db->path + "/" + db->images[idd][now]);
            db->images[idd].remove(now);
        }
        else
        {
            int n = now-db->images[idd].size();
            img.remove(uploaded[n] + "\n");
            uploadedtemp.remove(n);
            uploaded.remove(n);
            temppath.remove(n);
        }
        QIcon icn(vec[now]);
        fullphoto->setIconSize(QSize(150,150));
        fullphoto->setIcon(icn);
    }
    else if(now == vec.size()-1 && vec.size() > 1) // с конца
    {
        vec.remove(now);
        if(now <= db->images[idd].size()-1)
        {
            img.remove(db->images[idd][now] + "\n");
            removed.push_back(db->path + "/" + db->images[idd][now]);
            db->images[idd].remove(now);
        }
        else
        {
            int n = now-db->images[idd].size();
            img.remove(uploaded[n] + "\n");
            uploadedtemp.remove(n);
            uploaded.remove(n);
            temppath.remove(n);
        }
        now--;
        QIcon icn(vec[now]);
        fullphoto->setIconSize(QSize(150,150));
        fullphoto->setIcon(icn);
    }
    else if(vec.size() == 1) // только одно фото
    {
        vec.remove(now);
        if(now <= db->images[idd].size()-1)
        {
            img.remove(db->images[idd][now] + "\n");
            removed.push_back(db->path + "/" + db->images[idd][now]);
            db->images[idd].remove(now);
        }
        else
        {
            int n = now-db->images[idd].size();
            img.remove(uploaded[n] + "\n");
            uploadedtemp.remove(n);
            uploaded.remove(n);
            temppath.remove(n);
        }
        pix = new QPixmap(150,150);
        pix->fill(Qt::transparent);
        QIcon icn(*pix);
        fullphoto->setIconSize(QSize(150,150));
        fullphoto->setIcon(icn);
    }
}

void NewEvent::openfull()
{
    int idd;
    if(!inter->isFilter())
    {
        idd = db->id[inter->table->currentRow()];
    }
    else
    {
        idd = db->tempid[inter->table->currentRow()];
    }
    if(vec.size() > 0 && now <= (db->images[idd].size()-1))
    {
        QUrl url(db->path + "/" + db->images[idd][now]);
        QDesktopServices process;
        process.openUrl(url);
    }
    else if(vec.size() > 0 && now > (db->images[idd].size()-1))
    {
        int n = now-db->images[idd].size();
        QUrl url(temppath[n]);
        QDesktopServices process;
        process.openUrl(url);
    }
}

// листаем фото вперед
void NewEvent::next()
{
    if(now < vec.size()-1)
    {
        now++;
        QIcon icn(vec[now]);
        fullphoto->setIcon(icn);
    }
}

// листаем фото назад
void NewEvent::prev()
{
    if(now > 0)
    {
        now--;
        QIcon icn(vec[now]);
        fullphoto->setIcon(icn);
    }
}

// загружаем фото
void NewEvent::upload()
{
    QString str = dir->fileName(view->selectionModel()->currentIndex());
    str = str.toLower();
    if(str.indexOf(".png") == -1 && str.indexOf(".jpg") == -1 && str.indexOf(".jpeg") == -1) // только .PNG или .JPG/.JPEG
    {
        qDebug() << "Not found";
    }
    else
    {
        pix->load(dir->filePath(view->selectionModel()->currentIndex()));
        QIcon icn(pix->scaled(150,150,Qt::KeepAspectRatio));
        fullphoto->setIcon(icn);
        vec.push_back(pix->scaled(150,150,Qt::KeepAspectRatio));
        now = vec.size()-1;
        QFile file;
        int n;
        for(int i = 0; i != 100; i++) // вычисляем порядковый номер изоображения
        {
            QString str(db->path + "/" + "image" + QString::number(i) + ".jpg");
            QString str2("image" + QString::number(i) + ".jpg");
            if(!file.exists(str))
            {
                int c = 0;
                for(int j = 0; j != uploaded.size(); j++)
                {
                    if(str2 == uploaded[j])
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
        QString str("image" + QString::number(n) + ".jpg");
        n++;
        // временные векторы с изображениями
        temppath.push_back(dir->filePath(view->selectionModel()->currentIndex()));
        uploaded.push_back(str);
        uploadedtemp.push_back(*pix);
        // ----------------------
        QString strF(str + "\n");
        img += strF;
        wgt->hide();
        cimg++;
    }
}

void NewEvent::save()
{
    if(item != 0) // обновление события
    {
        int id;
        if(!inter->isFilter())
        {
            id = db->id[inter->table->currentRow()];
        }
        else
        {
            id = db->tempid[inter->table->currentRow()];
        }
        db->update(day->currentText(),month->currentText(),year->currentText(),theme->currentText(),sdesc->text(),ldesc->toPlainText(),place->currentText(),source->currentText(),extra->text(),img,id);
        inter->up(day->currentText(),month->currentText(),year->currentText(),sdesc->text(),place->currentText(),source->currentText(),(uploaded.size() > 0)?"Есть":"Нет");
    }
    else // создание события
    {
        db->save(day->currentText(),month->currentText(),year->currentText(),theme->currentText(),sdesc->text(),ldesc->toPlainText(),place->currentText(),source->currentText(),extra->text(),img);
        inter->set(day->currentText(),month->currentText(),year->currentText(),sdesc->text(),place->currentText(),source->currentText(),(uploaded.size() > 0)?"Есть":"Нет");
    }
    for(int i = 0; i != uploaded.size(); i++) // сохранение изображений в указанной папке
    {
        uploadedtemp[i].save(db->path + "/" + uploaded[i], "JPG", db->quality);
    }
    for(int i = 0; i != removed.size(); i++) // удаление указанных изображений
    {
        QFile file(removed[i]);
        file.remove();
    }
    this->close();
}
