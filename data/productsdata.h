#ifndef PRODUCTSDATA_H
#define PRODUCTSDATA_H

#include <QObject>
#include <QSqlError>
#include <QPrinter>

#include "productsmodel.h"
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
#define SUBPROD_AMOUNT 2
#define SUBPROD_SIZE 3
#define SUBPROD_BARCODE 4
#define SUBPROD_NOTE 5

#define PROD_TABLE "products"
#define SUBPROD_TABLE "subproducts"

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

    bool addSubProduct(int product_id, int amount, int size);
    QSqlError subProductsLastError() const { return subProductsModel.lastError(); }

    QHash<int, QString> getNameAndKey(QString table, QString key, QString value);
    QString generateBarcode();

    QVariant productsData(int row, int column) const;
    QVariant subProductsData(int row, int column) const;

    bool filterSubProducts(int product_id);

    bool removeProduct(int product_row);
    bool removeSubProduct(int subProduct_row);

    bool addNewProperty(QString table, QString name);
    bool removeProperty(QString table, QString name);

    bool hasProducts();
    bool hasSubProducts();

    void printBarcode(QModelIndex subProduct);

protected:
    ProductsData();
    ~ProductsData();

private:
    ProductsModel productsModel;
    ProductsModel subProductsModel;
    EAN13 ean13;

    static ProductsData *p_instance;
    QPrinter printer;
};

#endif // PRODUCTSDATA_H
