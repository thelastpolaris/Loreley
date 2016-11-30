#ifndef PRODUCTSDATA_H
#define PRODUCTSDATA_H

#include <QObject>
#include <QSqlError>
#include <QPrinter>

#include "productsmodel.h"
#include "subproductsmodel.h"
#include "auxiliary/ean13.h"

class EAN13;

#define PROD_ID 0
#define PROD_NAME 1
#define PROD_CAT 2
#define PROD_PRICE 3
#define PROD_BRAND 4
#define PROD_COLOR 5
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

class ProductsData : public QObject
{
    Q_OBJECT
public:
    static ProductsData* Create();
    static ProductsData* Instance() { return p_instance; }

    ProductsModel* getProductsModel() { return &productsModel; }
    ProductsModel* getSubProductsModel() { return &subProductsModel; }

    bool addProduct(QString name, QVariant category, int price, QVariant color, QVariant brand, QString note = QString());
    QSqlError productsLastError() const { return productsModel.lastError(); }

    bool addSubProduct(int product_id, int amount, int size, QDate arrivalDate);
    QSqlError subProductsLastError() const { return subProductsModel.lastError(); }

    QHash<int, QString> getNameAndKey(QString table, QString key, QString value);
    QString generateBarcode();

    QVariant productsData(int row, int column) const;
    QVariant subProductsData(int row, int column) const;

    void filterSubProducts(int product_id);

    bool removeProduct(int productRow);
    bool removeSubProduct(int subProductRow);
    bool reduceSubProduct(int subProductID, int reduceAmount, int reasonID);

    bool addNewProperty(QString table, QString name);
    bool removeProperty(QString table, QString name);
    bool editProperty(QString table, QString name, QString newName);

    bool hasProducts();
    bool hasSubProducts();

    void printBarcode(QModelIndex subProduct, QModelIndex product);

protected:
    ProductsData();
    ~ProductsData();

private:
    ProductsModel productsModel;
    SubProductsModel subProductsModel;
    EAN13 ean13;

    static ProductsData *p_instance;
    QPrinter printer;
};

#endif // PRODUCTSDATA_H
