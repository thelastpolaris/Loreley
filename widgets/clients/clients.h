#ifndef CLIENTS_H
#define CLIENTS_H

#include <QWidget>

namespace Ui {
class Clients;
}

class AddClientDialog;

class Clients : public QWidget
{
    Q_OBJECT

public:
    explicit Clients(QWidget *parent = 0);
    ~Clients();

private slots:
    bool startDeletingClient();

signals:
    void clientSelected(bool isClientSelected);

private:
    Ui::Clients *ui;
    AddClientDialog* addClientDialog;
};

#endif // CLIENTS_H
