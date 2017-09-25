#include "addclientdialog.h"
#include "ui_addclientdialog.h"
#include "data/clientsdata.h"

#include <QMessageBox>

AddClientDialog::AddClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddClientDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(finishAddingClient()));
    connect(ui->buttonBox, SIGNAL(rejected()), SLOT(close()));
}

AddClientDialog::~AddClientDialog()
{
    delete ui;
}

void AddClientDialog::startAddingClient() {
    show();
}

void AddClientDialog::finishAddingClient() {
    ClientsData *client = ClientsData::Instance();

    QString name = ui->nameEdit->text();
    QString surname = ui->surnameEdit->text();
    QString fathersName = ui->fatherEdit->text();
    QDate DOB = ui->DOBEdit->date();
    QString phoneNum1 = ui->phone1Edit->text();
    QString phoneNum2 = ui->phone2Edit->text();
    QString instagram = ui->instagramEdit->text();
    QString email = ui->emailEdit->text();
    bool smsSend = ui->smsCB->isChecked();
    bool emailSend = ui->emailCB->isChecked();
    QString = ui->noteTextEdit->toPlainText();

    if(name.isEmpty()) {
        QMessageBox::warning(this, tr("Name was not entered"), tr("Please enter name of the client"));
        return;
    }

    if(surname.isEmpty()) {
        QMessageBox::warning(this, tr("Surname was not entered"), tr("Please enter surname of the client"));
        return;
    }

    if(fathersName.isEmpty()) {
        QMessageBox::warning(this, tr("Father's name was not entered"), tr("Please enter the name of the client's father"));
        return;
    }

    if(DOB.isValid()) {
        QMessageBox::warning(this, tr("Date of Birth is not valid"), tr("Please enter valid date of birth"));
        return;
    }

    if(phoneNum1.isEmpty()) {
        QMessageBox::warning(this, tr("Phone number was not entered"), tr("Please enter valid phone number"));
        return;
    }

    if(prod->addProduct(
                name,
                ui->categoryCB->currentData(),
                ui->priceEdit->text().remove(" ").toInt(),
                ui->colorCB->currentData(),
                ui->brandCB->currentData(),
                ui->noteTextEdit->toPlainText()))
    {
        close();
        QMessageBox::information(this,tr("Success!"), tr("Successfully added product <b>%1</b> to the database").arg(name));
    } else {
        QSqlError err = prod->productsLastError();
        QMessageBox::information(this,tr("Error!"), tr("Error - Product <b>%1</b> was not added to the database due to the following errors"
                                                       "<br> <b>Database Error:</b> %2"
                                                       "<br> <b>Driver Error:</b> %3").arg(name, err.databaseText(), err.driverText() ));
    }
}
}
