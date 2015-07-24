#include "eventsmodel.h"
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
            "show INTEGER"
            ");");
    QString string = "INSERT INTO settings (path,quality,show) VALUES ('%1','%2','%3')";
    QString query = string.arg(QApplication::applicationDirPath() + "/images").arg(50).arg(1);
    db.exec(query);
    size = 0;
    img = 0;
}

// новое событие
void EventsModel::save(int day, QString month, int year, QString theme, QString sdesc, QString ldesc, QString place, QString source, QString extra, QString img)
{
    QString string = "INSERT INTO events (day,month,year,theme,sdesc,ldesc,place,source,extra,images) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10')";
    QString query = string.arg(day).arg(month).arg(year).arg(theme).arg(sdesc).arg(ldesc).arg(place).arg(source).arg(extra).arg(img);
    db.exec(query);
}

// обновить событие
void EventsModel::update(int day, QString month, int year, QString theme, QString sdesc, QString ldesc, QString place, QString source, QString extra, QString img, int row)
{
    QString string = "UPDATE events SET day='%1',month='%2',year='%3',theme='%4',sdesc='%5',ldesc='%6',place='%7',source='%8',extra='%9',images='%10' WHERE id='%11'";
    QString query = string.arg(day).arg(month).arg(year).arg(theme).arg(sdesc).arg(ldesc).arg(place).arg(source).arg(extra).arg(img).arg(id[row]);
    db.exec(query);
}

// обновить настройки
void EventsModel::upsettings(QString path, int quality, int show)
{
    QString string = "UPDATE settings SET path='%1',quality='%2',show='%3'";
    QString query = string.arg(path).arg(quality).arg(show);
    db.exec(query);
}

// удалить событие
void EventsModel::del(int row)
{
    getsettings();
    int n = getimages(row);
    QFile file;
    for(int i = 0; i != n; i++)
    {
        file.remove(path + "/" + images[row][i]);
    }
    QString string = "DELETE FROM events WHERE id='%1'";
    QString query = string.arg(id[row]);
    id.remove(row);
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
        show = query->value(rec.indexOf("show")).toInt();
    }
}

// получить изображения
int EventsModel::getimages(int row)
{
    query = new QSqlQuery(db);
    query->exec("SELECT * FROM events");
    rec = query->record();
    for(int i = 0; i != 100; i++)
    {
        images[i].erase(images[i].begin(),images[i].end());
    }
    int temp = 0;
    while(query->next())
    {
        QString str = query->value(rec.indexOf("images")).toString();
        int count = str.count(QChar('\n'));
        for(int i = 0; i != count; i++)
        {
            int n = str.indexOf(QChar('\n'));
            QString str2 = str.mid(0,n);
            str.remove(0,n+1);
            images[temp].push_back(str2);
        }
        temp++;
    }
    return images[row].size();
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
    return n;
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
    return n;
}

// получить все данные
void EventsModel::getdata()
{
    query = new QSqlQuery(db);
    query->exec("SELECT * FROM events");
    rec = query->record();
    id.erase(id.begin(),id.end());
    day.erase(day.begin(),day.end());
    month.erase(month.begin(),month.end());
    year.erase(year.begin(),year.end());
    theme.erase(theme.begin(),theme.end());
    sdesc.erase(sdesc.begin(),sdesc.end());
    ldesc.erase(ldesc.begin(),ldesc.end());
    place.erase(place.begin(),place.end());
    source.erase(source.begin(),source.end());
    extra.erase(extra.begin(),extra.end());
    for(int i = 0; i != 100; i++)
    {
        images[i].erase(images[i].begin(),images[i].end());
    }
    size = 0;
    img = 0;
    while(query->next())
    {
        id.push_back(size);
        day.push_back(query->value(rec.indexOf("day")).toInt());
        month.push_back(query->value(rec.indexOf("month")).toString());
        year.push_back(query->value(rec.indexOf("year")).toInt());
        theme.push_back(query->value(rec.indexOf("theme")).toString());
        sdesc.push_back(query->value(rec.indexOf("sdesc")).toString());
        ldesc.push_back(query->value(rec.indexOf("ldesc")).toString());
        place.push_back(query->value(rec.indexOf("place")).toString());
        source.push_back(query->value(rec.indexOf("source")).toString());
        extra.push_back(query->value(rec.indexOf("extra")).toString());
        QString str = query->value(rec.indexOf("images")).toString();
        int count = str.count(QChar('\n'));
        for(int i = 0; i != count; i++)
        {
            int n = str.indexOf(QChar('\n'));
            QString str2 = str.mid(0,n);
            str.remove(0,n+1);
            images[size].push_back(str2);
            img++;
        }
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
