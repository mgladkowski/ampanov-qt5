#include "engine.h"

EngineModel::EngineModel() {}


bool EngineModel::exists_locus() {

    bool result = false;

    QString sql = "SELECT rowid, locus_date FROM engine WHERE locus_date=:locus_date;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            int   rowid      = map["rowid"].toInt();
            QDate locus_date = map["locus_date"].toDate();
            result = ((rowid > 0) && (locus_date.isValid()));
            result = ((locus_date.daysTo(QDate::currentDate()) == 0) && result);
        }
    }
    q.finish();
    return result;
}

bool EngineModel::insert_locus() {

    if (EngineModel::exists_locus() == false) {

        QString sql = "INSERT INTO engine (locus_date) VALUES (:locus_date);";
        QSqlQuery q(Database::database());
        q.prepare(sql);
        q.bindValue(":locus_date", QDate::currentDate());
        bool result = q.exec();
        q.finish();
        result = EngineModel::exists_locus();
        return result;

    } else {

        return true;
    }
}


bool EngineModel::is_enabled() {

    bool result = false;
    QString sql = "SELECT"
                  " is_enabled"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            result = map["is_enabled"].toBool();
        }
    }
    q.finish();
    return result;
}

bool EngineModel::is_enabled(bool value) {

    QString sql = "UPDATE engine SET"
                  " enabled=:value"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}

Task EngineModel::onstart_markets_update() {

    Task data;
    QString sql = "SELECT"
                  " onstart_markets_update,"
                  " onstart_markets_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onstart_markets_update"].toInt();
            data.time   = map["onstart_markets_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onstart_markets_update(int value) {

    QString sql = "UPDATE engine SET"
                  " onstart_markets_update=:value,"
                  " onstart_markets_update_time=CURTIME()"
                  " WHERE locus_date=:now;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::onstart_symbols_update() {

    Task data;
    QString sql = "SELECT"
                  " onstart_symbols_update,"
                  " onstart_symbols_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onstart_symbols_update"].toInt();
            data.time   = map["onstart_symbols_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onstart_symbols_update(int value) {

    QString sql = "UPDATE engine SET"
                  " onstart_symbols_update=:value,"
                  " onstart_symbols_update_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::onstart_symbol_update() {

    Task data;
    QString sql = "SELECT"
                  " onstart_symbol_update,"
                  " onstart_symbol_update_time,"
                  " onstart_symbol_update_data"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onstart_symbol_update"].toInt();
            data.time   = map["onstart_symbol_update_time"].toTime();
            data.data   = map["onstart_symbol_update_data"].toString();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onstart_symbol_update(int value, QString data) {

    QString sql = "UPDATE engine SET"
                  " onstart_symbol_update=:value,"
                  " onstart_symbol_update_time=CURTIME(),"
                  " onstart_symbol_update_data=:data"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":data", data);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::onstart_symbols_pattern() {

    Task data;
    QString sql = "SELECT"
                  " onstart_symbols_pattern,"
                  " onstart_symbols_pattern_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onstart_symbols_pattern"].toInt();
            data.time   = map["onstart_symbols_pattern_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onstart_symbols_pattern(int value) {

    QString sql = "UPDATE engine SET"
                  " onstart_symbols_pattern=:value,"
                  " onstart_symbols_pattern_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::onstart_account_update() {

    Task data;
    QString sql = "SELECT"
                  " onstart_account_update,"
                  " onstart_account_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onstart_account_update"].toInt();
            data.time   = map["onstart_account_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onstart_account_update(int value) {

    QString sql = "UPDATE engine SET"
                  " onstart_account_update=:value,"
                  " onstart_account_update_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::onopen_options_update() {

    Task data;
    QString sql = "SELECT"
                  " onopen_options_update,"
                  " onopen_options_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onopen_options_update"].toInt();
            data.time   = map["onopen_options_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onopen_options_update(int value) {

    QString sql = "UPDATE engine SET"
                  " onopen_options_update=:value,"
                  " onopen_options_update_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::intraday_minute_update() {

    Task data;
    QString sql = "SELECT"
                  " intraday_minute_update,"
                  " intraday_minute_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["intraday_minute_update"].toInt();
            data.time   = map["intraday_minute_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::intraday_minute_update(int value) {

    QString sql = "UPDATE engine SET"
                  " intraday_minute_update=:value,"
                  " intraday_minute_update_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::intraday_minute_pattern() {

    Task data;
    QString sql = "SELECT"
                  " intraday_minute_pattern,"
                  " intraday_minute_pattern_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["intraday_minute_pattern"].toInt();
            data.time   = map["intraday_minute_pattern_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::intraday_minute_pattern(int value) {

    QString sql = "UPDATE engine SET"
                  " intraday_minute_pattern=:value,"
                  " intraday_minute_pattern_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::intraday_thirty_update() {

    Task data;
    QString sql = "SELECT"
                  " intraday_thirty_update,"
                  " intraday_thirty_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["intraday_thirty_update"].toInt();
            data.time   = map["intraday_thirty_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::intraday_thirty_update(int value) {

    QString sql = "UPDATE engine SET"
                  " intraday_thirty_update=:value,"
                  " intraday_thirty_update_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::intraday_thirty_pattern() {

    Task data;
    QString sql = "SELECT"
                  " intraday_thirty_pattern,"
                  " intraday_thirty_pattern_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["intraday_thirty_pattern"].toInt();
            data.time   = map["intraday_thirty_pattern_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::intraday_thirty_pattern(int value) {

    QString sql = "UPDATE engine SET"
                  " intraday_thirty_pattern=:value,"
                  " intraday_thirty_pattern_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::intraday_hour_update() {

    Task data;
    QString sql = "SELECT"
                  " intraday_hour_update,"
                  " intraday_hour_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["intraday_hour_update"].toInt();
            data.time   = map["intraday_hour_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::intraday_hour_update(int value) {

    QString sql = "UPDATE engine SET"
                  " intraday_hour_update=:value,"
                  " intraday_hour_update_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::intraday_hour_pattern() {

    Task data;
    QString sql = "SELECT"
                  " intraday_hour_pattern,"
                  " intraday_hour_pattern_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["intraday_hour_pattern"].toInt();
            data.time   = map["intraday_hour_pattern_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::intraday_hour_pattern(int value) {

    QString sql = "UPDATE engine SET"
                  " intraday_hour_pattern=:value,"
                  " intraday_hour_pattern_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::intraday_account_update() {

    Task data;
    QString sql = "SELECT"
                  " intraday_account_update,"
                  " intraday_account_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["intraday_account_update"].toInt();
            data.time   = map["intraday_account_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::intraday_account_update(int value) {

    QString sql = "UPDATE engine SET"
                  " intraday_account_update=:value,"
                  " intraday_account_update_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::intraday_trades_open() {

    Task data;
    QString sql = "SELECT"
                  " intraday_trades_open,"
                  " intraday_trades_open_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["intraday_trades_open"].toInt();
            data.time   = map["intraday_trades_open_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::intraday_trades_open(int value) {

    QString sql = "UPDATE engine SET"
                  " intraday_trades_open=:value,"
                  " intraday_trades_open_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::intraday_trades_setup() {

    Task data;
    QString sql = "SELECT"
                  " intraday_trades_setup,"
                  " intraday_trades_setup_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["intraday_trades_setup"].toInt();
            data.time   = map["intraday_trades_setup_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::intraday_trades_setup(int value) {

    QString sql = "UPDATE engine SET"
                  " intraday_trades_setup=:value,"
                  " intraday_trades_setup_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::intraday_trades_enter() {

    Task data;
    QString sql = "SELECT"
                  " intraday_trades_enter,"
                  " intraday_trades_enter_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["intraday_trades_enter"].toInt();
            data.time   = map["intraday_trades_enter_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::intraday_trades_enter(int value) {

    QString sql = "UPDATE engine SET"
                  " intraday_trades_enter=:value,"
                  " intraday_trades_enter_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::onclose_options_update() {

    Task data;
    QString sql = "SELECT"
                  " onclose_options_update,"
                  " onclose_options_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onclose_options_update"].toInt();
            data.time   = map["onclose_options_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onclose_options_update(int value) {

    QString sql = "UPDATE engine SET"
                  " onclose_options_update=:value,"
                  " onclose_options_update_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::onclose_symbols_update() {

    Task data;
    QString sql = "SELECT"
                  " onclose_symbols_update,"
                  " onclose_symbols_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onclose_symbols_update"].toInt();
            data.time   = map["onclose_symbols_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onclose_symbols_update(int value) {

    QString sql = "UPDATE engine SET"
                  " onclose_symbols_update=:value,"
                  " onclose_symbols_update_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::onclose_symbol_update() {

    Task data;
    QString sql = "SELECT"
                  " onclose_symbol_update,"
                  " onclose_symbol_update_time,"
                  " onclose_symbol_update_data"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onclose_symbol_update"].toInt();
            data.time   = map["onclose_symbol_update_time"].toTime();
            data.data   = map["onclose_symbol_update_data"].toString();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onclose_symbol_update(int value, QString data) {

    QString sql = "UPDATE engine SET"
                  " onclose_symbol_update=:value,"
                  " onclose_symbol_update_time=CURTIME(),"
                  " onclose_symbol_update_data=:data"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":data", data);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::onclose_symbols_pattern() {

    Task data;
    QString sql = "SELECT"
                  " onclose_symbols_pattern,"
                  " onclose_symbols_pattern_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onclose_symbols_pattern"].toInt();
            data.time   = map["onclose_symbols_pattern_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onclose_symbols_pattern(int value) {

    QString sql = "UPDATE engine SET"
                  " onclose_symbols_pattern=:value,"
                  " onclose_symbols_pattern_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}


Task EngineModel::onclose_account_update() {

    Task data;
    QString sql = "SELECT"
                  " onclose_account_update,"
                  " onclose_account_update_time"
                  " FROM engine"
                  " WHERE locus_date=:locus_date;";

    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":locus_date", QDate::currentDate());
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            data.state  = map["onclose_account_update"].toInt();
            data.time   = map["onclose_account_update_time"].toTime();
        }
    }
    q.finish();
    return data;
}

bool EngineModel::onclose_account_update(int value) {

    QString sql = "UPDATE engine SET"
                  " onclose_account_update=:value,"
                  " onclose_account_update_time=CURTIME()"
                  " WHERE locus_date=:now;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    q.bindValue(":value", value);
    q.bindValue(":now", QDate::currentDate());
    bool result = q.exec();
    q.finish();
    return result;
}
