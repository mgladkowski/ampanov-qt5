#include "scan.h"


Scan ScanModel::fetch(QSqlQuery & query) {

    Scan row;
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
            row.status              = map["status"].toString();
            row.created_date        = map["created_date"].toDateTime();
            row.created_by          = map["created_by"].toString();
            row.candle_row_id       = map["candle_row_id"].toInt();
            row.candle_pattern_id   = map["candle_pattern_id"].toInt();
            row.candle_eval_code    = map["candle_eval_code"].toInt();
            row.candle_eval_text    = map["candle_eval_text"].toString();
            row.chart_pattern_id    = map["chart_pattern_id"].toInt();
            row.chart_eval_code     = map["chart_eval_code"].toInt();
            row.chart_eval_text     = map["chart_eval_text"].toString();
        }
    }
    return row;
}


Scans ScanModel::fetchAll(QSqlQuery query) {

    Scans rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            Scan row;
            row.rowid               = map["rowid"].toInt();
            row.symbol_id           = map["symbol_id"].toInt();
            row.symbol_interval     = map["symbol_interval"].toString();
            row.status              = map["status"].toString();
            row.created_date        = map["created_date"].toDateTime();
            row.created_by          = map["created_by"].toString();
            row.candle_row_id       = map["candle_row_id"].toInt();
            row.candle_pattern_id   = map["candle_pattern_id"].toInt();
            row.candle_eval_code    = map["candle_eval_code"].toInt();
            row.candle_eval_text    = map["candle_eval_text"].toString();
            row.chart_pattern_id    = map["chart_pattern_id"].toInt();
            row.chart_eval_code     = map["chart_eval_code"].toInt();
            row.chart_eval_text     = map["chart_eval_text"].toString();
            rows.append(row);
        }
    }
    return rows;
}


Scan ScanModel::select_active(int symbol_id, QString interval, int candle_id) {

    Scan row;
    QString sql = "SELECT * FROM scan"
                  " WHERE symbol_id=:symbol_id"
                  " AND symbol_interval=:symbol_interval"
                  " AND candle_row_id=:candle_row_id"
                  " AND status='A'"
                  " LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", symbol_id);
    q.bindValue(":symbol_interval", interval);
    q.bindValue(":candle_row_id", candle_id);
    row = fetch(q);
    return row;
}


bool ScanModel::insert(Scan data, bool is_live) {

    QString table;
    if (is_live == true) table = "scan";
    if (is_live == false) table = "scan_replay";

    QString sql = "INSERT INTO ";
            sql.append(table);
            sql.append("("
                  " symbol_id,"
                  " symbol_interval,"
                  " status,"
                  " created_date,"
                  " created_by,"
                  " candle_row_id,"
                  " candle_pattern_id"
                  ") VALUES ("
                  " :symbol_id,"
                  " :symbol_interval,"
                  " :status,"
                  " :created_date,"
                  " :created_by,"
                  " :candle_row_id,"
                  " :candle_pattern_id"
                  ");");

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":symbol_id", data.symbol_id);
    q.bindValue(":symbol_interval", data.symbol_interval);
    q.bindValue(":status", data.status);
    q.bindValue(":created_date", QDateTime::currentDateTime());
    q.bindValue(":created_by", QString("ampanov-server"));
    q.bindValue(":candle_row_id", data.candle_row_id);
    q.bindValue(":candle_pattern_id", data.candle_pattern_id);
    bool result = q.exec();
    q.finish();
    return result;
}


bool ScanModel::update(Scan data, bool is_live) {

    QString table;
    if (is_live == true) table = "scan";
    if (is_live == false) table = "scan_replay";

    QString sql = "UPDATE ";
            sql.append(table);
            sql.append(" SET"
                  " status=:status,"
                  " candle_eval_code=:candle_eval_code,"
                  " candle_eval_text=:candle_eval_text,"
                  " chart_pattern_id=:chart_pattern_id,"
                  " chart_eval_code=:chart_eval_code,"
                  " chart_eval_text=:chart_eval_text"
                  " WHERE rowid=:rowid;");

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":status", data.status);
    q.bindValue(":candle_eval_code", data.candle_eval_code);
    q.bindValue(":candle_eval_text", data.candle_eval_text);
    q.bindValue(":chart_pattern_id", data.chart_pattern_id);
    q.bindValue(":chart_eval_code", data.chart_eval_code);
    q.bindValue(":chart_eval_text", data.chart_eval_text);
    bool result = q.exec();
    q.finish();
    return result;
}


bool ScanModel::save(Scan data, bool is_live) {

    bool result = false;
    if (data.rowid > 0) {
        result = ScanModel::update(data, is_live);
    } else {
        result = ScanModel::insert(data, is_live);
    }
    return result;
}
