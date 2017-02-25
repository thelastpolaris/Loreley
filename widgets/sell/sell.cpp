#include "sell.h"
#include "ui_sell.h"

#include "data/selldata.h"
#include "data/productsdata.h"

#include "../products/products.h"
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QDialogButtonBox>
#include <QInputDialog>
#include "mainwindow.h"
#include "scanbarcodedialog.h"

Sell::Sell(QWidget *parent) :
    QWidget(parent), ui(new Ui::Sell), prodWidget(new Products(true)), scanDialog(new ScanBarcodeDialog(this))
{
    ui->setupUi(this);
    ui->splitter->insertWidget(0, prodWidget);
    ui->cartTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->cartTable->horizontalHeader()->setStretchLastSection(true);
    ui->cartTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->splitter->setStretchFactor(0, 2);

    HTMLDelegate* delegate = new HTMLDelegate;
    ui->cartTable->setItemDelegate(delegate);

    SellData* sellData = SellData::Instance();

    //Set width of 'Price' label
    ui->labelPrice->setMaximumWidth(ui->labelPrice->fontMetrics().width(ui->labelPrice->text()));
    ui->price->setWordWrap(true);

    ui->cartTable->setModel(sellData->getCartModel());

    //Set signals
    connect(sellData, &SellData::displayPriceChanged, [=](QString changedPrice) {
        ui->price->setText(changedPrice);
    });

    connect(prodWidget, &Products::subProductSelected, [=](bool selected, int row) {
        if(selected) {
            rowToAdd = row;
        } else {
            rowToAdd = -1;
        }
        if(rowToAdd != -1 && ProductsData::Instance()->subProductsData(rowToAdd, SUBPROD_AMOUNT) > 0) {
            ui->addToCart->setEnabled(true);
        } else {
            ui->addToCart->setEnabled(false);
        }
    });

    connect(ui->addToCart, &QPushButton::clicked, [=] {
        QString error;
        if(!sellData->addToCart(
                    ProductsData::Instance()->subProductsData(rowToAdd, SUBPROD_ID).toInt(),
                    error)) {
            QMessageBox::warning(this, tr("Error while adding subproduct to cart"), error);
        } else {
            //Update subproducts model
            prodWidget->selectSubProdsWithSelection();
        }
    });

    connect(ui->cartTable->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(handleSelection(QItemSelection)));

    connect(ui->removeFromCart, &QPushButton::clicked, [=] {
        if(!sellData->removeFromCart(rowToRemove)) {
            QMessageBox::warning(this, tr("Error"), tr("Couldn't delete subproduct from the cart "
                                                       "(no row with the number %1)").arg(rowToRemove));
        } else {
            //Update subproducts model
            prodWidget->selectSubProdsWithSelection();
        }
    });

    connect(ui->finishSale, &QPushButton::clicked, [=] {
        if(sellData->commitSale(0,0)) {
            ui->cartTable->selectionModel()->clearSelection();
        }
    });

    //Handle errors
    connect(sellData, &SellData::errorOccured, [=](QString errorText) {
        QMessageBox::warning(this, tr("Problems with sell module"), errorText);
    });

    //Handle sale was done
    connect(sellData, &SellData::saleDone, [=](int price) {
        // Reset subproducts amount once sale is finished
        prodWidget->selectSubProdsWithSelection();
        QMessageBox::information(this, tr("Sale is successfully finished"),
                                 tr("Sale for the price <b>%1</b> was succesfully added to database").arg(QString::number(price)));
    });

    connect(ui->addDiscount, SIGNAL(pressed()), this, SLOT(addDiscount()));
    connect(ui->removeDiscount, SIGNAL(pressed()), this, SLOT(deleteDiscount()));

    connect(ui->scanBarcodeButton, SIGNAL(pressed()), scanDialog, SLOT(show()));

}

void Sell::prepareSell() {
    prodWidget->prepareProducts();
}

void Sell::handleSelection(const QItemSelection &selected) {
    if(!selected.isEmpty()) {
        rowToRemove = selected.indexes()[0].row();
        ui->removeFromCart->setEnabled(true);
        ui->addDiscount->setEnabled(true);

        ui->removeDiscount->setEnabled(
                    SellData::Instance()->getCartModel()->hasDiscount(rowToRemove));
    } else {
        ui->removeFromCart->setEnabled(false);
        ui->addDiscount->setEnabled(false);
        ui->removeDiscount->setEnabled(false);
        rowToRemove = -1;
    }
}

void Sell::addDiscount() {
    CartModel* model = SellData::Instance()->getCartModel();
    if(rowToRemove >= 0) {
        QModelIndex indexPrice = model->index(rowToRemove, PRICE);
        double price = indexPrice.data().toDouble();

        bool ok;
        double value = QInputDialog::getInt(this, tr("Enter discount"), tr("Enter discount: "), 1, 1, price - 1, 1, &ok);
        if(ok) {
            if(!model->addDiscount(rowToRemove, value)) {
                QMessageBox::warning(this, tr("Error adding discount"),
                                     tr("Something went wrong. Discount was not added"));
                return;
            }

            ui->cartTable->resizeColumnsToContents();
        }
    } else {
        QMessageBox::warning(this, tr("No product is selected"),
                             tr("Please select a product in the cart to add a discount"));
    }
}

void Sell::deleteDiscount() {
    CartModel* model = SellData::Instance()->getCartModel();
    if(rowToRemove >= 0) {
        if(model->hasDiscount(rowToRemove)) {
            if(!model->deleteDiscount(rowToRemove)) {
                QMessageBox::warning(this, tr("Error deleting discount"),
                                     tr("Something went wrong. Discount was not deleted"));
                return;
            }
            ui->cartTable->resizeColumnsToContents();
        }
    } else {
        QMessageBox::warning(this, tr("No product is selected"),
                             tr("Please select a product in the cart to remove a discount"));
    }
}

void HTMLDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 options = option;
    initStyleOption(&options, index);

    painter->save();

    QTextDocument doc;
    doc.setHtml(options.text);

    options.text = "";
    options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

    // shift text right to make icon visible
    QSize iconSize = options.icon.actualSize(options.rect.size());
    painter->translate(options.rect.left()+iconSize.width(), options.rect.top());
    QRect clip(0, 0, options.rect.width()+iconSize.width(), options.rect.height());

    //doc.drawContents(painter, clip);

    painter->setClipRect(clip);
    QAbstractTextDocumentLayout::PaintContext ctx;
    // set text color to red for selected item
    //    if (option.state & QStyle::State_Selected)
    //        ctx.palette.setColor(QPalette::Text, QColor("red"));
    ctx.clip = clip;
    doc.documentLayout()->draw(painter, ctx);

    painter->restore();
}

QSize HTMLDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    //    initStyleOption(&option, index);

    QTextDocument doc;
    QString text = index.data().toString();
    doc.setHtml(text);
    doc.setTextWidth(option.fontMetrics.width(text));
    return QSize(doc.idealWidth(), doc.size().height());
}

Sell::~Sell()
{
    delete ui;
}
