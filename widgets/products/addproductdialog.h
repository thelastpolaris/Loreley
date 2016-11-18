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
    void clearFields();

    ~AddProductDialog();
    Ui::AddProductDialog *ui;
public slots:
    void finishAddingProduct();
    void startAddingProduct();

};

#endif // ADDPRODUCTDIALOG_H
