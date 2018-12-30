#include "chart.h"


Chart ChartModel::fetch(QSqlQuery & query) {

    Chart row;
    row.rowid = 0;
    if (query.exec()) {
        if (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            row.rowid               = map["rowid"].toInt();
            row.symbol_id           = map["symbol_id"].toInt();
            row.symbol_interval     = map["symbol_interval"].toString();
            row.candle_id           = map["candle_id"].toInt();
            row.event               = map["event"].toString();
        }
    }
    return row;
}


Charts ChartModel::fetchAll(QSqlQuery & query) {

    Charts rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            Chart row;
            row.rowid               = map["rowid"].toInt();
            row.symbol_id           = map["symbol_id"].toInt();
            row.symbol_interval     = map["symbol_interval"].toString();
            row.candle_id           = map["candle_id"].toInt();
            row.event               = map["event"].toString();
            rows.append(row);
        }
    }
    return rows;
}


/**
 * Select one chart datum
 */
Chart ChartModel::select_one( int symbol_id, QString interval, int candle_id ) {

    Chart row;
    QString sql = "SELECT * FROM chart"
                  " WHERE symbol_id=:symbol_id"
                  " AND symbol_interval=:symbol_interval"
                  " AND candle_id=:candle_id"
                  " LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    q.bindValue(":symbol_interval", interval);
    q.bindValue(":candle_id", candle_id);
    row = fetch(q);
    return row;
}


/**
 * Insert chart
 */
bool ChartModel::insert(Chart data) {

    QString sql = "INSERT INTO chart ("
                  " symbol_id,"
                  " symbol_interval,"
                  " candle_id,"
                  " event"
                  " ) VALUES ( "
                  " :symbol_id,"
                  " :symbol_interval,"
                  " :candle_id,"
                  " :event"
                  " );";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":symbol_interval", data.symbol_interval);
    q.bindValue(":candle_id", data.candle_id);
    q.bindValue(":event", data.event);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Update chart
 */
bool ChartModel::save(Chart data) {

    QString sql = "UPDATE chart SET "
                  " symbol_id=:symbol_id,"
                  " symbol_interval=:symbol_interval,"
                  " candle_id=:candle_id,"
                  " event=:event"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":symbol_interval", data.symbol_interval);
    q.bindValue(":candle_id", data.candle_id);
    q.bindValue(":event", data.event);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Clear chart table
 */
bool ChartModel::truncate() {

    QString sql = "TRUNCATE chart;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    bool result = q.exec();
    q.finish();
    return result;
}
