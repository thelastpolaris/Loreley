#ifndef SELLDATA_H
#define SELLDATA_H

#include <QObject>
#include "models/cartmodel.h"

class SellData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayPrice MEMBER m_displayPrice NOTIFY displayPriceChanged)
public:
    static SellData* Create();
    static SellData* Instance() { return p_instance; }
    /**
     * @brief addToCart - if the ID of subproduct is known in advance use this function
     * @param subProdID - ID of subproduct
     * @param error - reference to string that will store the error description
     * @return true if products were added to cart
     */
    bool addToCart(int subProdID, QString& error);

    /**
     * @brief addToCart - adds subproduct to cart using its barcode
     * @param barCode - barcode of the subproduct
     * @param error - reference to string that will store the error description
     * @return true if products were added to cart
     */
    bool addToCart(QString barCode, QString& error);

    /**
     * @brief removeFromCart - removes subproducts from the cart and inserts it back to other subproducts
     * @param row - id of subproducts row in cart
     */
    bool removeFromCart(int row);

    CartModel* getCartModel() { return &m_productCart; }

    /**
     * @brief commitSale - Adds a new sale to the databse. Reduces the amount of subproducts
     * @param subProds - Key - individual discount for subproduct (should be 0 if there is no discount), value - subproduct
     * @param discPercents - discount for the whole cart in percents
     * @param discount - discount for the whole cart in plain sum
     * @return true if sale was successfully added to DB
     */
    bool commitSale(int discPercents = 0, int discount = 0);

protected:
    explicit SellData(QObject *parent = 0);

signals:
    /**
     * @brief saleDone - emitted when sale is finished (cart is clear)
     */
    void saleDone(double price);

    void errorOccured(QString errorText);

    void displayPriceChanged(QString displayPrice);
public slots:
    void handleDisplayPrice();

private:
    static SellData *p_instance;
    CartModel m_productCart;
    QList<int> m_prodIDs;
    QString m_displayPrice;
};

#endif // SELLDATA_H
