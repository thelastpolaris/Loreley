#include "cartmodel.h"

CartModel::CartModel(QObject* parent)
    :QAbstractTableModel(parent), columns(COLS_NUMBER), m_price(0), m_discounts(0)
{

}

int CartModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return columns[0].size();
}

bool CartModel::hasDiscount(int row) {
    if(row >= 0 && row < rowCount()) {
        return columns[DISCOUNT][row].toDouble() > 0;
    }
    return false;
}

void CartModel::addToCart(QString category, QString name, QString size, QString price, int id) {
    emit beginInsertRows(QModelIndex(), columns[0].size(), columns[0].size());
    columns[CATEGORY].append(category);
    columns[NAME].append(name);
    columns[SIZE].append(size);
    columns[PRICE].append(price);
    columns[DISCOUNT].append(QString::number(0));
    columns[ID].append(QString::number(id));

    if(IDsWithAmount.contains(id)) {
        IDsWithAmount[id]++;
    } else {
        IDsWithAmount.insert(id, 1);
    }
    emit IDsWithAmountChanged(IDsWithAmount);

    setProperty("price", m_price + price.toDouble());
    emit endInsertRows();
}

bool CartModel::removeFromCart(int row) {
    if(row >= 0 && row < columns[0].size()) {
        emit beginRemoveRows(QModelIndex(), row, row);
        columns[CATEGORY].remove(row);
        columns[NAME].remove(row);
        columns[SIZE].remove(row);

        double price = columns[PRICE][row].toDouble();
        columns[PRICE].remove(row);
        double discount = columns[DISCOUNT][row].toDouble();
        columns[DISCOUNT].remove(row);

        int id = columns[ID][row].toInt();
        if(IDsWithAmount.contains(id)) {
            if(IDsWithAmount[id] == 1) {
                IDsWithAmount.remove(id);
            } else {
                IDsWithAmount[id]--;
            }
            emit IDsWithAmountChanged(IDsWithAmount);
        }

        columns[ID].remove(row);

        setProperty("price", m_price - price);
        setProperty("discounts", m_discounts - discount);

        emit endRemoveRows();
        return true;
    }
    return false;
}

void CartModel::clearCart() {
    emit beginResetModel();
    for(int i = 0; i < columns.size(); ++i) columns[i].clear();

    IDsWithAmount.clear();
    setProperty("price", 0);
    setProperty("discounts", 0);

    emit endResetModel();
}

QVariant CartModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case CATEGORY:
                return tr("Category");
            case NAME:
                return tr("Name");
            case SIZE:
                return tr("Size");
            case PRICE:
                return tr("Price");
            }
        }
    }
    return QVariant();
}

int CartModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return columns.size() - 2; //1 is for ID and Discount (we don't want to show them)
}

QModelIndex CartModel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(parent);
    if(row >= 0 && row < rowCount() && column >= 0 && column <= COLS_NUMBER) {
        return createIndex(row, column);
    }
    return QModelIndex();
}

QVariant CartModel::data(const QModelIndex& index, int role) const {
    if(index.isValid() && role == Qt::DisplayRole) {
        double discount = columns[DISCOUNT][index.row()].toDouble();
        if(index.column() == PRICE && discount != 0) {
            double finalPrice = columns[PRICE][index.row()].toDouble() - discount;
            return QString("<s>%1</s> %2 <font color=#006400>-%3</font>").arg(columns[PRICE][index.row()], QString::number(finalPrice), columns[DISCOUNT][index.row()]);
        }
        return columns[index.column()][index.row()];
    }
    return QVariant::Invalid;
}

QVariant CartModel::data(int row, int column, int role) const {
    QModelIndex _index = index(row, column);
    if(!_index.isValid()) return QVariant();
    return data(_index, role);
}

bool CartModel::addDiscount(int row, double discount) {
    if(row >= 0 && row < rowCount() && discount > 0) {
        emit beginResetModel();
        double oldDisc = columns[DISCOUNT][row].toDouble();
        columns[DISCOUNT][row] = QString::number(discount);
        // We subtract old discount to handle cases when discount is edited and not added
        setProperty("discounts", m_discounts - oldDisc + discount);
        emit endResetModel();
        return true;
    }
    return false;
}

bool CartModel::deleteDiscount(int row) {
    if(row >= 0 && row < rowCount()) {
        double discount = columns[DISCOUNT][row].toDouble();
        if(discount > 0) {
           emit beginResetModel();
           columns[DISCOUNT][row] = QString::number(0);
           setProperty("discounts", m_discounts - discount);
           emit endResetModel();
           return true;
        }
    }
    return false;
}

//bool CartModel::setData(const QModelIndex & index, const QVariant & value, int role)
//{
////    if (role == Qt::EditRole)
////    {
////        //save value from editor to member m_gridData
////        m_gridData[index.row()][index.column()] = value.toString();
////        //for presentation purposes only: build and emit a joined string
////        QString result;
////        for(int row= 0; row < ROWS; row++)
////        {
////            for(int col= 0; col < COLS; col++)
////            {
////                result += m_gridData[row][col] + " ";
////            }
////        }
////        emit editCompleted( result );
////    }
////    return true;
//}

