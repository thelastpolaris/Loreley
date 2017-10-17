#ifndef ADDGLOBALDISCOUNT_H
#define ADDGLOBALDISCOUNT_H

#include <QDialog>

namespace Ui {
class AddGlobalDiscount;
}

class AddGlobalDiscount : public QDialog
{
    Q_OBJECT

public:
    explicit AddGlobalDiscount(QWidget *parent = 0);
    ~AddGlobalDiscount();

    bool isActive();
    void setMaxDiscountVal(int maxDiscount);

private:
    Ui::AddGlobalDiscount *ui;
    bool m_isActive;
};

#endif // ADDGLOBALDISCOUNT_H
