#include "order.h"

Order OrderModel::fetch(QSqlQuery & query) {

    Order row;
    row.rowid = 0;
    if (query.exec()) {
        if (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            row.rowid               = map["rowid"].toInt();
            row.trade_id            = map["trade_id"].toInt();
            row.questrade_id        = map["questrade_id"].toInt();
            row.exchange_order_id   = map["exchange_order_id"].toString();
            row.account_number      = map["account_number"].toString();
            row.symbol              = map["symbol"].toString();
            row.symbol_id           = map["symbol_id"].toInt();
            row.side                = map["side"].toString();
            row.source              = map["source"].toString();
            row.state               = map["state"].toString();
            row.client_reason       = map["client_reason"].toString();
            row.strategy_type       = map["strategy_type"].toString();
            row.order_type          = map["order_type"].toString();
            row.time_in_force       = map["time_in_force"].toString();
            row.action              = map["action"].toString();
            row.quantity            = map["quantity"].toInt();
            row.total_quantity      = map["total_quantity"].toInt();
            row.open_quantity       = map["open_quantity"].toInt();
            row.filled_quantity     = map["filled_quantity"].toInt();
            row.canceled_quantity   = map["canceled_quantity"].toInt();
            row.limit_price         = map["limit_price"].toFloat();
            row.stop_price          = map["stop_price"].toFloat();
            row.avg_exec_price      = map["avg_exec_price"].toFloat();
            row.last_exec_price     = map["last_exec_price"].toFloat();
            row.commission_charged  = map["commission_charged"].toFloat();
            row.is_allornone        = map["is_allornone"].toBool();
            row.is_anonymous        = map["is_anonymous"].toBool();
            row.primary_route       = map["primary_route"].toString();
            row.secondary_route     = map["secondary_route"].toString();
            row.order_route         = map["order_route"].toString();
            row.created_by          = map["created_by"].toString();
            row.created_date        = map["created_date"].toDateTime();
            row.modified_by         = map["modified_by"].toString();
            row.modified_date       = map["modified_date"].toDateTime();
            row.questrade_created   = map["questrade_created"].toString();
            row.questrade_updated   = map["questrade_updated"].toString();
        }
    }
    return row;
}


Orders OrderModel::fetchAll(QSqlQuery query) {

    Orders rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            Order row;
            row.rowid               = map["rowid"].toInt();
            row.trade_id            = map["trade_id"].toInt();
            row.questrade_id        = map["questrade_id"].toInt();
            row.exchange_order_id   = map["exchange_order_id"].toString();
            row.account_number      = map["account_number"].toString();
            row.symbol              = map["symbol"].toString();
            row.symbol_id           = map["symbol_id"].toInt();
            row.side                = map["side"].toString();
            row.source              = map["source"].toString();
            row.state               = map["state"].toString();
            row.client_reason       = map["client_reason"].toString();
            row.strategy_type       = map["strategy_type"].toString();
            row.order_type          = map["order_type"].toString();
            row.time_in_force       = map["time_in_force"].toString();
            row.action              = map["action"].toString();
            row.quantity            = map["quantity"].toInt();
            row.total_quantity      = map["total_quantity"].toInt();
            row.open_quantity       = map["open_quantity"].toInt();
            row.filled_quantity     = map["filled_quantity"].toInt();
            row.canceled_quantity   = map["canceled_quantity"].toInt();
            row.limit_price         = map["limit_price"].toFloat();
            row.stop_price          = map["stop_price"].toFloat();
            row.avg_exec_price      = map["avg_exec_price"].toFloat();
            row.last_exec_price     = map["last_exec_price"].toFloat();
            row.commission_charged  = map["commission_charged"].toFloat();
            row.is_allornone        = map["is_allornone"].toBool();
            row.is_anonymous        = map["is_anonymous"].toBool();
            row.primary_route       = map["primary_route"].toString();
            row.secondary_route     = map["secondary_route"].toString();
            row.order_route         = map["order_route"].toString();
            row.created_by          = map["created_by"].toString();
            row.created_date        = map["created_date"].toDateTime();
            row.modified_by         = map["modified_by"].toString();
            row.modified_date       = map["modified_date"].toDateTime();
            row.questrade_created   = map["questrade_created"].toString();
            row.questrade_updated   = map["questrade_updated"].toString();
            rows.append(row);
        }
    }
    return rows;
}


/**
 * Select active orders
 */
Orders OrderModel::select_active() {

    Orders rows;
    QString sql = "SELECT * FROM orders"
                  ";";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    rows = fetchAll(q);
    return rows;
}


/**
 * Select one order by questrade ID
 */
Order OrderModel::select_one(int id) {

    Order row;
    QString sql = "SELECT * FROM orders WHERE id=:id LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":id", id);
    row = fetch(q);
    return row;
}


/**
 * Select one order by rowid
 */
Order OrderModel::select_one_row(int rowid) {

    Order row;
    QString sql = "SELECT * FROM orders WHERE rowid=:rowid LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", rowid);
    row = fetch(q);
    return row;
}


/**
 * Insert order
 */
bool OrderModel::insert(Order data) {

    QString sql = "INSERT INTO orders ("
                  " trade_id,"
                  " account_number,"
                  " symbol,"
                  " symbol_id,"
                  " order_type,"
                  " time_in_force,"
                  " action,"
                  " quantity,"
                  " limit_price,"
                  " is_allornone,"
                  " is_anonymous,"
                  " primary_route,"
                  " secondary_route,"
                  " created_by,"
                  " created_date"
                  " ) VALUES ( "
                  " :trade_id,"
                  " :account_number,"
                  " :symbol,"
                  " :symbol_id,"
                  " :order_type,"
                  " :time_in_force,"
                  " :action,"
                  " :quantity,"
                  " :limit_price,"
                  " :is_allornone,"
                  " :is_anonymous,"
                  " :primary_route,"
                  " :secondary_route,"
                  " 'ampanov-server',"
                  " NOW()"
                  " );";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":trade_id", data.trade_id);
    q.bindValue(":account_number", data.account_number);
    q.bindValue(":symbol", data.symbol);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":order_type", data.order_type);
    q.bindValue(":time_in_force", data.time_in_force);
    q.bindValue(":action", data.action);
    q.bindValue(":quantity", data.quantity);
    q.bindValue(":limit_price", data.limit_price);
    q.bindValue(":is_allornone", data.is_allornone);
    q.bindValue(":is_anonymous", data.is_anonymous);
    q.bindValue(":primary_route", data.primary_route);
    q.bindValue(":secondary_route", data.secondary_route);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Update order
 */
bool OrderModel::update(Order data) {

    QString sql = "UPDATE orders SET"
                  " trade_id=:trade_id,"
                  " account_number=:account_number,"
                  " symbol=:symbol,"
                  " symbol_id=:symbol_id,"
                  " order_type=:order_type,"
                  " time_in_force=:time_in_force,"
                  " action=:action,"
                  " quantity=:quantity,"
                  " limit_price=:limit_price,"
                  " is_allornone=:is_allornone,"
                  " is_anonymous=:is_anonymous,"
                  " primary_route=:primary_route,"
                  " secondary_route=:secondary_route,"
                  " modified_by='ampanov-server',"
                  " modified_date=NOW()"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":trade_id", data.trade_id);
    q.bindValue(":account_number", data.account_number);
    q.bindValue(":symbol", data.symbol);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":order_type", data.order_type);
    q.bindValue(":time_in_force", data.time_in_force);
    q.bindValue(":action", data.action);
    q.bindValue(":quantity", data.quantity);
    q.bindValue(":limit_price", data.limit_price);
    q.bindValue(":is_allornone", data.is_allornone);
    q.bindValue(":is_anonymous", data.is_anonymous);
    q.bindValue(":primary_route", data.primary_route);
    q.bindValue(":secondary_route", data.secondary_route);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Save order (update or insert)
 */
bool OrderModel::save(Order data) {

    bool result = false;
    if (data.rowid > 0) {
        result = OrderModel::update(data);
    } else {
        result = OrderModel::insert(data);
    }
    return result;
}


/**
 * Insert order
 */
bool OrderModel::insert_actual(Order data) {

    QString sql = "INSERT INTO orders ("
                  " questrade_id,"
                  " exchange_order_id,"
                  " account_number,"
                  " symbol,"
                  " symbol_id,"
                  " side,"
                  " source,"
                  " state,"
                  " client_reason,"
                  " strategy_type,"
                  " order_type,"
                  " time_in_force,"
                  " total_quantity,"
                  " open_quantity,"
                  " filled_quantity,"
                  " canceled_quantity,"
                  " limit_price,"
                  " stop_price,"
                  " avg_exec_price,"
                  " last_exec_price,"
                  " commission_charged,"
                  " is_allornone,"
                  " is_anonymous,"
                  " primary_route,"
                  " secondary_route,"
                  " order_route,"
                  " questrade_created,"
                  " questrade_updated,"
                  " created_by,"
                  " created_date"
                  " ) VALUES ( "
                  " :questrade_id,"
                  " :exchange_order_id,"
                  " :account_number,"
                  " :symbol,"
                  " :symbol_id,"
                  " :side,"
                  " :source,"
                  " :state,"
                  " :client_reason,"
                  " :strategy_type,"
                  " :order_type,"
                  " :time_in_force,"
                  " :total_quantity,"
                  " :open_quantity,"
                  " :filled_quantity,"
                  " :canceled_quantity,"
                  " :limit_price,"
                  " :stop_price,"
                  " :avg_exec_price,"
                  " :last_exec_price,"
                  " :commission_charged,"
                  " :is_allornone,"
                  " :is_anonymous,"
                  " :primary_route,"
                  " :secondary_route,"
                  " :order_route,"
                  " :questrade_created,"
                  " :questrade_updated,"
                  " 'ampanov-server',"
                  " NOW()"
                  " );";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":questrade_id", data.questrade_id);
    q.bindValue(":exchange_order_id", data.exchange_order_id);
    q.bindValue(":account_number", data.account_number);
    q.bindValue(":symbol", data.symbol);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":side", data.side);
    q.bindValue(":source", data.source);
    q.bindValue(":state", data.state);
    q.bindValue(":client_reason", data.client_reason);
    q.bindValue(":strategy_type", data.strategy_type);
    q.bindValue(":order_type", data.order_type);
    q.bindValue(":time_in_force", data.time_in_force);
    q.bindValue(":total_quantity", data.total_quantity);
    q.bindValue(":open_quantity", data.open_quantity);
    q.bindValue(":filled_quantity", data.filled_quantity);
    q.bindValue(":canceled_quantity", data.canceled_quantity);
    q.bindValue(":limit_price", data.limit_price);
    q.bindValue(":stop_price", data.stop_price);
    q.bindValue(":avg_exec_price", data.avg_exec_price);
    q.bindValue(":last_exec_price", data.last_exec_price);
    q.bindValue(":commission_charged", data.commission_charged);
    q.bindValue(":is_allornone", data.is_allornone);
    q.bindValue(":is_anonymous", data.is_anonymous);
    q.bindValue(":primary_route", data.primary_route);
    q.bindValue(":secondary_route", data.secondary_route);
    q.bindValue(":order_route", data.order_route);
    q.bindValue(":questrade_created", data.questrade_created);
    q.bindValue(":questrade_updated", data.questrade_updated);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Update order
 */
bool OrderModel::update_actual(Order data) {

    QString sql = "UPDATE orders SET"
                  " questrade_id=:questrade_id,"
                  " exchange_order_id=:exchange_order_id,"
                  " account_number=:account_number,"
                  " symbol=:symbol,"
                  " symbol_id=:symbol_id,"
                  " side=:side,"
                  " source=:source,"
                  " state=:state,"
                  " client_reason=:client_reason,"
                  " strategy_type=:strategy_type,"
                  " order_type=:order_type,"
                  " time_in_force=:time_in_force,"
                  " total_quantity=:total_quantity,"
                  " open_quantity=:open_quantity,"
                  " filled_quantity=:filled_quantity,"
                  " canceled_quantity=:canceled_quantity,"
                  " limit_price=:limit_price,"
                  " stop_price=:stop_price,"
                  " avg_exec_price=:avg_exec_price,"
                  " last_exec_price=:last_exec_price,"
                  " commission_charged=:commission_charged,"
                  " is_allornone=:is_allornone,"
                  " is_anonymous=:is_anonymous,"
                  " primary_route=:primary_route,"
                  " secondary_route=:secondary_route,"
                  " order_route=:order_route,"
                  " questrade_created=:questrade_created,"
                  " questrade_updated=:questrade_updated,"
                  " modified_by='ampanov-server',"
                  " modified_date=NOW()"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":questrade_id", data.trade_id);
    q.bindValue(":exchange_order_id", data.exchange_order_id);
    q.bindValue(":account_number", data.account_number);
    q.bindValue(":symbol", data.symbol);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":side", data.side);
    q.bindValue(":source", data.source);
    q.bindValue(":state", data.state);
    q.bindValue(":client_reason", data.client_reason);
    q.bindValue(":strategy_type", data.strategy_type);
    q.bindValue(":order_type", data.order_type);
    q.bindValue(":time_in_force", data.time_in_force);
    q.bindValue(":total_quantity", data.total_quantity);
    q.bindValue(":open_quantity", data.open_quantity);
    q.bindValue(":filled_quantity", data.filled_quantity);
    q.bindValue(":canceled_quantity", data.canceled_quantity);
    q.bindValue(":limit_price", data.limit_price);
    q.bindValue(":stop_price", data.stop_price);
    q.bindValue(":avg_exec_price", data.avg_exec_price);
    q.bindValue(":last_exec_price", data.last_exec_price);
    q.bindValue(":commission_charged", data.commission_charged);
    q.bindValue(":is_allornone", data.is_allornone);
    q.bindValue(":is_anonymous", data.is_anonymous);
    q.bindValue(":primary_route", data.primary_route);
    q.bindValue(":secondary_route", data.secondary_route);
    q.bindValue(":order_route", data.order_route);
    q.bindValue(":questrade_created", data.questrade_created);
    q.bindValue(":questrade_updated", data.questrade_updated);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Save actual (update or insert)
 */
bool OrderModel::save_actual(Order data) {

    bool result = false;
    if (data.rowid > 0) {
        result = OrderModel::update_actual(data);
    } else {
        result = OrderModel::insert_actual(data);
    }
    return result;
}
