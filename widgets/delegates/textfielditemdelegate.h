#ifndef TEXTFIELDITEMDELEGATE_H
#define TEXTFIELDITEMDELEGATE_H
#include <QStyledItemDelegate>

class TextFieldItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TextFieldItemDelegate(QObject *parent = 0);
    ~TextFieldItemDelegate();

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
};

#endif // TEXTFIELDITEMDELEGATE_H
