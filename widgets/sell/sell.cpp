#include "sell.h"
#include "ui_sell.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>

Sell::Sell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sell)
{
    ui->setupUi(this);
    connect(ui->openCamera, &QPushButton::clicked, this, &Sell::scanBarCode);
}

void Sell::scanBarCode() {
    QCamera* camera = new QCamera;
    camera->start();

    QCameraViewfinder* viewfinder = new QCameraViewfinder();
    viewfinder->show();

    camera->setViewfinder(viewfinder);

    QCameraImageCapture* imageCapture = new QCameraImageCapture(camera);
}

Sell::~Sell()
{
    delete ui;
}
