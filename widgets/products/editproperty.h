#ifndef EDITPROPERTY_H
#define EDITPROPERTY_H

#include <QDialog>
#include "auxiliary/propertiesmodel.h"

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
    void show(QString _propertySingular, QString _tableName, int _fieldID, bool _product = true);
    void addProperty();
    void removeProperty();
    void editPropertySlot();

private:
    Ui::EditProperty *ui;
    QString propertySingular;
    QString tableName;
    int fieldID;
    bool product;
    PropertiesModel propertiesModel;
};

#endif // EDITPROPERTY_H
