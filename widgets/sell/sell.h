#ifndef SELL_H
#define SELL_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QItemSelection>

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
    Products* getProdWidget() { return prodWidget; }

    void prepareSell();
    void keyPressEvent(QKeyEvent *event);
signals:
    void subProductSelected(bool selected, int row = -1);

public slots:
    void addDiscount();
    void deleteDiscount();
    void handleSelection(const QItemSelection &selected);
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

    /// Stores barcode that is being read
    QString barCode;
};


class HTMLDelegate : public QStyledItemDelegate
{
protected:
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif // SELL_H
