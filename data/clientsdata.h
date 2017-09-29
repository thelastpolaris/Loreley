#ifndef CLIENTSDATA_H
#define CLIENTSDATA_H

#include <QObject>
#include <QSqlError>
#include "models/productsmodel.h"

#define CLIENT_ID 0
#define CLIENT_NAME 1
#define CLIENT_SURNAME 2
#define CLIENT_FATHERS_NAME 3
#define CLIENT_DOB 4
#define CLIENT_PHONE_1 5
#define CLIENT_PHONE_2 6
#define CLIENT_EMAIL 7
#define CLIENT_INSTAGRAM 8
#define CLIENT_NOTE 9

#define CLIENT_TABLE "clients"

class ProductsModel;

class ClientsData : public QObject
{
    Q_OBJECT
public:
    static ClientsData* Create();
    static ClientsData* Instance() { return p_instance; }

    bool addClient(QString id, QString name, QString surname, QString fathersName, QDate DOB, QString phoneNum1,
                  QString phoneNum2, QString instagram, QString email, QString note);

    void initModels();
    ProductsModel* getClientsModel() { return clientsModel; }
    QSqlError clientsLastError() const { return clientsModel->lastError(); }

    QVariant clientsData(int row, int column) const;

    bool removeClient(int clientRow);
    bool hasProducts();

signals:

protected:
    explicit ClientsData(QObject *parent = 0);

private:
    ProductsModel* clientsModel;
    static ClientsData *p_instance;
};

#endif // ClientsData_H
