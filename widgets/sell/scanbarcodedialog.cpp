#include "scanbarcodedialog.h"
#include "ui_scanbarcodedialog.h"
#include "data/selldata.h"

#include <QMessageBox>
#include <QKeyEvent>

ScanBarcodeDialog::ScanBarcodeDialog(QWidget *parent)
    :QDialog(parent), ui(new Ui::ScanBarcodeDialog)
{
    ui->setupUi(this);
    setModal(true);
}

void ScanBarcodeDialog::keyPressEvent(QKeyEvent *event) {
    if(!event->text().isEmpty()) {
        if(event->text() == "\r") {
            QString error;
            SellData::Instance()->addToCart(barCode, error);
            if(!error.isEmpty()) {
                QMessageBox::warning(this, tr("Error while adding product to cart"),
                                     error);
            }
            barCode.clear();
        } else {
            barCode += event->text();
        }

        event->accept();
    }
}

ScanBarcodeDialog::~ScanBarcodeDialog()
{
    delete ui;
}
