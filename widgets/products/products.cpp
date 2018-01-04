#include "products.h"
#include "ui_products.h"

#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include "data/productsdata.h"
#include "data/models/productsmodel.h"

#include "addproductdialog.h"
//#include "ui_addproductdialog.h"
#include "addsubproductdialog.h"
//#include "ui_addsubproductdialog.h"
#include "reducesubproductdialog.h"

#include "editproperty.h"
#include "ui_editproperty.h"

#include "../delegates/datepickeritemdelegate.h"
#include "../delegates/comboboxitemdelegate.h"
#include "searchproductdialog.h"
#include "../sell/sell.h"

#include "mainwindow.h"

#include "../sell/scanbarcodedialog.h"

Products::Products(bool _saleMode) :
    ui(new Ui::Products), addProdDialog(new AddProductDialog(this)),
    addSubProdDialog(new AddSubProductDialog(this)), reduceSubProdDialog(new ReduceSubProductDialog(this)),
    searchProdDialog(new SearchProductDialog(this)), editPropertyWindow(new EditProperty(this)), saleMode(_saleMode),
    scanDialog(new ScanBarcodeDialog(this))
{
    ProductsData* prodData = ProductsData::Instance();

    ui->setupUi(this);

    ui->tableView->setModel(prodData->getProductsModel());
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //Setup delegates for products
    setupPropertyDelegates("categories");
    setupPropertyDelegates("colors");
    setupPropertyDelegates("brands");

    //Search by barcode
    connect(ui->searchBarcodeButton, SIGNAL(pressed()), scanDialog, SLOT(show()));
    connect(scanDialog, &ScanBarcodeDialog::newBarcode, [=](QString barCode) {
        selectByBarcode(barCode);
        scanDialog->close();
    });

    ui->tableView_2->setModel(prodData->getSubProductsModel());
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView_2->setColumnHidden(SUBPROD_PROD_ID,true);
    ui->tableView_2->horizontalHeader()->moveSection(SUBPROD_AMOUNT, SUBPROD_NOTE);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    setupPropertyDelegates("sizes");

    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            SLOT(updateSubProducts(const QItemSelection &, const QItemSelection &)));
    connect(ui->tableView_2->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(subProdSelectionChanged(QItemSelection,QItemSelection)));

    if(!saleMode) {
        //Setup buttons for products
        connect(ui->addProductButton, SIGNAL(clicked(bool)), addProdDialog, SLOT(startAddingProduct()));
        connect(ui->deleteProductButton, SIGNAL(clicked(bool)), SLOT(startDeletingProduct()));
        connect(ui->printBarcodeButton, SIGNAL(clicked(bool)), SLOT(startPrintingBarcode()));
    }
    connect(ui->searchButton, SIGNAL(clicked(bool)), searchProdDialog, SLOT(startSearch()));
    //Setup cancel filter button and text
    connect(prodData, SIGNAL(productsFilterChanged(QString)), this, SLOT(updateFilter(QString)));
    connect(ui->cancelFilterButton, &QPushButton::clicked, [=]() {
        prodData->cancelProductsFilter();
    });
    updateFilter(prodData->getProductsFilter());

    connect(this, SIGNAL(productSelected(bool)), ui->deleteProductButton, SLOT(setEnabled(bool)));

    if(!saleMode) {
        //Setup buttons for subProducts
        connect(ui->addSubProductButton, &QPushButton::clicked, [=]() {
            QString size = prodData->subProductsData(ui->tableView_2->currentIndex().row(), SUBPROD_SIZE).toString();
            addSubProdDialog->startAddingSubProduct(size);
        });
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
    }

    connect(prodData, &ProductsData::propertiesChanged, [=](QString table) {
        setupPropertyDelegates(table);
        prodData->initModels(true); //Re-initialize models to allow selection of new property
    });

    //TODO - Find better solution
    connect(prodData, &ProductsData::propertyEdited, [=]() {
        prodData->getProductsModel()->select();
        updateSubProducts();
    });

    if(!saleMode) {
        //Setup menu buttons for editing various properties
        connect(MainWindow::Instance(), &MainWindow::categoriesTriggered, [=]() {
            editPropertyWindow->show(tr("Category"), PROP_CAT, PROD_CAT);
        });
        connect(MainWindow::Instance(), &MainWindow::colorsTriggered, [=]() {
            editPropertyWindow->show(tr("Color"), PROP_COLOR, PROD_COLOR);
        });
        connect(MainWindow::Instance(), &MainWindow::brandsTriggered, [=]() {
            editPropertyWindow->show(tr("Brand"), PROP_BRAND, PROD_BRAND);
        });
        connect(MainWindow::Instance(), &MainWindow::sizeTriggered, [=]() {
            editPropertyWindow->show(tr("Size"), PROP_SIZE, SUBPROD_SIZE, false);
        });
        connect(MainWindow::Instance(), &MainWindow::importExcelTriggered, [=]() {
            QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                            "/home",
                                                            tr("Excel spreadsheet (*.xlsx)"));
            prodData->importFromExcel(fileName);
        });

    } else {
        ui->addProductButton->setVisible(false);
        ui->deleteProductButton->setVisible(false);
        ui->printBarcodeButton->setVisible(false);

        ui->addSubProductButton->setVisible(false);
        ui->reduceSubProductButton->setVisible(false);
        ui->deleteSubProductButton->setVisible(false);
    }

    ui->tableView->hideColumn(PROD_ID);
    ui->tableView_2->hideColumn(SUBPROD_ID);

    //Initialize button states
    emit productSelected(false);
    emit subProductSelected(false);
}

void Products::prepareProducts() {
    ui->tableView->clearSelection();
    updateSubProducts();
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
        emit productSelected(true, productRow);
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
        emit subProductSelected(true, selected.indexes()[0].row());
    } else {
        emit subProductSelected(false);
    }
}

void Products::selectSubProdsWithSelection() {
    int currentRow = ui->tableView_2->currentIndex().row();
    int currentCol = ui->tableView_2->currentIndex().column();
    ProductsData::Instance()->selectSubProducts();
    ui->tableView_2->setCurrentIndex(ProductsData::Instance()->getSubProductsModel()->index(currentRow, currentCol));
}

bool Products::startDeletingProduct() {
    QModelIndex currentIndex = ui->tableView->selectionModel()->currentIndex();
    ProductsData *prodData = ProductsData::Instance();

    if(currentIndex.isValid()) {
        int delRow = currentIndex.row();
        QString name = prodData->productsData(delRow, PROD_NAME).toString();

        int response = QMessageBox::question(this, tr("Delete product"),tr("Are you sure you want to delete <b>%1</b> ?").arg(name), QDialogButtonBox::Ok, QDialogButtonBox::Cancel);
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

        int response = QMessageBox::question(this, tr("Delete subproduct"),tr("Are you sure you want to delete subproduct of product <b>%1</b> ?<br>"
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

void Products::updateFilter(QString newFilter) {
    bool visible = false;
    if(!newFilter.isEmpty()) {
        visible = true;
        ui->filterText->setText(newFilter);
    }

    ui->cancelFilterButton->setVisible(visible);
    ui->filterText->setVisible(visible);
}

void Products::selectByBarcode(QString barCode) {
    ProductsData *prodData = ProductsData::Instance();
    QPair<int, int> ids = prodData->searchBarcode(barCode);
    if(ids.first < 0 || ids.second < 0) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid barcode. No product or subproduct found."));
        return;
    }
    ui->tableView->selectRow(prodData->getProductsModel()->rowByID(ids.first, PROD_ID));
    /* I assume that selectRow is asynchronious. That is why without explicitly adding
       filter for product id to subproducts model we will get segfault */
    prodData->filterSubProducts(ids.first);
    ui->tableView_2->selectRow(prodData->getSubProductsModel()->rowByID(ids.second, SUBPROD_ID));
}

Products::~Products()
{
    delete ui;
}
