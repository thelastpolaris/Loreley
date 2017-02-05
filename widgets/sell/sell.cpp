#include "sell.h"
#include "ui_sell.h"

#include "data/selldata.h"
#include "data/productsdata.h"

#include "../products/products.h"
#include <QMessageBox>
#include <QDebug>

Sell::Sell(QWidget *parent) :
    QWidget(parent), ui(new Ui::Sell), prodWidget(new Products(true))
{
    ui->setupUi(this);
    ui->splitter->insertWidget(0, prodWidget);
    ui->cartTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->cartTable->horizontalHeader()->setStretchLastSection(true);
    ui->splitter->setStretchFactor(0, 2);

    //Set width of 'Price' label
    ui->labelPrice->setMaximumWidth(ui->labelPrice->fontMetrics().width(ui->labelPrice->text()));

    ui->cartTable->setModel(SellData::Instance()->getCartModel());

    //Set signals
    connect(SellData::Instance(), &SellData::priceChanged, [=](int changedPrice) {
        ui->price->setText(QString::number(changedPrice));
    });

    connect(prodWidget, &Products::subProductSelected, [=](bool selected, int row) {
        if(selected) {
            ui->addToCart->setEnabled(true);
            rowToAdd = row;
        } else {
            ui->addToCart->setEnabled(false);
            rowToAdd = -1;
        }
    });

    connect(ui->addToCart, &QPushButton::clicked, [=] {
        QString error;
        if(!SellData::Instance()->addToCart(
                    ProductsData::Instance()->subProductsData(rowToAdd, SUBPROD_ID).toInt(),
                    error)) {
            QMessageBox::warning(this, tr("Error while adding subproduct to cart"), error);
        }
    });

    connect(ui->cartTable->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected) {
        if(!selected.isEmpty()) {
            rowToRemove = selected.indexes()[0].row();
            ui->removeFromCart->setEnabled(true);
        } else {
            ui->removeFromCart->setEnabled(false);
            rowToRemove = -1;
        }
    });

    connect(ui->removeFromCart, &QPushButton::clicked, [=] {
        if(!SellData::Instance()->removeFromCart(rowToRemove)) {
            QMessageBox::warning(this, tr("Error"), tr("Couldn't delete subproduct from the cart "
                                                       "(no row with the number %1").arg(rowToRemove));
        }
    });
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

Sell::~Sell()
{
    delete ui;
}
