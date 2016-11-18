#include "datepickeritemdelegate.h"

#include <QDateEdit>

//TODO - Change QDateEdit with QCalendarWidget

DatePickerItemDelegate::DatePickerItemDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}

DatePickerItemDelegate::~DatePickerItemDelegate() {

}

QWidget* DatePickerItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QDateEdit *dateEdit = new QDateEdit(parent);

    return dateEdit;
}

void DatePickerItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (QDateEdit* dateEdit = qobject_cast<QDateEdit*>(editor)) {
       dateEdit->setDate(index.data().toDate());
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void DatePickerItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (QDateEdit* dateEdit = qobject_cast<QDateEdit*>(editor))
        // save the current date of dateEdit as the current value of the item
        model->setData(index, dateEdit->date(), Qt::EditRole);
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}


