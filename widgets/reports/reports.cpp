#include "reports.h"
#include "ui_reports.h"
#include <QDebug>
#include "data/reportsdata.h"

Reports::Reports(QWidget *parent) :
    QWidget(parent), ui(new Ui::Reports)
{
    ui->setupUi(this);
    ReportsData* reportsData = ReportsData::Instance();
    SellingsModel* sellingsModel = reportsData->getSellingsModel();
    SellingsSubProdsModel* sellingsSubProdsModel = reportsData->getSellingsSubProdsModel();

    ui->totalValue->setText(QString::number(sellingsModel->getTotalPrice()));

    connect(ui->calendarWidget, &QCalendarWidget::selectionChanged, [=]() {
        sellingsModel->setDate(ui->calendarWidget->selectedDate());
        ui->totalValue->setText(QString::number(sellingsModel->getTotalPrice()));
        sellingsSubProdsModel->setSellingID(-1);
    });

    sellingsModel->setDate(QDate::currentDate());
    ui->salesTable->setModel(sellingsModel);
    ui->salesTable->hideColumn(SELLING_ID);
    ui->salesTable->hideColumn(SELLING_CLIENT);
    ui->salesTable->horizontalHeader()->setStretchLastSection(true);
    ui->salesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->salesTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->subProductsTable->setModel(sellingsSubProdsModel);
    connect(ui->salesTable->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            SLOT(updateSubProdsModel(const QItemSelection &, const QItemSelection &)));
    ui->subProductsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void Reports::selectDate(QDate date)
{
    ui->calendarWidget->setSelectedDate(date);
}

void Reports::updateSubProdsModel(const QItemSelection &first, const QItemSelection &last)
{
    ReportsData* reportsData = ReportsData::Instance();
    SellingsSubProdsModel* sellingsSubProdsModel = reportsData->getSellingsSubProdsModel();
    SellingsModel* sellingsModel = reportsData->getSellingsModel();

    int row = first.indexes()[0].row();
    int sellingID = sellingsModel->data(sellingsModel->index(row, SELLING_ID)).toInt();

    sellingsSubProdsModel->setSellingID(sellingID);
}

Reports::~Reports()
{
    delete ui;
}
