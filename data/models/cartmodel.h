#ifndef CARTMODEL_H
#define CARTMODEL_H

#include <QAbstractTableModel>

class CartModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(double price MEMBER m_price NOTIFY priceChanged)
    Q_PROPERTY(double discounts MEMBER m_discounts NOTIFY discountsChanged)
public:
#define COLS_NUMBER 6

#define CATEGORY 0
#define NAME 1
#define SIZE 2
#define PRICE 3
#define DISCOUNT 4 //Not visible to user
#define ID 5 //Not visible to user

#define SELL_REASON 3 //ID of "Sell" reason for subproduct reduce

    CartModel(QObject* parent = 0);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    /**
     * @brief data - Convenience function that simply constructs QModelIndex
     */
    QVariant data(int row, int column, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool addDiscount(int row, double discount);
    bool deleteDiscount(int row);
//    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    void addToCart(QString category, QString name, QString size, QString price, int id);
    /**
     * @brief removeFromCart - removes item from cart
     * @return true if row was successfully deleted
     */
    bool removeFromCart(int row);

    /**
     * @return true if subproduct at row position has discount
     */
    bool hasDiscount(int row);

    /**
     * @brief getIDsWithAmount - returns QHash with ID of subproduct as a key and ints amount as a value
     */
    QHash<int, int> getIDsWithAmount() const { return IDsWithAmount; }

    /**
     * @brief clearCart - deletes all products in the products cart
     */
    void clearCart();

    /**
     * @brief getFinalPrice - returns final price with all the discounts
     */
    double getFinalPrice();

private:
    QVector<QVector<QString>> columns;
signals:
    void priceChanged(double changedPrice);
    void discountsChanged(double changedDiscounts);
    void IDsWithAmountChanged(QHash<int, int> IDsWithAmount);
private:
    QHash<int, int> IDsWithAmount;
    double m_price;
    double m_discounts;
};

#endif // CARTMODEL_H
