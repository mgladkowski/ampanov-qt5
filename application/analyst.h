#ifndef ANALYST_H
#define ANALYST_H

#include "models/candle.h"
#include "models/chart.h"

class Analyst {
public:

    Analyst();
    ~Analyst();

    const int       CANDLES_MAX     = 10000;
    const int       CANDLES_SLICE   = 200;

    int             symbol_id;
    int             period;
    int             index;
    QString         interval;

    Candles         candles;
    Chart           chart;
    ChartPoints     chart_points;

    bool            clear();
    bool            first();
    bool            previous();
    bool            next();
    bool            last();

    bool            load( int SymbolID, QString Interval );
    bool            loadslice( int SymbolID, QString Interval );

    bool            calculate_all();
    bool            calculate_candle();
    bool            calculate_chart();
};

#endif // ANALYST_H
