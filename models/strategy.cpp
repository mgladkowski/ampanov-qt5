#include "strategy.h"


Strategy StrategyModel::fetch(QSqlQuery & query) {

    Strategy row;
    if (query.exec()) {
        if (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            row.rowid               = map["rowid"].toInt();
            row.status              = map["status"].toString();
            row.name                = map["name"].toString();
            row.direction           = map["direction"].toString();
        }
    }
    return row;
}


Strategies StrategyModel::fetchAll(QSqlQuery query) {

    Strategies rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            Strategy row;
            row.rowid               = map["rowid"].toInt();
            row.status              = map["status"].toString();
            row.name                = map["name"].toString();
            row.direction           = map["direction"].toString();
            rows.append(row);
        }
    }
    return rows;
}


/**
 * Select all active strategies
 */
Strategies StrategyModel::select_all_active() {

    QString sql = "SELECT *"
                  " FROM strategy"
                  " WHERE status='A';";

    Strategies rows;
    QSqlQuery q(Database::database());
    q.prepare(sql);
    rows = fetchAll(q);
    return rows;
}


/**
 * Select one by rowid
 */
Strategy StrategyModel::select_one_row(int rowid) {

    QString sql = "SELECT *"
                  " FROM strategy"
                  " WHERE rowid=:rowid"
                  " LIMIT 1;";

    Strategy row;
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", rowid);
    row = fetch(q);
    return row;
}
