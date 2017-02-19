#include "productsmodel.h"
#include <QSqlRecord>

ProductsModel::ProductsModel(QObject *parent)
    :QSqlRelationalTableModel(parent), filterValue(-1)
{

}

Qt::ItemFlags ProductsModel::flags(const QModelIndex &index) const {
    if(readOnlyCols.indexOf(index.column()) != -1) { // If true then column is in the list of readOnly cols
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    } else {
        return QSqlRelationalTableModel::flags(index);
    }
}

QSqlRecord ProductsModel::getOriginalRecord(int row) const {
    QSqlTableModel mod;
    mod.setTable(tableName());
    mod.select();

    if(row <= -1) {
        QSqlRecord rec = mod.record();
        return rec;
    } else {
        QSqlRecord rec = QSqlQueryModel::record(row);
        mod.setFilter(QString("id = %2").arg(rec.value("id").toInt()));
        return mod.record(0);
    }
}

void ProductsModel::setFilterByInteger(const QString &field, int value) {
    filterField = field;
    filterValue = value;
    setFilter(field + "='" + QString::number(value) + "'");
}
