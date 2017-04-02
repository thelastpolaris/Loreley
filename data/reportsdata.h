#ifndef REPORTSDATA_H
#define REPORTSDATA_H

#include <QObject>
#include "models/sellingsmodel.h"
#include "models/sellingssubprodsmodel.h"

class ReportsData : public QObject
{
    Q_OBJECT
public:
    static ReportsData* Create();
    static ReportsData* Instance() { return p_instance; }

    SellingsModel* getSellingsModel() { return &sellingsModel; }
    SellingsSubProdsModel* getSellingsSubProdsModel() { return &sellingSubProdsModel; }

protected:
    explicit ReportsData(QObject *parent = 0);
    ~ReportsData();

signals:

public slots:
private:
    static ReportsData *p_instance;
    SellingsModel sellingsModel;
    SellingsSubProdsModel sellingSubProdsModel;
};

#endif // REPORTSDATA_H
