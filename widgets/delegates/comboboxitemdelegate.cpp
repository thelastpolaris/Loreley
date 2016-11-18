#include "comboboxitemdelegate.h"
#include <QComboBox>
 
ComboBoxItemDelegate::ComboBoxItemDelegate(QHash<int, QString> _cbValues, QObject* parent)
    : QStyledItemDelegate(parent), cbValues(_cbValues)
{

}

ComboBoxItemDelegate::ComboBoxItemDelegate(QHash<int, QString> _cbValues, QHash<int, QString> cbColorsVals, QObject* parent)
    : QStyledItemDelegate(parent), cbValues(_cbValues)
{
    for(QHash<int, QString>::const_iterator i = cbColorsVals.begin(); i != cbColorsVals.end(); ++i) {
        QPixmap px(15,15);//create pixmap,size choose yourself, by your taste
        px.fill(QColor(i.value()));//all pixmap will be red
        QIcon icon(px);
        cbColors.insert(i.key(), icon);
    }
}
 
ComboBoxItemDelegate::~ComboBoxItemDelegate()
{

}

 
QWidget* ComboBoxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // Create the combobox and populate it
    QComboBox* cb = new QComboBox(parent);
    for(QHash<int, QString>::const_iterator i = cbValues.begin(); i != cbValues.end(); ++i) {
        if(!cbColors.isEmpty()) {
            cb->addItem(cbColors.value(i.key()), i.value(), i.key());
        } else {
            cb->addItem(i.value(), i.key());
        }
    }
    return cb;
}
 
 
void ComboBoxItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor)) {
       // get the index of the text in the combobox that matches the current value of the itenm
       QString currentText = index.data(Qt::EditRole).toString();
       int cbIndex = cb->findText(currentText);
       // if it is valid, adjust the combobox
       if (cbIndex >= 0)
           cb->setCurrentIndex(cbIndex);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}
 
 
void ComboBoxItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    if (QComboBox* cb = qobject_cast<QComboBox*>(editor))
        // save the current text of the combo box as the current value of the item
        model->setData(index, cb->currentData(), Qt::EditRole);
    else
        QStyledItemDelegate::setModelData(editor, model, index);
} 
