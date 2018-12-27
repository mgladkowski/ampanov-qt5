#include "ampanov.h"


Ampanov::Ampanov(QObject *parent) : QObject(parent) {

    isRunning       = false;
    isPaused        = false;

    time_start      = QTime(0,0,0);
    time_open_ext   = QTime(7,0,0);
    time_open       = QTime(9,45,0);    // WARNING : DELAYED DATA IN USE
    time_close      = QTime(16,15,0);   // WARNING : DELAYED DATA IN USE
    time_close_ext  = QTime(18,0,0);
    time_stop       = QTime(23,59,59);

    questrade       = new QuestradeApi(this);
    scanner         = new Scanner();
    trader          = new Trader();

    loopTimer       = new QTimer(this);
    loopTimer->setSingleShot(false);
    loopTimer->setInterval(loopInterval);

    connect( loopTimer, SIGNAL(timeout()), this, SLOT(think()), Qt::QueuedConnection );

    qDebug() << "Ampanov constructed";
    qWarning() << "Using delayed market data";
}


Ampanov::~Ampanov() {

    delete questrade;
    questrade = nullptr;

    delete scanner;
    scanner = nullptr;

    delete trader;
    trader = nullptr;

    qDebug() << "Ampanov deconstructed";
}


/**
 * Command line : workspace
 */
void Ampanov::dev() {

    qInfo() << "Workspace";


    return;
}


/**
 *  Command line : recalculate indicators
 */
void Ampanov::recalc( QString symbol ) {

    Q_UNUSED(symbol);
//    Stocks stocks = StockModel::select_all_active();
//    Stock item;

//    foreach (item, stocks) {

//        if (symbol == "-A" || symbol == item.symbol) {

//            Analyst chart;

//            qInfo() << "Recalculating indicators for " << item.symbol << "OneDay";
//            chart.load(item.symbol_id, "OneDay");
//            chart.calculate_all();

//            qInfo() << "Recalculating indicators for " << item.symbol << "OneHour";
//            chart.load(item.symbol_id, "OneHour");
//            chart.calculate_all();

//            qInfo() << "Recalculating indicators for " << item.symbol << "HalfHour";
//            chart.load(item.symbol_id, "HalfHour");
//            chart.calculate_all();

//            QThread::msleep(3000);
//        }
//    }
    return;
}


/**
 * Command line : replay history
 */
void Ampanov::replay( QString symbol, int strategy_id ) {

    Q_UNUSED(symbol);
    Q_UNUSED(strategy_id);
//    isLive          = false;
//    isReplay        = true;
//    replayStrategy  = strategy_id;

//    Stocks stocks = StockModel::select_all_active();
//    Stock item;

//    foreach (item, stocks) {

//        if (symbol == "-A" || symbol == item.symbol) {

//            qInfo() << "Running backtest of " << item.symbol;

//            replaySymbol = item.symbol;

//            Candle candlef = CandleModel::select_first(item.symbol_id, "OneDay");

//            qDebug() << candlef.start;


//        }
//    }

    return;
}


/**
 * Systems check
 */
bool Ampanov::isHealthy() {

    bool pass = true;

    if (Database::test()) {
        qInfo() << "Database : ok";
    } else {
        qCritical() << "Database : fail";
        pass = false;
    }

    if (ZeroMq::test()) {
        qInfo() << "ZMQ : ok";
    } else {
        qCritical() << "ZMQ : fail";
        pass = false;
    }

    return pass;
}



/**
 * Service control
 */
bool Ampanov::start() {

    if (! isHealthy()) {
        qCritical() << "Health check failed";
        return false;
    }

    isRunning = true;
    loopTimer->start();

    Control::start();

    return true;
}


bool Ampanov::stop() {

    isRunning = false;
    loopTimer->stop();

    Control::stop();

    return true;
}


bool Ampanov::pause() {

    if (isRunning != true) return false;

    isPaused = true;
    loopTimer->stop();

    Control::pause();

    return true;
}


bool Ampanov::resume() {

    if (isRunning != true) return false;

    if (! isHealthy()) {
        qCritical() << "Health check failed";
        return false;
    }

    isPaused = false;
    loopTimer->start();

    Control::resume();

    return true;
}



/**
 * Game loop
 */
void Ampanov::think() {

    if (isRunning != true) return tx_done();
    if (isPaused == true) return tx_done();
    //if (Helpers::isWeekday() != true) return tx_done();

    bool    result = false;

    QTime   time_now;


    // HEARTBEAT

    Control::heartbeat();


    // CHECKLIST

    result = Control::valid_checklist();
    if (result != true) return tx_done();


    // IS AWAKE

    time_now = QTime::currentTime();

    if (time_now <= time_start || time_now >= time_stop) return tx_done();


    // STARTUP

    if (Control::onstart_markets_update().state != TaskState::Success) {

        if ( onstart_markets_update() != true ) return tx_done();
    }

    if (Control::onstart_account_update().state != TaskState::Success) {

        if ( onstart_account_update() != true ) return tx_done();
    }

    if (Control::onstart_symbols_update().state != TaskState::Success) {

        if ( onstart_symbols_update() != true ) return tx_done();
    }

    if (Control::onstart_symbols_pattern().state != TaskState::Success) {

        if ( onstart_symbols_pattern() != true ) return tx_done();
    }


    // OPEN

    time_now = QTime::currentTime();

    if (time_now <= time_open) return tx_done();

    if (time_now > time_open && time_now < time_close) {


        // ONCE ON OPEN

        if (Control::onopen_options_update().state != TaskState::Success) {

            if ( onopen_options_update() != true ) return tx_done();
        }


        // INTRADAY

        QTime now;
        Task last;

        // check hour

        now = QTime::currentTime();
        last = Control::intraday_hour_update();

        if ( now.hour() > 9 &&
             last.time.hour() < now.hour() ) {

            // update candles

            intraday_hour_update();

            // check for candle patterns

            intraday_hour_pattern();
        }

        // check thirty minutes

        now = QTime::currentTime();
        last = Control::intraday_thirty_update();

        if ( now > QTime(10,0,0) &&
             QTime(last.time.hour(), 0, 0)
                < QTime(now.hour(), 0, 0) ) {

            // update candles

            intraday_thirty_update();

            // check for candle patterns

            intraday_thirty_pattern();
        }

        // check minute

        now = QTime::currentTime();
        last = Control::intraday_minute_update();

        if ( now > QTime(9,30,0) &&
             QTime(last.time.hour(), last.time.minute(), 0)
                < QTime(now.hour(), now.minute(), 0) ) {

            // 3s delay seems to ensure questrade has current minute candle ready
            QThread::msleep(3000);

            // update candles

            intraday_minute_update();

            // check for candle patterns

            intraday_minute_pattern();

            // update account balances

            intraday_account_update();

            // check on open trades

            intraday_trades_open();

            // check for new setups to create

            intraday_trades_setup();

            // check for triggered entries

            intraday_trades_enter();

        }

    }


    // 5M BEFORE CLOSE

    time_now = QTime::currentTime();

    if (time_now.addSecs(300) > time_close && time_now < time_close) {

        if (Control::onclose_options_update().state != TaskState::Success) {

            if ( onclose_options_update() != true ) return tx_done();
        }
    }


    // CLOSE

    time_now = QTime::currentTime();

    if (time_now <= time_close) return tx_done();

    if (Control::onclose_symbols_update().state != TaskState::Success) {

        if ( onclose_symbols_update() != true ) return tx_done();
    }

    if (Control::onclose_symbols_pattern().state != TaskState::Success) {

        if ( onclose_symbols_pattern() != true ) return tx_done();
    }

    if (Control::onclose_account_update().state != TaskState::Success) {

        if ( onclose_account_update() != true ) return tx_done();
    }

    return tx_done();
}


void Ampanov::tx_done() {

    qApp->processEvents(QEventLoop::AllEvents);
}


void Ampanov::tx_fail() {

    qApp->processEvents(QEventLoop::AllEvents);
}


/**
 * TASKS
 */

bool Ampanov::onstart_markets_update() {

    Control::onstart_markets_update( TaskState::Started );

    QDateTime now(QDateTime::currentDateTime());

    // get market exchange hours for today

    bool result = questrade->update_markets();

    if (result == true) {

        // check market hours for today

        Market m = MarketModel::select_one("NYSE");

        if (m.rowid > 0 && now.date().daysTo(m.startTime.date()) == 0) {

            time_open_ext   = m.extendedStartTime.time();
            time_open       = m.startTime.time();
            time_close      = m.endTime.time();
            time_close_ext  = m.extendedEndTime.time();

        } else {

            result = false;
        }
    }

    Control::onstart_markets_update( result ? TaskState::Success : TaskState::Fail );

    return result;
}


bool Ampanov::onstart_symbols_update() {

    bool result = false;

    Control::onstart_symbols_update(TaskState::Started);

    // update thru yesterday

    QDateTime from;
    QDateTime to(QDateTime::currentDateTime().addDays(-1));
    to.setTime(QTime(23,59,59));

    Stocks stocks = StockModel::select_all_active();
    Stock item;
    Candle last;

    foreach (item, stocks) {

        Control::onstart_symbol_update(TaskState::Started, item.symbol);

        // symbol data

        result = questrade->update_symbol(item.symbol_id);
        if (result != true) break;

        // daily candles

        last = CandleModel::select_last(item.symbol_id, "OneDay");
        from = last.start;
        if (from > to) from = to;
        from.setTime(QTime(0,0,0));

        result = questrade->update_candles_day(item.symbol_id, from, to);
        if (result != true) break;

        QDateTime ifrom = last.start;
        QDateTime ito = ifrom;
        int n = last.start.daysTo(to);
        int i;
        for (i=0; i <= n; i++) {

            ifrom = last.start.addDays(i);
            ito = ifrom;
            ito.setTime(QTime(16,0,0));

            // hour candles

            ifrom.setTime(QTime(9,0,0));
            result = questrade->update_candles_hour(item.symbol_id, ifrom, ito);
            if (result != true) break;

            // half hour candles

            ifrom.setTime(QTime(9,30,0));
            result = questrade->update_candles_thirty(item.symbol_id, ifrom, ito);
            if (result != true) break;

            // minute candles

            ifrom.setTime(QTime(9,30,0));
            result = questrade->update_candles_minute(item.symbol_id, ifrom, ito);
            if (result != true) break;
        }

        // option chain

        if (false && item.has_options == true) {

            result = questrade->update_option_chain(item.symbol_id);
            if (result != true) break;
        }

        if (result == true) {
            Control::onstart_symbol_update(TaskState::Success, item.symbol);
        } else {
            Control::onstart_symbol_update(TaskState::Fail, item.symbol);
            break;
        }
    }

    Control::onstart_symbols_update( result ? TaskState::Success : TaskState::Fail );

    return result;
}


bool Ampanov::onstart_symbols_pattern() {

    bool result = false;

    Control::onstart_symbols_pattern(TaskState::Started);

    Stocks stocks = StockModel::select_all_active();
    Stock item;

    foreach (item, stocks) {

        result = scanner->run_live(item.symbol_id, "OneDay");
    }

    if (result == true) {
        Control::onstart_symbols_pattern(TaskState::Success);
    } else {
        Control::onstart_symbols_pattern(TaskState::Fail);
    }

    return result;
}


bool Ampanov::onstart_account_update() {

    bool result = false;

    Control::onstart_account_update(TaskState::Started);

    Account account = AccountModel::select_data_account();

    result  = questrade->update_account_balances(account);
    result &= questrade->update_account_positions(account);
    result &= questrade->update_account_orders(account);

    if (result == true) {
        Control::onstart_account_update(TaskState::Success);
    } else {
        Control::onstart_account_update(TaskState::Fail);
    }

    return result;
}


bool Ampanov::onopen_options_update() {

    bool result = false;

    Control::onopen_options_update(TaskState::Started);

    Stocks stocks = StockModel::select_all_active();
    Stock item;

    result = true;

    foreach (item, stocks) {

        if (item.has_options == true) {

            result &= questrade->update_option(item.symbol_id);
        }
        QThread::sleep(1);
    }

    if (result == true) {
        Control::onopen_options_update(TaskState::Success);
    } else {
        Control::onopen_options_update(TaskState::Fail);
    }

    return result;
}


bool Ampanov::intraday_minute_update() {

    // minute candle update

    bool result = false;

    Control::intraday_minute_update(TaskState::Started);

    QDateTime now(QDateTime::currentDateTime());
    QDateTime from = now;
    from.setTime(QTime(0,0,0));

    Stocks stocks = StockModel::select_all_active();
    Stock item;
    Candle last;

    foreach (item, stocks) {

        last = CandleModel::select_last(item.symbol_id, "OneDay");
        result = questrade->update_candles_day(item.symbol_id, from, now);
        if (result != true) break;

        QDateTime ifrom = now;
        ifrom.setTime(QTime(9, 30, 0));
        last = CandleModel::select_last(item.symbol_id, "OneMinute");
        if (last.start > ifrom) ifrom = last.start;

        QDateTime ito = now;
        ito.setTime(QTime(now.time().hour(), now.time().minute(), 0));

        result = questrade->update_candles_minute(item.symbol_id, ifrom, ito);
        if (result != true) break;

    }
    if (result == true) {
        Control::intraday_minute_update(TaskState::Success);
    } else {
        Control::intraday_minute_update(TaskState::Fail);
    }
    return result;
}


bool Ampanov::intraday_minute_pattern() {

    bool result = false;

    Control::intraday_minute_pattern(TaskState::Started);

    Stocks stocks = StockModel::select_all_active();
    Stock item;

//    foreach (item, stocks) {

//        result = scanner->run_live(item.symbol_id, "OneMinute");
//    }
    result = true;

    if (result == true) {
        Control::intraday_minute_pattern(TaskState::Success);
    } else {
        Control::intraday_minute_pattern(TaskState::Fail);
    }

    return result;
}


bool Ampanov::intraday_thirty_update() {

    // thirty minute candle update

    bool result = false;

    Control::intraday_thirty_update(TaskState::Started);

    QDateTime now(QDateTime::currentDateTime());
    Candle last;

    Stocks stocks = StockModel::select_all_active();
    Stock item;

    foreach (item, stocks) {

        QDateTime ifrom = now;
        ifrom.setTime(QTime(9, 30, 0));
        last = CandleModel::select_last(item.symbol_id, "HalfHour");
        if (last.start > ifrom) ifrom = last.start;

        QDateTime ito = now;
        ito.setTime(QTime(now.time().hour(), 0, 0));

        result = questrade->update_candles_thirty(item.symbol_id, ifrom, ito);
        if (result != true) break;

    }
    if (result == true) {
        Control::intraday_thirty_update(TaskState::Success);
    } else {
        Control::intraday_thirty_update(TaskState::Fail);
    }
    return result;
}


bool Ampanov::intraday_thirty_pattern() {

    bool result = false;

    Control::intraday_thirty_pattern(TaskState::Started);

    Stocks stocks = StockModel::select_all_active();
    Stock item;

    foreach (item, stocks) {

        result = scanner->run_live(item.symbol_id, "HalfHour");
    }

    if (result == true) {
        Control::intraday_thirty_pattern(TaskState::Success);
    } else {
        Control::intraday_thirty_pattern(TaskState::Fail);
    }

    return result;
}


bool Ampanov::intraday_hour_update() {


    // hourly candle update

    bool result = false;

    Control::intraday_hour_update(TaskState::Started);

    QDateTime now(QDateTime::currentDateTime());
    Candle last;

    Stocks stocks = StockModel::select_all_active();
    Stock item;

    foreach (item, stocks) {

        QDateTime ifrom = now;
        ifrom.setTime(QTime(9, 0, 0));
        last = CandleModel::select_last(item.symbol_id, "OneHour");
        if (last.start > ifrom) ifrom = last.start;

        QDateTime ito = now;
        ito.setTime(QTime(now.time().hour(), 0, 0));

        result = questrade->update_candles_hour(item.symbol_id, ifrom, ito);
        if (result != true) break;

    }
    if (result == true) {
        Control::intraday_hour_update(TaskState::Success);
    } else {
        Control::intraday_hour_update(TaskState::Fail);
    }
    return result;
}


bool Ampanov::intraday_hour_pattern() {

    bool result = false;

    Control::intraday_hour_pattern(TaskState::Started);

    Stocks stocks = StockModel::select_all_active();
    Stock item;

    foreach (item, stocks) {

        result = scanner->run_live(item.symbol_id, "OneHour");
    }

    if (result == true) {
        Control::intraday_hour_pattern(TaskState::Success);
    } else {
        Control::intraday_hour_pattern(TaskState::Fail);
    }

    return result;
}


bool Ampanov::intraday_account_update() {

    bool result = false;

    Control::intraday_account_update(TaskState::Started);

    Account account = AccountModel::select_data_account();

    result  = questrade->update_account_balances(account);
    result &= questrade->update_account_positions(account);
    result &= questrade->update_account_orders(account);

    if (result == true) {
        Control::intraday_account_update(TaskState::Success);
    } else {
        Control::intraday_account_update(TaskState::Fail);
    }

    return result;
}


bool Ampanov::intraday_trades_open() {

    bool result = false;

    Control::intraday_trades_open(TaskState::Started);

    result = trader->process_open();

    if (result == true) {
        Control::intraday_trades_open(TaskState::Success);
    } else {
        Control::intraday_trades_open(TaskState::Fail);
    }

    return result;
}


bool Ampanov::intraday_trades_setup() {

    bool result = false;

    Control::intraday_trades_setup(TaskState::Started);

    result = trader->process_setup();

    if (result == true) {
        Control::intraday_trades_setup(TaskState::Success);
    } else {
        Control::intraday_trades_setup(TaskState::Fail);
    }

    return result;
}


bool Ampanov::intraday_trades_enter() {

    bool result = false;

    Control::intraday_trades_enter(TaskState::Started);

    result = trader->process_entry();

    if (result == true) {
        Control::intraday_trades_enter(TaskState::Success);
    } else {
        Control::intraday_trades_enter(TaskState::Fail);
    }

    return result;
}


bool Ampanov::onclose_options_update() {

    bool result = false;

    Control::onclose_options_update(TaskState::Started);

    Stocks stocks = StockModel::select_all_active();
    Stock item;

    result = true;

    foreach (item, stocks) {

        if (item.has_options == true) {

            result &= questrade->update_option(item.symbol_id);
        }
    }

    if (result == true) {
        Control::onclose_options_update(TaskState::Success);
    } else {
        Control::onclose_options_update(TaskState::Fail);
    }

    return result;
}


bool Ampanov::onclose_symbols_update() {

    bool result = false;

    Control::onclose_symbols_update(TaskState::Started);

    Stocks stocks = StockModel::select_all_active();
    Stock item;

    foreach (item, stocks) {

        Control::onclose_symbol_update(TaskState::Started, item.symbol);

        // symbol data

        result = questrade->update_symbol(item.symbol_id);

        QDateTime ifrom(QDateTime::currentDateTime());
        QDateTime ito(QDateTime::currentDateTime());

        // daily candles

        if (result == true) {
            ifrom.setTime(QTime(0,0,0));
            ito.setTime(QTime(23,59,59));
            result = questrade->update_candles_day(item.symbol_id, ifrom, ito);
        }

        // hour candles

        if (result == true) {
            ifrom.setTime(QTime(9,0,0));
            ito.setTime(QTime(16,0,0));
            result = questrade->update_candles_hour(item.symbol_id, ifrom, ito);
        }

        // half hour candles

        if (result == true) {
            ifrom.setTime(QTime(9,30,0));
            ito.setTime(QTime(16,0,0));
            result = questrade->update_candles_thirty(item.symbol_id, ifrom, ito);
        }

        // minute candles

        if (result == true) {
            ifrom.setTime(QTime(9,30,0));
            ito.setTime(QTime(16,0,0));
            result = questrade->update_candles_minute(item.symbol_id, ifrom, ito);
        }

        if (result == true) {
            Control::onclose_symbol_update(TaskState::Success, item.symbol);
        } else {
            Control::onclose_symbol_update(TaskState::Fail, item.symbol);
            break;
        }
    }
    if (result == true) {
        Control::onclose_symbols_update(TaskState::Success);
    } else {
        Control::onclose_symbols_update(TaskState::Fail);
    }
    return result;
}


bool Ampanov::onclose_symbols_pattern() {

    bool result = false;

    Control::onclose_symbols_pattern(TaskState::Started);

    Stocks stocks = StockModel::select_all_active();
    Stock item;

    foreach (item, stocks) {

        result = scanner->run_live(item.symbol_id, "OneDay");
        result = result
              && scanner->run_live(item.symbol_id, "OneHour");
    }
    if (result == true) {
        Control::onclose_symbols_pattern(TaskState::Success);
    } else {
        Control::onclose_symbols_pattern(TaskState::Fail);
    }
    return result;
}


bool Ampanov::onclose_account_update() {

    bool result = false;

    Control::onclose_account_update(TaskState::Started);

    Account account = AccountModel::select_data_account();

    result  = questrade->update_account_balances(account);
    result &= questrade->update_account_positions(account);
    result &= questrade->update_account_orders(account);

    if (result == true) {
        Control::onclose_account_update(TaskState::Success);
    } else {
        Control::onclose_account_update(TaskState::Fail);
    }

    return result;
}

