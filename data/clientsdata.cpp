#include "clientsdata.h"
#include <QSqlRecord>

ClientsData* ClientsData::p_instance = 0;

ClientsData::ClientsData(QObject *parent)
    :QObject(parent)
{

}

int ClientsData::addClient(QString name, QString surname, QString fathersName, QDate DOB, QString phoneNum1,
                           QString phoneNum2, QString instagram, QString email, bool smsSend, bool emailSend,
                           QString note)
{
//#if (QT_VERSION > QT_VERSION_CHECK(5, 5, 1)) // We provide sql query version for old Qt versions where insertRecord always returns false
    QSqlRecord newRow = productsModel.record();
    newRow.setValue(CLIENT_NAME, QVariant(name));
    newRow.setValue(CLIENT_SURNAME, QVariant(surname));
    newRow.setValue(CLIENT_FATHERS_NAME, QVariant(fathersName));
    newRow.setValue(CLIENT_DOB, QVariant(DOB));
    newRow.setValue(CLIENT_PHONE_1, QVariant(phoneNum1));
    newRow.setValue(CLIENT_PHONE_2, QVariant(phoneNum2));
    newRow.setValue(CLIENT_EMAIL, QVariant(email));
    newRow.setValue(CLIENT_INSTAGRAM, QVariant(instagram));
    newRow.setValue(CLIENT_SMS_SEND, QVariant(smsSend));
    newRow.setValue(CLIENT_EMAIL_SEND, QVariant(emailSend));
    newRow.setValue(CLIENT_NOTE, QVariant(note));

    bool status = productsModel.insertRecord(-1, newRow);
    if(!status) {
        qDebug() << productsModel.lastError().text();
    }
    productsModel.select();
    // Clear all subproducts in subprod model because there will be no selection in products model
    filterSubProducts(-1);
    return status;
#else
    QSqlQuery addProduct;
    addProduct.prepare("INSERT INTO " + QString(PROD_TABLE) +
                       "(brand, category_id, name, color, price, note)"
                       "VALUES (:brand, :category, :name, :color, :price, :note)");
    addProduct.bindValue(":brand", brand);
    addProduct.bindValue(":category", QVariant(category));
    addProduct.bindValue(":name", QVariant(name));
    addProduct.bindValue(":color", QVariant(color));
    addProduct.bindValue(":price", QVariant(price));
    addProduct.bindValue(":note", QVariant(note));
    if(!addProduct.exec()) {
        qDebug() << addProduct.lastError().text();
    }
    productsModel.select();
    return addProduct.lastInsertId().toInt();
#endif
}

ClientsData* ClientsData::Create() {
    if(p_instance) delete p_instance;
    p_instance = new ClientsData;
    return p_instance;
}



