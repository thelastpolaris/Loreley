#ifndef SUBPRODUCTSMODEL_H
#define SUBPRODUCTSMODEL_H

#include "productsmodel.h"

class SubProductsModel : public ProductsModel
{
    Q_OBJECT
public:
    SubProductsModel(QObject *parent = 0);
    virtual ~SubProductsModel() { }
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief selectSubProds - select subproducts and set their amount according to shopping cart
     * @param idsInCart - key - subproduct id, value - its amount in shopping cart
     */
    void selectSubProds(QHash<int, int> idsInCart);
private:
    QHash<int, int> idsInCart;
    QString historyTable;
};

#endif // SUBPRODUCTSMODEL_H