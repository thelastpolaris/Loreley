#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QPrintDialog>
#include <QPainter>
#include <QDate>

#include "productsdata.h"
#include "auxiliary/ean13.h"

ProductsData* ProductsData::p_instance = 0;

ProductsData* ProductsData::Create() {
    if(p_instance) delete p_instance;
    p_instance = new ProductsData;
    return p_instance;
}

ProductsData::ProductsData()
    :subProductsModel()
{
    initModels();

    //Setup printer
    printer.setPrinterName("QL-570");
    printer.setPaperSize(QSize(62, 40), QPrinter::Millimeter);
    //    printer.setPageMargins(2,0,2,1, QPrinter::Millimeter);
//    printer.setOutputFileName("testBarcode.pdf");
    printer.setResolution(260);
}

ProductsData::~ProductsData() {

}

void ProductsData::initModels() {
    //Setup productsModel
    productsModel.setTable("products");
    productsModel.setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    //Name headers
    productsModel.setHeaderData(PROD_ID, Qt::Horizontal, tr("Product ID"));
    productsModel.setHeaderData(PROD_CHAR, Qt::Horizontal, tr("Characteristic"));
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
    subProductsModel.setReadOnlyCols(QList<int>{SUBPROD_ID, SUBPROD_AMOUNT, SUBPROD_SIZE, SUBPROD_BARCODE});
    //Select all subproducts
    subProductsModel.select();
}

QVariant ProductsData::productsData(int row, int column) const {
    return productsModel.data(productsModel.index(row,column));
}

QVariant ProductsData::subProductsData(int row, int column) const {
    return subProductsModel.data(subProductsModel.index(row,column));
}

void ProductsData::filterSubProducts(int product_id) {
    subProductsModel.setFilterByInteger("product_id", product_id);
    subProductsModel.select();
}

bool ProductsData::removeProduct(int productRow) {
    bool status = productsModel.removeRow(productRow);
    productsModel.select();
    subProductsModel.select();
    return status;
}

bool ProductsData::removeSubProduct(int subProductRow) {
    bool status = subProductsModel.removeRow(subProductRow);
    subProductsModel.select();
    return status;
}

bool ProductsData::reduceSubProduct(int subProductID, int reduceAmount, int reasonID) {
    QSqlQuery reduceSubProd;

    reduceSubProd.prepare("INSERT INTO " + QString(SUBPROD_REDUCE_TABLE) +
                          "(subprod_id, amount, reason)"
                          "VALUES (:subprod_id, :amount, :reason)");
    reduceSubProd.bindValue(":subprod_id", QVariant(subProductID));
    reduceSubProd.bindValue(":amount", QVariant(reduceAmount));
    reduceSubProd.bindValue(":reason", QVariant(reasonID));
    bool status = reduceSubProd.exec();
    subProductsModel.select();
    return status;
}

bool ProductsData::addNewProperty(QString table, QString name) {
    QSqlQuery addProperty;
    addProperty.prepare("INSERT INTO " + table + " (name) "
                                                 "VALUES (:name)");
    addProperty.bindValue(":name", name);
    return addProperty.exec();
}

bool ProductsData::removeProperty(QString table, int id) {
    QSqlQuery removeProperty;
    removeProperty.prepare("DELETE FROM " + table + " WHERE id=:id");
    removeProperty.bindValue(":id", id);
    return removeProperty.exec();
}

bool ProductsData::editProperty(QString table, QString name, QString newName) {
    QSqlQuery editProperty;
    editProperty.prepare("UPDATE " + table + " SET name=:newName WHERE name=:name");
    editProperty.bindValue(":newName", newName);
    editProperty.bindValue(":name", name);
    return editProperty.exec();
}

QList<int> ProductsData::getRowsWithProperty(int fieldID, int propertyValue) {
    QList<int> productRows;
    for(int i = 0; i < productsModel.rowCount(); ++i) {
        if(productsModel.getOriginalRecord(i).value(fieldID).toInt() == propertyValue) {
            productRows.append(i);
        }
    }

    return productRows;
}

bool ProductsData::setPropertyForProducts(QList<int> productRows, int fieldID, int propertyValue) {
    for(int i = 0; i < productsModel.rowCount(); ++i) {
        if(productRows.contains(i)) {
            //            if() {
            //                return false;
            //            }
            bool st = productsModel.setData(productsModel.index(i, fieldID), QVariant(propertyValue));
            qDebug() << st;
        }
    }
    return true;
}

bool ProductsData::hasProducts() {
    return productsModel.rowCount() > 0;
}

bool ProductsData::hasSubProducts() {
    return subProductsModel.rowCount() > 0;
}

void ProductsData::printBarcode(QModelIndex subProduct, QModelIndex product) {
    QString barcode = subProductsModel.data(subProductsModel.index(subProduct.row(),SUBPROD_BARCODE)).toString();
    QString size = subProductsModel.data(subProductsModel.index(subProduct.row(), SUBPROD_SIZE)).toString();

    QString name = productsModel.data(productsModel.index(product.row(), PROD_CHAR)).toString();
    QString category = productsModel.data(productsModel.index(product.row(), PROD_CAT)).toString();
    QString color = productsModel.data(productsModel.index(product.row(), PROD_COLOR)).toString();
    QString price = productsModel.data(productsModel.index(product.row(), PROD_PRICE)).toString();

    QPixmap barcodePixmap(540, 160);
    ean13.EAN13ToImage(barcodePixmap, barcode);

    QPainter painter;
    barcodePixmap.save("/home/polaris/test.png");

#define SEPARATOR_LINE_WIDTH 3
#define BARCODE_SEPARATOR_DISTANCE 10

#define SEPARATOR_NAME_DISTANCE 5
#define NUM_LINES_NAME 2

#define NAME_PRICE_DISTANCE 10
#define LABEL_PRICE_DISTANCE 5

    painter.begin(&printer);
    //Draw label
    QRect barcodeRect(barcodePixmap.rect());
    int beginX = barcodeRect.bottomLeft().x();
    int endX = barcodeRect.bottomRight().x();

    QRect devRect(0,0,painter.device()->width(), painter.device()->height());
    barcodeRect.moveCenter(devRect.center());
    painter.drawPixmap(barcodeRect.topLeft().x(), 0, barcodePixmap);

    painter.setPen(QPen(painter.brush(), SEPARATOR_LINE_WIDTH));
    int separator_y = barcodePixmap.height() + BARCODE_SEPARATOR_DISTANCE;
    painter.drawLine(beginX, separator_y, barcodeRect.bottomRight().x(), separator_y );

    //Draw name, category and color
    /***********************************************************/
    QString barcodeName = QString("%1, %2, %3").arg(name, category, color);
    painter.setFont(QFont("Arial", 8));
    QRectF nameBound(beginX, separator_y + SEPARATOR_NAME_DISTANCE, barcodeRect.bottomRight().x() - beginX,
                     QFontMetrics(painter.font()).height()*NUM_LINES_NAME);
    painter.drawText(nameBound, Qt::TextWrapAnywhere,  barcodeName, &nameBound);
    /***********************************************************/

    int blockWidth = (barcodeRect.bottomRight().x())/3;
    //Draw price
    /***********************************************************/
    painter.setFont(QFont("Arial", 12));
    QRectF priceLabelBound(beginX, nameBound.bottomLeft().y() + NAME_PRICE_DISTANCE, blockWidth, QFontMetrics(painter.font()).height());
    painter.drawText(priceLabelBound, Qt::TextWrapAnywhere, tr("Price"), &priceLabelBound);

    painter.setFont(QFont("Arial", 12, QFont::Bold));
    QRectF priceBound(beginX, priceLabelBound.bottomLeft().y() + LABEL_PRICE_DISTANCE, blockWidth, QFontMetrics(painter.font()).height());
    painter.drawText(priceBound, Qt::TextWrapAnywhere, price, &priceBound);
    /***********************************************************/

    //Draw discount (to be added)
    //    int blockWidth = nameBound.width()/3;
    //    /***********************************************************/
    //    painter.setFont(QFont("Arial", 8, QFont::Bold));
    //    QRectF priceLabelBound(beginX, nameBound.bottomLeft().y() + NAME_PRICE_DISTANCE, blockWidth, QFontMetrics(painter.font()).height());
    //    painter.drawText(priceLabelBound, Qt::TextWrapAnywhere, tr("Price"), &priceLabelBound);

    //    painter.setFont(QFont("Arial", 8));
    //    QRectF priceBound(beginX, priceLabelBound.bottomLeft().y() + LABEL_PRICE_DISTANCE, priceWidth, QFontMetrics(painter.font()).height());
    //    painter.drawText(priceBound, Qt::TextWrapAnywhere, price, &priceBound);
    /***********************************************************/

    //Draw size
    /***********************************************************/
    // Determine maximum width
    QString sizeLabel = tr("Size");
    int sizeLabelWidth = QFontMetrics(painter.font()).width(sizeLabel);
    int sizeWidth = QFontMetrics(painter.font()).width(size);

    double maxWidth = sizeLabelWidth > sizeWidth ? sizeLabelWidth : sizeWidth;

    painter.setFont(QFont("Arial", 12));
    QRectF sizeLabelBound(endX - maxWidth, nameBound.bottomLeft().y() + NAME_PRICE_DISTANCE,
                          blockWidth, QFontMetrics(painter.font()).height());
    painter.drawText(sizeLabelBound, Qt::TextWrapAnywhere, sizeLabel, &sizeLabelBound);

    painter.setFont(QFont("Arial", 12, QFont::Bold));
    QRectF sizeBound(endX - maxWidth, sizeLabelBound.bottomLeft().y() + LABEL_PRICE_DISTANCE,
                     blockWidth, QFontMetrics(painter.font()).height());
    painter.drawText(sizeBound, Qt::TextWrapAnywhere, size, &sizeBound);
    /***********************************************************/

    painter.end();
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
    QString barcode("203000000000");
    barcode.replace(barcode.size() - id.size(), id.size(),id );
    return ean13.generateBarcode(barcode);
}

bool ProductsData::addProduct(QString name, QVariant category, int price, QVariant color, QVariant brand, QString note) {
    QSqlRecord newRow = productsModel.record();
    newRow.setValue(PROD_CHAR, QVariant(name));
    newRow.setValue(PROD_CAT, category);
    newRow.setValue(PROD_PRICE, QVariant(price));
    newRow.setValue(PROD_COLOR, color);
    newRow.setValue(PROD_BRAND, brand);
    newRow.setValue(PROD_NOTE, QVariant(note));

    bool status = productsModel.insertRecord(-1, newRow);
    productsModel.select();
    return status;
}

bool ProductsData::addSubProduct(int product_id, int amount, int size, QDate arrivalDate) {
    QSqlQuery checkSubProduct;

    if(checkSubProduct.exec(QString("SELECT * FROM `%1` WHERE `product_id`= %2 "
                                    "AND `size`=%3").arg(SUBPROD_TABLE).arg(product_id).arg(size)))
    {
        QVariant subprod_id;
        if(!checkSubProduct.size()) {
            QSqlQuery updateSubProduct;
            updateSubProduct.prepare("INSERT INTO subproducts (product_id, size, barcode) "
                                     "VALUES (:product_id, :size, :barcode)");
            updateSubProduct.bindValue(":product_id", product_id);
            updateSubProduct.bindValue(":size", size);
            updateSubProduct.bindValue(":barcode", generateBarcode());
            if(!updateSubProduct.exec()) return false;
            else checkSubProduct.exec(); //If subProduct was added retrieve its ID
        }

        checkSubProduct.next();
        subprod_id = checkSubProduct.value(SUBPROD_ID);

        QSqlQuery updateSubProduct;
        updateSubProduct.prepare("INSERT INTO " + QString(SUBPROD_ARRIVAL_TABLE) +
                                 "(subprod_id, amount, arrival_date)"
                                 "VALUES (:subprod_id, :amount, :arrival_date)");
        updateSubProduct.bindValue(":subprod_id", QVariant(subprod_id));
        updateSubProduct.bindValue(":amount", QVariant(amount));
        updateSubProduct.bindValue(":arrival_date", QVariant(arrivalDate));
        bool status = updateSubProduct.exec();
        subProductsModel.select();
        return status;
    }
    return false;
}
