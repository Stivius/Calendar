#ifndef MODEL_H
#define MODEL_H

#include <QSqlTableModel>

enum EventsModelRoles {
    Id = Qt::UserRole + 1,
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
    QVariant data(const QModelIndex& index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    QString theme(int row) const;
    QString place(int row) const;
    int column(int role) const;
    QString date(int row) const;
    QString shortDescription(int row) const;
    QString source(int row) const;
    int day(int row) const;
    int month(int row) const;
    int year(int row) const;
};

#endif // MODEL_H
