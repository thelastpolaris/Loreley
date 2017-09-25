#include "home.h"
#include "ui_home.h"
#include "mainwindow.h"

#include <QDebug>

Home::Home(QWidget *parent) :
    QWidget(parent), ui(new Ui::Home)
{
    ui->setupUi(this);
    connect(ui->openProducts, SIGNAL(pressed()), MainWindow::Instance(), SLOT(openProducts()));
    /******* Separator ********/
    connect(ui->openSell, SIGNAL(pressed()), MainWindow::Instance(), SLOT(openSell()));
    connect(ui->openReports, SIGNAL(pressed()), MainWindow::Instance(), SLOT(openReports()));
    connect(ui->openClients, SIGNAL(pressed()), MainWindow::Instance(), SLOT(openClients()));
}

Home::~Home()
{
    delete ui;
}
