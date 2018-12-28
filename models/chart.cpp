#include "chart.h"

Chart ChartModel::load(int symbol_id, QString interval) {

    Q_UNUSED(symbol_id);
    Q_UNUSED(interval);
    Chart c;
    return c;
}


bool ChartModel::save(Chart data, QString interval) {

    Q_UNUSED(data);
    Q_UNUSED(interval);
    return true;
}


ChartPoint ChartModel::loadpoint(int symbol_id, QString interval) {

    Q_UNUSED(symbol_id);
    Q_UNUSED(interval);
    ChartPoint c;
    return c;
}


bool ChartModel::savepoint(ChartPoint data, QString interval) {

    Q_UNUSED(data);
    Q_UNUSED(interval);
    return true;
}

