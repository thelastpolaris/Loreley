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

private:
    AddClientDialog* addClientDialog;

    Ui::Clients *ui;
};

#endif // CLIENTS_H
