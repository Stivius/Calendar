#ifndef MODEL_H
#define MODEL_H

#include <QSqlTableModel>

enum EventsModelRoles {
    Id = Qt::UserRole,
    Date,
    ShortDescription,
    Images,
    Place,
    Source,
    Theme,
    LongDescription,
    ExtraDescription
};

class EventsSqlModel: public QSqlTableModel
{
    Q_OBJECT
public:
    EventsSqlModel(QSqlDatabase database, QObject* parent = nullptr);
    ~EventsSqlModel();
    QVariant data(const QModelIndex& index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    void insertEvent(const QString& date, const QString& eventDescription);
    int column(int role) const;
    QString theme(int row) const;
    QString place(int row) const;
    QString source(int row) const;
    QString shortDescription(int row) const;
    QStringList imagesList(int row) const;
    void setImagesList(int row, QStringList imagesList);
    int day(int row) const;
    int month(int row) const;
    int year(int row) const;
    QString date(int row) const;
    bool setDate(int row, int day, int month, int year);

public slots:
    void updateHeadersData();

private:
    QString getFormat(int day, int month, int year, const QString &format) const;

};

#endif // MODEL_H
