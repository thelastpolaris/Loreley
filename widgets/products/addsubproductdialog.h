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
    void startAddingSubProduct();

private:
    Ui::AddSubProductDialog *ui;
};

#endif // ADDSUBPRODUCTDIALOG_H
