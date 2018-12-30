#ifndef AMPANOV_H
#define AMPANOV_H

#include <QObject>
#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include "control.h"
#include "helpers.h"
#include "scanner.h"
#include "trader.h"
#include "models/account.h"
#include "models/candle.h"
#include "models/stock.h"
#include "models/strategy.h"
#include "models/trade.h"
#include "broker/simulator.h"
#include "broker/questrade.h"

class Ampanov : public QObject {

    Q_OBJECT


private:

    QuestradeApi    *questrade;
    Scanner         *scanner;
    Trader          *trader;

    QTimer          *loopTimer;
    const int       loopInterval = 1000;
    bool            isRunning;
    bool            isPaused;

    bool            isLive;
    bool            isReplay;
    QString         replaySymbol;
    int             replayStrategy;

    QTime           time_start;
    QTime           time_open_ext;
    QTime           time_open;
    QTime           time_close;
    QTime           time_close_ext;
    QTime           time_stop;

    bool            onstart_markets_update();
    bool            onstart_symbols_update();
    bool            onstart_symbols_pattern();
    bool            onstart_account_update();
    bool            onopen_options_update();
    bool            intraday_minute_update();
    bool            intraday_minute_pattern();
    bool            intraday_hour_update();
    bool            intraday_hour_pattern();
    bool            intraday_account_update();
    bool            intraday_trades_open();
    bool            intraday_trades_setup();
    bool            intraday_trades_enter();
    bool            onclose_options_update();
    bool            onclose_symbols_update();
    bool            onclose_symbols_pattern();
    bool            onclose_account_update();

public:

    explicit Ampanov(QObject *parent = nullptr);
    ~Ampanov();

    void            dev();
    void            recalc( QString symbol );
    void            replay( QString symbol, int strategy_id );

    bool            isHealthy();
    bool            start();
    bool            stop();
    bool            pause();
    bool            resume();

    void            tx_done();
    void            tx_fail();


private slots:

    void            think();
};

#endif // AMPANOV_H
