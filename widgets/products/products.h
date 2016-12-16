#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <QWidget>
#include <QItemSelection>
#include <QSqlError>
#include <QSplitter>

class ComboBoxItemDelegate;
class ProductsModel;
class EditProperty;

class AddProductDialog;
class AddSubProductDialog;
class ReduceSubProductDialog;

namespace Ui {
    class Products;
}

class Products : public QWidget
{
    Q_OBJECT

public:
    explicit Products();
    virtual ~Products() override;

public slots:
    void updateSubProducts(const QItemSelection & selected = QItemSelection(), const QItemSelection & deselected = QItemSelection());
    void subProdSelectionChanged(const QItemSelection & selected = QItemSelection(), const QItemSelection & deselected = QItemSelection());
    //Products
    bool startDeletingProduct();
    //Subproducts
    bool startDeletingSubProduct();
    bool starReducingSubProductAmount();

    void setupPropertyDelegates(QString propTableName);

    void startPrintingBarcode();

signals:
    void productSelected(bool selected);
    void subProductSelected(bool selected);

private:
    static Products* p_instance;

    Ui::Products* ui;

    AddProductDialog* addProdDialog;
    AddSubProductDialog* addSubProdDialog;
    ReduceSubProductDialog* reduceSubProdDialog;

    EditProperty* editPropertyWindow;
};

#endif // PRODUCTS_H
