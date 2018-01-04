#include "subproductsmodel.h"
#include <QSqlQuery>
#include <QDebug>
#include "../productsdata.h"

SubProductsModel::SubProductsModel(QObject *parent)
    :ProductsModel(parent)
{

}

QVariant SubProductsModel::data(const QModelIndex &item, int role) const {
    if(role == Qt::DisplayRole) {
        if(item.column() == SUBPROD_AMOUNT) {
            QVariant subProdID = ProductsModel::data(index(item.row(), SUBPROD_ID), role);

            int amount = ProductsData::Instance()->getAmountOfSubProd(subProdID.toInt());
            return QVariant(amount);
        }
    }
    return ProductsModel::data(item, role);
}

int SubProductsModel::columnCount(const QModelIndex &parent) const {
    return ProductsModel::columnCount(parent) + 1;
}

int SubProductsModel::rowByID(int subProductID, int column) {
    return match(index(0, column), Qt::DisplayRole, subProductID)[0].row();
}
