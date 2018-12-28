#ifndef ENGINE_H
#define ENGINE_H

#include <QSql>
#include <QDateTime>
#include "core/database.h"
#include "core/enums.h"

class EngineModel {
public:

    EngineModel();

    static bool         exists_locus();
    static bool         insert_locus();

    static bool         is_enabled();
    static bool         is_enabled(bool value);

    static Task         onstart_markets_update();
    static bool         onstart_markets_update(int);

    static Task         onstart_symbols_update();
    static bool         onstart_symbols_update(int);

    static Task         onstart_symbol_update();
    static bool         onstart_symbol_update(int, QString);

    static Task         onstart_symbols_pattern();
    static bool         onstart_symbols_pattern(int);

    static Task         onstart_account_update();
    static bool         onstart_account_update(int);

    static Task         onopen_options_update();
    static bool         onopen_options_update(int);

    static Task         intraday_minute_update();
    static bool         intraday_minute_update(int);

    static Task         intraday_minute_pattern();
    static bool         intraday_minute_pattern(int);

    static Task         intraday_hour_update();
    static bool         intraday_hour_update(int);

    static Task         intraday_hour_pattern();
    static bool         intraday_hour_pattern(int);

    static Task         intraday_account_update();
    static bool         intraday_account_update(int);

    static Task         intraday_trades_open();
    static bool         intraday_trades_open(int);

    static Task         intraday_trades_setup();
    static bool         intraday_trades_setup(int);

    static Task         intraday_trades_enter();
    static bool         intraday_trades_enter(int);

    static Task         onclose_options_update();
    static bool         onclose_options_update(int);

    static Task         onclose_symbols_update();
    static bool         onclose_symbols_update(int);

    static Task         onclose_symbol_update();
    static bool         onclose_symbol_update(int, QString);

    static Task         onclose_symbols_pattern();
    static bool         onclose_symbols_pattern(int);

    static Task         onclose_account_update();
    static bool         onclose_account_update(int);

};

#endif // ENGINE_H
