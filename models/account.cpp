#include "account.h"


Account AccountModel::fetch(QSqlQuery & query) {

    Account row;
    if (query.exec()) {
        if (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            row.rowid           = map["rowid"]          .toInt();
            row.user            = map["user"]           .toString();
            row.status          = map["status"]         .toString();
            row.type            = map["type"]           .toString();
            row.allow_ai        = map["allow_ai"]       .toString();
            row.name            = map["name"]           .toString();
            row.account         = map["account"]        .toString();
            row.currency        = map["currency"]       .toString();
            row.is_data         = map["is_data"]        .toBool();
            row.requires_key    = map["requires_key"]   .toBool();
            row.refresh_token   = map["refresh_token"]  .toString();
            row.refresh_expires = map["refresh_expires"].toDateTime();
            row.access_token    = map["access_token"]   .toString();
            row.access_type     = map["access_type"]    .toString();
            row.access_expires  = map["access_expires"] .toDateTime();
            row.api_server      = map["api_server"]     .toString();
            row.risk_unit       = map["risk_unit"]      .toFloat();
            row.risk_cap        = map["risk_cap"]       .toFloat();
            row.allow_stock     = map["allow_stock"]    .toBool();
            row.allow_option    = map["allow_option"]   .toBool();
            row.prefer_trade    = map["prefer_trade"]   .toString();
            row.created_by      = map["created_by"]     .toString();
            row.created_date    = map["created_date"]   .toDateTime();
            row.modified_by     = map["modified_by"]    .toString();
            row.modified_date   = map["modified_date"]  .toDateTime();
            row.cash            = map["cash"]           .toFloat();
            row.market_value    = map["market_value"]   .toFloat();
            row.total_equity    = map["total_equity"]   .toFloat();
            row.buying_power    = map["buying_power"]   .toFloat();
            row.maintenance_excess = map["maintenance_excess"].toFloat();
            row.is_real_time    = map["is_real_time"]   .toBool();
            row.updated_date    = map["updated_date"]   .toDateTime();
        }
    }
    return row;
}


Accounts AccountModel::fetchAll(QSqlQuery query) {

    Accounts rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            Account row;
            row.rowid           = map["rowid"]          .toInt();
            row.user            = map["user"]           .toString();
            row.status          = map["status"]         .toString();
            row.type            = map["type"]           .toString();
            row.allow_ai        = map["allow_ai"]       .toString();
            row.name            = map["name"]           .toString();
            row.account         = map["account"]        .toString();
            row.currency        = map["currency"]       .toString();
            row.is_data         = map["is_data"]        .toBool();
            row.requires_key    = map["requires_key"]   .toBool();
            row.refresh_token   = map["refresh_token"]  .toString();
            row.refresh_expires = map["refresh_expires"].toDateTime();
            row.access_token    = map["access_token"]   .toString();
            row.access_type     = map["access_type"]    .toString();
            row.access_expires  = map["access_expires"] .toDateTime();
            row.api_server      = map["api_server"]     .toString();
            row.risk_unit       = map["risk_unit"]      .toFloat();
            row.risk_cap        = map["risk_cap"]       .toFloat();
            row.allow_stock     = map["allow_stock"]    .toBool();
            row.allow_option    = map["allow_option"]   .toBool();
            row.prefer_trade    = map["prefer_trade"]   .toString();
            row.created_by      = map["created_by"]     .toString();
            row.created_date    = map["created_date"]   .toDateTime();
            row.modified_by     = map["modified_by"]    .toString();
            row.modified_date   = map["modified_date"]  .toDateTime();
            row.cash            = map["cash"]           .toFloat();
            row.market_value    = map["market_value"]   .toFloat();
            row.total_equity    = map["total_equity"]   .toFloat();
            row.buying_power    = map["buying_power"]   .toFloat();
            row.maintenance_excess = map["maintenance_excess"].toFloat();
            row.is_real_time    = map["is_real_time"]   .toBool();
            row.updated_date    = map["updated_date"]   .toDateTime();
            rows.append(row);
        }
    }
    return rows;
}


/**
 * Select one active data account
 */
Account AccountModel::select_data_account() {

    QString sql = "SELECT *"
                  " FROM account"
                  " WHERE is_data=1"
                  " LIMIT 1;";

    Account row;
    QSqlQuery query(Database::database());
    query.prepare(sql);
    row = fetch(query);
    return row;
}


/**
 * Select one rowid
 */
Account AccountModel::select_row(int rowid) {

    QString sql = "SELECT *"
                  " FROM account"
                  " WHERE rowid=:rowid"
                  " LIMIT 1;";

    Account row;
    QSqlQuery query(Database::database());
    query.prepare(sql);
    query.bindValue(":rowid", rowid);
    row = fetch(query);
    return row;
}


/**
 * Update
 */
bool AccountModel::save(Account data) {

    QString sql = "UPDATE account SET"
                  " refresh_token=:refresh_token,"
                  " refresh_expires=:refresh_expires,"
                  " access_token=:access_token,"
                  " access_type=:access_type,"
                  " access_expires=:access_expires,"
                  " api_server=:api_server,"
                  " modified_by='ampanov-server',"
                  " modified_date=NOW()"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":refresh_token", data.refresh_token);
    q.bindValue(":refresh_expires", data.refresh_expires);
    q.bindValue(":access_token", data.access_token);
    q.bindValue(":access_type", data.access_type);
    q.bindValue(":access_expires", data.access_expires);
    q.bindValue(":api_server", data.api_server);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Update authorization data
 */
bool AccountModel::save_auth(Account data) {

    QString sql = "UPDATE account SET"
                  " refresh_token=:refresh_token,"
                  " refresh_expires=:refresh_expires,"
                  " access_token=:access_token,"
                  " access_type=:access_type,"
                  " access_expires=:access_expires,"
                  " api_server=:api_server,"
                  " modified_by='ampanov-server',"
                  " modified_date=NOW()"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":refresh_token", data.refresh_token);
    q.bindValue(":refresh_expires", data.refresh_expires);
    q.bindValue(":access_token", data.access_token);
    q.bindValue(":access_type", data.access_type);
    q.bindValue(":access_expires", data.access_expires);
    q.bindValue(":api_server", data.api_server);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Update balance
 */
bool AccountModel::save_balance(Account data) {

    QString sql = "UPDATE account SET"
                  " cash=:cash,"
                  " market_value=:market_value,"
                  " total_equity=:total_equity,"
                  " buying_power=:buying_power,"
                  " maintenance_excess=:maintenance_excess,"
                  " is_real_time=:is_real_time,"
                  " updated_date=NOW()"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":cash", data.cash);
    q.bindValue(":market_value", data.market_value);
    q.bindValue(":total_equity", data.total_equity);
    q.bindValue(":buying_power", data.buying_power);
    q.bindValue(":maintenance_excess", data.maintenance_excess);
    q.bindValue(":is_real_time", data.is_real_time);
    bool result = q.exec();
    q.finish();
    return result;
}
