#include "eventsmodel.h"
#include <qglobal.h>
#include <QApplication>

// инициализация база данных
EventsModel::EventsModel()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "connection");
    db.setDatabaseName("db");
    db.setHostName("local");
    db.setUserName("user");
    db.setPassword("123");
    db.open();
    db.exec("CREATE TABLE events"
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
    db.exec("CREATE TABLE settings"
            "("
            "path VARCHAR(500),"
            "quality INTEGER,"
            "font INTEGER,"
            "anniver INTEGER"
            ");");
    query = new QSqlQuery(db);
    query->exec("SELECT * FROM settings");
    if(!query->first())
    {
        QString string = "INSERT INTO settings (path,quality,font,anniver) VALUES ('%1','%2','%3','%4')";
        QString query = string.arg(QApplication::applicationDirPath() + "/images").arg(50).arg(11).arg(0);
        db.exec(query);
    }
    network = new QNetworkAccessManager;
    QUrl url("ftp://ftp.tech-century.com/version.txt");
    url.setPassword("123456");
    url.setUserName("stivius@tech-century.com");
    QNetworkRequest req(url);
    rep = network->get(req);
    connect(rep,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(erorCode(QNetworkReply::NetworkError)));
    connect(rep,SIGNAL(finished()),this,SLOT(checkVersion()));
    size = 0;
    img = 0;
    version = "1.0.1";
}

void EventsModel::erorCode(QNetworkReply::NetworkError error)
{
    qDebug() << "Error code: " <<  error;
}

void EventsModel::checkVersion()
{
    if(rep->bytesAvailable() != 0)
    {
        QString string = rep->readAll();
        if(version != string)
        {
            int in = QMessageBox::information(0,"Обновление","Доступна более свежая версия данной программы. Установить обновление?",QMessageBox::Ok,QMessageBox::Discard);
            if(in == QMessageBox::Ok)
            {
                QUrl url("ftp://ftp.tech-century.com/updater.exe");
                url.setPassword("123456");
                url.setUserName("stivius@tech-century.com");
                QNetworkRequest req(url);
                rep2 = network->get(req);
                bar = new QProgressBar;
                bar->setAlignment(Qt::AlignHCenter);
                bar->setRange(0,rep2->size());
                bar->show();
                connect(rep2,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(showProgress(quint64,quint64)));
                connect(rep2,SIGNAL(finished()),this,SLOT(downloadUpgrader()));
            }
        }
    }
}

void EventsModel::showProgress(qint64 received, qint64 total)
{
    total += 0;
    bar->setValue(received);
}

void EventsModel::downloadUpgrader()
{
    if(rep2->bytesAvailable() != 0)
    {
        QFile file(QApplication::applicationDirPath() + "/updater.exe");
        file.open(QIODevice::ReadWrite);
        file.write(rep2->readAll());
        file.close();
        QProcess upd;
        QString string = QApplication::applicationDirPath() + "/updater.exe";
        upd.startDetached(string,QStringList());
        QApplication::quit();
    }
}

// новое событие
void EventsModel::save(QString day, QString month, QString year, QString theme, QString sdesc, QString ldesc, QString place, QString source, QString extra, QString img)
{
    if(day == "Неизвестно")
    {
        day = "0";
    }
    if(year == "Неизвестно")
    {
        year = "0";
    }
    QString string = "INSERT INTO events (day,month,year,theme,sdesc,ldesc,place,source,extra,images) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10')";
    QString query = string.arg(day).arg(month).arg(year).arg(theme).arg(sdesc).arg(ldesc).arg(place).arg(source).arg(extra).arg(img);
    db.exec(query);
}

// обновить событие
void EventsModel::update(QString day, QString month, QString year, QString theme, QString sdesc, QString ldesc, QString place, QString source, QString extra, QString img, int id)
{
    if(day == "Неизвестно")
    {
        day = "0";
    }
    if(year == "Неизвестно")
    {
        year = "0";
    }
    QString string = "UPDATE events SET day='%1',month='%2',year='%3',theme='%4',sdesc='%5',ldesc='%6',place='%7',source='%8',extra='%9',images='%10' WHERE ID='%11'";
    QString query = string.arg(day).arg(month).arg(year).arg(theme).arg(sdesc).arg(ldesc).arg(place).arg(source).arg(extra).arg(img).arg(id);
    db.exec(query);
}

void EventsModel::upfont(int font)
{
    QString string = "UPDATE settings SET font='%1'";
    QString query = string.arg(font);
    db.exec(query);
}

// обновить настройки
void EventsModel::upsettings(QString path, int quality, int anniver)
{
    QString string = "UPDATE settings SET path='%1',quality='%2',anniver='%3'";
    QString query = string.arg(path).arg(quality).arg(anniver);
    db.exec(query);
}

// удалить событие
void EventsModel::del(int idd)
{
    QFile file;
    for(int i = 0; i != images[idd].size(); i++)
    {
        file.remove(path + "/" + images[idd][i]);
    }
    QString string = "DELETE FROM events WHERE ID='%1'";
    QString query = string.arg(idd);
    db.exec(query);
}

// получить настройки
void EventsModel::getsettings()
{
    query = new QSqlQuery(db);
    query->exec("SELECT * FROM settings");
    rec = query->record();
    while(query->next())
    {
        path = query->value(rec.indexOf("path")).toString();
        quality = query->value(rec.indexOf("quality")).toInt();
        anniver = query->value(rec.indexOf("anniver")).toInt();
        font = query->value(rec.indexOf("font")).toInt();
    }
}

// узнаем месяц
int EventsModel::getmonth(int i)
{
    int n;
    if(month[i] == "Январь")
        n = 1;
    else if(month[i] == "Февраль")
        n = 2;
    else if(month[i] == "Март")
        n = 3;
    else if(month[i] == "Апрель")
        n = 4;
    else if(month[i] == "Май")
        n = 5;
    else if(month[i] == "Июнь")
        n = 6;
    else if(month[i] == "Июль")
        n = 7;
    else if(month[i] == "Август")
        n = 8;
    else if(month[i] == "Сентябрь")
        n = 9;
    else if(month[i] == "Октябрь")
        n = 10;
    else if(month[i] == "Ноябрь")
        n = 11;
    else if(month[i] == "Декабрь")
        n = 12;
    else if(month[i] == "Неизвестно")
        n = 0;
    return n;
}

QString EventsModel::getmonthname(int n)
{
    QString str;
    if(n == 1)
        str = "Январь";
    else if(n == 2)
        str = "Февраль";
    else if(n == 3)
        str = "Март";
    else if(n == 4)
        str = "Апрель";
    else if(n == 5)
        str = "Май";
    else if(n == 6)
        str = "Июнь";
    else if(n == 7)
        str = "Июль";
    else if(n == 8)
        str = "Август";
    else if(n == 9)
        str = "Сентябрь";
    else if(n == 10)
        str = "Октябрь";
    else if(n == 11)
        str = "Ноябрь";
    else if(n == 12)
        str = "Декабрь";
    else if(n == 0)
        str = "Неизвестно";
    return str;
}

// узнаем месяц (перегруженная функция)
int EventsModel::getmonth(QString str)
{
    int n;
    if(str == "Январь")
        n = 1;
    else if(str == "Февраль")
        n = 2;
    else if(str == "Март")
        n = 3;
    else if(str == "Апрель")
        n = 4;
    else if(str == "Май")
        n = 5;
    else if(str == "Июнь")
        n = 6;
    else if(str == "Июль")
        n = 7;
    else if(str == "Август")
        n = 8;
    else if(str == "Сентябрь")
        n = 9;
    else if(str == "Октябрь")
        n = 10;
    else if(str == "Ноябрь")
        n = 11;
    else if(str == "Декабрь")
        n = 12;
    else if(str == "Неизвестно")
        n = 0;
    return n;
}

// получить все данные
void EventsModel::getdata()
{
    query = new QSqlQuery(db);
    query->exec("SELECT * FROM events");
    rec = query->record();
    id.erase(id.begin(),id.end());
    day.clear();
    month.clear();
    year.clear();
    theme.clear();
    sdesc.clear();
    ldesc.clear();
    place.clear();
    source.clear();
    extra.clear();
    images.clear();
    size = 0;
    img = 0;
    while(query->next())
    {
        id.push_back(query->value(rec.indexOf("ID")).toInt());
        int i = id[id.size()-1];
        day[i] = query->value(rec.indexOf("day")).toInt();
        month[i] = query->value(rec.indexOf("month")).toString();
        year[i] = query->value(rec.indexOf("year")).toInt();
        theme[i] = query->value(rec.indexOf("theme")).toString();
        sdesc[i] = query->value(rec.indexOf("sdesc")).toString();
        ldesc[i] = query->value(rec.indexOf("ldesc")).toString();
        place[i] = query->value(rec.indexOf("place")).toString();
        source[i] = query->value(rec.indexOf("source")).toString();
        extra[i] = query->value(rec.indexOf("extra")).toString();
        QString str = query->value(rec.indexOf("images")).toString();
        int count = str.count(QChar('\n'));
        QVector<QString> vec;
        for(int i = 0; i != count; i++)
        {
            int n = str.indexOf(QChar('\n'));
            QString str2 = str.mid(0,n);
            str.remove(0,n+1);
            vec.push_back(str2);
            img++;
        }
        images[i] = vec;
        size++;
    }
}

// кол-во событий
int EventsModel::count()
{
    return size;
}

// кол-во изображений
int EventsModel::imgcount()
{
    return img;
}
