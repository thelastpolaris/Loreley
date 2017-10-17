#include "selldata.h"
#include "productsdata.h"
#include <QSqlQuery>
#include <QDebug>
#include "helpers.h"

SellData* SellData::p_instance = 0;

SellData* SellData::Create() {
    if(p_instance) delete p_instance;
    p_instance = new SellData;
    return p_instance;
}

SellData::SellData(QObject *parent)
    :QObject(parent)
{
    connect(&m_productCart, SIGNAL(priceChanged(double)), this, SLOT(handleDisplayPrice()));
    connect(&m_productCart, SIGNAL(discountsChanged(double)), this, SLOT(handleDisplayPrice()));
}

void SellData::handleDisplayPrice() {
    double price = m_productCart.property("price").toDouble();
    double discounts = m_productCart.property("discounts").toDouble();

    if(discounts != 0) {
        setProperty("displayPrice",
                    QString("<s>%1</s> %2 <font color=#006400>-%3</font>").arg(
                        QString::number(price),
                        QString::number(price - discounts),
                        QString::number(discounts)));
    } else {
        setProperty("displayPrice", QString::number(price));
    }
}

bool SellData::addToCart(QString barCode, QString& error) {
    if(!barCode.isEmpty() && barCode.size() == 13) {
        int subProdID =  getValueFromDB(SUBPROD_TABLE, "barcode", barCode, "id").toInt();
        if(subProdID > 0) {
            return addToCart(subProdID, error);
        } else {
            error = tr("No subproduct with barcode %1").arg(barCode);
            return false;
        }
    }
    error = tr("Invalid barcode provied");
    return false;
}

bool SellData::commitSale(int discPercents, int discount) {
    QSqlQuery query;
    query.prepare("INSERT INTO sells (price, client) VALUES (:price, :client)");
    query.bindValue(":price", QString::number(m_productCart.getFinalPrice()));
    query.bindValue(":client", -1); //Under development

    if(query.exec()) {
        query.prepare("SELECT LAST_INSERT_ID()");
        if(!query.exec()) {
            emit errorOccured(query.lastError().text());
        }
        query.next(); //Get insert ID
        int sellID = query.value(0).toInt();

        for(int i = 0; i < m_productCart.rowCount(); ++i) {
            int subProdID = m_productCart.data(i, ID).toInt();
            int amount = 1;//subProds[subProdID];
            double discount = m_productCart.data(i, DISCOUNT).toDouble();

            query.prepare("INSERT INTO subprod_reduce (subprod_id, amount, reason) "
                          "VALUES (:subprod_id, :amount, :reason);");
            query.bindValue(":subprod_id", subProdID);
            query.bindValue(":amount", amount);
            query.bindValue(":reason", SELL_REASON);

            if(!query.exec()) {
                emit errorOccured(query.lastError().text());
                return false;
            }

            query.prepare("INSERT INTO sells_subproducts (sell, subproduct, amount, discount)"
                          "VALUES (:sell, :subprod_id, :amount, :discount);");
            query.bindValue(":sell", sellID);
            query.bindValue(":subprod_id", subProdID);
            query.bindValue(":amount", amount);
            query.bindValue(":discount", discount);

            if(!query.exec()) {
                emit errorOccured(query.lastError().text());
                return false;
            }
        }

        setProperty("displayPrice", QString::number(0));
        emit saleDone(m_productCart.property("price").toDouble() - m_productCart.property("discounts").toDouble());
        m_productCart.clearCart();
        return true;
    } else {
        emit errorOccured(query.lastError().text());
    }
    return false;
}

bool SellData::addToCart(int subProdID, QString& error) {
    if(ProductsData::Instance()->getAmountOfSubProd(subProdID) >= 1) {

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
                return true;
            } else {
                error = tr("No product with ID %1").arg(QString::number(productID));
                return false;
            }
        } else {
            error = tr("No subproduct with ID %1").arg(QString::number(subProdID));
        }
    } else {
        error = tr("Not enough product with ID %1").arg(QString::number(subProdID));
    }

    return false;
}

bool SellData::removeFromCart(int row) {
    return m_productCart.removeFromCart(row);
}
