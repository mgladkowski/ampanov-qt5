#ifndef SCANNER_H
#define SCANNER_H

#include "models/scan.h"
#include "analyst.h"

class Scanner : public Analyst {

public:

    Scanner();

    bool    is_live = false;

    bool    run_live(int SymbolID, QString Interval);
    bool    run_replay(int SymbolID, QString Interval);

    bool    scan_candle();
    bool    scan_chart();

    bool    save_detected_candle(int pattern_id);
    bool    save_detected_chart(int pattern_id);

    bool    check_WRU();
    bool    check_WRD();
    bool    check_MSR();
    bool    check_ESR();
    bool    check_OWS();
    bool    check_NBC();

};

#endif // SCANNER_H
