#ifndef DATEPICKERITEMDELEGATE_H
#define DATEPICKERITEMDELEGATE_H

#include <QStyledItemDelegate>
//#include <QCalendarWidget>

class DatePickerItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DatePickerItemDelegate(QObject* parent = 0);
    ~DatePickerItemDelegate();

    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
private:

};

#endif // DATEPICKERITEMDELEGATE_H
