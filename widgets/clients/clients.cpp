#include "clients.h"
#include "ui_clients.h"
#include "addclientdialog.h"

Clients::Clients(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clients), addClientDialog(new AddClientDialog(this))
{
    ui->setupUi(this);

    connect(ui->addClientButton, SIGNAL(clicked(bool)), addClientDialog, SLOT(startAddingClient()));
//    connect(ui->deleteClientButton, SIGNAL(clicked(bool)), SLOT(startDeletingProduct()));
}

Clients::~Clients()
{
    delete ui;
}
