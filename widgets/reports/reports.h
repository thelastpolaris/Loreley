#ifndef REPORTS_H
#define REPORTS_H

#include <QWidget>
#include <QItemSelection>

namespace Ui {
class Reports;
}

class Reports : public QWidget
{
    Q_OBJECT

public:
    explicit Reports(QWidget *parent = 0);
    ~Reports();

private slots:
    void updateSubProdsModel(const QItemSelection &, const QItemSelection &);

private:
    Ui::Reports *ui;
};

#endif // REPORTS_H
