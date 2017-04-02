#include "sellingssubprodsmodel.h"
#include <QSqlError>
#include <QDebug>

SellingsSubProdsModel::SellingsSubProdsModel(QObject* parent)
    :QSqlQueryModel(parent)
{
    setSellingID(-1);
    setHeaderData(SELLSUB_PRODNAME, Qt::Horizontal, tr("Product's name"));
    setHeaderData(SELLSUB_CATEGORIE, Qt::Horizontal, tr("Category"));
    setHeaderData(SELLSUB_SIZE, Qt::Horizontal, tr("Size"));
    setHeaderData(SELLSUB_PRICE, Qt::Horizontal, tr("Price"));
    setHeaderData(SELLSUB_DISCOUNT, Qt::Horizontal, tr("Discount"));
    setHeaderData(SELLSUB_COLOR, Qt::Horizontal, tr("Color"));
}

void SellingsSubProdsModel::setSellingID(int id)
{
    QString query = QString("SELECT products.name, categories.name, sizes.name, products.price, sells_subproducts.discount, colors.name "
                            "FROM sells_subproducts "
                            "LEFT JOIN subproducts ON subproducts.id = sells_subproducts.subproduct "
                            "LEFT JOIN sizes ON sizes.id = subproducts.size "
                            "LEFT JOIN products ON products.id = subproducts.product_id "
                            "LEFT JOIN categories ON categories.id = products.category_id "
                            "LEFT JOIN colors ON colors.id = products.color "
                            "WHERE sells_subproducts.sell=%1").arg(QString::number(id));
    setQuery(query);
}

QVariant SellingsSubProdsModel::data(const QModelIndex &item, int role) const {
//    if(role == Qt::DisplayRole) {
//        if(item.column() == SUBPROD_AMOUNT) {
//            QVariant subProdID = ProductsModel::data(index(item.row(), SUBPROD_ID), role);

//            return QVariant(amount);
//        }
//    }
    return QSqlQueryModel::data(item, role);
}
