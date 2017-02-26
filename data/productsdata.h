#ifndef PRODUCTSDATA_H
#define PRODUCTSDATA_H

#include <QObject>
#include <QSqlError>
#include <QPrinter>

#include "models/productsmodel.h"
#include "models/subproductsmodel.h"
#include "auxiliary/ean13.h"

class EAN13;

#define PROD_ID 0
#define PROD_BRAND 1
#define PROD_CAT 2
#define PROD_NAME 3
#define PROD_COLOR 4
#define PROD_PRICE 5
#define PROD_NOTE 6

#define SUBPROD_ID 0
#define SUBPROD_PROD_ID 1
#define SUBPROD_SIZE 2
#define SUBPROD_BARCODE 3
#define SUBPROD_NOTE 4
#define SUBPROD_AMOUNT 5

#define SUBPROD_HISTORY_AMOUNT 2

#define PROD_TABLE "products"
#define SUBPROD_TABLE "subproducts"
#define SUBPROD_ARRIVAL_TABLE "subprod_arrival"
#define SUBPROD_REDUCE_TABLE "subprod_reduce"

//Properties tables
#define PROP_CAT "categories"
#define PROP_COLOR "colors"
#define PROP_BRAND "brands"
#define PROP_SIZE "sizes"

class ProductsData : public QObject
{
    Q_OBJECT
public:
    static ProductsData* Create();
    static ProductsData* Instance() { return p_instance; }

    /**
     * @param onlyProducts - if true will (re)initialize only products model
     */
    void initModels(bool onlyProducts = false);

    Q_PROPERTY(QString productsFilter READ getProductsFilter WRITE setProductsFilter NOTIFY productsFilterChanged)

    ProductsModel* getProductsModel() { return &productsModel; }
    SubProductsModel* getSubProductsModel() { return &subProductsModel; }

    int addProduct(QString name, QVariant category, int price, QVariant color, QVariant brand, QString note = QString());
    QSqlError productsLastError() const { return productsModel.lastError(); }

    bool addSubProduct(int product_id, int amount, int size, QDate arrivalDate);
    QSqlError subProductsLastError() const { return subProductsModel.lastError(); }

    QHash<int, QString> getNameAndKey(QString table, QString key, QString value);
    QString generateBarcode();

    QVariant productsData(int row, int column) const;
    QVariant subProductsData(int row, int column) const;

    /**
     * @brief cancelProductsFilter - cancels filter for productsModel
     */
    void cancelProductsFilter();
    void filterSubProducts(int product_id);
    /**
     * @brief searchForValues - filters productModel according to parameters
     * @return number of rows in filtered row
     */
    int searchForValues(QString name, int category, int color, int brand,
                         int price, QString comment);

    bool removeProduct(int productRow);
    bool removeSubProduct(int subProductRow);
    bool reduceSubProduct(int subProductID, int reduceAmount, int reasonID);

    bool addNewProperty(QString table, QString name);
    bool removeProperty(QString table, int id);
    bool editProperty(QString table, QString name, QString newName);
    bool propertyExist(QString table, QString name);
    int getPropertyID(QString table, QString name);

    QList<int> getRowsWithProperty(int fieldID, int propertyValue);
    bool setPropertyForProducts(QList<int> productIDs, int fieldID, int propertyValue);

    bool hasProducts();
    bool hasSubProducts();

    void printBarcode(QModelIndex subProduct, QModelIndex product);

    QString getProductsFilter() const { return m_productsFilter; }
    void setProductsFilter(QString productsFilter);

    /**
     * @brief importFromExcel - import products from xslx file
     * @param importXlsx - path to file
     */
    void importFromExcel(const QString& importXlsx);

    /**
     * @brief selectSubProducts - select subproducts according to subproducts in the shopping cart
     */
    void selectSubProducts();

    int getAmountOfSubProd(int subProdID);

signals:
    void productsFilterChanged(QString productsFilter);
    void propertiesChanged(QString _tableName);
    void propertyEdited();

protected:
    ProductsData();
    ~ProductsData();

private:
    ProductsModel productsModel;
    SubProductsModel subProductsModel;
    EAN13 ean13;

    QString m_productsFilter;

    static ProductsData *p_instance;
    QPrinter printer;
};

#endif // PRODUCTSDATA_H
