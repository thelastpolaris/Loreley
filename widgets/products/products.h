#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <QWidget>
#include <QItemSelection>
#include <QSqlError>
#include <QSplitter>

class ComboBoxItemDelegate;
class AddProductDialog;
class AddSubProductDialog;
class ProductsModel;
class EditProperty;
class PropertiesModel;

namespace Ui {
    class Products;
}

class Products : public QWidget
{
    Q_OBJECT

public:
    static Products* Create();
    static Products* Instance() { return p_instance; }

protected:
    explicit Products();
    virtual ~Products() override;

public slots:
    void updateSubProducts(const QItemSelection & selected, const QItemSelection & deselected);
    void subProdSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    //Products
    bool startDeletingProduct();
    //Subproducts
    bool startDeletingSubProduct();
    //Edit properties
    void showEditCategories();
    void showEditColors();
    void showEditBrands();

    void showEditSizes();
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
    EditProperty* editPropertyWindow;

    PropertiesModel* propertiesModel;
};

#endif // PRODUCTS_H
