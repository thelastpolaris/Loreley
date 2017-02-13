#ifndef SELLDATA_H
#define SELLDATA_H

#include <QObject>
#include <QAbstractTableModel>

class CartModel : public QAbstractTableModel
{
    Q_OBJECT
public:
#define COLS_NUMBER 5

#define CATEGORY 0
#define NAME 1
#define SIZE 2
#define PRICE 3
#define ID 4 //Not visible to user

    CartModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void addToCart(QString category, QString name, QString size, QString price, int id);
    /**
     * @brief removeFromCart - removes item from cart
     * @param price - price of deleted subproduct
     * @return true if row was successfully deleted
     */
    bool removeFromCart(int row, int& price);

    /**
     * @brief getIDsWithAmount - returns QHash with ID of subproduct as a key and ints amount as a value
     */
    QHash<int, int> getIDsWithAmount();
private:
    QHash<int, int> m_IDsWithAmount;
    QVector<QVector<QString>> columns;
};

class SellData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int price MEMBER m_price NOTIFY priceChanged)
public:
    static SellData* Create();
    static SellData* Instance() { return p_instance; }
    /**
     * @brief addToCart - if the ID of subproduct is known in advance use this function
     * @param subProdID - ID of subproduct
     * @param error - reference to string that will store the error description
     * @return true if products was added to cart
     */
    bool addToCart(int subProdID, QString& error);

    /**
     * @brief addToCart - adds subproduct to cart using its barcode
     * @param barCode - barcode of the subproduct
     * @param error - reference to string that will store the error description
     * @return true if products was added to cart
     */
    bool addToCart(QString barCode, QString& error);

    /**
     * @brief removeFromCart - removes subproducts from the cart and inserts it back to other subproducts
     * @param row - id of subproducts row in cart
     */
    bool removeFromCart(int row);

    CartModel* getCartModel() { return &m_productCart; }
protected:
    explicit SellData(QObject *parent = 0);

signals:
    void priceChanged(int changedPrice);
public slots:

private:
    static SellData *p_instance;
    CartModel m_productCart;
    QList<int> m_prodIDs;
    int m_price;
};

#endif // SELLDATA_H
