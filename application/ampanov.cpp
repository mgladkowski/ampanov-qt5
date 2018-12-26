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

    loopTimer = new QTimer(this);
    loopTimer->setSingleShot(false);
    loopTimer->setInterval(loopInterval);

    connect( loopTimer, SIGNAL(timeout()), this, SLOT(think()), Qt::QueuedConnection );

    //questrade       = new QuestradeApi(this);

    qDebug() << "Ampanov constructed";
    qWarning() << "Delayed market data";
}


Ampanov::~Ampanov() {

    //delete questrade;
    //questrade = nullptr;

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

//    if (ZeroMq::test()) {
//        qInfo() << "ZMQ : ok";
//    } else {
//        qCritical() << "ZMQ : fail";
//        pass = false;
//    }

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

    //Control::start();

    return true;
}


bool Ampanov::stop() {

    isRunning = false;
    loopTimer->stop();

    //Control::stop();

    return true;
}


bool Ampanov::pause() {

    if (isRunning != true) return false;

    isPaused = true;
    loopTimer->stop();

    //Control::pause();

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

    //Control::resume();

    return true;
}



/**
 * Game loop
 */
void Ampanov::think() {

    qDebug() << '.';

    return tx_done();
}


void Ampanov::tx_done() {

    qApp->processEvents(QEventLoop::AllEvents);
}


void Ampanov::tx_fail() {

    qApp->processEvents(QEventLoop::AllEvents);
}

