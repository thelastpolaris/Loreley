#include "reportsdata.h"
#include <QSqlTableModel>

ReportsData* ReportsData::p_instance = 0;

ReportsData* ReportsData::Create()
{
    if(p_instance) delete p_instance;
    p_instance = new ReportsData;
    return p_instance;
}

ReportsData::ReportsData(QObject *parent)
    :QObject(parent), sellingsModel(this), sellingSubProdsModel(this)
{
//    sellsModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
//    sellsModel->setHeaderData(1, Qt::Horizontal, tr("Salary"));
}

ReportsData::~ReportsData()
{

}
