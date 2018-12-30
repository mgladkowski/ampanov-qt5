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
    double      open;
    double      high;
    double      low;
    double      close;
    int         volume;
    double      vwap;

    double      true_range;
    double      abr_20;
    double      afr_20;
    double      atr_20;
    double      vol_20;
    double      ema_10;
    double      ema_20;
    double      ema_50;
    double      sma_10;
    double      sma_20;
    double      sma_40;
    double      sma_100;
    double      sma_200;

    int         signal_candle;
    int         signal_chart;

    bool        calculate();

    bool        bullish;
    bool        bearish;
    bool        flat;
    double      full_range;
    double      body_range;
    double      body_upper_price;
    double      body_lower_price;
    double      wick_upper_range;
    double      wick_lower_range;
    double      wick_range;
    double      wick_upper_size;
    double      wick_lower_size;
    double      wick_size;
    bool        long_upper_wick;
    bool        long_lower_wick;
    bool        short_upper_wick;
    bool        short_lower_wick;

    double      vol_magnitude;
    double      abr_magnitude;
    double      afr_magnitude;
    double      atr_magnitude;
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

    static bool     reset();
    static bool     optimize();

private:

    static Candles  fetchAll(QSqlQuery &query);
    static Candle   fetch(QSqlQuery &query);
    static bool     zmqEmit(Candle candle);
};

#endif // CANDLEMODEL_H
