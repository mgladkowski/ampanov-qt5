#include "control.h"


bool Control::locusValid = false;
QDate Control::locusCreated = QDate();


/**
 *  Ampanov::start event
 */
void Control::start() {

    QJsonObject data;
    data["type"] = "service";
    data["data"] = "start";
    data["time"] = QTime::currentTime().toString();

    ZeroMq::push(data);
}


/**
 *  Ampanov::pause event
 */
void Control::pause() {

    QJsonObject data;
    data["type"] = "service";
    data["data"] = "pause";
    data["time"] = QTime::currentTime().toString();

    ZeroMq::push(data);
}


/**
 *  Ampanov::resume event
 */
void Control::resume() {

    QJsonObject data;
    data["type"] = "service";
    data["data"] = "resume";
    data["time"] = QTime::currentTime().toString();

    ZeroMq::push(data);
}


/**
 *  Ampanov::stop event
 */
void Control::stop() {

    QJsonObject data;
    data["type"] = "service";
    data["data"] = "stop";
    data["time"] = QTime::currentTime().toString();

    ZeroMq::push(data);
}


/**
 *  Ampanov::think event
 */
void Control::heartbeat() {

    QJsonObject data;
    data["type"] = "h";
    data["time"] = QTime::currentTime().toString();

    ZeroMq::push(data);
}


/**
 * Ampanov::create_checklist event
 */
bool Control::valid_checklist() {

    if (Control::is_locus_valid() == true) {

        return true;

    } else {

        Control::create_checklist(TaskState::Started);

        Control::locusValid = EngineModel::insert_locus();

        if (Control::locusValid == true) {

            Control::locusCreated = QDate::currentDate();
            Control::create_checklist(TaskState::Success);

        } else {

            Control::create_checklist(TaskState::Fail);
            return false;
        }
    }

    return Control::is_locus_valid();
}

void Control::create_checklist(int state) {

    QTime now(QTime::currentTime());

    QJsonObject data;
    data["id"] = "create-checklist";
    data["state"] = state;
    data["time"] = now.toString();
    data["data"] = "";

    QJsonDocument doc(data);
    QByteArray bytes = doc.toJson();
    ZeroMq::push(bytes);

    qInfo() << "[checklist] " << qPrintable(TaskState::text(state));
}

bool Control::exists_checklist() {

    return EngineModel::exists_locus();
}


/**
 * Ampanov::onstart_markets_update event
 */
void Control::onstart_markets_update(int state) {

    bool result = EngineModel::onstart_markets_update(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onstart-markets-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onstart] markets update : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onstart_markets_update() {

    return EngineModel::onstart_markets_update();
}


/**
 * Ampanov::onstart_symbols_update event
 */
void Control::onstart_symbols_update(int state) {

    bool result = EngineModel::onstart_symbols_update(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onstart-symbols-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onstart] stocks update : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onstart_symbols_update() {

    return EngineModel::onstart_symbols_update();
}


/**
 * Ampanov::onstart_symbol_update event
 */
void Control::onstart_symbol_update(int state, QString symbol) {

    bool result = EngineModel::onstart_symbol_update(state, symbol);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onstart-symbol-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = symbol;

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onstart] stock data : " << qPrintable(symbol) << " : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onstart_symbol_update() {

    return EngineModel::onstart_symbol_update();
}


/**
 * Ampanov::onstart_symbols_pattern event
 */
void Control::onstart_symbols_pattern(int state) {

    bool result = EngineModel::onstart_symbols_pattern(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onstart-symbols-pattern";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onstart] stocks pattern : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onstart_symbols_pattern() {

    return EngineModel::onstart_symbols_pattern();
}


/**
 * Ampanov::onstart_account_update event
 */
void Control::onstart_account_update(int state) {

    bool result = EngineModel::onstart_account_update(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onstart-account-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onstart] accounts : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onstart_account_update() {

    return EngineModel::onstart_account_update();
}


/**
 * Ampanov::onopen_options_update event
 */
void Control::onopen_options_update(int state) {

    bool result = EngineModel::onopen_options_update(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onopen-options-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onopen] option quotes : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onopen_options_update() {

    return EngineModel::onopen_options_update();
}


/**
 * Ampanov::intraday_minute_update event
 */
void Control::intraday_minute_update(int state) {

    bool result = EngineModel::intraday_minute_update(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "intraday-minute-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[intraday] 1m update : " << qPrintable(TaskState::text(state));
    }
}

Task Control::intraday_minute_update() {

    return EngineModel::intraday_minute_update();
}


/**
 * Ampanov::intraday_minute_pattern event
 */
void Control::intraday_minute_pattern(int state) {

    bool result = EngineModel::intraday_minute_pattern(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "intraday-minute-pattern";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[intraday] 1m candle pattern : " << qPrintable(TaskState::text(state));
    }
}

Task Control::intraday_minute_pattern() {

    return EngineModel::intraday_minute_pattern();
}


/**
 * Ampanov::intraday_hour_update event
 */
void Control::intraday_hour_update(int state) {

    bool result = EngineModel::intraday_hour_update(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "intraday-hour-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[intraday] 1h update : " << qPrintable(TaskState::text(state));
    }
}

Task Control::intraday_hour_update() {

    return EngineModel::intraday_hour_update();
}


/**
 * Ampanov::intraday_hour_pattern event
 */
void Control::intraday_hour_pattern(int state) {

    bool result = EngineModel::intraday_hour_pattern(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "intraday-hour-pattern";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[intraday] 1h candle pattern : " << qPrintable(TaskState::text(state));
    }
}

Task Control::intraday_hour_pattern() {

    return EngineModel::intraday_hour_pattern();
}


/**
 * Ampanov::intraday_account_update event
 */
void Control::intraday_account_update(int state) {

    bool result = EngineModel::intraday_account_update(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "intraday-account-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[intraday] accounts : " << qPrintable(TaskState::text(state));
    }
}

Task Control::intraday_account_update() {

    return EngineModel::intraday_account_update();
}


/**
 * Ampanov::intraday_trades_open event
 */
void Control::intraday_trades_open(int state) {

    bool result = EngineModel::intraday_trades_open(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "intraday-trades-open";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[intraday] check open trades : " << qPrintable(TaskState::text(state));
    }
}

Task Control::intraday_trades_open() {

    return EngineModel::intraday_trades_open();
}


/**
 * Ampanov::intraday_trades_setup event
 */
void Control::intraday_trades_setup(int state) {

    bool result = EngineModel::intraday_trades_setup(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "intraday-trades-queue";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[intraday] check trade setups : " << qPrintable(TaskState::text(state));
    }
}

Task Control::intraday_trades_setup() {

    return EngineModel::intraday_trades_setup();
}


/**
 * Ampanov::intraday_trades_enter event
 */
void Control::intraday_trades_enter(int state) {

    bool result = EngineModel::intraday_trades_enter(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "intraday-trades-new";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[intraday] check for trade entry : " << qPrintable(TaskState::text(state));
    }
}

Task Control::intraday_trades_enter() {

    return EngineModel::intraday_trades_enter();
}


/**
 * Ampanov::onclose_options_update event
 */
void Control::onclose_options_update(int state) {

    bool result = EngineModel::onclose_options_update(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onclose-options-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onclose] option quotes : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onclose_options_update() {

    return EngineModel::onclose_options_update();
}


/**
 * Ampanov::onclose_symbols_update event
 */
void Control::onclose_symbols_update(int state) {

    bool result = EngineModel::onclose_symbols_update(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onclose-symbols-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onclose] stocks update : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onclose_symbols_update() {

    return EngineModel::onclose_symbols_update();
}


/**
 * Ampanov::onclose_symbol_update event
 */
void Control::onclose_symbol_update(int state, QString symbol) {

    bool result = EngineModel::onclose_symbol_update(state, symbol);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onclose-symbol-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = symbol;

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onclose] stock data : " << qPrintable(symbol) << " : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onclose_symbol_update() {

    return EngineModel::onclose_symbol_update();
}


/**
 * Ampanov::onclose_symbols_pattern event
 */
void Control::onclose_symbols_pattern(int state) {

    bool result = EngineModel::onclose_symbols_pattern(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onclose-symbols-pattern";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onclose] candle pattern : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onclose_symbols_pattern() {

    return EngineModel::onclose_symbols_pattern();
}


/**
 * Ampanov::onclose_account_update event
 */
void Control::onclose_account_update(int state) {

    bool result = EngineModel::onclose_account_update(state);
    if (result == true) {

        QTime now(QTime::currentTime());

        QJsonObject data;
        data["id"] = "onclose-account-update";
        data["state"] = state;
        data["time"] = now.toString();
        data["data"] = "";

        QJsonDocument doc(data);
        QByteArray bytes = doc.toJson();
        ZeroMq::push(bytes);

        qInfo() << "[onclose] accounts : " << qPrintable(TaskState::text(state));
    }
}

Task Control::onclose_account_update() {

    return EngineModel::onclose_account_update();
}

