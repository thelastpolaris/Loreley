#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

#include "productsdata.h"
#include "auxiliary/ean13.h"

ProductsData* ProductsData::p_instance = 0;

ProductsData* ProductsData::Create() {
    if(p_instance) delete p_instance;
    p_instance = new ProductsData;
    return p_instance;
}

ProductsData::ProductsData()
{
    //Setup productsModel
    productsModel.setTable("products");
    productsModel.setEditStrategy(QSqlRelationalTableModel::OnRowChange);
    //Name headers
    productsModel.setHeaderData(PROD_ID, Qt::Horizontal, tr("Product ID"));
    productsModel.setHeaderData(PROD_NAME, Qt::Horizontal, tr("Name"));
    productsModel.setHeaderData(PROD_CAT, Qt::Horizontal, tr("Category"));
    productsModel.setHeaderData(PROD_PRICE, Qt::Horizontal, tr("Price"));
    productsModel.setHeaderData(PROD_BRAND, Qt::Horizontal, tr("Brand"));
    productsModel.setHeaderData(PROD_COLOR, Qt::Horizontal, tr("Color"));
    productsModel.setHeaderData(PROD_NOTE, Qt::Horizontal, tr("Note"));
    //Set relations
    productsModel.setRelation(PROD_CAT, QSqlRelation("categories", "id", "name"));
    productsModel.setRelation(PROD_BRAND, QSqlRelation("brands", "id", "name"));
    productsModel.setRelation(PROD_COLOR, QSqlRelation("colors", "id", "name"));
    //Setup readonly columns
    productsModel.setReadOnlyCols(QList<int>{PROD_ID});
    //Select all products
    productsModel.select();

    //Setup subProductsModel
    subProductsModel.setTable("subproducts");
    subProductsModel.setEditStrategy(QSqlRelationalTableModel::OnRowChange);
    //Name headers
    subProductsModel.setHeaderData(SUBPROD_ID, Qt::Horizontal, tr("Subproduct ID"));
    subProductsModel.setHeaderData(SUBPROD_AMOUNT, Qt::Horizontal, tr("Amount"));
    subProductsModel.setHeaderData(SUBPROD_SIZE, Qt::Horizontal, tr("Size"));
    subProductsModel.setHeaderData(SUBPROD_BARCODE, Qt::Horizontal, tr("Barcode"));
    subProductsModel.setHeaderData(SUBPROD_NOTE, Qt::Horizontal, tr("Note"));
    //Set relations
    subProductsModel.setRelation(SUBPROD_SIZE, QSqlRelation("sizes", "id", "name"));
    //Initialize filter so that no subproducts are displayed
    filterSubProducts(-1);
    //Setup readonly columns
    subProductsModel.setReadOnlyCols(QList<int>{SUBPROD_ID, SUBPROD_SIZE, SUBPROD_BARCODE});
    //Select all subproducts
    subProductsModel.select();

    QPixmap pixmap(500,500);

    qDebug() << ean13.generateBarcode("202000000007");
    ean13.EAN13ToImage(pixmap, QString("202000000007"));
    qDebug() << pixmap.save("/home/polaris/test.png");
}

ProductsData::~ProductsData() {

}

QVariant ProductsData::productsData(int row, int column) const {
    return productsModel.data(productsModel.index(row,column));
}

QVariant ProductsData::subProductsData(int row, int column) const {
    return subProductsModel.data(subProductsModel.index(row,column));
}

bool ProductsData::filterSubProducts(int product_id) {
    subProductsModel.setFilterByInteger("product_id", product_id);
    subProductsModel.select();
}

bool ProductsData::removeProduct(int product_row) {
    bool status = productsModel.removeRow(product_row);
    productsModel.select();
    subProductsModel.select();
    return status;
}

bool ProductsData::removeSubProduct(int subProduct_row) {
    bool status = subProductsModel.removeRow(subProduct_row);
    subProductsModel.select();
    return status;
}

bool ProductsData::addNewProperty(QString table, QString name) {
    QSqlQuery addProperty;
    addProperty.prepare("INSERT INTO " + table + " (name) "
                             "VALUES (:name)");
    addProperty.bindValue(":name", name);
    bool status = addProperty.exec();
    return status;
}

bool ProductsData::removeProperty(QString table, QString name) {
    QSqlQuery removeProperty;
    removeProperty.prepare("DELETE FROM " + table + " WHERE name=:name");
    removeProperty.bindValue(":name", name);
    bool status = removeProperty.exec();
    return status;
}

bool ProductsData::hasProducts() {
    return productsModel.rowCount() > 0;
}

bool ProductsData::hasSubProducts() {
    return subProductsModel.rowCount() > 0;
}

QHash<int, QString> ProductsData::getNameAndKey(QString table, QString key, QString value) {
    QSqlQuery query("SELECT * from `" + table + "`");
    query.exec();

    QHash<int, QString> values;
    while (query.next())
        values.insert(query.value(key).toInt(), query.value(value).toString());

    return values;
}

QString ProductsData::generateBarcode() {
    //We are using the latest id to generate unique barcode
    QSqlQuery maxIDQuery("SELECT MAX(id) FROM subproducts");
    maxIDQuery.first();
    QString id = maxIDQuery.value(0).toString();
    QString barcode("2030000000000");
    barcode.insert(barcode.size() - id.size(),id );
    return ean13.generateBarcode(barcode);
}

bool ProductsData::addProduct(QString name, QVariant category, int price, QVariant color, QVariant brand, QString note) {
    QSqlRecord newRow = productsModel.record();
    newRow.setValue(PROD_NAME, QVariant(name));
    newRow.setValue(PROD_CAT, category);
    newRow.setValue(PROD_PRICE, QVariant(price));
    newRow.setValue(PROD_COLOR, color);
    newRow.setValue(PROD_BRAND, brand);
    newRow.setValue(PROD_NOTE, QVariant(note));

    bool status = productsModel.insertRecord(-1, newRow);
    productsModel.select();
    return status;
}

bool ProductsData::addSubProduct(int product_id, int amount, int size) {
    QSqlQuery checkSubProduct(QString("SELECT * FROM `%1` WHERE `product_id`= %2 AND `size`=%3").arg(SUBPROD_TABLE).arg(product_id).arg(size));

    if(checkSubProduct.exec()) {
        if(checkSubProduct.size()) {
            QSqlQuery updateSubProduct;
            updateSubProduct.prepare("UPDATE subproducts SET amount= amount + :amount WHERE product_id=:product_id AND size=:size");
            updateSubProduct.bindValue(":amount", amount);
            updateSubProduct.bindValue(":product_id", product_id);
            updateSubProduct.bindValue(":size", size);
            bool status = updateSubProduct.exec();
            subProductsModel.select();
            return status;
        } else {
            QSqlQuery updateSubProduct;
            updateSubProduct.prepare("INSERT INTO subproducts (product_id, amount, size, barcode) "
                                     "VALUES (:product_id, :amount, :size, :barcode)");
            updateSubProduct.bindValue(":product_id", product_id);
            updateSubProduct.bindValue(":amount", amount);
            updateSubProduct.bindValue(":size", size);
            updateSubProduct.bindValue(":barcode", generateBarcode());
            bool status = updateSubProduct.exec();
            subProductsModel.select();
            return status;
        }
    }

    return false;
}
