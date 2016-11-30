#ifndef REDUCESUBPRODUCTDIALOG_H
#define REDUCESUBPRODUCTDIALOG_H

#include <QDialog>

namespace Ui {
class ReduceSubProductDialog;
}

class ReduceSubProductDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReduceSubProductDialog(QWidget *parent = 0);
    void startReducingSubProduct(int _subProdID, int _curAmount);
    ~ReduceSubProductDialog();
public slots:
    void updateSubProdsLeft();
    void finishReducingSubProduct();
private:
    int curAmount;
    int subProdID;
    Ui::ReduceSubProductDialog *ui;
};

#endif // REDUCESUBPRODUCTDIALOG_H
