#include "clients.h"
#include "ui_clients.h"
#include "addclientdialog.h"
#include "data/clientsdata.h"
#include "data/models/productsmodel.h"
#include <QMessageBox>
#include <QDialogButtonBox>

Clients::Clients(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clients), addClientDialog(new AddClientDialog(this))
{
    ui->setupUi(this);

    connect(ui->addClientButton, SIGNAL(clicked(bool)), addClientDialog, SLOT(startAddingClient()));
    connect(ui->deleteClientButton, SIGNAL(clicked(bool)), SLOT(startDeletingClient()));

    ClientsData* clientsData = ClientsData::Instance();

    ui->clientsTable->setModel(clientsData->getClientsModel());

    connect(this, SIGNAL(clientSelected(bool)), ui->deleteClientButton, SLOT(setEnabled(bool)));
}

bool Clients::startDeletingClient() {
    QModelIndex currentIndex = ui->clientsTable->selectionModel()->currentIndex();
    ClientsData *clientData = ClientsData::Instance();

    if(currentIndex.isValid()) {
        int delRow = currentIndex.row();
        QString name = clientData->clientsData(delRow, CLIENT_NAME).toString();

        int response = QMessageBox::question(this, tr("Delete product"),tr("Are you sure you want to delete <b>%1</b> ?").arg(name), QDialogButtonBox::Ok, QDialogButtonBox::Cancel);
        if(response == QMessageBox::Ok) {
            clientData->removeClient(delRow);
            ui->clientsTable->selectRow(delRow); // Select next row
            if(!clientData->hasProducts()) {
                emit clientSelected(false);
            }
            QMessageBox::information(this, tr("Success!"), tr("Successfully deleted product <b>%1</b>").arg(name));
            return true;
        }
    } else {
        QMessageBox::warning(this, tr("Error - No product selected"), tr("No product was selected to delete"));
    }
    return false; // Row was not deleted
}

Clients::~Clients()
{
    delete ui;
}
