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

Sell::Sell(QWidget *parent) :
    QWidget(parent), ui(new Ui::Sell), prodWidget(new Products(true))
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

}

void Sell::prepareSell() {
    prodWidget->prepareProducts();
}

void Sell::keyPressEvent(QKeyEvent *event) {
    if(!event->text().isEmpty()) {
        qDebug() << event->text();
        if(event->text() == "\r") {
            qDebug() << barCode;
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

void Sell::scanBarCode() {
    //    VideoCapture cap(0); // open the video camera no. 0

    //    if (!cap.isOpened())  // if not success, exit program
    //    {
    //        cout << "Cannot open the video cam" << endl;
    //        return;
    //    }

    //    ImageScanner scanner;
    //    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    //    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    //    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    //    cout << "Frame size : " << dWidth << " x " << dHeight << endl;
    //    while (1)
    //    {
    //        Mat frame;

    //        bool bSuccess = cap.read(frame); // read a new frame from video

    //         if (!bSuccess) //if not success, break loop
    //        {
    //             cout << "Cannot read a frame from video stream" << endl;
    //             break;
    //        }

    //        Mat grey;
    //        cvtColor(frame,grey,CV_BGR2GRAY);

    //        int width = frame.cols;
    //        int height = frame.rows;
    //        uchar *raw = (uchar *)grey.data;
    //        // wrap image data
    //        Image image(width, height, "Y800", raw, width * height);
    //        // scan the image for barcodes
    //        int n = scanner.scan(image);
    //        // extract results
    //        for(Image::SymbolIterator symbol = image.symbol_begin();
    //        symbol != image.symbol_end();
    //        ++symbol) {
    //                vector<Point> vp;
    //        // do something useful with results
    //        cout << "decoded " << symbol->get_type_name()  << " symbol \"" << symbol->get_data() << '"' <<" "<< endl;
    //           int n = symbol->get_location_size();
    //           for(int i=0;i<n;i++){
    //                vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
    //           }
    //           RotatedRect r = minAreaRect(vp);
    //           Point2f pts[4];
    //           r.points(pts);
    //           for(int i=0;i<4;i++){
    //                line(frame,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
    //           }
    //           //cout<<"Angle: "<<r.angle<<endl;
    //        }

    ////        imshow("MyVideo", frame); //show the frame in "MyVideo" window

    //    }
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
