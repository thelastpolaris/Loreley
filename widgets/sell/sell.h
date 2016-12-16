#ifndef SELL_H
#define SELL_H

#include <QWidget>

namespace Ui {
class Sell;
}

class Sell : public QWidget
{
    Q_OBJECT
public:
    explicit Sell(QWidget *parent = 0);
    ~Sell();

private:
    Ui::Sell *ui;
};

#endif // SELL_H
