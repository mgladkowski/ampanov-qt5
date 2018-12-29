#include "analyst.h"


Analyst::Analyst() {
    clear();
}


Analyst::~Analyst() {}


bool Analyst::clear() {

    symbol_id       = 0;
    period          = 0;
    index           = -1;
    interval        = "";

    candles.clear();

    return true;
}


bool Analyst::load(int SymbolID, QString Interval) {

    bool result = false;

    clear();

    symbol_id = SymbolID;
    interval = Interval;

    candles = CandleModel::select_n(symbol_id, QDateTime::currentDateTime(), CANDLES_MAX, interval);

    result = (candles.count() > 0);
    period = candles.count();

    if (result == true) {
        result = last();
    }
    return result;
}


bool Analyst::loadslice(int SymbolID, QString Interval) {

    bool result = false;

    clear();

    symbol_id = SymbolID;
    interval = Interval;

    candles = CandleModel::select_n(symbol_id, QDateTime::currentDateTime(), CANDLES_SLICE, interval);

    result = (candles.count() > 0);
    period = candles.count();

    if (result == true) {
        result = last();
    }
    return result;
}


bool Analyst::first() {

    index = (candles.count() > 0) ? 0 : -1;
    return (index == 0);
}


bool Analyst::previous() {

    if (index > 0) {
        index--;
        return true;
    }
    return false;
}


bool Analyst::next() {

    if (index < (candles.count() - 1)) {
        index++;
        return true;
    }
    return false;
}


bool Analyst::last() {

    index = candles.count() - 1;
    return (index == candles.count() - 1);
}


bool Analyst::calculate_all() {

    first();

    while (next()) {

        calculate_candle();

        QThread::usleep(1000);
    }
    return true;
}


bool Analyst::calculate_candle() {

    double ema10multiplier = 2.0 / 11.0;
    double ema20multiplier = 2.0 / 21.0;
    double ema50multiplier = 2.0 / 51.0;

    Candle c = candles[index];
    Candle p = candles[index-1];

    // chart properties at current index

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

    CandleModel::savecalc(c, interval);
    candles[index] = c;

    return true;
}


bool Analyst::calculate_chart() {

    if (index < 50) return false;

    Candle c = candles[index];
    Candle p = candles[index-1];

    if ( ((c.close > c.ema_10) && (c.ema_10 > c.ema_20) && (c.ema_20 > c.ema_50)) &&
        !((p.close > p.ema_10) && (p.ema_10 > p.ema_20) && (p.ema_20 > p.ema_50)) ){

        qDebug() << c.start.toLocalTime() << "  "
                 << c.close << " "
                 << c.ema_10 << " "
                 << c.ema_20 << " "
                 << c.ema_50 << " bull";
    }
    else if ( ((c.close < c.ema_10) && (c.ema_10 < c.ema_20) && (c.ema_20 < c.ema_50)) &&
             !((p.close < p.ema_10) && (p.ema_10 < p.ema_20) && (p.ema_20 < p.ema_50)) ){

        qDebug() << c.start.toLocalTime() << "  "
                 << c.close << " "
                 << c.ema_10 << " "
                 << c.ema_20 << " "
                 << c.ema_50 << " bear";
    }

    return true;
}


bool Analyst::analyze() {

    first();

    while (next()) {

        calculate_chart();

        QThread::usleep(1000);
    }
    return true;
}


