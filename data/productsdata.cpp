#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
//#include <QPrintDialog>
#include <QPainter>
#include <QDate>
#include <QSqlResult>

#include "xlsxdocument.h"
#include "productsdata.h"
#include "auxiliary/ean13.h"
#include "selldata.h"
#include "helpers.h"

ProductsData* ProductsData::p_instance = 0;

ProductsData* ProductsData::Create() {
    if(p_instance) delete p_instance;
    p_instance = new ProductsData;
    return p_instance;
}

ProductsData::ProductsData(QObject *parent)
    :QObject(parent)
{
    initModels();
    //Setup printer
//    printer.setPrinterName("Xprinter XP-350B");
    printer.setPaperSize(QSize(60, 40), QPrinter::Millimeter);
    printer.setPageMargins(2,0,2,1, QPrinter::Millimeter);
    printer.setOutputFileName("testBarcode.pdf");
    printer.setResolution(260);
}

ProductsData::~ProductsData() {

}

void ProductsData::initModels(bool onlyProducts) {
    //Setup productsModel
    productsModel.setTable("products");
    productsModel.setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    //Name headers
    productsModel.setHeaderData(PROD_ID, Qt::Horizontal, tr("Product ID"));
    productsModel.setHeaderData(PROD_NAME, Qt::Horizontal, tr("Characteristic"));
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

    if(!onlyProducts) {
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

        //Setup readonly columns
        subProductsModel.setReadOnlyCols(QList<int>{SUBPROD_ID, SUBPROD_AMOUNT, SUBPROD_SIZE, SUBPROD_BARCODE});
    }

    //Initialize filter so that no subproducts are displayed
    filterSubProducts(-1);
}

QVariant ProductsData::productsData(int row, int column) const {
    return productsModel.data(productsModel.index(row,column));
}

QVariant ProductsData::subProductsData(int row, int column) const {
    return subProductsModel.data(subProductsModel.index(row,column));
}

void ProductsData::cancelProductsFilter() {
    productsModel.setFilter("");
    setProductsFilter("");
}

void ProductsData::filterSubProducts(int product_id) {
    subProductsModel.setFilterByInteger("product_id", product_id);
    subProductsModel.select();
}

int ProductsData::searchForValues(QString name, int category, int color, int brand,
                                  int price, QString comment)
{
    //    productsModel.setFilter(""); //Reset filters
    QSqlRecord r = productsModel.getOriginalRecord(-1);
    QStringList filter;
    QStringList coolFilter;
    if(!name.isEmpty()) {
        filter.append(QString(PROD_TABLE) + "." + r.fieldName(PROD_NAME) + " LIKE '%" + name + "%'");
        coolFilter.append(productsModel.headerData(PROD_NAME, Qt::Horizontal).toString() + " - " + name);
    }
    if(category >= 0) {
        filter.append(QString(PROD_TABLE) + "." + r.fieldName(PROD_CAT) + "=" + QString::number(category));
        coolFilter.append(productsModel.headerData(PROD_CAT, Qt::Horizontal).toString() + " - " + getValueFromDB(PROP_CAT, "id", category, "name").toString());
    }
    if(color >= 0) {
        filter.append(QString(PROD_TABLE) + "." + r.fieldName(PROD_COLOR) + "=" + QString::number(color));
        coolFilter.append(productsModel.headerData(PROD_COLOR, Qt::Horizontal).toString() + " - " + getValueFromDB(PROP_COLOR, "id", color, "name").toString());
    }
    if(brand >= 0) {
        filter.append(QString(PROD_TABLE) + "." + r.fieldName(PROD_BRAND) + "=" + QString::number(brand));
        coolFilter.append(productsModel.headerData(PROD_BRAND, Qt::Horizontal).toString() + " - " + getValueFromDB(PROP_BRAND, "id", brand, "name").toString());
    }
    if(price != 0) {
        filter.append(QString(PROD_TABLE) + "." + r.fieldName(PROD_PRICE) + "=" + QString::number(price));
        coolFilter.append(productsModel.headerData(PROD_PRICE, Qt::Horizontal).toString() + " - " + QString::number(price));
    }
    if(!comment.isEmpty()) {
        filter.append(QString(PROD_TABLE) + "." + r.fieldName(PROD_NOTE) + " LIKE '%" + comment + "%'");
        coolFilter.append(productsModel.headerData(PROD_NOTE, Qt::Horizontal).toString() + " - " + comment);
    }

    QString finalFilter;
    if(filter.size() == 1) {
        finalFilter = filter[0];
    } else {
        finalFilter = filter.join(" AND ");
    }
    productsModel.setFilter(finalFilter);
    filterSubProducts(-1);

    if(coolFilter.size() == 1) {
        setProductsFilter(coolFilter[0]);
    } else {
        setProductsFilter(coolFilter.join(", "));
    }

    return productsModel.rowCount();
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
    name = name;
    addProperty.prepare("INSERT INTO " + table + " (name) "
                                                 "VALUES (:name)");
    addProperty.bindValue(":name", name);
    bool status = addProperty.exec();
    emit propertiesChanged(table);
    return status;
}

bool ProductsData::removeProperty(QString table, int id) {
    QSqlQuery removeProperty;
    removeProperty.prepare("DELETE FROM " + table + " WHERE id=:id");
    removeProperty.bindValue(":id", id);
    emit propertiesChanged(table);
    return removeProperty.exec();
}

bool ProductsData::editProperty(QString table, QString name, QString newName) {
    QSqlQuery editProperty;
    editProperty.prepare("UPDATE " + table + " SET name=:newName WHERE name=:name");
    editProperty.bindValue(":newName", newName);
    editProperty.bindValue(":name", name);
    emit propertiesChanged(table);
    return editProperty.exec();
}

int ProductsData::getPropertyID(QString table, QString name) {
    QSqlQuery propertyId("SELECT * FROM " + table);

    while (propertyId.next()) {
        if(propertyId.value("name").toString().toLower().trimmed() == name.toLower().trimmed()) return propertyId.value("id").toInt();
    }

    return -1;
}

bool ProductsData::propertyExist(QString table, QString name) {
    QSqlQuery checkProperty;
    checkProperty.prepare("SELECT * FROM " + table + " WHERE name=:name");
    checkProperty.bindValue(":name",name);
    checkProperty.exec();
    return checkProperty.size();
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
    bool status = false;
    for(int i = 0; i < productsModel.rowCount(); ++i) {
        if(productRows.contains(i)) {
            status = productsModel.setData(productsModel.index(i, fieldID), QVariant(propertyValue));
        }
    }
    return status;
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

    QString name = productsModel.data(productsModel.index(product.row(), PROD_NAME)).toString();
    QString category = productsModel.data(productsModel.index(product.row(), PROD_CAT)).toString();
    QString brand = productsModel.data(productsModel.index(product.row(), PROD_BRAND)).toString();
    QString color = productsModel.data(productsModel.index(product.row(), PROD_COLOR)).toString();
    QString price = productsModel.data(productsModel.index(product.row(), PROD_PRICE)).toString();

    QPixmap barcodePixmap(540, 160);
    ean13.EAN13ToImage(barcodePixmap, barcode);

    QPainter painter;
#define SEPARATOR_LINE_WIDTH 3
#define BARCODE_SEPARATOR_DISTANCE 10

#define SEPARATOR_NAME_DISTANCE 5
#define NUM_LINES_NAME 2

#define NAME_PRICE_DISTANCE 10
#define LABEL_PRICE_DISTANCE 5
    QPixmap label(540, 410);
    label.fill();
    painter.begin(&label);

//    painter.begin(&printer);
    //Draw label
    QRect barcodeRect(barcodePixmap.rect());
    int beginX = barcodeRect.bottomLeft().x();
    int endX = barcodeRect.bottomRight().x();

    QRect devRect(0,0,painter.device()->width(), painter.device()->height());
    barcodeRect.moveCenter(devRect.center());
    painter.drawPixmap(barcodeRect.topLeft().x(), 0, barcodePixmap);

    painter.setRenderHint(QPainter::Antialiasing, true);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    painter.setBrush(brush);

    qreal fontSize = 30;

    painter.setPen(QPen(brush, SEPARATOR_LINE_WIDTH));
    int separator_y = barcodePixmap.height() + BARCODE_SEPARATOR_DISTANCE;
    painter.drawLine(beginX, separator_y, barcodeRect.bottomRight().x(), separator_y );

    //Draw name, category and color
    /***********************************************************/
    QString barcodeName = QString("%1, %2, %3, %4").arg(category, brand, name, color);
    painter.setFont(QFont("Arial", 25));
    QRectF nameBound(beginX, separator_y + SEPARATOR_NAME_DISTANCE, barcodeRect.bottomRight().x() - beginX,
                     QFontMetrics(painter.font()).height()*NUM_LINES_NAME);
    painter.drawText(nameBound, Qt::TextWrapAnywhere,  barcodeName, &nameBound);
    /***********************************************************/

    int blockWidth = (barcodeRect.bottomRight().x())/3;
    //Draw price
    /***********************************************************/
    painter.setFont(QFont("Arial", fontSize));
    QRectF priceLabelBound(beginX, nameBound.bottomLeft().y() + NAME_PRICE_DISTANCE, blockWidth, QFontMetrics(painter.font()).height());
    painter.drawText(priceLabelBound, Qt::TextWrapAnywhere, tr("Price"), &priceLabelBound);

    painter.setFont(QFont("Arial", fontSize, QFont::Bold));
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

    painter.setFont(QFont("Arial", fontSize));
    QRectF sizeLabelBound(endX - maxWidth, nameBound.bottomLeft().y() + NAME_PRICE_DISTANCE,
                          blockWidth, QFontMetrics(painter.font()).height());
    painter.drawText(sizeLabelBound, Qt::TextWrapAnywhere, sizeLabel, &sizeLabelBound);

    painter.setFont(QFont("Arial", fontSize, QFont::Bold));
    QRectF sizeBound(endX - maxWidth, sizeLabelBound.bottomLeft().y() + LABEL_PRICE_DISTANCE,
                     blockWidth, QFontMetrics(painter.font()).height());
    painter.drawText(sizeBound, Qt::TextWrapAnywhere, size, &sizeBound);
    /***********************************************************/

    painter.end();

    QPainter painterPrint;
    painterPrint.begin(&printer);
    painterPrint.drawPixmap(50,20, label);
    painterPrint.end();
}


QHash<int, QString> ProductsData::getNameAndKey(QString table, QString key, QString value) {
    QSqlQuery query("SELECT * from " + table);

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

int ProductsData::addProduct(QString name, QVariant category, int price, QVariant color, QVariant brand, QString note) {
#if (QT_VERSION > QT_VERSION_CHECK(5, 5, 1)) // We provide sql query version for old Qt versions where insertRecord always returns false
    QSqlRecord newRow = productsModel.record();
    newRow.setValue(PROD_NAME, QVariant(name));
    newRow.setValue(PROD_CAT, category);
    newRow.setValue(PROD_PRICE, QVariant(price));
    newRow.setValue(PROD_COLOR, color);
    newRow.setValue(PROD_BRAND, brand);
    newRow.setValue(PROD_NOTE, QVariant(note));

    bool status = productsModel.insertRecord(-1, newRow);
    if(!status) {
        qDebug() << productsModel.lastError().text();
    }
    productsModel.select();
    // Clear all subproducts in subprod model because there will be no selection in products model
    filterSubProducts(-1);
    return status;
#else
    QSqlQuery addProduct;
    addProduct.prepare("INSERT INTO " + QString(PROD_TABLE) +
                       "(brand, category_id, name, color, price, note)"
                       "VALUES (:brand, :category, :name, :color, :price, :note)");
    addProduct.bindValue(":brand", brand);
    addProduct.bindValue(":category", QVariant(category));
    addProduct.bindValue(":name", QVariant(name));
    addProduct.bindValue(":color", QVariant(color));
    addProduct.bindValue(":price", QVariant(price));
    addProduct.bindValue(":note", QVariant(note));
    if(!addProduct.exec()) {
        qDebug() << addProduct.lastError().text();
    }
    productsModel.select();
    return addProduct.lastInsertId().toInt();
#endif
}

bool ProductsData::addSubProduct(int product_id, int amount, int size, QDate arrivalDate) {
    QSqlQuery checkSubProduct;

    if(checkSubProduct.exec(QString("SELECT * FROM %1 WHERE product_id= %2 "
                                    "AND size=%3").arg(SUBPROD_TABLE).arg(product_id).arg(size)))
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

void ProductsData::setProductsFilter(QString productsFilter) {
    if(m_productsFilter != productsFilter) {
        m_productsFilter = productsFilter;
        emit productsFilterChanged(productsFilter);
    }
}

void ProductsData::selectSubProducts() {
    subProductsModel.select();
}

void ProductsData::importFromExcel(const QString& importXlsx) {
#define PROD_COLS 6 //The first column with sizes
    QXlsx::Document xlsx(importXlsx);
    //First add sizes
    int emptyCellCounter = 0;
    int cellNum = PROD_COLS; //First
    QMap<int, QString> sizes; //Mapping of sizes to column no.
    while(emptyCellCounter < 10) {
        if (QXlsx::Cell *cell = xlsx.cellAt(1, cellNum)) {
            emptyCellCounter = 0;
            QString sizeName = cell->value().toString();
            if(!propertyExist(PROP_SIZE, sizeName)) {
                addNewProperty(PROP_SIZE, sizeName);
            }
            sizes.insert(cellNum, sizeName);
        } else {
            emptyCellCounter++;
        }
        cellNum++;
    }

    int emptyRowCounter = 0; /// This variable stores a number of empty rows encountered in a row
    int row = 2;
    QList<int> emptyRows; ///No. of rows that wasn't added will be in this list

    while(emptyRowCounter < 50) { /// If we encountered > 50 empty rows then exit, this is an end of the spreadsheet
        //Add product
        int category = 0;
        int brand = 0;
        QString name = " ";
        int color = 0;
        int price = 0;
        bool isRowEmpty = false;

        if (QXlsx::Cell *cell = xlsx.cellAt(row, 1)) { //Check categories
            QString prodCat = cell->value().toString();
            if(!propertyExist(PROP_CAT, prodCat)) {
                addNewProperty(PROP_CAT, prodCat.toLower());
            }
            category = getPropertyID(PROP_CAT, prodCat);
        } else {
            isRowEmpty = true;
        }

        if (QXlsx::Cell *cell = xlsx.cellAt(row, 2)) { //Check brands
            QString prodBrand = cell->value().toString();
            if(!propertyExist(PROP_BRAND, prodBrand)) {
                addNewProperty(PROP_BRAND, prodBrand);
            }
            brand = getPropertyID(PROP_BRAND, prodBrand);
        } else {
            isRowEmpty = true;
        }

        if (QXlsx::Cell *cell = xlsx.cellAt(row, 3)) { //Check name
            name = cell->value().toString();
        }

        if (QXlsx::Cell *cell = xlsx.cellAt(row, 4)) { //Check categories
            QString prodColor = cell->value().toString();
            if(!propertyExist(PROP_COLOR, prodColor)) {
                addNewProperty(PROP_COLOR, prodColor.toLower());
            }
            color = getPropertyID(PROP_COLOR, prodColor);
        } else {
            isRowEmpty = true;
        }

        if (QXlsx::Cell *cell = xlsx.cellAt(row, 5)) { //Check categories
            price = cell->value().toInt();
        } else {
            isRowEmpty = true;
        }

        int productID = -1;
        if(isRowEmpty) {
            emptyRows.append(row);
            emptyRowCounter++;
            row++;
            continue;
        } else {
            productID = addProduct(name, QVariant(category),price, QVariant(color), QVariant(brand));
            emptyRowCounter = 0; //Once we found non-empty row - reset emptyrowCounter
        }

        //Add subproducts
        for(QMap<int, QString>::iterator i = sizes.begin(); i != sizes.end(); ++i) {
            if (QXlsx::Cell *cell = xlsx.cellAt(row, i.key())) { //Check categories
                addSubProduct(productID, cell->value().toInt(), getPropertyID(PROP_SIZE, i.value()), QDate::currentDate());
            }
        }
        row++;
    }
}

int ProductsData::getAmountOfSubProd(int subProdID) {
    QHash<int, int> idsInCart = SellData::Instance()->getCartModel()->getIDsWithAmount();

    QSqlQuery arrival;
    arrival.prepare("SELECT * from " + QString(SUBPROD_ARRIVAL_TABLE) +
                    " WHERE subprod_id=:subprod_id");
    arrival.bindValue(":subprod_id", subProdID);
    if(!arrival.exec()) {
        qDebug() << tr("Error retrieving amount of subproduct. Amount will be set to 0");
    }

    int amount = 0;
    while(arrival.next()) amount += arrival.value(SUBPROD_HISTORY_AMOUNT).toInt();

    QSqlQuery reduce;
    reduce.prepare("SELECT * from " + QString(SUBPROD_REDUCE_TABLE) +
                   " WHERE subprod_id=:subprod_id");
    reduce.bindValue(":subprod_id", subProdID);
    if(!reduce.exec()) {
        qDebug() << tr("Error retrieving amount of subproduct. Amount will be set to 0");
    }

    while(reduce.next()) amount -= reduce.value(SUBPROD_HISTORY_AMOUNT).toInt();

    if(idsInCart.contains(subProdID)) {
        amount -= idsInCart[subProdID];
    }

    return amount;
}


QPair<int, int> ProductsData::searchBarcode(QString barCode) {
    QSqlQuery search;
    QPair<int,int> ids;
    search.prepare("SELECT id, product_id from " + QString(SUBPROD_TABLE) +
                   " WHERE barcode=:barcode");
    search.bindValue(":barcode", barCode);
    if(!search.exec()) {
        qDebug() << tr("No subproduct with barcode %1").arg(barCode);
    }
    if(!search.size()) {
        ids.first = -1;
        ids.second = -1;
        return ids;
    }
    search.next();
    ids.first = search.value("product_id").toInt();
    ids.second = search.value("id").toInt();

    return ids;
}
