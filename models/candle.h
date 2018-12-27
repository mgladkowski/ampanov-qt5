#ifndef CANDLEMODEL_H
#define CANDLEMODEL_H

#include "core/database.h"
#include "core/zeromq.h"

class Candle {
public:

    int         rowid;
    int         symbol_id;
    QDateTime   start;
    QDateTime   end;
    float       open;
    float       high;
    float       low;
    float       close;
    int         volume;
    float       vwap;

    float       true_range;
    float       abr_20;
    float       afr_20;
    float       atr_20;
    float       vol_20;
    float       ema_10;
    float       ema_20;
    float       ema_50;
    float       sma_10;
    float       sma_20;
    float       sma_40;
    float       sma_100;
    float       sma_200;

    bool        calculate();

    bool        bullish;
    bool        bearish;
    bool        flat;
    float       full_range;
    float       body_range;
    float       body_upper_price;
    float       body_lower_price;
    float       wick_upper_range;
    float       wick_lower_range;
    float       wick_range;
    float       wick_upper_size;
    float       wick_lower_size;
    float       wick_size;
    bool        long_upper_wick;
    bool        long_lower_wick;
    bool        short_upper_wick;
    bool        short_lower_wick;

    float       vol_magnitude;
    float       abr_magnitude;
    float       afr_magnitude;
    float       atr_magnitude;
    bool        average_day;
    bool        short_day;
    bool        long_day;
    bool        marabozu;

    QJsonObject toJsonObject() {
        QJsonObject data;
        data["start"] = start.toLocalTime().toString();
        data["end"] = end.toLocalTime().toString();
        data["open"] = QString::number( open, 'f', 2 );
        data["high"] = QString::number( high, 'f', 2 );
        data["low"] = QString::number( low, 'f', 2 );
        data["close"] = QString::number( close, 'f', 2 );
        data["volume"] = QString::number( volume, 'f', 0 );
        data["vwap"] = QString::number( vwap, 'f', 2 );
        return data;
    }

};
typedef QList<Candle> Candles;


class CandleModel : public Database {
public:

    static Candles  select_range(int symbol_id, QDateTime from, QDateTime to, QString interval = "OneDay", bool descending = false);
    static Candles  select_n(int symbol_id, QDateTime to, int n, QString interval = "OneDay", bool descending = false);
    static Candle   select_last(int symbol_id, QString interval = "OneDay");
    static Candle   select_first(int symbol_id, QString interval = "OneDay");
    static Candle   select_one(int symbol_id, QDateTime from, QString interval = "OneDay");

    static bool     insert(Candle data, QString interval = "OneDay");
    static bool     save(Candle data, QString interval = "OneDay");
    static bool     savecalc(Candle data, QString interval = "OneDay");

private:

    static Candles  fetchAll(QSqlQuery &query);
    static Candle   fetch(QSqlQuery &query);
    static bool     zmqEmit(Candle candle);
};

#endif // CANDLEMODEL_H
