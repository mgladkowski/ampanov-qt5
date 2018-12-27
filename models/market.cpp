#include "market.h"


Market MarketModel::fetch(QSqlQuery & query) {

    Market row;
    row.rowid = 0;
    if (query.exec()) {
        if (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            row.rowid               = map["rowid"].toInt();
            row.name                = map["name"].toString();
            row.extendedStartTime   = map["extendedStartTime"].toDateTime();
            row.startTime           = map["startTime"].toDateTime();
            row.endTime             = map["endTime"].toDateTime();
            row.extendedEndTime     = map["extendedEndTime"].toDateTime();
            row.defaultTradingVenue = map["defaultTradingVenue"].toString();
        }
    }
    return row;
}


Markets MarketModel::fetchAll(QSqlQuery & query) {

    Markets rows;
    rows.clear();
    if (query.exec()) {
        while (query.next()) {
            QSqlRecord record = query.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), query.value(i));
            }
            Market row;
            row.rowid               = map["rowid"].toInt();
            row.name                = map["name"].toString();
            row.extendedStartTime   = map["extendedStartTime"].toDateTime();
            row.startTime           = map["startTime"].toDateTime();
            row.endTime             = map["endTime"].toDateTime();
            row.extendedEndTime     = map["extendedEndTime"].toDateTime();
            row.defaultTradingVenue = map["defaultTradingVenue"].toString();
            rows.append(row);
        }
    }
    return rows;
}


/**
 * Select one market by name
 */
Market MarketModel::select_one(QString name) {

    Market row;
    QString sql = "SELECT * FROM market WHERE name=:name LIMIT 1;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":name", name);
    row = fetch(q);
    return row;
}


/**
 * Insert market
 */
bool MarketModel::insert(Market data) {

    QString sql = "INSERT INTO market ("
                  " name,"
                  " extendedStartTime,"
                  " startTime,"
                  " endTime,"
                  " extendedEndTime,"
                  " defaultTradingVenue,"
                  " last_update,"
                  " last_update_by"
                  " ) VALUES ( "
                  " :name,"
                  " :extendedStartTime,"
                  " :startTime,"
                  " :endTime,"
                  " :extendedEndTime,"
                  " :defaultTradingVenue,"
                  " NOW(),"
                  " 'ampanov-server'"
                  " );";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":name", data.name);
    q.bindValue(":extendedStartTime", data.extendedStartTime);
    q.bindValue(":startTime", data.startTime);
    q.bindValue(":endTime", data.endTime);
    q.bindValue(":extendedEndTime", data.extendedEndTime);
    q.bindValue(":defaultTradingVenue", data.defaultTradingVenue);
    bool result = q.exec();
    q.finish();
    return result;
}


/**
 * Update market
 */
bool MarketModel::save(Market data) {

    QString sql = "UPDATE market SET "
                  " name=:name,"
                  " extendedStartTime=:extendedStartTime,"
                  " startTime=:startTime,"
                  " endTime=:endTime,"
                  " extendedEndTime=:extendedEndTime,"
                  " defaultTradingVenue=:defaultTradingVenue,"
                  " last_update=NOW(),"
                  " last_update_by='ampanov-server'"
                  " WHERE rowid=:rowid;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":rowid", data.rowid);
    q.bindValue(":name", data.name);
    q.bindValue(":extendedStartTime", data.extendedStartTime);
    q.bindValue(":startTime", data.startTime);
    q.bindValue(":endTime", data.endTime);
    q.bindValue(":extendedEndTime", data.extendedEndTime);
    q.bindValue(":defaultTradingVenue", data.defaultTradingVenue);
    bool result = q.exec();
    q.finish();
    return result;
}
