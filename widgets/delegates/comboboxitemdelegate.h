#ifndef COMBOBOXITEMDELEGATE_H
#define COMBOBOXITEMDELEGATE_H
 
#include <QStyledItemDelegate>
 
class ComboBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ComboBoxItemDelegate(QHash<int, QString> _cbValues, QObject* parent = 0);
    ComboBoxItemDelegate(QHash<int, QString> _cbValues, QHash<int, QString> cbColorsVals, QObject* parent = 0);
    ~ComboBoxItemDelegate();

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
private:
    QHash<int, QString> cbValues;
    //Colors
    QHash<int, QIcon> cbColors;
};
 
#endif // COMBOBOXITEMDELEGATE_H 
