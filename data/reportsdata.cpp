#include "reportsdata.h"
#include <QSqlTableModel>
#include <QDebug>

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
    sellingsModel.setHeaderData(SELLING_ID, Qt::Horizontal, tr("Selling ID"));
    sellingsModel.setHeaderData(SELLING_TIME, Qt::Horizontal, tr("Time"));
    sellingsModel.setHeaderData(SELLING_PRICE, Qt::Horizontal, tr("Price"));
    sellingsModel.setHeaderData(SELLING_CLIENT, Qt::Horizontal, tr("Client"));
    sellingsModel.setHeaderData(SELLING_GLOBAL_DISCOUNT, Qt::Horizontal, tr("Global Discount"));
    sellingsModel.select();

    sellingSubProdsModel.setHeaderData(SELLSUB_PRODNAME, Qt::Horizontal, tr("Product's name"));
    sellingSubProdsModel.setHeaderData(SELLSUB_CATEGORIE, Qt::Horizontal, tr("Category"));
    sellingSubProdsModel.setHeaderData(SELLSUB_SIZE, Qt::Horizontal, tr("Size"));
    sellingSubProdsModel.setHeaderData(SELLSUB_PRICE, Qt::Horizontal, tr("Price"));
    sellingSubProdsModel.setHeaderData(SELLSUB_DISCOUNT, Qt::Horizontal, tr("Discount"));
    sellingSubProdsModel.setHeaderData(SELLSUB_COLOR, Qt::Horizontal, tr("Color"));
    sellingSubProdsModel.setSellingID(-1);
}

ReportsData::~ReportsData()
{
}
