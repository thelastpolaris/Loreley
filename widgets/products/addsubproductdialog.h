#ifndef ADDSUBPRODUCTDIALOG_H
#define ADDSUBPRODUCTDIALOG_H

#include <QDialog>

namespace Ui {
class AddSubProductDialog;
}

class AddSubProductDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddSubProductDialog(QWidget *parent = 0);
    void clearFields();

    ~AddSubProductDialog();
public slots:
    void finishAddingSubProduct();
    /**
     * @brief startAddingSubProduct - if size is not empty pre-select the size in sizes dropbox
     */
    void startAddingSubProduct(QString size = "");

private:
    Ui::AddSubProductDialog *ui;
};

#endif // ADDSUBPRODUCTDIALOG_H
