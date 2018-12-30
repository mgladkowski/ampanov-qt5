#ifndef CANDLESET_H
#define CANDLESET_H

#include "models/candle.h"

class CandleSet {
public:

    CandleSet();

    const int       CANDLES_MAX     = 10000;
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
};

#endif // CANDLESET_H
