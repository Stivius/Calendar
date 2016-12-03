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
    QString theme(int row);
    QString place(int row);
    int column(int role);
};

#endif // MODEL_H
