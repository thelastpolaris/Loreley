#ifndef PROPERTIESMODEL_H
#define PROPERTIESMODEL_H
#include <QAbstractListModel>

class PropertiesModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum DemoRoles {
        PropertyValue = Qt::UserRole + 1,
    };

    explicit PropertiesModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const { return propMap.size(); }
    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual QHash<int, QByteArray> roleNames() const;
//    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    void setPropertiesList(QHash<int, QString> _propertiesList);
    QStringList getStringList();
    int getValueFromText(QString text);

private:
    QMap<QString, int> propMap;

};
#endif // PROPERTIESMODEL_H
