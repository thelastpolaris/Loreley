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
    void selectSubProds();

    /**
     * @param subProductID - id of subProduct
     * @param column - e.g SUBPROD_ID
     * @return row number of subProduct with id subProductID
     */
    int rowByID(int subProductID, int column);
private:
    QString historyTable;
};

#endif // SUBPRODUCTSMODEL_H
