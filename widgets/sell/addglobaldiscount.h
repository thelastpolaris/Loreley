#ifndef ADDGLOBALDISCOUNT_H
#define ADDGLOBALDISCOUNT_H

#include <QDialog>

namespace Ui {
class AddGlobalDiscount;
}

class AddGlobalDiscount : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
public:
    explicit AddGlobalDiscount(QWidget *parent = 0);
    ~AddGlobalDiscount();

    bool isActive() const { return m_isActive; }
    void setMaxDiscountVal(int maxDiscount);

    void setActive(bool isActive);
    void showDialog();
signals:
    void discountSet(bool isPercent, double value);
    void activeChanged(bool isActive);

private:
    Ui::AddGlobalDiscount *ui;
    bool m_isActive;
};

#endif // ADDGLOBALDISCOUNT_H
