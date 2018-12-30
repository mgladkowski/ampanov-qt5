#ifndef ANALYST_H
#define ANALYST_H

#include "models/candle.h"
#include "models/chart.h"

class Analyst {
public:

    Analyst();
    ~Analyst();

    const int       CANDLES_MAX     = 20000;
    const int       CANDLES_SLICE   = 200;

    int             symbol_id;
    int             period;
    int             index;
    QString         interval;

    Candles         candles;

    bool            clear();
    bool            first();
    bool            previous();
    bool            next();
    bool            last();

    bool            load( int SymbolID, QString Interval );
    bool            loadslice( int SymbolID, QString Interval );

    bool            calculate_all_candles();
    bool            calculate_candle();

    bool            calculate_all_charts();
    bool            calculate_chart();

    bool            analyze();
};

#endif // ANALYST_H
