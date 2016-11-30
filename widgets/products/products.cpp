#include "products.h"
#include "ui_products.h"

#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDialogButtonBox>
#include "data/productsdata.h"
#include "data/productsmodel.h"

#include "addproductdialog.h"
//#include "ui_addproductdialog.h"
#include "addsubproductdialog.h"
//#include "ui_addsubproductdialog.h"
#include "reducesubproductdialog.h"

#include "editproperty.h"
#include "ui_editproperty.h"

#include "../delegates/datepickeritemdelegate.h"
#include "../delegates/comboboxitemdelegate.h"

#include "auxiliary/propertiesmodel.h"

#include "mainwindow.h"

Products* Products::p_instance = 0;

Products* Products::Create() {
    if(p_instance) delete p_instance;
    p_instance = new Products;
    return p_instance;
}

Products::Products() :
    ui(new Ui::Products), addProdDialog(new AddProductDialog(this)),
    addSubProdDialog(new AddSubProductDialog(this)), reduceSubProdDialog(new ReduceSubProductDialog(this)),
    editPropertyWindow(new EditProperty(this)), propertiesModel(new PropertiesModel(this))
{
    ProductsData* prodData = ProductsData::Instance();

    ui->setupUi(this);

    ui->tableView->setModel(prodData->getProductsModel());

    //Setup delegates for products
    setupPropertyDelegates("categories");
    setupPropertyDelegates("colors");
    setupPropertyDelegates("brands");

    ui->tableView_2->setModel(prodData->getSubProductsModel());

    ui->tableView_2->setColumnHidden(SUBPROD_PROD_ID,true);
    ui->tableView_2->horizontalHeader()->moveSection(SUBPROD_AMOUNT, SUBPROD_NOTE);
    setupPropertyDelegates("sizes");

    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            SLOT(updateSubProducts(const QItemSelection &, const QItemSelection &)));
    connect(ui->tableView_2->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(subProdSelectionChanged(QItemSelection,QItemSelection)));

    //Setup buttons for products
    connect(ui->addProductButton, SIGNAL(clicked(bool)), addProdDialog, SLOT(startAddingProduct()));
    connect(ui->deleteProductButton, SIGNAL(clicked(bool)), SLOT(startDeletingProduct()));
    connect(ui->printBarcodeButton, SIGNAL(clicked(bool)), SLOT(startPrintingBarcode()));

    connect(this, SIGNAL(productSelected(bool)), ui->deleteProductButton, SLOT(setEnabled(bool)));

    //Setup buttons for subProducts
    connect(ui->addSubProductButton, SIGNAL(clicked(bool)), addSubProdDialog, SLOT(startAddingSubProduct()));
    connect(ui->reduceSubProductButton, SIGNAL(clicked(bool)), SLOT(starReducingSubProductAmount()));
    connect(ui->deleteSubProductButton, SIGNAL(clicked(bool)), SLOT(startDeletingSubProduct()));

    //Add subproduct
    connect(this, SIGNAL(productSelected(bool)), ui->addSubProductButton, SLOT(setEnabled(bool)));
    //Delete subproduct
    connect(this, SIGNAL(productSelected(bool)), ui->deleteSubProductButton, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(subProductSelected(bool)), ui->deleteSubProductButton, SLOT(setEnabled(bool)));
    //Reduce button
    connect(this, SIGNAL(productSelected(bool)), ui->reduceSubProductButton, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(subProductSelected(bool)), ui->reduceSubProductButton, SLOT(setEnabled(bool)));
    //Print button
    connect(this, SIGNAL(subProductSelected(bool)), ui->printBarcodeButton, SLOT(setEnabled(bool)));

    connect(editPropertyWindow, &EditProperty::propertiesChanged, [=](const QString &tableName) {
        propertiesModel->setPropertiesList(ProductsData::Instance()->getNameAndKey(tableName, "id", "name"));
    });

    connect(editPropertyWindow, SIGNAL(propertiesChanged(QString)), SLOT(setupPropertyDelegates(QString)));
    //TODO - Find better solution
    connect(editPropertyWindow, &EditProperty::propertyEdited, [=]() {
       prodData->getProductsModel()->select();
       updateSubProducts();
    });

    //Setup menu buttons for editing various properties
    editPropertyWindow->getUI()->propertiesLV->setModel(propertiesModel);
    connect(MainWindow::Instance(), SIGNAL(categoriesTriggered(bool)), SLOT(showEditCategories()));
    connect(MainWindow::Instance(), SIGNAL(colorsTriggered(bool)), SLOT(showEditColors()));
    connect(MainWindow::Instance(), SIGNAL(brandsTriggered(bool)), SLOT(showEditBrands()));

    connect(MainWindow::Instance(), SIGNAL(sizeTriggered(bool)), SLOT(showEditSizes()));

    //Initialize button states
    emit productSelected(false);
    emit subProductSelected(false);
}

void Products::showEditCategories() {
    propertiesModel->setPropertiesList(ProductsData::Instance()->getNameAndKey("categories", "id", "name"));
    editPropertyWindow->show("category", "categories");
}

void Products::showEditColors() {
    propertiesModel->setPropertiesList(ProductsData::Instance()->getNameAndKey("colors", "id", "name"));
    editPropertyWindow->show("color", "colors", true);
}

void Products::showEditBrands() {
    propertiesModel->setPropertiesList(ProductsData::Instance()->getNameAndKey("brands", "id", "name"));
    editPropertyWindow->show("brand", "brands");
}

void Products::showEditSizes() {
    propertiesModel->setPropertiesList(ProductsData::Instance()->getNameAndKey("sizes", "id", "name"));
    editPropertyWindow->show("size", "sizes");
}

void Products::setupPropertyDelegates(QString propTableName) {
    ProductsData *prodData = ProductsData::Instance();

    if(propTableName == "categories") {
        ui->tableView->setItemDelegateForColumn(PROD_CAT, new ComboBoxItemDelegate(prodData->getNameAndKey("categories", "id", "name"), ui->tableView));
    } else if(propTableName == "colors") {
        ui->tableView->setItemDelegateForColumn(PROD_COLOR, new ComboBoxItemDelegate(prodData->getNameAndKey("colors", "id", "name"), prodData->getNameAndKey("colors", "id", "color"), ui->tableView));
    } else if(propTableName == "brands") {
        ui->tableView->setItemDelegateForColumn(PROD_BRAND, new ComboBoxItemDelegate(prodData->getNameAndKey("brands", "id", "name"), ui->tableView));
    } else if(propTableName == "sizes") {
        ui->tableView_2->setItemDelegateForColumn(SUBPROD_SIZE, new ComboBoxItemDelegate(prodData->getNameAndKey("sizes", "id", "name"), ui->tableView_2));
    }
}

void Products::startPrintingBarcode() {
    ProductsData* prodData = ProductsData::Instance();

    int response = QMessageBox::question(this, tr("Print label for subproduct"), tr("Are you sure that you want to print label for this subproduct?"));
    if(response == QMessageBox::Yes) {
        prodData->printBarcode(ui->tableView_2->selectionModel()->currentIndex(),
                               ui->tableView->selectionModel()->currentIndex());
    }
}

void Products::updateSubProducts(const QItemSelection & selected, const QItemSelection & deselected) {
    Q_UNUSED(deselected)
    int filter = -1;
    ProductsData *prodData = ProductsData::Instance();
    if(!selected.isEmpty()) {
        int productRow = selected.indexes()[0].row();
        emit productSelected(true);
        filter = prodData->productsData(productRow, PROD_ID).toInt();
    } else {
        emit productSelected(false);
    }
    prodData->filterSubProducts(filter);
    emit subProductSelected(false);
}

void Products::subProdSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected) {
    Q_UNUSED(deselected)
    if(!selected.isEmpty()) {
        emit subProductSelected(true);
    } else {
        emit subProductSelected(false);
    }
}

bool Products::startDeletingProduct() {
    QModelIndex currentIndex = ui->tableView->selectionModel()->currentIndex();
    ProductsData *prodData = ProductsData::Instance();

    if(currentIndex.isValid()) {
        int delRow = currentIndex.row();
        QString name = prodData->productsData(delRow, PROD_NAME).toString();

        int response = QMessageBox::question(this, tr("Delete product"),tr("Are you sure you want to delete <b>%1</b>?").arg(name), QDialogButtonBox::Ok, QDialogButtonBox::Cancel);
        if(response == QMessageBox::Ok) {
            prodData->removeProduct(delRow);
            ui->tableView->selectRow(delRow); // Select next row
            if(!prodData->hasProducts()) {
                emit productSelected(false); // No products in table. Disable delete button
                emit subProductSelected(false); // No products. Disable add and delete subprod. buttons
            }
            QMessageBox::information(this, tr("Success!"), tr("Successfully deleted product <b>%1</b>").arg(name));
            return true;
        }
    } else {
        QMessageBox::warning(this, tr("Error - No product selected"), tr("No product was selected to delete"));
    }
    return false; // Row was not deleted
}

bool Products::startDeletingSubProduct() {
    ProductsData *prodData = ProductsData::Instance();

    QModelIndex currentIndex = ui->tableView_2->selectionModel()->currentIndex();
    if(currentIndex.isValid()) {
        int delRow = currentIndex.row();
        int parentProdRow = ui->tableView->selectionModel()->currentIndex().row();
        QString name = prodData->productsData(parentProdRow, PROD_NAME).toString();

        int response = QMessageBox::question(this, tr("Delete subproduct"),tr("Are you sure you want to delete subproduct of product <b>%1</b>?<br>"
                                                                              "This will erase all data associated with subproduct (arrival history, sells)").arg(name), QDialogButtonBox::Ok, QDialogButtonBox::Cancel);
        if(response == QDialogButtonBox::Ok) {
            prodData->removeSubProduct(delRow);
            if(!prodData->hasSubProducts()) {
                emit subProductSelected(false);
            }
            QMessageBox::information(this, tr("Success!"), tr("Successfully deleted subproduct of product %1").arg(name));
            return true;
        }
    } else {
        QMessageBox::warning(this, tr("Error - No subproduct selected"), tr("No subproduct was selected to delete"));
    }
    return false; // Row was not deleted
}

bool Products::starReducingSubProductAmount() {
    ProductsData *prodData = ProductsData::Instance();

    QModelIndex curSubProd = ui->tableView_2->selectionModel()->currentIndex();

    int curAmount = prodData->subProductsData(curSubProd.row(), SUBPROD_AMOUNT).toInt();
    int subProdID = prodData->subProductsData(curSubProd.row(), SUBPROD_ID).toInt();


    reduceSubProdDialog->startReducingSubProduct(subProdID, curAmount);
    return false;
}

Products::~Products()
{
    delete ui;
}
