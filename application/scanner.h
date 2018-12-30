#ifndef SCANNER_H
#define SCANNER_H

#include "models/candleset.h"

class Scanner : public CandleSet {

public:

    Scanner();

    bool    is_live = false;

    bool    run_live( int _symbol_id, QString _interval );
    bool    run_recalc( int _symbol_id, QString _interval );

    bool    calc_candle();
    bool    scan_candle();
    bool    scan_chart();

    bool    signal_WRU();
    bool    signal_WRD();
    bool    signal_MSR();
    bool    signal_ESR();
    bool    signal_OWS();
    bool    signal_NBC();

    bool    signal_ema_x();
};

#endif // SCANNER_H
