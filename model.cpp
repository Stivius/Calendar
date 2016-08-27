#include "model.h"

// инициализация база данных
Model::Model()
{
    // load Database
    db = new Database;
    db->getSettings(path, quality, anniver, font, headers);
    // separate EventModel and SettingsModel
    size = db->getData(id, days, months, years, themes, sDescriptions, lDescriptions, places, sources, extra, images);
}

Model::~Model()
{
    delete db;
}

QString Model::getPath()
{
    return path;
}

int Model::getQuality()
{
    return quality;
}

int Model::getFont()
{
    return font;
}

int Model::getHeader(int number)
{
    QString header = headers.split(QChar('\n'), QString::SkipEmptyParts).at(number);
    return header.toInt();
}

int Model::getDay(int index)
{
    return days[index];
}

int Model::getMonth(int index)
{
    return months[index];
}

int Model::getMonth(const QString& monthName)
{
    int n = 0;
    if(monthName == "Январь")
        n = 1;
    else if(monthName == "Февраль")
        n = 2;
    else if(monthName == "Март")
        n = 3;
    else if(monthName == "Апрель")
        n = 4;
    else if(monthName == "Май")
        n = 5;
    else if(monthName == "Июнь")
        n = 6;
    else if(monthName == "Июль")
        n = 7;
    else if(monthName == "Август")
        n = 8;
    else if(monthName == "Сентябрь")
        n = 9;
    else if(monthName == "Октябрь")
        n = 10;
    else if(monthName == "Ноябрь")
        n = 11;
    else if(monthName == "Декабрь")
        n = 12;
    else if(monthName == "Неизвестно")
        n = 0;
    return n;
}

QString Model::getMonthName(int n)
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

int Model::getYear(int index)
{
    return years[index];
}

QString Model::getExtra(int index)
{
    return extra[index];
}

QString Model::getSDescrpition(int index)
{
    return sDescriptions[index];
}

QString Model::getLDescrpition(int index)
{
    return lDescriptions[index];
}

QString Model::getDate(int index)
{
    return formatDate(days[index], months[index], years[index]);
}

QString Model::getTheme(int index)
{
    return themes[index];
}

QString Model::getPlace(int index)
{
    return places[index];
}

QString Model::getSource(int index)
{
    return sources[index];
}

QString Model::getImages(int index)
{
    return images[index];
}

void Model::removeEvent(int index)
{
    QStringList imagesList = images[index].split(QChar('\n'),QString::SkipEmptyParts);
    for(QString name: imagesList)
    {
        QFile file(path + "/" + name);
        if(file.exists())
            file.remove();
    }
    db->removeEvent(id[index]);
    id.remove(index);
    days.remove(index);
    months.remove(index);
    years.remove(index);
    themes.remove(index);
    sDescriptions.remove(index);
    lDescriptions.remove(index);
    places.remove(index);
    sources.remove(index);
    extra.remove(index);
    images.remove(index);
    --size;
}

// новое событие
void Model::insertEvent(QVector<QString>& data)
{
    int newID = db->insertEvent(data);
    id.push_back(newID);
    days.push_back(data[0].toInt());
    months.push_back(data[1].toInt()); // to digit
    years.push_back(data[2].toInt());
    themes.push_back(data[3]);
    sDescriptions.push_back(data[4]);
    lDescriptions.push_back(data[5]);
    places.push_back(data[6]);
    sources.push_back(data[7]);
    extra.push_back(data[8]);
    images.push_back(data[9]);
    ++size;
}

// обновить событие
void Model::updateEvent(int index, QVector<QString>& data)
{
    db->updateEvent(id[index], data);
    days[index] = data[0].toInt();
    months[index] = data[1].toInt(); // to digit
    years[index] = data[2].toInt();
    themes[index] = data[3];
    sDescriptions[index] = data[4];
    lDescriptions[index] = data[5];
    places[index] = data[6];
    sources[index] = data[7];
    extra[index] = data[8];
    images[index] = data[9];
}

void Model::updateFont(int _font)
{
    font = _font;
    db->updateFont(font);
}

void Model::updateHeaders(const QVector<int>& headers)
{
    QString sHeaders;
    for(int header: headers)
        sHeaders += QString::number(header) + "\n";
    db->updateHeaders(sHeaders);
}

void Model::updateSettings(const QString& _path, int _quality, int _anniver)
{
    path = _path;
    quality = _quality;
    anniver = _anniver;
    db->updateSettings(path, quality, anniver);
}

QString Model::formatDate(int day, int month, int year)
{
    QString sDay = formatDay(day);
    QString sMonth = formatMonth(month);
    QString sYear = formatYear(year);
    return sDay + "." + sMonth + "." + sYear;
}

QString Model::formatDay(int day)
{
    if(day < 10)
        return "0" + QString::number(day);
    else
        return QString::number(day);
}

QString Model::formatMonth(int month)
{
    if(month < 10)
        return "0" + QString::number(month);
    else
        return QString::number(month);
}

QString Model::formatYear(int year)
{
    if(year <= 0 || year > 9999)
        return "0000";
    return QString::number(year);
}

// кол-во событий
int Model::count()
{
    return size;
}

int Model::imagesCount(int index)
{
    return getImages(index).split(QChar('\n'),QString::SkipEmptyParts).size();
}

bool Model::startMultiInsertion()
{
    return db->startTransaction();
}

bool Model::finishMultiInsertion()
{
    return db->finishTransaction();
}
