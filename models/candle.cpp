#include "candle.h"


bool Candle::calculate() {

    // derived from candle core

    bullish             = ( close > 0 && close > open );

    bearish             = ( close > 0 && close < open );

    flat                = ( close > 0 && close == open );

    full_range          = high - low;

    body_range          = (bullish) ? close - open : open - close;

    body_upper_price    = (open > close) ? open : close;

    body_lower_price    = (open > close) ? open : close;

    wick_upper_range    = high - body_upper_price;

    wick_lower_range    = body_lower_price - low;

    wick_range          = wick_upper_range + wick_lower_range;

    wick_upper_size     = ( full_range == 0 )
                          ? 0
                          : wick_upper_range / full_range;

    wick_lower_size     = ( full_range == 0 )
                          ? 0
                          : wick_lower_range / full_range;

    wick_size           = wick_upper_size + wick_lower_size;

    long_upper_wick     = ( wick_upper_size >= 0.618 );

    long_lower_wick     = ( wick_lower_size >= 0.618 );

    short_upper_wick     = ( wick_upper_size <= 0.1 );

    short_lower_wick     = ( wick_lower_size >= 0.1 );


    // derived from candle indicators

    vol_magnitude = volume / vol_20;

    abr_magnitude = body_range / abr_20;

    afr_magnitude = full_range / afr_20;

    atr_magnitude = true_range / atr_20;

    average_day = ( abr_20 > 0 && abr_magnitude > 0.5 && abr_magnitude < 1.5 );

    short_day   = ( abr_20 > 0 && abr_magnitude <= 0.5 );

    long_day    = ( abr_20 > 0 && abr_magnitude >= 2 );

    marabozu    = ( long_day == true &&
                    wick_upper_size <= 0.02 &&
                    wick_lower_size <= 0.02 );

    return true;
}


/**
 * Database
 */

Candle CandleModel::fetch(QSqlQuery & query) {

    Candle row;
    row.rowid = 0;
    if (query.exec()) {
        if (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            row.rowid              = map["rowid"].toInt();
            row.symbol_id          = map["symbol_id"].toInt();
            row.start              .setTime_t(map["start"].toUInt());
            row.end                .setTime_t(map["end"].toUInt());
            row.open               = map["open"].toDouble();
            row.high               = map["high"].toDouble();
            row.low                = map["low"].toDouble();
            row.close              = map["close"].toDouble();
            row.volume             = map["volume"].toInt();
            row.vwap               = map["vwap"].toDouble();

            row.true_range         = map["true_range"].toDouble();
            row.abr_20             = map["abr_20"].toDouble();
            row.afr_20             = map["afr_20"].toDouble();
            row.atr_20             = map["atr_20"].toDouble();
            row.vol_20             = map["vol_20"].toDouble();
            row.ema_10             = map["ema_10"].toDouble();
            row.ema_20             = map["ema_20"].toDouble();
            row.ema_50             = map["ema_50"].toDouble();
            row.sma_10             = map["sma_10"].toDouble();
            row.sma_20             = map["sma_20"].toDouble();
            row.sma_40             = map["sma_40"].toDouble();
            row.sma_100            = map["sma_100"].toDouble();
            row.sma_200            = map["sma_200"].toDouble();

            row.calculate();
        }
    }
    return row;
}


Candles CandleModel::fetchAll(QSqlQuery & query) {

    Candles rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            Candle row;
            row.rowid              = map["rowid"].toInt();
            row.symbol_id          = map["symbol_id"].toInt();
            row.start              .setTime_t(map["start"].toUInt());
            row.end                .setTime_t(map["end"].toUInt());
            row.open               = map["open"].toDouble();
            row.high               = map["high"].toDouble();
            row.low                = map["low"].toDouble();
            row.close              = map["close"].toDouble();
            row.volume             = map["volume"].toInt();
            row.vwap               = map["vwap"].toDouble();

            row.true_range         = map["true_range"].toDouble();
            row.abr_20             = map["abr_20"].toDouble();
            row.afr_20             = map["afr_20"].toDouble();
            row.atr_20             = map["atr_20"].toDouble();
            row.vol_20             = map["vol_20"].toDouble();
            row.ema_10             = map["ema_10"].toDouble();
            row.ema_20             = map["ema_20"].toDouble();
            row.ema_50             = map["ema_50"].toDouble();
            row.sma_10             = map["sma_10"].toDouble();
            row.sma_20             = map["sma_20"].toDouble();
            row.sma_40             = map["sma_40"].toDouble();
            row.sma_100            = map["sma_100"].toDouble();
            row.sma_200            = map["sma_200"].toDouble();

            row.calculate();
            rows.append(row);
        }
    }
    return rows;
}


/**
 * Select candle range
 */
Candles CandleModel::select_range(int symbol_id, QDateTime from, QDateTime to, QString interval, bool descending) {

    Candles rows;
    QString table;
    if (interval == "OneDay") table = "candle";
    if (interval == "OneHour") table = "candle_hour";
    if (interval == "OneMinute") table = "candle_minute";
    if (table.size() == 0) return rows;

    QString sort;
    if (descending == true) sort = QString("DESC");
    if (descending == false) sort = QString("ASC");

    QString sql = "SELECT * FROM ";
            sql.append(table);
            sql.append(
                  " WHERE symbol_id = :symbol_id"
                  " AND start >= :start"
                  " AND start <= :end"
                  " ORDER BY start ");
            sql.append(sort);

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    q.bindValue(":start", from.toTime_t());
    q.bindValue(":end", to.toTime_t());
    rows = fetchAll(q);
    return rows;
}


/**
 * Select top n candles
 */
Candles CandleModel::select_n(int symbol_id, QDateTime start, int n, QString interval, bool descending) {

    Candles rows;
    QString table;
    if (interval == "OneDay") table = "candle";
    if (interval == "OneHour") table = "candle_hour";
    if (interval == "OneMinute") table = "candle_minute";
    if (table.size() == 0) return rows;

    QString sql = "SELECT * FROM ";
            sql.append(table);
            sql.append(" WHERE symbol_id = :symbol_id"
                       " AND start <= :start ORDER BY start DESC"
                       " LIMIT ");
            sql.append(QString::number(n));

    if (descending == false) {
        sql.prepend("SELECT * FROM (");
        sql.append(") AS c ORDER BY c.start ASC;");
    }

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    q.bindValue(":start", start.toTime_t());
    rows = fetchAll(q);
    return rows;
}


/**
 * Select one candle
 */
Candle CandleModel::select_one(int symbol_id, QDateTime from, QString interval) {

    Candle row;
    QString table;
    if (interval == "OneDay") table = "candle";
    if (interval == "OneHour") table = "candle_hour";
    if (interval == "OneMinute") table = "candle_minute";
    if (table.size() == 0) return row;

    QString sql = "SELECT * FROM ";
            sql.append(table);
            sql.append(" WHERE symbol_id = :symbol_id"
                       " AND start = :start"
                       " LIMIT 1;");

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    q.bindValue(":start", from.toTime_t());
    row = fetch(q);
    return row;
}


/**
 * Select latest candle
 */
Candle CandleModel::select_last(int symbol_id, QString interval) {

    Candle row;
    QString table;
    if (interval == "OneDay") table = "candle";
    if (interval == "OneHour") table = "candle_hour";
    if (interval == "OneMinute") table = "candle_minute";
    if (table.size() == 0) return row;

    QString sql = "SELECT * FROM ";
            sql.append(table);
            sql.append(
                  " WHERE symbol_id=:symbol_id"
                  " ORDER BY start DESC"
                  " LIMIT 1;");

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    row = fetch(q);
    return row;
}


/**
 * Select first candle
 */
Candle CandleModel::select_first(int symbol_id, QString interval) {

    Candle row;
    QString table;
    if (interval == "OneDay") table = "candle";
    if (interval == "OneHour") table = "candle_hour";
    if (interval == "OneMinute") table = "candle_minute";
    if (table.size() == 0) return row;

    QString sql = "SELECT * FROM ";
            sql.append(table);
            sql.append(
                  " WHERE symbol_id=:symbol_id"
                  " ORDER BY start ASC"
                  " LIMIT 1;");

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    row = fetch(q);
    return row;
}


/**
 * Insert candle
 */
bool CandleModel::insert(Candle data, QString interval) {

    QString table;
    if (interval == "OneDay") table = "candle";
    if (interval == "OneHour") table = "candle_hour";
    if (interval == "OneMinute") table = "candle_minute";
    if (table.size() == 0) return false;

    QString sql = "INSERT INTO ";
            sql.append(table);
            sql.append(" ("
                  " symbol_id,"
                  " start,"
                  " end,"
                  " open,"
                  " high,"
                  " low,"
                  " close,"
                  " volume,"
                  " vwap"
                  " ) VALUES ( "
                  " :symbol_id,"
                  " :start,"
                  " :end,"
                  " :open,"
                  " :high,"
                  " :low,"
                  " :close,"
                  " :volume,"
                  " :vwap"
                  " );");

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":start", data.start.toTime_t());
    q.bindValue(":end", data.end.toTime_t());
    q.bindValue(":open", data.open);
    q.bindValue(":high", data.high);
    q.bindValue(":low", data.low);
    q.bindValue(":close", data.close);
    q.bindValue(":volume", data.volume);
    q.bindValue(":vwap", data.vwap);
    bool result = q.exec();
    q.finish();

    if (interval == "OneDay") CandleModel::zmqEmit(data);

    return result;
}


/**
 * Update candle
 */
bool CandleModel::save(Candle data, QString interval) {

    QString table;
    if (interval == "OneDay") table = "candle";
    if (interval == "OneHour") table = "candle_hour";
    if (interval == "OneMinute") table = "candle_minute";
    if (table.size() == 0) return false;

    QString sql = "UPDATE ";
            sql.append(table);
            sql.append(" SET"
                  " symbol_id=:symbol_id,"
                  " start=:start,"
                  " end=:end,"
                  " open=:open,"
                  " high=:high,"
                  " low=:low,"
                  " close=:close,"
                  " volume=:volume,"
                  " vwap=:vwap"
                  " WHERE rowid=:rowid;");

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":start", data.start.toTime_t());
    q.bindValue(":end", data.end.toTime_t());
    q.bindValue(":open", data.open);
    q.bindValue(":high", data.high);
    q.bindValue(":low", data.low);
    q.bindValue(":close", data.close);
    q.bindValue(":volume", data.volume);
    q.bindValue(":vwap", data.vwap);
    bool result = q.exec();
    q.finish();

    if (interval == "OneDay") CandleModel::zmqEmit(data);

    return result;
}


/**
 * Update indicators
 */
bool CandleModel::savecalc(Candle data, QString interval) {

    QString table;
    if (interval == "OneDay") table = "candle";
    if (interval == "OneHour") table = "candle_hour";
    if (interval == "OneMinute") table = "candle_minute";
    if (table.size() == 0) return false;

    QString sql = "UPDATE ";
            sql.append(table);
            sql.append(" SET"
                  " true_range=:true_range,"
                  " abr_20=:abr_20,"
                  " afr_20=:afr_20,"
                  " atr_20=:atr_20,"
                  " ema_10=:ema_10,"
                  " ema_20=:ema_20,"
                  " ema_50=:ema_50,"
                  " sma_10=:sma_10,"
                  " sma_20=:sma_20,"
                  " sma_40=:sma_40,"
                  " sma_100=:sma_100,"
                  " sma_200=:sma_200,"
                  " vol_20=:vol_20"
                  " WHERE rowid=:rowid;");

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":true_range", data.true_range);
    q.bindValue(":abr_20", data.abr_20);
    q.bindValue(":afr_20", data.afr_20);
    q.bindValue(":atr_20", data.atr_20);
    q.bindValue(":ema_10", data.ema_10);
    q.bindValue(":ema_20", data.ema_20);
    q.bindValue(":ema_50", data.ema_50);
    q.bindValue(":sma_10", data.sma_10);
    q.bindValue(":sma_20", data.sma_20);
    q.bindValue(":sma_40", data.sma_40);
    q.bindValue(":sma_100", data.sma_100);
    q.bindValue(":sma_200", data.sma_200);
    q.bindValue(":vol_20", data.vol_20);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * ZeroMQ output
 */
bool CandleModel::zmqEmit(Candle candle) {

    QJsonObject data;
    data["type"] = "data";
    data["table"] = "candle";
    data["id"] = candle.symbol_id;
    data["row"] = candle.toJsonObject();

    QJsonDocument doc(data);
    QByteArray bytes = doc.toJson();
    ZeroMq::push(bytes);

    return true;
}
