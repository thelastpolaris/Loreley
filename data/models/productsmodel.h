#ifndef PRODUCTSMODEL_H
#define PRODUCTSMODEL_H

#include <QSqlRelationalTableModel>

class ProductsModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    ProductsModel(QObject *parent = 0);
    virtual ~ProductsModel() { }
    void setFilterByInteger(const QString &field, int value);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

    /**
     * @brief getOriginalRecord - returns record of table without setting relations to other tables
     * @param row - number of row in original row (-1 will return default row)
     * @return row from original table (without relations)
     */
    QSqlRecord getOriginalRecord(int row) const;

    void setReadOnlyCols(QList<int> cols) { readOnlyCols = cols; }

    QString getFilterField() const { return filterField; }
    /**
     * @brief selectStat - use for debug purposes
     */
    QString selectStat() { return selectStatement(); }
    int getFilterValue() const { return filterValue; }

    /**
     * @param productID - id of parent product
     * @param column - e.g PROD_ID
     * @return row number of product with id productID
     */
    int rowByID(int productID, int column);

private:
    QList<int> readOnlyCols;
    QString filterField;
    int filterValue;
};

#endif // PRODUCTSMODEL_H
