#include "searchproductdialog.h"
#include "ui_searchproductdialog.h"

#include "../../data/productsdata.h"
#include <QMessageBox>
#include "mainwindow.h"

SearchProductDialog::SearchProductDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::SearchProductDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(finishSearch()));
    connect(ui->buttonBox, SIGNAL(rejected()), SLOT(close()));
}

SearchProductDialog::~SearchProductDialog()
{
    delete ui;
}

void SearchProductDialog::startSearch() {
    ProductsData *prod = ProductsData::Instance();
    clearFields();

    QHash<int, QString> categories = prod->getNameAndKey("categories", "id", "name");
    ui->categoryCB->addItem(tr("All categories"), -1);
    for(QHash<int, QString>::const_iterator i = categories.begin(); i != categories.end(); ++i) {
            ui->categoryCB->addItem(i.value(), i.key());
    }

    ui->colorCB->addItem(tr("All colors"), -1);
    QHash<int, QString> colors = prod->getNameAndKey("colors", "id", "name");
    for(QHash<int, QString>::const_iterator i = colors.begin(); i != colors.end(); ++i) {
            ui->colorCB->addItem(i.value(), i.key());
    }

    ui->brandCB->addItem(tr("All brands"), -1);
    QHash<int, QString> brands = prod->getNameAndKey("brands", "id", "name");
    for(QHash<int, QString>::const_iterator i = brands.begin(); i != brands.end(); ++i) {
            ui->brandCB->addItem(i.value(), i.key());
    }

    if(!ui->categoryCB->count()) {
        QMessageBox::warning(MainWindow::Instance(), tr("Add category"), tr("Please, add at least one category, before adding product"));
        return;
    } else if(!ui->colorCB->count()) {
        QMessageBox::warning(MainWindow::Instance(), tr("Add color"), tr("Please, add at least one color, before adding product"));
        return;
    } else if(!ui->brandCB->count()) {
        QMessageBox::warning(MainWindow::Instance(), tr("Add brand"), tr("Please, add at least one brand, before adding product"));
        return;
    }

    show();
}

void SearchProductDialog::finishSearch() {
    int rows = ProductsData::Instance()->searchForValues(ui->nameEdit->text(), ui->categoryCB->currentData().toInt(),
                                              ui->colorCB->currentData().toInt(),
                                              ui->brandCB->currentData().toInt(),
                                              ui->priceEdit->text().remove(" ").toInt(), ui->noteTextEdit->toPlainText());
    close();
    QMessageBox::warning(this, tr("Finished searching"), tr("Found %1 products").arg(QString::number(rows)));
}

void SearchProductDialog::clearFields() {
    ui->nameEdit->clear();
    ui->categoryCB->clear();
    ui->colorCB->clear();
    ui->brandCB->clear();
    ui->priceEdit->clear();
    ui->noteTextEdit->clear();
}
