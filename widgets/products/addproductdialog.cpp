#include "addproductdialog.h"
#include "ui_addproductdialog.h"
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSqlError>
#include "data/productsdata.h"

AddProductDialog::AddProductDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddProductDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(finishAddingProduct()));
    connect(ui->buttonBox, SIGNAL(rejected()), SLOT(close()));
}

AddProductDialog::~AddProductDialog()
{
    delete ui;
}

void AddProductDialog::finishAddingProduct() {
    ProductsData *prod = ProductsData::Instance();

    if(ui->nameEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Name was not entered"), tr("Please enter name of the product"));
        return;
    }
    if(ui->priceEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Price was not entered"), tr("Please enter price of the product"));
        return;
    }

    QString name = ui->nameEdit->text();

    if(prod->addProduct(
                name,
                ui->categoryCB->currentData(),
                ui->priceEdit->text().remove(" ").toInt(),
                ui->colorCB->currentData(),
                ui->brandCB->currentData(),
                ui->noteTextEdit->toPlainText()))
    {
        close();
        QMessageBox::information(this,tr("Success!"), tr("Successfully added product <b>%1</b> to the database").arg(name));
    } else {
        QSqlError err = prod->productsLastError();
        QMessageBox::information(this,tr("Error!"), tr("Error - Product <b>%1</b> was not added to the database due to the following errors"
                                                       "<br> <b>Database Error:</b> %2"
                                                       "<br> <b>Driver Error:</b> %3").arg(name, err.databaseText(), err.driverText() ));
    }
}

void AddProductDialog::startAddingProduct() {
    ProductsData *prod = ProductsData::Instance();
    clearFields();

    QHash<int, QString> categories = prod->getNameAndKey("categories", "id", "name");
    for(QHash<int, QString>::const_iterator i = categories.begin(); i != categories.end(); ++i) {
            ui->categoryCB->addItem(i.value(), i.key());
    }

    QHash<int, QString> colors = prod->getNameAndKey("colors", "id", "name");
    for(QHash<int, QString>::const_iterator i = colors.begin(); i != colors.end(); ++i) {
            ui->colorCB->addItem(i.value(), i.key());
    }

    QHash<int, QString> brands = prod->getNameAndKey("brands", "id", "name");
    for(QHash<int, QString>::const_iterator i = brands.begin(); i != brands.end(); ++i) {
            ui->brandCB->addItem(i.value(), i.key());
    }

    show();
}

void AddProductDialog::clearFields() {
    ui->nameEdit->clear();
    ui->categoryCB->clear();
    ui->colorCB->clear();
    ui->brandCB->clear();
    ui->priceEdit->clear();
    ui->noteTextEdit->clear();
}
