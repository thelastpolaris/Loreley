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
    void setReadOnlyCols(QList<int> cols) { readOnlyCols = cols; }

    QString getFilterField() const { return filterField; }
    int getFilterValue() const { return filterValue; }
private:
    QList<int> readOnlyCols;
    QString filterField;
    int filterValue;
};

#endif // PRODUCTSMODEL_H
