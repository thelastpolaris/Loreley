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
class SearchProductDialog;

namespace Ui {
    class Products;
}

class Products : public QWidget
{
    Q_OBJECT

public:
    explicit Products(bool _saleMode = false);
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

    /**
     * @brief updateFilter - updates visibility of "Cancel Filter" button and text of filter.
     * Makes button and text invisible if newFilter is empty.
     */
    void updateFilter(QString newFilter);

    void selectSubProdsWithSelection();
signals:
    void productSelected(bool selected);
    void subProductSelected(bool selected, int row = -1);

private:
    Ui::Products* ui;

    AddProductDialog* addProdDialog;
    AddSubProductDialog* addSubProdDialog;
    ReduceSubProductDialog* reduceSubProdDialog;
    SearchProductDialog* searchProdDialog;

    EditProperty* editPropertyWindow;

    /**
     * @brief saleMode - true if don't need to display add/delete product/subproduct and print buttons
     */
    bool saleMode;
};

#endif // PRODUCTS_H
