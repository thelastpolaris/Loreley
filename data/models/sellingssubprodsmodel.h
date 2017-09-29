#ifndef SELLINGSSUBPRODSMODEL_H
#define SELLINGSSUBPRODSMODEL_H

#include <QSqlQueryModel>

#define SELLSUB_PRODNAME 0
#define SELLSUB_CATEGORIE 1
#define SELLSUB_SIZE 2
#define SELLSUB_PRICE 3
#define SELLSUB_DISCOUNT 4
#define SELLSUB_COLOR 5

class SellingsSubProdsModel : public QSqlQueryModel
{
public:
    SellingsSubProdsModel(QObject* parent = nullptr);
    QVariant data(const QModelIndex &item, int role) const;
    void setSellingID(int id);

private:
    int sellingID;
};

#endif // SELLINGSSUBPRODSMODEL_H
