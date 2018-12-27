#include "scanner.h"


Scanner::Scanner() {}


/**
 *  Run scanner algorithms on last candle
 */
bool Scanner::run_live(int SymbolID, QString Interval) {

    qInfo() << "Check candle patterns " << SymbolID << " " << qPrintable(Interval);

    bool result = false;

    if (SymbolID <= 0)          return false;
    if (Interval.length() <= 0) return false;

    result = loadslice(SymbolID, Interval);

    if (result != true)         return false;

    is_live = true;

    result  = scan_candle();
    //result &= scan_chart();

    return result;
}


/**
 *  Run scanner algorithms iteratively over entire history
 */
bool Scanner::run_replay(int SymbolID, QString Interval) {

    qInfo() << "Replay candle patterns " << SymbolID << " " << qPrintable(Interval);

    bool result = false;

    if (SymbolID <= 0)          return false;
    if (Interval.length() <= 0) return false;

    result  = load(SymbolID, Interval);
    result &= first();

    if (result != true)         return false;

    is_live = false;

    while (result && next()) {

        result  = scan_candle();
        //result &= scan_chart();
    }
    return result;
}


/**
 * Scan for candle patterns
 */
bool Scanner::scan_candle() {

    check_WRU();    // wide range bar bullish
    check_WRD();    // wide range bar bearish
    check_MSR();    // morning star reversal
    check_ESR();    // evening star reversal
    check_OWS();    // one white solider
    check_NBC();    // new black crow

    return true;
}


/**
 * Scan for chart patterns
 */
bool Scanner::scan_chart() {

    return true;
}


/**
 * Save candle detection
 */
bool Scanner::save_detected_candle(int pattern_id) {

    bool result = false;

    Scan scan = ScanModel::select_active(symbol_id, interval, candles[index].rowid);

    if (scan.rowid <= 0) {

        scan.symbol_id = symbol_id;
        scan.symbol_interval = interval;
        scan.created_by = "ampanov-server";
        scan.created_date = QDateTime::currentDateTime();
        scan.candle_row_id = candles[index].rowid;
        scan.candle_pattern_id = pattern_id;

        result = ScanModel::save(scan, is_live);
        return result;
    }
    return true;
}


/**
 * Save chart detection to database
 */
bool Scanner::save_detected_chart(int pattern_id) {

    bool result = false;

    Scan scan = ScanModel::select_active(symbol_id, interval, candles[index].rowid);

    if (scan.rowid <= 0) {

        scan.symbol_id = symbol_id;
        scan.symbol_interval = interval;
        scan.created_by = "ampanov-server";
        scan.created_date = QDateTime::currentDateTime();
        scan.candle_row_id = candles[index].rowid;
    }
    scan.chart_pattern_id = pattern_id;

    result = ScanModel::save(scan, is_live);
    return result;
}



/** ---------------
  * CANDLE PATTERNS
  * --------------- */

/**
 *  Wide range bar : bullish
 */
bool Scanner::check_WRU() {

    if (index < 0) return false;

    bool result = false;

    Candle c1 = candles[index];

    result  = c1.bullish;
    result &= c1.long_day;
    result &= c1.vol_magnitude > 2.5;

    if (result == true) {

        qInfo() << c1.start.toString() << " wide range bar up";

        save_detected_candle(1);
    }

    return result;
}


/**
 *  Wide range bar : bearish
 */
bool Scanner::check_WRD() {

    if (index < 0) return false;

    bool result = false;

    Candle c1 = candles[index];

    result  = c1.bearish;
    result &= c1.long_day;
    result &= c1.vol_magnitude > 2.5;

    if (result == true) {

        qInfo() << c1.start.toString() << " wide range bar down";

        save_detected_candle(2);
    }

    return result;
}


/**
 *  Morning star reveral : bullish
 */
bool Scanner::check_MSR() {

    if (index < 2) return false;

    bool result = false;

    Candle c1 = candles[index];
    Candle c2 = candles[index-1];
    Candle c3 = candles[index-2];

    result  = c3.bearish;

    result &= c2.short_day;
    result &= c2.long_lower_wick;

    result &= c1.bullish;

    if (result == true) {

        qInfo() << c1.start.toString() << " morning star reversal";

        save_detected_candle(3);
    }

    return result;
}


/**
 *  Evening star reversal : bearish
 */
bool Scanner::check_ESR() {

    if (index < 2) return false;

    bool result = false;

    Candle c1 = candles[index];
    Candle c2 = candles[index-1];
    Candle c3 = candles[index-2];

    result  = c3.bullish;

    result &= c2.short_day;
    result &= c2.long_upper_wick;

    result &= c1.bearish;

    if (result == true) {

        qInfo() << c1.start.toString() << " evening star reversal";

        save_detected_candle(4);
    }

    return result;
}


/**
 *  One white solider : bullish
 */
bool Scanner::check_OWS() {

    if (index < 1) return false;

    bool result = false;

    Candle c1 = candles[index];
    Candle c2 = candles[index-1];


    if (result == true) {

        qInfo() << c1.start.toString() << " one white solider";

        save_detected_candle(5);
    }

    return result;
}


/**
 *  New black crow : bearish
 */
bool Scanner::check_NBC() {

    if (index < 1) return false;

    bool result = false;

    Candle c1 = candles[index];
    Candle c2 = candles[index-1];


    if (result == true) {

        qInfo() << c1.start.toString() << " new black crow";

        save_detected_candle(6);
    }

    return result;
}

