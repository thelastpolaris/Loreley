#ifndef SEARCHPRODUCTDIALOG_H
#define SEARCHPRODUCTDIALOG_H

#include <QDialog>

namespace Ui {
class SearchProductDialog;
}

class SearchProductDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchProductDialog(QWidget *parent = 0);
    ~SearchProductDialog();

public slots:
    void startSearch();
    void finishSearch();

private:
    void clearFields();
    Ui::SearchProductDialog *ui;
};

#endif // SEARCHPRODUCTDIALOG_H
