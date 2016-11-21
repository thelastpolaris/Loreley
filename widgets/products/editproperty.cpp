#include "editproperty.h"
#include "ui_editproperty.h"
#include "data/productsdata.h"

#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

EditProperty::EditProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditProperty)
{
    ui->setupUi(this);
    connect(ui->addProperty, SIGNAL(clicked(bool)), SLOT(addProperty()));
    connect(ui->removeProperty, SIGNAL(clicked()), SLOT(removeProperty()));
}

EditProperty::~EditProperty()
{
    delete ui;
}

void EditProperty::addProperty() {
    bool status;
    QString newPropName = QInputDialog::getText(this, tr("Add new %1").arg(propertyName),
                          tr("Enter the name of new %1").arg(propertyName), QLineEdit::Normal,
                          "", &status);
    if(status) {
        if(ProductsData::Instance()->addNewProperty(tableName, newPropName)) {
            QMessageBox::information(this, tr("Success!"), tr("Successfully added new property with the name %1").arg(newPropName));
            emit propertiesChanged(tableName);
        } else {
            QSqlError err = QSqlDatabase::database().lastError();
            QMessageBox::information(this,tr("Error!"), tr("Error - property was not added to the database due to the following errors"
                                                           "<br> <b>Database Error:</b> %1"
                                                           "<br> <b>Driver Error:</b> %2").arg(err.databaseText(), err.driverText() ));
        }
    }
}

void EditProperty::removeProperty() {
    ProductsData* prodData = ProductsData::Instance();

    bool status;
    QString delPropName = ui->listView->currentIndex().data().toString();
    int response = QMessageBox::question(this, tr("Delete property"),tr("Are you sure you want to delete <b>%1</b>?").arg(delPropName), QMessageBox::Ok, QMessageBox::Cancel);
    if(response == QMessageBox::Ok) {
        if(prodData->removeProperty(tableName, delPropName)) {
            QMessageBox::information(this, tr("Success!"), tr("Successfully deleted property <b>%2</b>").arg(delPropName));
            emit propertiesChanged(tableName);
        } else {
            QSqlError err = QSqlDatabase::database().lastError();
            QMessageBox::information(this,tr("Error!"), tr("Error - property was not deleted to the database due to the following errors"
                                                           "<br> <b>Database Error:</b> %1"
                                                           "<br> <b>Driver Error:</b> %2").arg(err.databaseText(), err.driverText() ));
        }
    }
}

void EditProperty::show(QString _propertyName, QString _tableName, bool color)  {
    propertyName = _propertyName;
    tableName = _tableName;
//    if(color) {
//        ui->listView->setViewMode(QListView::IconMode);
//    }
//    else ui->listView->setViewMode(QListView::ListMode);
    QDialog::show();
}

