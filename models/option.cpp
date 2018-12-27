#include "option.h"

Option OptionModel::fetch(QSqlQuery & query) {

    Option row;
    row.rowid = 0;
    if (query.exec()) {
        if (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            row.rowid                   = map["rowid"].toInt();
            row.underlying_id           = map["underlying_id"].toInt();
            row.underlying              = map["underlying"].toString();
            row.symbol_id               = map["symbol_id"].toInt();
            row.symbol                  = map["symbol"].toString();
            row.bid_price               = map["bid_price"].toDouble();
            row.ask_price               = map["ask_price"].toDouble();
            row.last_trade_price_trhrs  = map["last_trade_price_trhrs"].toDouble();
            row.last_trade_price        = map["last_trade_price"].toDouble();
            row.volume                  = map["volume"].toInt();
            row.open                    = map["open"].toDouble();
            row.high                    = map["high"].toDouble();
            row.low                     = map["low"].toDouble();
            row.volatility              = map["volatility"].toDouble();
            row.delta                   = map["delta"].toDouble();
            row.gamma                   = map["gamma"].toDouble();
            row.theta                   = map["theta"].toDouble();
            row.vega                    = map["vega"].toDouble();
            row.rho                     = map["rho"].toDouble();
            row.vwap                    = map["vwap"].toDouble();
            row.open_interest           = map["open_interest"].toInt();
            row.delay                   = map["delay"].toInt();
            row.is_halted               = map["is_halted"].toBool();
            row.last_updated            = map["last_updated"].toDateTime();
        }
    }
    return row;
}


Options OptionModel::fetchAll(QSqlQuery query) {

    Options rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            Option row;
            row.rowid                   = map["rowid"].toInt();
            row.underlying_id           = map["underlying_id"].toInt();
            row.underlying              = map["underlying"].toString();
            row.symbol_id               = map["symbol_id"].toInt();
            row.symbol                  = map["symbol"].toString();
            row.bid_price               = map["bid_price"].toDouble();
            row.ask_price               = map["ask_price"].toDouble();
            row.last_trade_price_trhrs  = map["last_trade_price_trhrs"].toDouble();
            row.last_trade_price        = map["last_trade_price"].toDouble();
            row.volume                  = map["volume"].toInt();
            row.open                    = map["open"].toDouble();
            row.high                    = map["high"].toDouble();
            row.low                     = map["low"].toDouble();
            row.volatility              = map["volatility"].toDouble();
            row.delta                   = map["delta"].toDouble();
            row.gamma                   = map["gamma"].toDouble();
            row.theta                   = map["theta"].toDouble();
            row.vega                    = map["vega"].toDouble();
            row.rho                     = map["rho"].toDouble();
            row.vwap                    = map["vwap"].toDouble();
            row.open_interest           = map["open_interest"].toInt();
            row.delay                   = map["delay"].toInt();
            row.is_halted               = map["is_halted"].toBool();
            row.last_updated            = map["last_updated"].toDateTime();
            rows.append(row);
        }
    }
    return rows;
}


/**
 * Select one option by symbol_id
 */
Option OptionModel::select_one_symbol(int symbol_id) {

    Option row;
    QString sql = "SELECT * FROM `option` WHERE symbol_id=:symbol_id LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    row = fetch(q);
    return row;
}


/**
 * Select one at the money option
 */
Option OptionModel::select_one_atm(Stock stock, int horizon) {

    QDateTime now = QDateTime::currentDateTime();
    QDateTime to = now.addDays(horizon);

    Option row;
    QString sql = "SELECT * FROM option"
                  " WHERE underlying_id=:underlying_id"
                  " AND expiry_date > NOW()"
                  " AND expiry_date <= :to"
                  " AND strike_price >= ("
                  "  SELECT DISTINCT strike_price FROM option_chain"
                  " WHERE underlying_id=:underlying_id"
                  " AND expiry_date > NOW()"
                  " AND expiry_date <= :to"
                  " AND strike_price < :last_price"
                  " ORDER BY strike_price DESC"
                  " LIMIT 1,1"
                  ")"
                  " AND strike_price <= ("
                  "  SELECT DISTINCT strike_price FROM option_chain"
                  " WHERE underlying_id=:underlying_id"
                  " AND expiry_date > NOW()"
                  " AND expiry_date <= :to"
                  " AND strike_price >= :last_price"
                  " ORDER BY strike_price ASC"
                  " LIMIT 0,1"
                  ")"
                  " ORDER BY expiry_date, strike_price;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":underlying_id", stock.symbol_id);
    q.bindValue(":expiry_date", to);
    q.bindValue(":strike_price", stock.last_price);
    row = fetch(q);
    return row;
}


/**
 * Select one option by rowid
 */
Option OptionModel::select_one_row(int rowid) {

    Option row;
    QString sql = "SELECT * FROM `option` WHERE rowid=:rowid LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", rowid);
    row = fetch(q);
    return row;
}


/**
 * Insert option
 */
bool OptionModel::insert(Option data) {

    QString sql = "INSERT INTO `option` ("
                  " underlying_id,"
                  " underlying,"
                  " symbol_id,"
                  " symbol,"
                  " bid_price,"
                  " ask_price,"
                  " last_trade_price_trhrs,"
                  " last_trade_price,"
                  " volume,"
                  " open,"
                  " high,"
                  " low,"
                  " volatility,"
                  " delta,"
                  " gamma,"
                  " theta,"
                  " vega,"
                  " rho,"
                  " vwap,"
                  " open_interest,"
                  " delay,"
                  " is_halted,"
                  " last_updated"
                  " ) VALUES ( "
                  " :underlying_id,"
                  " :underlying,"
                  " :symbol_id,"
                  " :symbol,"
                  " :bid_price,"
                  " :ask_price,"
                  " :last_trade_price_trhrs,"
                  " :last_trade_price,"
                  " :volume,"
                  " :open,"
                  " :high,"
                  " :low,"
                  " :volatility,"
                  " :delta,"
                  " :gamma,"
                  " :theta,"
                  " :vega,"
                  " :rho,"
                  " :vwap,"
                  " :open_interest,"
                  " :delay,"
                  " :is_halted,"
                  " NOW()"
                  " );";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":underlying_id", data.underlying_id);
    q.bindValue(":underlying", data.underlying);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":symbol", data.symbol);
    q.bindValue(":bid_price", data.bid_price);
    q.bindValue(":ask_price", data.ask_price);
    q.bindValue(":last_trade_price_trhrs", data.last_trade_price_trhrs);
    q.bindValue(":last_trade_price", data.last_trade_price);
    q.bindValue(":volume", data.volume);
    q.bindValue(":open", data.open);
    q.bindValue(":high", data.high);
    q.bindValue(":low", data.low);
    q.bindValue(":volatility", data.volatility);
    q.bindValue(":delta", data.delta);
    q.bindValue(":gamma", data.gamma);
    q.bindValue(":theta", data.theta);
    q.bindValue(":vega", data.vega);
    q.bindValue(":rho", data.rho);
    q.bindValue(":vwap", data.vwap);
    q.bindValue(":open_interest", data.open_interest);
    q.bindValue(":delay", data.delay);
    q.bindValue(":is_halted", data.is_halted);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Update option
 */
bool OptionModel::update(Option data) {

    QString sql = "UPDATE `option` SET"
                  " underlying_id=:underlying_id,"
                  " underlying=:underlying,"
                  " symbol_id=:symbol_id,"
                  " symbol=:symbol,"
                  " bid_price=:bid_price,"
                  " ask_price=:ask_price,"
                  " last_trade_price_trhrs=:last_trade_price_trhrs,"
                  " last_trade_price=:last_trade_price,"
                  " volume=:volume,"
                  " open=:open,"
                  " high=:high,"
                  " low=:low,"
                  " volatility=:volatility,"
                  " delta=:delta,"
                  " gamma=:gamma,"
                  " theta=:theta,"
                  " vega=:vega,"
                  " rho=:rho,"
                  " vwap=:vwap,"
                  " open_interest=:open_interest,"
                  " delay=:delay,"
                  " is_halted=:is_halted,"
                  " last_updated=NOW()"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":underlying_id", data.underlying_id);
    q.bindValue(":underlying", data.underlying);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":symbol", data.symbol);
    q.bindValue(":bid_price", data.bid_price);
    q.bindValue(":ask_price", data.ask_price);
    q.bindValue(":last_trade_price_trhrs", data.last_trade_price_trhrs);
    q.bindValue(":last_trade_price", data.last_trade_price);
    q.bindValue(":volume", data.volume);
    q.bindValue(":open", data.open);
    q.bindValue(":high", data.high);
    q.bindValue(":low", data.low);
    q.bindValue(":volatility", data.volatility);
    q.bindValue(":delta", data.delta);
    q.bindValue(":gamma", data.gamma);
    q.bindValue(":theta", data.theta);
    q.bindValue(":vega", data.vega);
    q.bindValue(":rho", data.rho);
    q.bindValue(":vwap", data.vwap);
    q.bindValue(":open_interest", data.open_interest);
    q.bindValue(":delay", data.delay);
    q.bindValue(":is_halted", data.is_halted);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Save option (update or insert)
 */
bool OptionModel::save(Option data) {

    bool result = false;
    if (data.rowid > 0) {
        result = OptionModel::update(data);
    } else {
        result = OptionModel::insert(data);
    }
    return result;
}


/**
 * Option Chains
 */

OptionChain OptionChainModel::fetch(QSqlQuery & query) {

    OptionChain row;
    row.rowid = 0;
    if (query.exec()) {
        if (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            row.rowid               = map["rowid"].toInt();
            row.underlying_id       = map["underlying_id"].toInt();
            row.underlying          = map["underlying"].toString();
            row.expiry_date         = map["expiry_date"].toDateTime();
            row.strike_price        = map["strike_price"].toDouble();
            row.call_symbol_id      = map["call_symbol_id"].toInt();
            row.put_symbol_id       = map["put_symbol_id"].toInt();
            row.multiplier          = map["multiplier"].toInt();
            row.last_updated        = map["last_updated"].toDateTime();
        }
    }
    return row;
}


OptionChains OptionChainModel::fetchAll(QSqlQuery query) {

    OptionChains rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            OptionChain row;
            row.rowid               = map["rowid"].toInt();
            row.underlying_id       = map["underlying_id"].toInt();
            row.underlying          = map["underlying"].toString();
            row.expiry_date         = map["expiry_date"].toDateTime();
            row.strike_price        = map["strike_price"].toDouble();
            row.call_symbol_id      = map["call_symbol_id"].toInt();
            row.put_symbol_id       = map["put_symbol_id"].toInt();
            row.multiplier          = map["multiplier"].toInt();
            row.last_updated        = map["last_updated"].toDateTime();
            rows.append(row);
        }
    }
    return rows;
}


/**
 * Select option chains
 */
OptionChains OptionChainModel::select_best_range(Stock stock) {

    QDateTime now = QDateTime::currentDateTime();
    QDateTime to = now.addDays(120);

    OptionChains rows;
    QString sql = "SELECT * FROM option_chain"
                  " WHERE underlying_id=:underlying_id"
                  " AND expiry_date > NOW()"
                  " AND expiry_date <= :to"
                  " AND strike_price >= ("
                  "  SELECT DISTINCT strike_price FROM option_chain"
                  " WHERE underlying_id=:underlying_id"
                  " AND expiry_date > NOW()"
                  " AND expiry_date <= :to"
                  " AND strike_price < :last_price"
                  " ORDER BY strike_price DESC"
                  " LIMIT 1,1"
                  ")"
                  " AND strike_price <= ("
                  "  SELECT DISTINCT strike_price FROM option_chain"
                  " WHERE underlying_id=:underlying_id"
                  " AND expiry_date > NOW()"
                  " AND expiry_date <= :to"
                  " AND strike_price >= :last_price"
                  " ORDER BY strike_price ASC"
                  " LIMIT 0,1"
                  ")"
                  " ORDER BY expiry_date, strike_price;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":underlying_id", stock.symbol_id);
    q.bindValue(":to", to);
    q.bindValue(":last_price", stock.last_price);
    rows = fetchAll(q);
    return rows;
}


/**
 * Select one option chain
 */
OptionChain OptionChainModel::select_one_strike(int symbol_id, QDateTime expiry, float strike) {

    OptionChain row;
    QString sql = "SELECT * FROM option_chain"
                  " WHERE underlying_id=:underlying_id"
                  " AND expiry_date=:expiry_date"
                  " AND strike_price=:strike_price"
                  " LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":underlying_id", symbol_id);
    q.bindValue(":expiry_date", expiry.toLocalTime());
    q.bindValue(":strike_price", strike);
    row = fetch(q);
    return row;
}


/**
 * Select one option chain by rowid
 */
OptionChain OptionChainModel::select_one_row(int rowid) {

    OptionChain row;
    QString sql = "SELECT * FROM option_chain WHERE rowid=:rowid LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", rowid);
    row = fetch(q);
    return row;
}


/**
 * Insert option chain
 */
bool OptionChainModel::insert(OptionChain data) {

    QString sql = "INSERT INTO option_chain ("
                  " underlying_id,"
                  " underlying,"
                  " expiry_date,"
                  " strike_price,"
                  " call_symbol_id,"
                  " put_symbol_id,"
                  " multiplier,"
                  " last_updated"
                  " ) VALUES ( "
                  " :underlying_id,"
                  " :underlying,"
                  " :expiry_date,"
                  " :strike_price,"
                  " :call_symbol_id,"
                  " :put_symbol_id,"
                  " :multiplier,"
                  " NOW()"
                  " );";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":underlying_id", data.underlying_id);
    q.bindValue(":underlying", data.underlying);
    q.bindValue(":expiry_date", data.expiry_date);
    q.bindValue(":strike_price", data.strike_price);
    q.bindValue(":call_symbol_id", data.call_symbol_id);
    q.bindValue(":put_symbol_id", data.put_symbol_id);
    q.bindValue(":multiplier", data.multiplier);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Update option chain
 */
bool OptionChainModel::update(OptionChain data) {

    QString sql = "UPDATE option_chain SET"
                  " underlying_id=:underlying_id,"
                  " underlying=:underlying,"
                  " expiry_date=:expiry_date,"
                  " strike_price=:strike_price,"
                  " call_symbol_id=:call_symbol_id,"
                  " put_symbol_id=:put_symbol_id,"
                  " multiplier=:multiplier,"
                  " last_updated=NOW()"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":underlying_id", data.underlying_id);
    q.bindValue(":underlying", data.underlying);
    q.bindValue(":expiry_date", data.expiry_date);
    q.bindValue(":strike_price", data.strike_price);
    q.bindValue(":call_symbol_id", data.call_symbol_id);
    q.bindValue(":put_symbol_id", data.put_symbol_id);
    q.bindValue(":multiplier", data.multiplier);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Save option chain (update or insert)
 */
bool OptionChainModel::save(OptionChain data) {

    bool result = false;
    if (data.rowid > 0) {
        result = OptionChainModel::update(data);
    } else {
        result = OptionChainModel::insert(data);
    }
    return result;
}
