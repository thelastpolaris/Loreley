#include "editproperty.h"
#include "ui_editproperty.h"
#include "data/productsdata.h"

#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

EditProperty::EditProperty(QWidget *parent) :
    QDialog(parent), ui(new Ui::EditProperty), propertiesModel(this)
{
    ui->setupUi(this);

    connect(ProductsData::Instance(), &ProductsData::propertiesChanged, [=](const QString &tableName) {
        propertiesModel.setPropertiesList(ProductsData::Instance()->getNameAndKey(tableName, "id", "name"));
    });

    ui->propertiesLV->setModel(&propertiesModel);

    connect(ui->addProperty, SIGNAL(clicked()), SLOT(addProperty()));
    connect(ui->removeProperty, SIGNAL(clicked()), SLOT(removeProperty()));
    connect(ui->editProperty, SIGNAL(clicked()), SLOT(editPropertySlot()));
}

EditProperty::~EditProperty()
{
    delete ui;
}

void EditProperty::addProperty() {
    bool status;
    QString newPropName = QInputDialog::getText(this, tr("Add new %1").arg(propertySingular),
                          tr("Enter the name of new %1").arg(propertySingular), QLineEdit::Normal,
                          "", &status);
    if(status) {
        if(ProductsData::Instance()->addNewProperty(tableName, newPropName)) {
            QMessageBox::information(this, tr("Success!"), tr("Successfully added new property with the name %1").arg(newPropName));
            emit ProductsData::Instance()->propertiesChanged(tableName);
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

    if(!ui->propertiesLV->currentIndex().isValid()) return;
    QString delPropName = ui->propertiesLV->currentIndex().data().toString();
    int delIDProp = ui->propertiesLV->currentIndex().data(PropertiesModel::PropertyValue).toInt();
    int response = QMessageBox::question(this, tr("Delete property"),tr("Are you sure you want to delete <b>%1</b>?").arg(delPropName), QMessageBox::Ok, QMessageBox::Cancel);

    if(response == QMessageBox::Ok) {
        QList<int> idsProperty = prodData->getRowsWithProperty(fieldID, delIDProp);
        if(idsProperty.size()) {
            if(propertiesModel.rowCount() == 1) {
                QMessageBox::warning(this, tr("Cannot delete property"), tr("You are attempting to delete "
                                                                           "the last property, which is assigned to some products/subproducts. Please, add at least one more property"));
                return;
            }
            QStringList properties = propertiesModel.getStringList();
            properties.removeOne(delPropName);
            bool* assignStatus = new bool;
            QString newValText = QInputDialog::getItem(this, tr("Assign products/subproducts to new property"),
                                                       tr("It looks like there are some products or subproducts that has<br>"
                                                       "the property, which you want to delete. Please, choose new property<br>"
                                                       "for them. Otherwise, this property can not be deleted"), properties, 0, false, assignStatus);
            if(*assignStatus) {
                int newValue = propertiesModel.getValueFromText(newValText);
                prodData->setPropertyForProducts(idsProperty, fieldID, newValue);
            } else {
                return;
            }
        }

        if(prodData->removeProperty(tableName, delIDProp)) {
            QMessageBox::information(this, tr("Success!"), tr("Successfully deleted property <b>%1</b>").arg(delPropName));
            emit ProductsData::Instance()->propertiesChanged(tableName);
        } else {
            QSqlError err = QSqlDatabase::database().lastError();
            QMessageBox::information(this,tr("Error!"), tr("Error - property was not deleted due to the following errors"
                                                           "<br> <b>Database Error:</b> %1"
                                                           "<br> <b>Driver Error:</b> %2").arg(err.databaseText(), err.driverText() ));
        }
    }
}

void EditProperty::editPropertySlot() {
    ProductsData* prodData = ProductsData::Instance();
    if(!ui->propertiesLV->currentIndex().isValid()) return;

    QString propName = ui->propertiesLV->currentIndex().data().toString();

    bool status;
    QString newPropName = QInputDialog::getText(this, tr("Edit %1 property").arg(propName),
                          tr("Enter the new name"), QLineEdit::Normal,
                          propName, &status);
    if(status) {
        if(prodData->editProperty(tableName, propName, newPropName)) {
            QMessageBox::information(this, tr("Success!"), tr("Successfully changed name of property <b>%1</b> to <b>%2</b>").arg(propName, newPropName));
            emit ProductsData::Instance()->propertiesChanged(tableName);
            emit ProductsData::Instance()->propertyEdited();
        } else {
            QSqlError err = QSqlDatabase::database().lastError();
            QMessageBox::information(this,tr("Error!"), tr("Error - property was not changed due to the following errors"
                                                           "<br> <b>Database Error:</b> %1"
                                                           "<br> <b>Driver Error:</b> %2").arg(err.databaseText(), err.driverText() ));
        }
    }
}

void EditProperty::show(QString _propertySingular, QString _tableName, int _fieldID, bool _product)  {
    propertySingular = _propertySingular;
    tableName = _tableName;
    fieldID = _fieldID;
    product = _product;
    propertiesModel.setPropertiesList(ProductsData::Instance()->getNameAndKey(tableName, "id", "name"));
//    propertiesModel.sort();

//    if(color) {
//        ui->listView->setViewMode(QListView::IconMode);
//    }
//    else ui->listView->setViewMode(QListView::ListMode);
    QDialog::show();
}

