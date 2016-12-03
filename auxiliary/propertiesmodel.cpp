#include "propertiesmodel.h"

PropertiesModel::PropertiesModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

QHash<int, QByteArray> PropertiesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";
    roles[PropertyValue] = "value";
    return roles;
}

QVariant PropertiesModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    if(role == Qt::DisplayRole) {
        return QVariant(propertiesList[index.row()].second);
    } else if(role == PropertyValue) {
        return QVariant(propertiesList[index.row()].first);
    }
    return QVariant();
}

void PropertiesModel::setPropertiesList(QVector<QPair<int, QString>> _propertiesList) {
    emit beginResetModel();
    propertiesList = _propertiesList;

    emit endResetModel();
}

void PropertiesModel::setPropertiesList(QHash<int, QString> _propertiesList) {
    emit beginResetModel();

    propertiesList.clear();
    for(QHash<int, QString>::iterator i = _propertiesList.begin(); i != _propertiesList.end(); ++i) {
        propertiesList.append(QPair<int, QString>(i.key(), i.value()));
    }

    emit endResetModel();
}

QStringList PropertiesModel::getStringList() {
    QStringList stringList;
    for(int i = 0; i < propertiesList.size(); ++i) {
        stringList.append(propertiesList[i].second);
    }
    return stringList;
}

int PropertiesModel::getValueFromText(QString text) {
    for(int i = 0; i < propertiesList.size(); ++i) {
        if(propertiesList[i].second == text) {
            return propertiesList[i].first;
        }
    }
    return -1; // If nothing was found
}
