#include "scanner.h"


Scanner::Scanner() {}


/**
 *  Run scanner algorithms on last candle
 */
bool Scanner::run_live(int _symbol_id, QString _interval) {

    if (_symbol_id <= 0)            return false;
    if (_interval.length() <= 0)    return false;

    bool result = false;

    is_live = true;

    qInfo() << "Analyze current candle " << _symbol_id << " " << qPrintable(_interval);

    result  = loadslice( _symbol_id, _interval );

    result &= calc_candle();
    result &= scan_candle();
    result &= scan_chart();

    if (result == true) {
        CandleModel::savecalc( candles[index], _interval );
    }
    return result;
}


/**
 *  Run scanner algorithms over entire history
 */
bool Scanner::run_recalc(int _symbol_id, QString _interval) {

    if (_symbol_id <= 0)            return false;
    if (_interval.length() <= 0)    return false;

    bool result = false;

    is_live = false;

    qInfo() << "Recalculating analysis of " << _symbol_id <<  qPrintable(_interval);

    result  = load( _symbol_id, _interval);
    result &= first();

    while (result && next()) {

        result &= calc_candle();
        result &= scan_candle();
        result &= scan_chart();

        if (result == true) {
            CandleModel::savecalc( candles[index], _interval );
        } else {
            break;
        }

        QThread::usleep(1000);
    }
    return result;
}



/** ---------------
  * ANALYSIS STEPS
  * --------------- */

/**
 * Calculate candle indicators
 */
bool Scanner::calc_candle() {

    double ema10multiplier = 2.0 / 11.0;
    double ema20multiplier = 2.0 / 21.0;
    double ema50multiplier = 2.0 / 51.0;

    Candle c = candles[index];
    Candle p = candles[index-1];

    // candle properties at current index

    double true_range_1 = c.high - c.low;
    double true_range_2 = qFabs(c.high - p.close);
    double true_range_3 = qFabs(c.low - p.close);

    c.true_range = 0;
    c.true_range = true_range_1 > true_range_2 ? true_range_1 : true_range_2;
    c.true_range = c.true_range > true_range_3 ? c.true_range : true_range_3;

    double x_sum_close = 0;
    double x_sum_volume = 0;
    double x_sum_body_range = 0;
    double x_sum_full_range = 0;
    double x_sum_true_range = 0;
    double p_sma_50 = 0;

    c.vol_20 = 0;
    c.abr_20 = 0;
    c.afr_20 = 0;
    c.atr_20 = 0;
    c.sma_10 = 0;
    c.sma_20 = 0;
    c.sma_40 = 0;
    c.sma_100 = 0;
    c.sma_200 = 0;
    c.ema_10 = 0;
    c.ema_20 = 0;
    c.ema_50 = 0;

    int     i;
    int     ix;
    Candle  cx;

    for (i = 1; i <= 200; i++) {

        ix = index + 1 - i;
        if (ix < 0) break;

        cx = candles[ix];
        x_sum_close         += cx.close;
        x_sum_volume        += cx.volume;
        x_sum_body_range    += cx.body_range;
        x_sum_full_range    += cx.full_range;
        x_sum_true_range    += cx.true_range;

        if (i == 10) {
            c.sma_10 = x_sum_close / 10.0;
        }
        if (i == 20) {
            c.sma_20 = x_sum_close / 20.0;
            c.vol_20 = x_sum_volume / 20.0;
            c.abr_20 = x_sum_body_range / 20.0;
            c.afr_20 = x_sum_full_range / 20.0;
            c.atr_20 = x_sum_true_range / 20.0;
        }
        if (i == 40) {
            c.sma_40 = x_sum_close / 40.0;
        }
        if (i == 50) {
            p_sma_50 = x_sum_close / 50.0;
        }
        if (i == 100) {
            c.sma_100 = x_sum_close / 100.0;
        }
        if (i == 200) {
            c.sma_200 = x_sum_close / 200.0;
        }
    }

    double emaprev10 = (p.ema_10 > 0) ? p.ema_10 : p.sma_10;
    c.ema_10 = (emaprev10 > 0)
             ? ((c.close - emaprev10) * ema10multiplier) + emaprev10
             : 0;

    double ema20prev = (p.ema_20 > 0) ? p.ema_20 : c.sma_20;
    c.ema_20 = (ema20prev > 0)
             ? ((c.close - ema20prev) * ema20multiplier) + ema20prev
             : 0;

    double ema50prev = (p.ema_50 > 0) ? p.ema_50 : p_sma_50;
    c.ema_50 = (ema50prev > 0)
             ? ((c.close - ema50prev) * ema50multiplier) + ema50prev
             : 0;

    candles[index] = c;

    return true;
}


/**
 * Scan for candle patterns
 */
bool Scanner::scan_candle() {

    signal_WRU();       // wide range bar bullish
    signal_WRD();       // wide range bar bearish
    signal_MSR();       // morning star reversal
    signal_ESR();       // evening star reversal
    signal_OWS();       // one white solider
    signal_NBC();       // new black crow

    return true;
}


/**
 * Scan for chart patterns
 */
bool Scanner::scan_chart() {

    signal_ema_x();     // exp moving average cross

    return true;
}



/** ---------------
  * CANDLE PATTERNS
  * --------------- */

/**
 *  Wide range bar : bullish
 */
bool Scanner::signal_WRU() {

    if (index < 0) return false;

    bool test = false;

    Candle c1 = candles[index];

    test  = c1.bullish;
    test &= c1.long_day;
    test &= c1.vol_magnitude > 2.5;

    if (test == true) {
        qDebug() << c1.start.toLocalTime() << " candle - wide range up";
        c1.signal_candle = 1;
        candles[index] = c1;
    }

    return test;
}


/**
 *  Wide range bar : bearish
 */
bool Scanner::signal_WRD() {

    if (index < 0) return false;

    bool test = false;

    Candle c1 = candles[index];

    test  = c1.bearish;
    test &= c1.long_day;
    test &= c1.vol_magnitude > 2.5;

    if (test == true) {
        qDebug() << c1.start.toLocalTime() << " candle - wide range down";
        c1.signal_candle = 2;
        candles[index] = c1;
    }

    return test;
}


/**
 *  Morning star reveral : bullish
 */
bool Scanner::signal_MSR() {

    if (index < 2) return false;

    bool test = false;

    Candle c1 = candles[index];
    Candle c2 = candles[index-1];
    Candle c3 = candles[index-2];

    test  = c3.bearish;
    test &= c2.short_day;
    test &= c2.long_lower_wick;
    test &= c1.bullish;

    if (test == true) {
        qDebug() << c1.start.toLocalTime() << " candle - morning star";
        c1.signal_candle = 3;
        candles[index] = c1;
    }

    return test;
}


/**
 *  Evening star reversal : bearish
 */
bool Scanner::signal_ESR() {

    if (index < 2) return false;

    bool test = false;

    Candle c1 = candles[index];
    Candle c2 = candles[index-1];
    Candle c3 = candles[index-2];

    test  = c3.bullish;
    test &= c2.short_day;
    test &= c2.long_upper_wick;
    test &= c1.bearish;

    if (test == true) {
        qDebug() << c1.start.toLocalTime() << " candle - evening star";
        c1.signal_candle = 4;
        candles[index] = c1;
    }

    return test;
}


/**
 *  One white solider : bullish
 */
bool Scanner::signal_OWS() {

    if (index < 1) return false;

    bool test = false;

    Candle c1 = candles[index];
    Candle c2 = candles[index-1];


    if (test == true) {
        qDebug() << c1.start.toLocalTime() << " candle - one white soldier";
        c1.signal_candle = 5;
        candles[index] = c1;
    }

    return test;
}


/**
 *  New black crow : bearish
 */
bool Scanner::signal_NBC() {

    if (index < 1) return false;

    bool test = false;

    Candle c1 = candles[index];
    Candle c2 = candles[index-1];


    if (test == true) {
        qDebug() << c1.start.toLocalTime() << " candle - new black crow";
        c1.signal_candle = 6;
        candles[index] = c1;
    }

    return test;
}



/** ---------------
  * CHART PATTERNS
  * --------------- */

/**
 *  EMA cross
 */
bool Scanner::signal_ema_x() {

    if (index < 50) return false;

    Candle c = candles[index];
    Candle p = candles[index-1];

    if ( ((c.close > c.ema_10) && (c.ema_10 > c.ema_20) && (c.ema_20 > c.ema_50)) &&
        !((p.close > p.ema_10) && (p.ema_10 > p.ema_20) && (p.ema_20 > p.ema_50)) &&
        ((c.ema_10 > p.ema_10) && (c.ema_20 > p.ema_20) && (c.ema_50 > p.ema_50)) ){

        qDebug() << c.start.toLocalTime()
                 << " signal - ema-x-bull";

        c.signal_chart = 7;
        candles[index] = c;
    }
    else if ( ((c.close < c.ema_10) && (c.ema_10 < c.ema_20) && (c.ema_20 < c.ema_50)) &&
             !((p.close < p.ema_10) && (p.ema_10 < p.ema_20) && (p.ema_20 < p.ema_50)) &&
              ((c.ema_10 < p.ema_10) && (c.ema_20 < p.ema_20) && (c.ema_50 < p.ema_50)) ){

        qDebug() << c.start.toLocalTime()
                 << " signal - ema-x-bear";

        c.signal_chart = 8;
        candles[index] = c;
    }
    return true;
}
