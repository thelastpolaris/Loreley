#include "productsmodel.h"

ProductsModel::ProductsModel(QObject *parent)
    :QSqlRelationalTableModel(parent)
{

}

Qt::ItemFlags ProductsModel::flags(const QModelIndex &index) const {
    if(readOnlyCols.indexOf(index.column()) != -1) { // If true then column is in the list of readOnly cols
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    } else {
        return QSqlRelationalTableModel::flags(index);
    }
}

void ProductsModel::setFilterByInteger(const QString &field, int value) {
    filterField = field;
    filterValue = value;
    setFilter(field + "='" + QString::number(value) + "'");
}
