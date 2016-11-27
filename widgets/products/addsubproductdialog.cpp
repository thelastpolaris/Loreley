#include "addsubproductdialog.h"
#include "ui_addsubproductdialog.h"
#include "data/productsmodel.h"
#include "data/productsdata.h"
#include <QMessageBox>

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

void AddSubProductDialog::startAddingSubProduct() {
    ProductsData *prod = ProductsData::Instance();
    clearFields();

    QHash<int, QString> categories = prod->getNameAndKey("sizes", "id", "name");
    for(QHash<int, QString>::const_iterator i = categories.begin(); i != categories.end(); ++i) {
            ui->sizeCB->addItem(i.value(), i.key());
    }
    ui->arrivalDE->setDate(QDate::currentDate());
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
