#include "sellingsmodel.h"
#include <QDateTime>

SellingsModel::SellingsModel(QObject* parent)
    :QSqlTableModel(parent)
{
    setTable("sells");

    setHeaderData(SELLING_ID, Qt::Horizontal, tr("Selling ID"));
    setHeaderData(SELLING_TIME, Qt::Horizontal, tr("Time"));
    setHeaderData(SELLING_PRICE, Qt::Horizontal, tr("Price"));
    setHeaderData(SELLING_CLIENT, Qt::Horizontal, tr("Client"));

    select();
}

void SellingsModel::setDate(QDate date)
{
    QString dateFilter = QString("DATE(date)='%1'").arg(date.toString("yyyy-MM-dd"));
    setFilter(dateFilter);
}

QVariant SellingsModel::data(const QModelIndex &item, int role) const
{
    if(role == Qt::DisplayRole) {
        if(item.column() == SELLING_TIME) {
            QVariant dateTime = QSqlTableModel::data(index(item.row(), SELLING_TIME), role);
            QTime time = dateTime.toDateTime().time();

            return QVariant(time);
        }
    }
    return QSqlTableModel::data(item, role);
}

int SellingsModel::getTotalPrice() const
{
    int totalPrice = 0;
    for(int i = 0; i < rowCount(); i++) {
        totalPrice += data(index(i, SELLING_PRICE)).toInt();
    }
    return totalPrice;
}
