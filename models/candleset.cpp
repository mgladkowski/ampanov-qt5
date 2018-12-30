#include "candleset.h"


CandleSet::CandleSet() {

}


bool CandleSet::clear() {

    symbol_id       = 0;
    period          = 0;
    index           = -1;
    interval        = "";

    candles.clear();

    return true;
}


bool CandleSet::load(int SymbolID, QString Interval) {

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


bool CandleSet::loadslice(int SymbolID, QString Interval) {

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


bool CandleSet::first() {

    index = (candles.count() > 0) ? 0 : -1;
    return (index == 0);
}


bool CandleSet::previous() {

    if (index > 0) {
        index--;
        return true;
    }
    return false;
}


bool CandleSet::next() {

    if (index < (candles.count() - 1)) {
        index++;
        return true;
    }
    return false;
}


bool CandleSet::last() {

    index = candles.count() - 1;
    return (index == candles.count() - 1);
}
