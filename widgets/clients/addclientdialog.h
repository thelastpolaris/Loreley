#ifndef ADDCLIENTDIALOG_H
#define ADDCLIENTDIALOG_H

#include <QDialog>

namespace Ui {
class AddClientDialog;
}

class AddClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddClientDialog(QWidget *parent = 0);
    ~AddClientDialog();

private slots:
    void startAddingClient();
    void finishAddingClient();

private:
    Ui::AddClientDialog *ui;
};

#endif // ADDCLIENTDIALOG_H
