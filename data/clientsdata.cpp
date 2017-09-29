#include "clientsdata.h"
#include <QSqlRecord>
#include <QDate>
#include <QDebug>
#include <QSqlQuery>

ClientsData* ClientsData::p_instance = 0;

ClientsData::ClientsData(QObject *parent)
    :QObject(parent), clientsModel(new ProductsModel(this))
{
    initModels();
}

void ClientsData::initModels()
{
    //Setup productsModel
    clientsModel->setTable("clients");
    clientsModel->setEditStrategy(QSqlRelationalTableModel::OnFieldChange);
    //Name headers
    clientsModel->setHeaderData(CLIENT_NAME, Qt::Horizontal, tr("Name"));
    clientsModel->setHeaderData(CLIENT_SURNAME, Qt::Horizontal, tr("Surname"));
    clientsModel->setHeaderData(CLIENT_FATHERS_NAME, Qt::Horizontal, tr("Father's Name"));
    clientsModel->setHeaderData(CLIENT_DOB, Qt::Horizontal, tr("Date of Birth"));
    clientsModel->setHeaderData(CLIENT_PHONE_1, Qt::Horizontal, tr("Tel. 1"));
    clientsModel->setHeaderData(CLIENT_PHONE_2, Qt::Horizontal, tr("Tel. 2"));
    clientsModel->setHeaderData(CLIENT_EMAIL, Qt::Horizontal, tr("EMail"));
    clientsModel->setHeaderData(CLIENT_INSTAGRAM, Qt::Horizontal, tr("Instagram"));
    clientsModel->setHeaderData(CLIENT_NOTE, Qt::Horizontal, tr("Note"));
    //Setup readonly columns
    clientsModel->setReadOnlyCols(QList<int>{CLIENT_ID});
    //Select all products
    clientsModel->select();
}

bool ClientsData::addClient(QString id, QString name, QString surname, QString fathersName, QDate DOB, QString phoneNum1,
                            QString phoneNum2, QString instagram, QString email, QString note)
{
    QSqlQuery addClient;
    addClient.prepare("INSERT INTO " + QString(CLIENT_TABLE) +
                      "(id, name, surname, fathers_name, birthdate, phone_1, phone_2,"
                      "email, instagram, note)"
                      "VALUES (:id, :name, :surname, :fathers_name, :birthdate, :phone_1, :phone_2,"
                      ":email, :instagram, :note)");
    addClient.bindValue(":id", QVariant(id));
    addClient.bindValue(":name", QVariant(name));
    addClient.bindValue(":surname", QVariant(surname));
    addClient.bindValue(":fathers_name", QVariant(fathersName));
    addClient.bindValue(":birthdate", QVariant(DOB));
    addClient.bindValue(":phone_1", QVariant(phoneNum1));
    addClient.bindValue(":phone_2", QVariant(phoneNum2));
    addClient.bindValue(":email", QVariant(email));
    addClient.bindValue(":instagram", QVariant(instagram));
    addClient.bindValue(":note", QVariant(note));
    bool success = addClient.exec();

    if(!success) {
        qDebug() << addClient.lastError().text();
    }
    clientsModel->select();
    return success;
}

QVariant ClientsData::clientsData(int row, int column) const {
    return clientsModel->data(clientsModel->index(row,column));
}

bool ClientsData::removeClient(int clientRow) {
    bool status = clientsModel->removeRow(clientRow);
    clientsModel->select();
    return status;
}

bool ClientsData::hasProducts() {
    return clientsModel->rowCount() > 0;
}

ClientsData* ClientsData::Create() {
    if(p_instance) delete p_instance;
    p_instance = new ClientsData;
    return p_instance;
}
