#ifndef EDITPROPERTY_H
#define EDITPROPERTY_H

#include <QDialog>

namespace Ui {
    class EditProperty;
}

class PropertiesModel;

class EditProperty : public QDialog
{
    Q_OBJECT

public:
    explicit EditProperty(QWidget *parent = 0);
    ~EditProperty();
public slots:
    void show(QString _propertyType, QString _tableName, bool color = false);
    void addProperty();
    void removeProperty();
    void editPropertySlot();
signals:
    void propertiesChanged(QString _tableName);
    void propertyEdited();
private:
    Ui::EditProperty *ui;
    QString propertyType;
    QString tableName;
    PropertiesModel* propertiesModel;
};

#endif // EDITPROPERTY_H
