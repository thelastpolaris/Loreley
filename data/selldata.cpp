#include "selldata.h"
#include "productsdata.h"
#include <QSqlQuery>
#include <QDebug>

SellData* SellData::p_instance = 0;

SellData* SellData::Create() {
    if(p_instance) delete p_instance;
    p_instance = new SellData;
    return p_instance;
}

QVariant getValueFromDB(QString table, QString field, QVariant value, QString targetField) {
    QSqlQuery query;
    query.prepare("SELECT * FROM " + table + " WHERE " + field + "=:value");
    query.bindValue(":value", value);
    query.exec();
    if(query.next()) {
        return query.value(targetField);
    }
    return QVariant::Invalid;
}

QSqlQuery getQueryFromDB(QString table, QString field, QVariant value) {
    QSqlQuery query;
    query.prepare("SELECT * FROM " + table + " WHERE " + field + "=:value");
    query.bindValue(":value", value);
    query.exec();
    return query;
}

SellData::SellData(QObject *parent) : QObject(parent)
{

}

bool SellData::addToCart(QString barCode, QString& error) {
    return addToCart(getValueFromDB(SUBPROD_TABLE, "barcode", barCode, "id").toInt(), error);
}

bool SellData::addToCart(int subProdID, QString& error) {
    QSqlQuery getSubProd = getQueryFromDB(SUBPROD_TABLE, "id", subProdID);
    if(getSubProd.next()) {
        int productID = getSubProd.value(SUBPROD_PROD_ID).toInt();
        QSqlQuery getProd = getQueryFromDB(PROD_TABLE, "id", productID);

        if(getProd.next()) {
            QSqlQuery getCat = getQueryFromDB(PROP_CAT, "id", getProd.value(PROD_CAT));
            getCat.next();
            m_productCart.addToCart(getCat.value("name").toString(),
                                 getProd.value(PROD_NAME).toString(),
                                 getValueFromDB(PROP_SIZE, "id", getSubProd.value(SUBPROD_SIZE).toString(), "name").toString(),
                                 getProd.value(PROD_PRICE).toString(),
                                    subProdID);
            setProperty("price", m_price + getProd.value(PROD_PRICE).toInt());
            return true;
        } else {
            error = tr("No product with ID %1").arg(QString::number(productID));
            return false;
        }
    } else {
        error = tr("No subproduct with ID %1").arg(QString::number(subProdID));
    }
    return false;
}

bool SellData::removeFromCart(int row) {
    int price = 0;
    if(m_productCart.removeFromCart(row, price)) {
        setProperty("price", m_price - price);
        return true;
    }
    return false;
}

CartModel::CartModel()
    :QAbstractTableModel(), columns(COLS_NUMBER)
{

}

int CartModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return columns[0].size();
}

void CartModel::addToCart(QString category, QString name, QString size, QString price, int id) {
    emit beginInsertRows(QModelIndex(), columns[0].size(), columns[0].size());
    columns[CATEGORY].append(category);
    columns[NAME].append(name);
    columns[SIZE].append(size);
    columns[PRICE].append(price);
    columns[ID].append(QString::number(id));
    emit endInsertRows();
}

bool CartModel::removeFromCart(int row, int& price) {
    if(columns[0].size() > row && 0 <= row) {
        emit beginRemoveRows(QModelIndex(), row, row);
        columns[CATEGORY].remove(row);
        columns[NAME].remove(row);
        columns[SIZE].remove(row);

        price = columns[PRICE][row].toInt();
        columns[PRICE].remove(row);
        columns[ID].remove(row);
        emit endRemoveRows();
        return true;
    }
    return false;
}

QHash<int, int> CartModel::getIDsWithAmount() {
    QHash<int, int> ids;
    for(int i = 0; i < columns[0].size(); ++i) {
        int id = columns[ID][i].toInt();
        if(ids.contains(id)) {
           ids[id]++;
        } else {
            ids.insert(id, 1);
        }
    }
    return ids;
}

QVariant CartModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case CATEGORY:
                return tr("Category");
            case NAME:
                return tr("Name");
            case SIZE:
                return tr("Size");
            case PRICE:
                return tr("Price");
            }
        }
    }
    return QVariant();
}

int CartModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return columns.size() - 1; //1 is for ID (we don't want to show it)
}

QVariant CartModel::data(const QModelIndex& index, int role) const {
    if(role == Qt::DisplayRole && index.row() != ID) {
        return columns[index.column()][index.row()];
    }
    return QVariant::Invalid;
}
