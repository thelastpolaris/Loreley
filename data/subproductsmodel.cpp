#include "subproductsmodel.h"
#include <QSqlQuery>
#include <QDebug>

SubProductsModel::SubProductsModel(QString _historyTable, int _amountField, int _subProdIDField,
                                   int _amountHistoryField, QObject *parent)
    :ProductsModel(parent), historyTable(_historyTable), amountField(_amountField),
      subProdIDField(_subProdIDField), amountHistoryField(_amountHistoryField)
{

}

QVariant SubProductsModel::data(const QModelIndex &item, int role) const {
    if(role == Qt::DisplayRole) {
        if(item.column() == amountField) {
            QVariant subProdID = ProductsModel::data(index(item.row(), subProdIDField), role);

            QSqlQuery query;
            query.prepare("SELECT * from subprod_history WHERE subprod_id=:subprod_id");
            query.bindValue(":subprod_id", subProdID);
            if(!query.exec()) {
                qDebug() << tr("Error retrieving amount of subproduct. Amount will be set to 0");
            }

            int amount = 0;
            while(query.next()) amount += query.value(amountHistoryField).toInt();

            return QVariant(amount);
        }
    }
    return ProductsModel::data(item, role);
}

int SubProductsModel::columnCount(const QModelIndex &parent) const {
    return ProductsModel::columnCount(parent) + 1;
}
