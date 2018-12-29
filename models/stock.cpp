#include "stock.h"


Stock StockModel::fetch(QSqlQuery & query) {

    Stock row;
    if (query.exec()) {
        if (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            row.rowid               = map["rowid"].toInt();
            row.status              = map["status"].toString();
            row.symbol              = map["symbol"].toString();
            row.symbol_id           = map["symbol_id"].toInt();
            row.description         = map["description"].toString();
            row.currency            = map["currency"].toString();
            row.exchange            = map["exchange"].toString();
            row.trade_unit          = map["trade_unit"].toInt();
            row.is_index            = map["is_index"].toBool();
            row.is_tradable         = map["is_tradable"].toBool();
            row.is_quotable         = map["is_quotable"].toBool();
            row.has_options         = map["has_options"].toBool();
            row.security_type       = map["security_type"].toString();
            row.prev_day_close_price = map["prev_day_close_price"].toFloat();
            row.high_price_52       = map["high_price_52"].toFloat();
            row.low_price_52        = map["low_price_52"].toFloat();
            row.average_vol_90      = map["average_vol_90"].toInt();
            row.average_vol_20      = map["average_vol_20"].toInt();
            row.outstanding_shares  = map["outstanding_shares"].toInt();
            row.industry_sector     = map["industry_sector"].toString();
            row.industry_group      = map["industry_group"].toString();
            row.industry_subgroup   = map["industry_subgroup"].toString();
            row.last_update         = map["last_update"].toDateTime();
            row.last_update_by      = map["last_update_by"].toString();
            row.last_price          = map["last_price"].toFloat();
            row.last_price_volume   = map["last_price_volume"].toInt();
            row.last_price_time     = map["last_price_time"].toDateTime();
        }
    }
    return row;
}


Stocks StockModel::fetchAll(QSqlQuery query) {

    Stocks rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            Stock row;
            row.rowid               = map["rowid"].toInt();
            row.status              = map["status"].toString();
            row.symbol              = map["symbol"].toString();
            row.symbol_id           = map["symbol_id"].toInt();
            row.description         = map["description"].toString();
            row.currency            = map["currency"].toString();
            row.exchange            = map["exchange"].toString();
            row.trade_unit          = map["trade_unit"].toInt();
            row.is_index            = map["is_index"].toBool();
            row.is_tradable         = map["is_tradable"].toBool();
            row.is_quotable         = map["is_quotable"].toBool();
            row.has_options         = map["has_options"].toBool();
            row.security_type       = map["security_type"].toString();
            row.prev_day_close_price = map["prev_day_close_price"].toFloat();
            row.high_price_52       = map["high_price_52"].toFloat();
            row.low_price_52        = map["low_price_52"].toFloat();
            row.average_vol_90      = map["average_vol_90"].toInt();
            row.average_vol_20      = map["average_vol_20"].toInt();
            row.outstanding_shares  = map["outstanding_shares"].toInt();
            row.industry_sector     = map["industry_sector"].toString();
            row.industry_group      = map["industry_group"].toString();
            row.industry_subgroup   = map["industry_subgroup"].toString();
            row.last_update         = map["last_update"].toDateTime();
            row.last_update_by      = map["last_update_by"].toString();
            row.last_price          = map["last_price"].toFloat();
            row.last_price_volume   = map["last_price_volume"].toInt();
            row.last_price_time     = map["last_price_time"].toDateTime();
            rows.append(row);
        }
    }
    return rows;
}


/**
 * Select all active stocks
 */
Stocks StockModel::select_all_active() {

    QString sql = "SELECT *"
                  " FROM stock"
                  " WHERE status='A';";

    Stocks rows;
    QSqlQuery q(Database::database());
    q.prepare(sql);
    rows = fetchAll(q);
    return rows;
}


/**
 * Select one by symbol_id
 */
Stock StockModel::select_one_symbol(int symbol_id) {

    QString sql = "SELECT *"
                  " FROM stock"
                  " WHERE symbol_id=:symbol_id"
                  " LIMIT 1;";

    Stock row;
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    row = fetch(q);
    return row;
}


/**
 * Select one by rowid
 */
Stock StockModel::select_one_row(int rowid) {

    QString sql = "SELECT *"
                  " FROM stock"
                  " WHERE rowid=:rowid"
                  " LIMIT 1;";

    Stock row;
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", rowid);
    row = fetch(q);
    return row;
}


/**
 * Update one
 */
bool StockModel::save(Stock data) {

    QString sql = "UPDATE stock SET"
                  " status=:status,"
                  " symbol=:symbol,"
                  " symbol_id=:symbol_id,"
                  " description=:description,"
                  " currency=:currency,"
                  " exchange=:exchange,"
                  " trade_unit=:trade_unit,"
                  " is_tradable=:is_tradable,"
                  " is_quotable=:is_quotable,"
                  " has_options=:has_options,"
                  " security_type=:security_type,"
                  " prev_day_close_price=:prev_day_close_price,"
                  " high_price_52=:high_price_52,"
                  " low_price_52=:low_price_52,"
                  " average_vol_90=:average_vol_90,"
                  " average_vol_20=:average_vol_20,"
                  " outstanding_shares=:outstanding_shares,"
                  " industry_sector=:industry_sector,"
                  " industry_group=:industry_group,"
                  " industry_subgroup=:industry_subgroup,"
                  " last_update_by='ampanov-server',"
                  " last_update=NOW()"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":status", data.status);
    q.bindValue(":symbol", data.symbol);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":description", data.description);
    q.bindValue(":currency", data.currency);
    q.bindValue(":exchange", data.exchange);
    q.bindValue(":trade_unit", data.trade_unit);
    q.bindValue(":is_tradable", data.is_tradable);
    q.bindValue(":is_quotable", data.is_quotable);
    q.bindValue(":has_options", data.has_options);
    q.bindValue(":security_type", data.security_type);
    q.bindValue(":prev_day_close_price", data.prev_day_close_price);
    q.bindValue(":high_price_52", data.high_price_52);
    q.bindValue(":low_price_52", data.low_price_52);
    q.bindValue(":average_vol_90", data.average_vol_90);
    q.bindValue(":average_vol_20", data.average_vol_20);
    q.bindValue(":outstanding_shares", data.outstanding_shares);
    q.bindValue(":industry_sector", data.industry_sector);
    q.bindValue(":industry_group", data.industry_group);
    q.bindValue(":industry_subgroup", data.industry_subgroup);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Update last price
 */
bool StockModel::update_last_price(int symbol_id, double last_price, int last_volume, QDateTime last_time) {

    QString sql = "UPDATE stock SET"
                  " last_price=:last_price,"
                  " last_price_volume=:last_price_volume,"
                  " last_price_time=:last_price_time"
                  " WHERE symbol_id=:symbol_id;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    q.bindValue(":last_price", last_price);
    q.bindValue(":last_price_volume", last_volume);
    q.bindValue(":last_price_time", last_time);
    bool result = q.exec();
    q.finish();

    Stock s = StockModel::select_one_symbol(symbol_id);
    StockModel::zmqEmit(s);

    return result;
}


/**
 * ZeroMQ output
 */
bool StockModel::zmqEmit(Stock stock) {

    QJsonObject data;
    data["type"] = "data";
    data["table"] = "stock";
    data["id"] = stock.symbol_id;
    data["row"] = stock.toJsonObject();

    QJsonDocument doc(data);
    QByteArray bytes = doc.toJson();
    ZeroMq::push(bytes);

    return true;
}
