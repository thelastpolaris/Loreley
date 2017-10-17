#include "addglobaldiscount.h"
#include "ui_addglobaldiscount.h"

AddGlobalDiscount::AddGlobalDiscount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGlobalDiscount), m_isActive(false)
{
    ui->setupUi(this);

    connect(ui->globalDiscBB, &QDialogButtonBox::accepted, [=] {
        m_isActive = true;
        close();
    });

    connect(ui->globalDiscBB, &QDialogButtonBox::rejected, [=] {
        m_isActive = false;
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

AddGlobalDiscount::~AddGlobalDiscount()
{
    delete ui;
}

bool AddGlobalDiscount::isActive()
{
    return m_isActive;
}

void AddGlobalDiscount::setMaxDiscountVal(int maxDiscount)
{
    ui->valueSB->setMaximum(maxDiscount);
}
