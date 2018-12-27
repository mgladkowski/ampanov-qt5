#ifndef CHART_H
#define CHART_H

#include "core/database.h"

class Chart {
public:

    int         rowid;
    int         symbol_id;
    QString     symbol_interval;
    int         candle_id;
    int         candle_start;
    QString     trend_major;
    QString     trend_inter;
    QString     trend_minor;
    float       peak_major;
    float       peak_inter;
    float       peak_minor;
    float       support;
    float       support_strength;
    float       resistance;
    float       resistance_strength;
};
typedef QList<Chart> Charts;


class ChartPoint {
public:

    int         rowid;
    int         symbol_id;
    QString     symbol_interval;
    int         candle_id;
    QString     point_name;
    QString     point_direction;
    QString     point_text;
};
typedef QList<ChartPoint> ChartPoints;


class ChartModel : public Database {

    static Chart        load( int symbol_id, QString interval );
    static bool         save( Chart data, QString interval );

    static ChartPoint   loadpoint( int symbol_id, QString interval );
    static bool         savepoint( ChartPoint data, QString interval );
};

#endif // CHART_H
