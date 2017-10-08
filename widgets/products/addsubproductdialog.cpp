#include "addsubproductdialog.h"
#include "ui_addsubproductdialog.h"
#include "data/models/productsmodel.h"
#include "data/productsdata.h"
#include <QMessageBox>
#include "mainwindow.h"

AddSubProductDialog::AddSubProductDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSubProductDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(finishAddingSubProduct()));
}

void AddSubProductDialog::finishAddingSubProduct() {
    ProductsData *prod = ProductsData::Instance();

    int product_id = prod->getSubProductsModel()->getFilterValue();
    if(prod->addSubProduct(product_id, ui->amountSB->value(), ui->sizeCB->currentData().toInt(),
                           ui->arrivalDE->date())) {
        close();
        QMessageBox::information(this,tr("Success!"), tr("Successfully added subproduct to the database"));
    } else {
        QSqlError err = prod->subProductsLastError();
        QMessageBox::information(this,tr("Error!"), tr("Error - subproduct was not added to the database due to the following errors"
                                                       "<br> <b>Database Error:</b> %1"
                                                       "<br> <b>Driver Error:</b> %2").arg(err.databaseText(), err.driverText() ));
    }
}

void AddSubProductDialog::startAddingSubProduct(QString size) {
    ProductsData *prod = ProductsData::Instance();
    clearFields();

    QHash<int, QString> categories = prod->getNameAndKey("sizes", "id", "name");
    QList<QString> sorted = categories.values();
    qSort(sorted);
    for(QList<QString>::const_iterator i = sorted.begin(); i != sorted.end(); ++i) {
            ui->sizeCB->addItem(*i, categories.key(*i));
    }

    ui->arrivalDE->setDate(QDate::currentDate());

    ui->sizeCB->setCurrentText(size);

    if(!ui->sizeCB->count()) {
        QMessageBox::warning(MainWindow::Instance(), tr("Add size"), tr("Please, add at least one size, before adding product"));
        return;
    }

    show();
}

void AddSubProductDialog::clearFields() {
    ui->amountSB->setValue(1);
    ui->sizeCB->clear();
}

AddSubProductDialog::~AddSubProductDialog()
{
    delete ui;
}
