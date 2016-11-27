#ifndef SUBPRODUCTSMODEL_H
#define SUBPRODUCTSMODEL_H

#include "productsmodel.h"

class SubProductsModel : public ProductsModel
{
    Q_OBJECT
public:
    SubProductsModel(QString _historyTable, int _amountField, int _subProdIDField,
                     int _amountHistoryField, QObject *parent = 0);
    virtual ~SubProductsModel() { }
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
private:
    QString historyTable;
    int amountField;
    int subProdIDField;
    int amountHistoryField;
};

#endif // SUBPRODUCTSMODEL_H
