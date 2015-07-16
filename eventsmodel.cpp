#include "eventsmodel.h"

EventsModel::EventsModel()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "connection");
    db.setDatabaseName("db");
    db.setHostName("local");
    db.setUserName("user");
    db.setPassword("123");
    qDebug() << db.open();
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
    size = 0;
    img = 0;
}

void EventsModel::save(int day, QString month, int year, QString theme, QString sdesc, QString ldesc, QString place, QString source, QString extra, QString img)
{
    QString string = "INSERT INTO events (day,month,year,theme,sdesc,ldesc,place,source,extra,images) VALUES ('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10')";
    QString query = string.arg(day).arg(month).arg(year).arg(theme).arg(sdesc).arg(ldesc).arg(place).arg(source).arg(extra).arg(img);
    db.exec(query);
}

void EventsModel::update(int day, QString month, int year, QString theme, QString sdesc, QString ldesc, QString place, QString source, QString extra, QString img, QString sdesc2)
{
    QString string = "UPDATE events SET day='%1',month='%2',year='%3',theme='%4',sdesc='%5',ldesc='%6',place='%7',source='%8',extra='%9',images='%10' WHERE sdesc='%11'";
    QString query = string.arg(day).arg(month).arg(year).arg(theme).arg(sdesc).arg(ldesc).arg(place).arg(source).arg(extra).arg(img).arg(sdesc2);
    db.exec(query);
}

void EventsModel::del(int row, QString str)
{
    // сделать SELECT, на основе SELECT'a заполнить удалить
    QFile file;
    for(int i = 0; i != images[row].size(); i++)
    {
        file.remove(images[row][i]);
    }
    QString string = "DELETE FROM events WHERE sdesc='%1'";
    QString query = string.arg(str);
    db.exec(query);
}

void EventsModel::getdata()
{
    query = new QSqlQuery(db);
    query->exec("SELECT * FROM events");
    rec = query->record();
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

int EventsModel::count()
{
    return size;
}

int EventsModel::imgcount()
{
    return img;
}
