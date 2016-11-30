#include "subproductsmodel.h"
#include <QSqlQuery>
#include <QDebug>
#include "productsdata.h"

SubProductsModel::SubProductsModel(QObject *parent)
    :ProductsModel(parent)
{

}

QVariant SubProductsModel::data(const QModelIndex &item, int role) const {
    if(role == Qt::DisplayRole) {
        if(item.column() == SUBPROD_AMOUNT) {
            QVariant subProdID = ProductsModel::data(index(item.row(), SUBPROD_ID), role);

            QSqlQuery arrival;
            arrival.prepare("SELECT * from " + QString(SUBPROD_ARRIVAL_TABLE) +
                          " WHERE subprod_id=:subprod_id");
            arrival.bindValue(":subprod_id", subProdID);
            if(!arrival.exec()) {
                qDebug() << tr("Error retrieving amount of subproduct. Amount will be set to 0");
            }

            int amount = 0;
            while(arrival.next()) amount += arrival.value(SUBPROD_HISTORY_AMOUNT).toInt();

            QSqlQuery reduce;
            reduce.prepare("SELECT * from " + QString(SUBPROD_REDUCE_TABLE) +
                          " WHERE subprod_id=:subprod_id");
            reduce.bindValue(":subprod_id", subProdID);
            if(!reduce.exec()) {
                qDebug() << tr("Error retrieving amount of subproduct. Amount will be set to 0");
            }

            while(reduce.next()) amount -= reduce.value(SUBPROD_HISTORY_AMOUNT).toInt();

            return QVariant(amount);
        }
    }
    return ProductsModel::data(item, role);
}

int SubProductsModel::columnCount(const QModelIndex &parent) const {
    return ProductsModel::columnCount(parent) + 1;
}
