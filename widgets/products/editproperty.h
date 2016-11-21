#ifndef EDITPROPERTY_H
#define EDITPROPERTY_H

#include <QDialog>

namespace Ui {
class EditProperty;
}

class EditProperty : public QDialog
{
    Q_OBJECT

public:
    explicit EditProperty(QWidget *parent = 0);
    ~EditProperty();
    Ui::EditProperty* getUI() const { return ui; }
public slots:
    void show(QString _propertyName, QString _tableName, bool color = false);
    void addProperty();
    void removeProperty();
signals:
    void propertiesChanged(QString _tableName);
private:
    Ui::EditProperty *ui;
    QString propertyName;
    QString tableName;
};

#endif // EDITPROPERTY_H
