#ifndef CONTROL_H
#define CONTROL_H

#include <QDateTime>
#include "core/zeromq.h"
#include "models/engine.h"

class Control {

public:

    static bool     locusValid;
    static QDate    locusCreated;

    static bool     is_locus_valid() {
        return (Control::locusValid &&
                Control::locusCreated.isValid() &&
                Control::locusCreated.daysTo(QDate::currentDate()) == 0);
    }

    static void     start();
    static void     stop();
    static void     pause();
    static void     resume();
    static void     heartbeat();

    static bool     valid_checklist();
    static bool     exists_checklist();
    static void     create_checklist(int);

    static Task     onstart_markets_update();
    static void     onstart_markets_update(int);

    static Task     onstart_symbols_update();
    static void     onstart_symbols_update(int);

    static Task     onstart_symbol_update();
    static void     onstart_symbol_update(int, QString);

    static Task     onstart_symbols_pattern();
    static void     onstart_symbols_pattern(int);

    static Task     onstart_account_update();
    static void     onstart_account_update(int);

    static Task     onopen_options_update();
    static void     onopen_options_update(int);

    static Task     intraday_minute_update();
    static void     intraday_minute_update(int);

    static Task     intraday_minute_pattern();
    static void     intraday_minute_pattern(int);

    static Task     intraday_hour_update();
    static void     intraday_hour_update(int);

    static Task     intraday_hour_pattern();
    static void     intraday_hour_pattern(int);

    static Task     intraday_account_update();
    static void     intraday_account_update(int);

    static Task     intraday_trades_open();
    static void     intraday_trades_open(int);

    static Task     intraday_trades_setup();
    static void     intraday_trades_setup(int);

    static Task     intraday_trades_enter();
    static void     intraday_trades_enter(int);

    static Task     onclose_options_update();
    static void     onclose_options_update(int);

    static Task     onclose_symbols_update();
    static void     onclose_symbols_update(int);

    static Task     onclose_symbol_update();
    static void     onclose_symbol_update(int, QString);

    static Task     onclose_symbols_pattern();
    static void     onclose_symbols_pattern(int);

    static Task     onclose_account_update();
    static void     onclose_account_update(int);

};

#endif // CONTROL_H
