#ifndef CLIENTSDATA_H
#define CLIENTSDATA_H

#include <QObject>

#define CLIENT_ID 0
#define CLIENT_NAME 1
#define CLIENT_SURNAME 2
#define CLIENT_FATHERS_NAME 3
#define CLIENT_DOB 4
#define CLIENT_PHONE_1 5
#define CLIENT_PHONE_2 6
#define CLIENT_EMAIL 7
#define CLIENT_INSTAGRAM 8
#define CLIENT_SMS_SEND 9
#define CLIENT_EMAIL_SEND 10
#define CLIENT_NOTE 11

class ClientsData : public QObject
{
    Q_OBJECT
public:
    static ClientsData* Create();
    static ClientsData* Instance() { return p_instance; }

    int addClient(QString name, QString surname, QString fathersName, QDate DOB, QString phoneNum1,
                  QString phoneNum2, QString instagram, QString email, bool smsSend, bool emailSend,
                  QString note);
signals:

protected:
    explicit ClientsData(QObject *parent = 0);

private:
    static ClientsData *p_instance;
};

#endif // ClientsData_H
