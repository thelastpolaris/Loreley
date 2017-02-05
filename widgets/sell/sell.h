#ifndef SELL_H
#define SELL_H

#include <QWidget>

namespace Ui {
class Sell;
}

class Products;

class Sell : public QWidget
{
    Q_OBJECT
public:
    explicit Sell(QWidget *parent = 0);
    void scanBarCode();
    ~Sell();

signals:
    void subProductSelected(bool selected, int row = -1);

private:
    QList<int> currentCart;
    Ui::Sell *ui;
    Products* prodWidget;

    /**
     * @brief rowToAdd - the id of row that is currently selected in subproducts table
     */
    int rowToAdd;

    /**
     * @brief rowToAdd - the id of row that is currently selected in cart table
     */
    int rowToRemove;
};

#endif // SELL_H
