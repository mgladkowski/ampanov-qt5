#include "trade.h"


Trade TradeModel::fetch(QSqlQuery & query) {

    Trade row;
    row.rowid = 0;
    if (query.exec()) {
        if (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            row.rowid               = map["rowid"].toInt();
            row.account_id          = map["account_id"].toInt();
            row.symbol_id           = map["symbol_id"].toInt();
            row.symbol              = map["symbol"].toString();
            row.opt_symbol_id       = map["opt_symbol_id"].toInt();
            row.opt_symbol          = map["opt_symbol"].toString();
            row.type                = map["type"].toString();
            row.status              = map["status"].toString();
            row.direction           = map["direction"].toString();
            row.risk_unit           = map["risk_unit"].toFloat();
            row.plan_date           = map["plan_date"].toDateTime();
            row.plan_entry          = map["plan_entry"].toFloat();
            row.plan_stop           = map["plan_stop"].toFloat();
            row.plan_target         = map["plan_target"].toFloat();
            row.plan_risk           = map["plan_risk"].toFloat();
            row.plan_reward         = map["plan_reward"].toFloat();
            row.plan_ratio          = map["plan_ratio"].toFloat();
            row.plan_shares         = map["plan_shares"].toInt();
            row.plan_options        = map["plan_options"].toInt();
            row.exec_quantity       = map["exec_quantity"].toInt();
            row.exec_open_date      = map["exec_open_date"].toDateTime();
            row.exec_close_date     = map["exec_close_date"].toDateTime();
            row.exec_entry          = map["exec_entry"].toFloat();
            row.exec_entry_id       = map["exec_entry_id"].toInt();
            row.exec_stop           = map["exec_stop"].toFloat();
            row.exec_stop_id        = map["exec_stop_id"].toInt();
            row.exec_target         = map["exec_target"].toFloat();
            row.exec_target_id      = map["exec_target_id"].toInt();
            row.position_price      = map["position_price"].toFloat();
            row.position_average    = map["position_average"].toFloat();
            row.position_open       = map["position_open"].toInt();
            row.position_open_pnl   = map["position_open_pnl"].toFloat();
            row.position_closed     = map["position_closed"].toInt();
            row.position_closed_pnl = map["position_closed_pnl"].toFloat();
            row.position_total_cost = map["position_total_cost"].toFloat();
            row.position_value      = map["position_value"].toFloat();
            row.position_realtime   = map["position_realtime"].toBool();
            row.position_reorg      = map["position_reorg"].toBool();
            row.position_updated    = map["position_updated"].toDateTime();
        }
    }
    return row;
}


Trades TradeModel::fetchAll(QSqlQuery query) {

    Trades rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            Trade row;
            row.rowid               = map["rowid"].toInt();
            row.account_id          = map["account_id"].toInt();
            row.symbol_id           = map["symbol_id"].toInt();
            row.symbol              = map["symbol"].toString();
            row.opt_symbol_id       = map["opt_symbol_id"].toInt();
            row.opt_symbol          = map["opt_symbol"].toString();
            row.type                = map["type"].toString();
            row.status              = map["status"].toString();
            row.direction           = map["direction"].toString();
            row.risk_unit           = map["risk_unit"].toFloat();
            row.plan_date           = map["plan_date"].toDateTime();
            row.plan_entry          = map["plan_entry"].toFloat();
            row.plan_stop           = map["plan_stop"].toFloat();
            row.plan_target         = map["plan_target"].toFloat();
            row.plan_risk           = map["plan_risk"].toFloat();
            row.plan_reward         = map["plan_reward"].toFloat();
            row.plan_ratio          = map["plan_ratio"].toFloat();
            row.plan_shares         = map["plan_shares"].toInt();
            row.plan_options        = map["plan_options"].toInt();
            row.exec_quantity       = map["exec_quantity"].toInt();
            row.exec_open_date      = map["exec_open_date"].toDateTime();
            row.exec_close_date     = map["exec_close_date"].toDateTime();
            row.exec_entry          = map["exec_entry"].toFloat();
            row.exec_entry_id       = map["exec_entry_id"].toInt();
            row.exec_stop           = map["exec_stop"].toFloat();
            row.exec_stop_id        = map["exec_stop_id"].toInt();
            row.exec_target         = map["exec_target"].toFloat();
            row.exec_target_id      = map["exec_target_id"].toInt();
            row.position_price      = map["position_price"].toFloat();
            row.position_average    = map["position_average"].toFloat();
            row.position_open       = map["position_open"].toInt();
            row.position_open_pnl   = map["position_open_pnl"].toFloat();
            row.position_closed     = map["position_closed"].toInt();
            row.position_closed_pnl = map["position_closed_pnl"].toFloat();
            row.position_total_cost = map["position_total_cost"].toFloat();
            row.position_value      = map["position_value"].toFloat();
            row.position_realtime   = map["position_realtime"].toBool();
            row.position_reorg      = map["position_reorg"].toBool();
            row.position_updated    = map["position_updated"].toDateTime();
            rows.append(row);
        }
    }
    return rows;
}


/**
 * Select trades open
 */
Trades TradeModel::select_open() {

    Trades rows;
    QString sql = "SELECT * FROM trade"
                  " WHERE status='O'"
                  " ORDER BY exec_open_date ASC";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    rows = fetchAll(q);
    return rows;
}


/**
 * Select trades open per symbol
 */
Trades TradeModel::select_open(int symbol_id) {

    Trades rows;
    QString sql = "SELECT * FROM trade"
                  " WHERE status='O'"
                  " AND (symbol_id=:symbol_id OR opt_symbol_id=:symbol_id)"
                  " ORDER BY exec_open_date ASC";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    rows = fetchAll(q);
    return rows;
}


/**
 * Select one active trade
 */
Trade TradeModel::select_one_active(int account_row, int symbol_id) {

    Trade row;
    QString sql = "SELECT * FROM trade"
                  " WHERE account_id=:account_id"
                  " AND (symbol_id=:symbol_id OR opt_symbol_id=:symbol_id)"
                  " AND (status='A' OR status='E' OR status='O')"
                  " LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":account_id", account_row);
    q.bindValue(":symbol_id", symbol_id);
    row = fetch(q);
    return row;
}


/**
 * Select one trade
 */
Trade TradeModel::select_one_row(int rowid) {

    Trade row;
    QString sql = "SELECT * FROM trade WHERE rowid=:rowid LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", rowid);
    row = fetch(q);
    return row;
}


/**
 * Insert trade
 */
bool TradeModel::insert(Trade data) {

    QString sql = "INSERT INTO trade ("
                  " account_id,"
                  " symbol_id,"
                  " symbol,"
                  " opt_symbol_id,"
                  " opt_symbol,"
                  " type,"
                  " status,"
                  " direction,"
                  " risk_unit,"
                  " plan_date,"
                  " plan_entry,"
                  " plan_stop,"
                  " plan_target,"
                  " plan_risk,"
                  " plan_reward,"
                  " plan_ratio,"
                  " plan_shares,"
                  " plan_options,"
                  " exec_quantity,"
                  " exec_open_date,"
                  " exec_close_date,"
                  " exec_entry,"
                  " exec_entry_id,"
                  " exec_stop,"
                  " exec_stop_id,"
                  " exec_target,"
                  " exec_target_id,"
                  " position_price,"
                  " position_average,"
                  " position_open,"
                  " position_open_pnl,"
                  " position_closed,"
                  " position_closed_pnl,"
                  " position_total_cost,"
                  " position_value,"
                  " position_realtime,"
                  " position_reorg,"
                  " position_updated"
                  " ) VALUES ( "
                  " :account_id,"
                  " :symbol_id,"
                  " :symbol,"
                  " :opt_symbol_id,"
                  " :opt_symbol,"
                  " :type,"
                  " :status,"
                  " :direction,"
                  " :risk_unit,"
                  " :plan_date,"
                  " :plan_entry,"
                  " :plan_stop,"
                  " :plan_target,"
                  " :plan_risk,"
                  " :plan_reward,"
                  " :plan_ratio,"
                  " :plan_shares,"
                  " :plan_options,"
                  " :exec_quantity,"
                  " :exec_open_date,"
                  " :exec_close_date,"
                  " :exec_entry,"
                  " :exec_entry_id,"
                  " :exec_stop,"
                  " :exec_stop_id,"
                  " :exec_target,"
                  " :exec_target_id,"
                  " :position_price,"
                  " :position_average,"
                  " :position_open,"
                  " :position_open_pnl,"
                  " :position_closed,"
                  " :position_closed_pnl,"
                  " :position_total_cost,"
                  " :position_value,"
                  " :position_realtime,"
                  " :position_reorg,"
                  " NOW()"
                  " );";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":account_id", data.account_id);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":symbol", data.symbol);
    q.bindValue(":opt_symbol_id", data.opt_symbol_id);
    q.bindValue(":opt_symbol", data.opt_symbol);
    q.bindValue(":type", data.type);
    q.bindValue(":status", data.status);
    q.bindValue(":direction", data.direction);
    q.bindValue(":risk_unit", data.risk_unit);
    q.bindValue(":plan_date", data.plan_date);
    q.bindValue(":plan_entry", data.plan_entry);
    q.bindValue(":plan_stop", data.plan_stop);
    q.bindValue(":plan_target", data.plan_target);
    q.bindValue(":plan_risk", data.plan_risk);
    q.bindValue(":plan_reward", data.plan_reward);
    q.bindValue(":plan_ratio", data.plan_ratio);
    q.bindValue(":plan_shares", data.plan_shares);
    q.bindValue(":plan_options", data.plan_options);
    q.bindValue(":exec_quantity", data.exec_quantity);
    q.bindValue(":exec_open_date", data.exec_open_date);
    q.bindValue(":exec_close_date", data.exec_close_date);
    q.bindValue(":exec_entry", data.exec_entry);
    q.bindValue(":exec_entry_id", data.exec_entry_id);
    q.bindValue(":exec_stop", data.exec_stop);
    q.bindValue(":exec_stop_id", data.exec_stop_id);
    q.bindValue(":exec_target", data.exec_target);
    q.bindValue(":exec_target_id", data.exec_target_id);
    q.bindValue(":position_price", data.position_price);
    q.bindValue(":position_average", data.position_average);
    q.bindValue(":position_open", data.position_open);
    q.bindValue(":position_open_pnl", data.position_open_pnl);
    q.bindValue(":position_closed", data.position_closed);
    q.bindValue(":position_closed_pnl", data.position_closed_pnl);
    q.bindValue(":position_total_cost", data.position_total_cost);
    q.bindValue(":position_value", data.position_value);
    q.bindValue(":position_realtime", data.position_realtime);
    q.bindValue(":position_reorg", data.position_reorg);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Update trade
 */
bool TradeModel::update(Trade data) {

    QString sql = "UPDATE trade SET"
                  " account_id=:account_id,"
                  " symbol_id=:symbol_id,"
                  " symbol=:symbol,"
                  " opt_symbol_id=:opt_symbol_id,"
                  " opt_symbol=:opt_symbol,"
                  " type=:type,"
                  " status=:status,"
                  " direction=:direction,"
                  " risk_unit=:risk_unit,"
                  " plan_date=:plan_date,"
                  " plan_entry=:plan_entry,"
                  " plan_stop=:plan_stop,"
                  " plan_target=:plan_target,"
                  " plan_risk=:plan_risk,"
                  " plan_reward=:plan_reward,"
                  " plan_ratio=:plan_ratio,"
                  " plan_shares=:plan_shares,"
                  " plan_options=:plan_options,"
                  " exec_quantity=:exec_quantity,"
                  " exec_open_date=:exec_open_date,"
                  " exec_close_date=:exec_close_date,"
                  " exec_entry=:exec_entry,"
                  " exec_entry_id=:exec_entry_id,"
                  " exec_stop=:exec_stop,"
                  " exec_stop_id=:exec_stop_id,"
                  " exec_target=:exec_target,"
                  " exec_target_id=:exec_target_id,"
                  " position_price=:position_price,"
                  " position_average=:position_average,"
                  " position_open=:position_open,"
                  " position_open_pnl=:position_open_pnl,"
                  " position_closed=:position_closed,"
                  " position_closed_pnl=:position_closed_pnl,"
                  " position_total_cost=:position_total_cost,"
                  " position_value=:position_value,"
                  " position_realtime=:position_realtime,"
                  " position_reorg=:position_reorg,"
                  " position_updated=NOW()"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":account_id", data.account_id);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":symbol", data.symbol);
    q.bindValue(":opt_symbol_id", data.opt_symbol_id);
    q.bindValue(":opt_symbol", data.opt_symbol);
    q.bindValue(":type", data.type);
    q.bindValue(":status", data.status);
    q.bindValue(":direction", data.direction);
    q.bindValue(":risk_unit", data.risk_unit);
    q.bindValue(":plan_date", data.plan_date);
    q.bindValue(":plan_entry", data.plan_entry);
    q.bindValue(":plan_stop", data.plan_stop);
    q.bindValue(":plan_target", data.plan_target);
    q.bindValue(":plan_risk", data.plan_risk);
    q.bindValue(":plan_reward", data.plan_reward);
    q.bindValue(":plan_ratio", data.plan_ratio);
    q.bindValue(":plan_shares", data.plan_shares);
    q.bindValue(":plan_options", data.plan_options);
    q.bindValue(":exec_quantity", data.exec_quantity);
    q.bindValue(":exec_open_date", data.exec_open_date);
    q.bindValue(":exec_close_date", data.exec_close_date);
    q.bindValue(":exec_entry", data.exec_entry);
    q.bindValue(":exec_entry_id", data.exec_entry_id);
    q.bindValue(":exec_stop", data.exec_stop);
    q.bindValue(":exec_stop_id", data.exec_stop_id);
    q.bindValue(":exec_target", data.exec_target);
    q.bindValue(":exec_target_id", data.exec_target_id);
    q.bindValue(":position_price", data.position_price);
    q.bindValue(":position_average", data.position_average);
    q.bindValue(":position_open", data.position_open);
    q.bindValue(":position_open_pnl", data.position_open_pnl);
    q.bindValue(":position_closed", data.position_closed);
    q.bindValue(":position_closed_pnl", data.position_closed_pnl);
    q.bindValue(":position_total_cost", data.position_total_cost);
    q.bindValue(":position_value", data.position_value);
    q.bindValue(":position_realtime", data.position_realtime);
    q.bindValue(":position_reorg", data.position_reorg);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Save trade (update or insert)
 */
bool TradeModel::save(Trade data) {

    bool result = false;
    if (data.rowid > 0) {
        result = TradeModel::update(data);
    } else {
        result = TradeModel::insert(data);
    }
    return result;
}
