#include "textfielditemdelegate.h"
#include <QTextEdit>


TextFieldItemDelegate::TextFieldItemDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}


TextFieldItemDelegate::~TextFieldItemDelegate() {

}

QWidget* TextFieldItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QTextEdit *dateEdit = new QTextEdit(parent);
    dateEdit->setLineWrapMode(QTextEdit::WidgetWidth);

    return dateEdit;
}

void TextFieldItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor)) {
        textEdit->setPlainText(index.data().toString());
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void TextFieldItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    if (QTextEdit* textEdit = qobject_cast<QTextEdit*>(editor))
        // save the current date of dateEdit as the current value of the item
        model->setData(index, textEdit->toPlainText(), Qt::EditRole);
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}
