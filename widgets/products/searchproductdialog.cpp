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
    QList<QString> sorted = categories.values();
    qSort(sorted);
    for(QList<QString>::const_iterator i = sorted.begin(); i != sorted.end(); ++i) {
            ui->categoryCB->addItem(*i, categories.key(*i));
    }

    QHash<int, QString> colors = prod->getNameAndKey("colors", "id", "name");
    sorted = colors.values();
    qSort(sorted);
    for(QList<QString>::const_iterator i = sorted.begin(); i != sorted.end(); ++i) {
            ui->colorCB->addItem(*i, colors.key(*i));
    }

    QHash<int, QString> brands = prod->getNameAndKey("brands", "id", "name");
    sorted = brands.values();
    qSort(sorted);
    for(QList<QString>::const_iterator i = sorted.begin(); i != sorted.end(); ++i) {
            ui->brandCB->addItem(*i, brands.key(*i));
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
