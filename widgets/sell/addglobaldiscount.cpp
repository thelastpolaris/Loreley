#include "addglobaldiscount.h"
#include "ui_addglobaldiscount.h"

AddGlobalDiscount::AddGlobalDiscount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGlobalDiscount), m_isActive(false)
{
    ui->setupUi(this);

    connect(ui->globalDiscBB, &QDialogButtonBox::accepted, [=] {
        setActive(true);
        bool isPercents = false;
        double value = 0;

        if (ui->percentsSB->isEnabled()) {
            isPercents = true;
            value = ui->percentsSB->value();
        } else {
            value = ui->valueSB->value();
        }

        if(value) emit discountSet(isPercents, value);

        close();
    });

    connect(ui->globalDiscBB, &QDialogButtonBox::rejected, [=] {
        setActive(false);
        close();
    });

    connect(ui->percentsSB, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=] {
        if(ui->percentsSB->value() > 0) ui->valueSB->setEnabled(false);
        else ui->valueSB->setEnabled(true);
    });

    connect(ui->valueSB, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=] {
        if(ui->valueSB->value() > 0) ui->percentsSB->setEnabled(false);
        else ui->percentsSB->setEnabled(true);
    });
}

void AddGlobalDiscount::setActive(bool isActive)
{
    if(m_isActive != isActive) {
        m_isActive = isActive;
        emit activeChanged(isActive);
    }
}

void AddGlobalDiscount::showDialog()
{
    ui->valueSB->setValue(0);
    ui->percentsSB->setValue(0);
    show();
}

AddGlobalDiscount::~AddGlobalDiscount()
{
    delete ui;
}

void AddGlobalDiscount::setMaxDiscountVal(int maxDiscount)
{
    ui->valueSB->setMaximum(maxDiscount);
}
