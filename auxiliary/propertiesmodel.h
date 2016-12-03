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

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const { return propertiesList.size(); }
    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual QHash<int, QByteArray> roleNames() const;

    void setPropertiesList(QVector<QPair<int, QString>> _propertiesList);
    void setPropertiesList(QHash<int, QString> _propertiesList);
    QStringList getStringList();
    int getValueFromText(QString text);

private:
    QVector<QPair<int, QString>> propertiesList;

};
#endif // PROPERTIESMODEL_H
