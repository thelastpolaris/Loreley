#include "productsmodel.h"
#include <QSqlRecord>

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

QSqlRecord ProductsModel::getOriginalRecord(int row) const
{
    QSqlTableModel mod;
    QSqlRecord rec = QSqlQueryModel::record(row);

    mod.setTable(tableName());
    mod.select();
    mod.setFilter(QString("id = %2").arg(rec.value("id").toInt()));

    return mod.record(0);
}

void ProductsModel::setFilterByInteger(const QString &field, int value) {
    filterField = field;
    filterValue = value;
    setFilter(field + "='" + QString::number(value) + "'");
}
