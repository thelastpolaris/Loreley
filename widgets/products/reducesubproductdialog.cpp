#include "reducesubproductdialog.h"
#include "ui_reducesubproductdialog.h"
#include "data/productsdata.h"
#include <QMessageBox>

ReduceSubProductDialog::ReduceSubProductDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReduceSubProductDialog)
{
    ui->setupUi(this);
    ui->reduceSpinBox->setMinimum(0);

    connect(ui->reduceSpinBox, SIGNAL(valueChanged(int)), SLOT(updateSubProdsLeft()));
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(finishReducingSubProduct()));
}

void ReduceSubProductDialog::finishReducingSubProduct() {
    int reduceAmount = ui->reduceSpinBox->value();

    if(!reduceAmount) {
        QMessageBox::information(this, tr("Nothing was changed"), tr("The amount of subproduct wasn't reduced"));
    } else {
        ProductsData* prodData = ProductsData::Instance();

        prodData->reduceSubProduct(subProdID, reduceAmount, ui->reasonComboBox->currentData().toInt());
    }
}

void ReduceSubProductDialog::startReducingSubProduct(int _subProdID, int _curAmount) {
    ProductsData* prodData = ProductsData::Instance();

    subProdID = _subProdID;
    curAmount = _curAmount;

    ui->reduceSpinBox->setMaximum(curAmount);
    ui->reduceSpinBox->setValue(0);

    ui->reasonComboBox->clear();
    QHash<int, QString> cbValues = prodData->getNameAndKey("subprod_reasons", "id", "name");

    for(QHash<int, QString>::const_iterator i = cbValues.begin(); i != cbValues.end(); ++i) {
        ui->reasonComboBox->addItem(i.value(), i.key());
    }

    updateSubProdsLeft();
    QDialog::show();
}

void ReduceSubProductDialog::updateSubProdsLeft() {
    ui->subProdLeft->setText(QString(tr("Products left after reduce: %1")).arg(
                                 curAmount - ui->reduceSpinBox->value()));
}

ReduceSubProductDialog::~ReduceSubProductDialog()
{
    delete ui;
}
