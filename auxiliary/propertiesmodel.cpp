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
        return propMap.keys().at(index.row());
    } else if(role == PropertyValue) {
        return propMap.values().at(index.row());
    }
    return QVariant();
}

void PropertiesModel::setPropertiesList(QHash<int, QString> _propertiesList) {
    emit beginResetModel();

    propMap.clear();
    for(QHash<int, QString>::iterator i = _propertiesList.begin(); i != _propertiesList.end(); ++i) {
        propMap.insert(i.value(), i.key());
    }

    emit endResetModel();
}

QStringList PropertiesModel::getStringList() {
    QStringList stringList(propMap.keys());
    return stringList;
}

int PropertiesModel::getValueFromText(QString text) {
    return propMap.value(text);
}
