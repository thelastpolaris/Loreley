#ifndef ADDPRODUCTDIALOG_H
#define ADDPRODUCTDIALOG_H

#include <QDialog>

namespace Ui {
class AddProductDialog;
}

class AddProductDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddProductDialog(QWidget *parent = 0);

    ~AddProductDialog();
    Ui::AddProductDialog *ui;
public slots:
    void finishAddingProduct();
    void startAddingProduct();

private:
    void clearFields();

};

#endif // ADDPRODUCTDIALOG_H
