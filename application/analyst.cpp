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
        //calculate_chart();

        QThread::usleep(1000);
    }
    return true;
}


bool Analyst::calculate_candle() {

    float ema10multiplier = 2 / 11;
    float ema20multiplier = 2 / 21;
    float ema50multiplier = 2 / 51;

    Candle c = candles[index];
    Candle p = candles[index-1];

    // chart properties at current index

    float true_range_1 = c.high - c.low;
    float true_range_2 = qFabs(c.high - p.close);
    float true_range_3 = qFabs(c.low - p.close);

    c.true_range = 0;
    c.true_range = true_range_1 > true_range_2 ? true_range_1 : true_range_2;
    c.true_range = c.true_range > true_range_3 ? c.true_range : true_range_3;

    float x_sum_close = 0;
    float x_sum_volume = 0;
    float x_sum_body_range = 0;
    float x_sum_full_range = 0;
    float x_sum_true_range = 0;
    float p_sma_50 = 0;

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
            c.sma_10 = x_sum_close / 10;
        }
        if (i == 20) {
            c.sma_20 = x_sum_close / 20;
            c.vol_20 = x_sum_volume / 20;
            c.abr_20 = x_sum_body_range / 20;
            c.afr_20 = x_sum_full_range / 20;
            c.atr_20 = x_sum_true_range / 20;
        }
        if (i == 40) {
            c.sma_40 = x_sum_close / 40;
        }
        if (i == 50) {
            p_sma_50 = x_sum_close / 50;
        }
        if (i == 100) {
            c.sma_100 = x_sum_close / 100;
        }
        if (i == 200) {
            c.sma_200 = x_sum_close / 200;
        }
    }

    float ema10previous = (p.ema_10 > 0) ? p.ema_10 : p.sma_10;
    c.ema_10 = (p.ema_10 > 0 || c.sma_10 > 0)
             ? ((c.close - ema10previous) * ema10multiplier) + ema10previous
             : 0;

    float ema20previous = (p.ema_20 > 0) ? p.ema_20 : c.sma_20;
    c.ema_20 = (p.ema_20 > 0 || c.sma_20 > 0)
             ? ((c.close - ema20previous) * ema20multiplier) + ema20previous
             : 0;

    float ema50previous = (p.ema_50 > 0) ? p.ema_50 : p_sma_50;
    c.ema_50 = (p.ema_50 > 0 || p_sma_50 > 0)
             ? ((c.close - ema50previous) * ema50multiplier) + ema50previous
             : 0;

    CandleModel::savecalc(c, interval);
    candles[index] = c;

    return true;
}


bool Analyst::calculate_chart() {

    if (index < 1) return false;

    // major trend

    if (candles[index].sma_100 > candles[index].sma_200)
        chart.trend_major = "U";

    if (candles[index].sma_100 < candles[index].sma_200)
        chart.trend_major = "D";

    // intermediate trend

    if (candles[index].sma_20 > candles[index-2].sma_20)
        chart.trend_inter = "U";

    if (candles[index].sma_20 < candles[index-2].sma_20)
        chart.trend_inter = "D";

    // minor trend

    if (candles[index].ema_10 > candles[index].ema_20)
        chart.trend_minor = "U";

    if (candles[index].ema_10 < candles[index].ema_20)
        chart.trend_minor = "D";

    // has DMA crossed
    // yes : save peak as pivot and switch directions
    // no : update peak

    if (chart.trend_inter == "U"
        && candles[index].sma_20) {};


    return true;
}