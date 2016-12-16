#include "sell.h"
#include "ui_sell.h"

Sell::Sell(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sell)
{
    ui->setupUi(this);
}

Sell::~Sell()
{
    delete ui;
}
