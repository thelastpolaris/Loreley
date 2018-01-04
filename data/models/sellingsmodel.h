#ifndef SELLINGSMODEL_H
#define SELLINGSMODEL_H

#include <QSqlTableModel>

#define SELLING_ID 0
#define SELLING_TIME 1
#define SELLING_PRICE 2
#define SELLING_CLIENT 3
#define SELLING_GLOBAL_DISCOUNT 4

class SellingsModel : public QSqlTableModel
{
public:
    SellingsModel(QObject* parent = nullptr);
    void setDate(QDate date);
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    int getTotalPrice() const;
};

#endif // SELLINGSMODEL_H
