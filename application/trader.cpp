#include "trader.h"

Trader::Trader() {

}


/**
 * Check any open trades for stop or target trigger
 */
bool Trader::process_open() {

    Trades trades = TradeModel::select_open();
    Trade trade;

    foreach (trade, trades) {

        qInfo() << "open trade : " << qPrintable(trade.symbol) << " qty " << trade.position_open << " pnl " << trade.position_open_pnl;


        // VALIDATE DATA

        Stock stock = StockModel::select_one_symbol(trade.symbol_id);

        bool is_fresh = false;
        is_fresh  = (trade.position_updated.addSecs(300) > QDateTime::currentDateTime());

        if (is_fresh != true)
            return tx_fail("process_open : stale position data");

        is_fresh &= (stock.last_price_time.addSecs(300) > QDateTime::currentDateTime());

        if (is_fresh != true)
            return tx_fail("process_open : stale stock data");


        // CHECK FOR STOP REACHED

        if ((is_fresh == true)

            && (trade.position_open > 0)

            && ( (trade.position_open_pnl <= (trade.risk_unit * -1) )

            || (stock.last_price <= trade.plan_stop))

        ){
            qInfo() << "executing STOP LOSS : " << qPrintable(trade.symbol) << " qty " << trade.position_open << " pnl " << trade.position_open_pnl;

            execute_stop(trade);
        }


        // CHECK FOR TARGET REACHED

        else if ((is_fresh == true)

            && (trade.position_open > 0)

            && (stock.last_price >= trade.plan_target)

        ){
            qInfo() << "executing TAKE PROFIT : " << qPrintable(trade.symbol) << " qty " << trade.position_open << " pnl " << trade.position_open_pnl;

            execute_target(trade);
        }

    }
    return true;
}


/**
 * Check any new trade candidates to create
 */
bool Trader::process_setup() {

    return true;
}


/**
 * Check any trade candidates for entry trigger
 */
bool Trader::process_entry() {

    return true;
}


/**
 * Send entry order
 */
bool Trader::execute_entry(Trade trade) {

    bool result = false;

    if (trade.rowid < 1)
        return tx_fail("execute_entry : empty trade.rowid");

    Account account = AccountModel::select_row(trade.account_id);
    if (account.rowid < 1 || account.rowid != trade.account_id)
        return tx_fail("execute_entry : invalid trade.account_id");

    Stock stock = StockModel::select_one_symbol(trade.symbol_id);
    if (stock.rowid < 1 || stock.symbol_id != trade.symbol_id)
        return tx_fail("execute_entry : invalid trade.symbol_id");

    // update trade

    trade.exec_open_date    = QDateTime::currentDateTime();
    trade.exec_entry        = stock.last_price;
    trade.exec_quantity     = trade.type == "Option" ? trade.plan_options : trade.exec_quantity;
    trade.exec_quantity     = trade.type == "Stock" ? trade.plan_shares : trade.exec_quantity;

    if (trade.exec_quantity < 1)
        return tx_fail("execute_entry : unable to determine quantity");

    result = TradeModel::save(trade);

    if (result != true)
        return tx_fail("execute_entry : failed to save trade");

    // create order

    Order order;
    order.trade_id          = trade.rowid;
    order.account_number    = account.account;

    result &= OrderModel::save(order);

    if (result != true)
        return tx_fail("execute_entry : failed to save order");

    // send live order to broker

    if (account.allow_ai == "R") {

        return tx_success("execute_entry : sent order to Questrade");
    }

    // send virtual order to simulator

    else {

        return tx_success("execute_entry : sent order to Simulator");
    }
    return tx_fail("execute_entry : skipped order submit");
}


/**
 * Send stop loss order
 */
bool Trader::execute_stop(Trade trade) {

    bool result = false;

    if (trade.rowid < 1)
        return tx_fail("execute_stop : empty trade.rowid");

    Account account = AccountModel::select_row(trade.account_id);
    if (account.rowid < 1 || account.rowid != trade.account_id)
        return tx_fail("execute_stop : invalid trade.account_id");

    Stock stock = StockModel::select_one_symbol(trade.symbol_id);
    if (stock.rowid < 1 || stock.symbol_id != trade.symbol_id)
        return tx_fail("execute_stop : invalid trade.symbol_id");

    // update trade

    trade.exec_close_date   = QDateTime::currentDateTime();
    trade.exec_stop         = stock.last_price;

    result = TradeModel::save(trade);

    if (result != true)
        return tx_fail("execute_stop : failed to save trade");

    // create order

    Order order;
    order.trade_id          = trade.rowid;
    order.account_number    = account.account;

    result &= OrderModel::save(order);

    if (result != true)
        return tx_fail("execute_stop : failed to save order");

    // send live order to broker

    if (account.allow_ai == "R") {

        return tx_success("execute_stop : sent order to Questrade");
    }

    // send virtual order to simulator

    else {

        return tx_success("execute_stop : sent order to Simulator");
    }
    return tx_fail("execute_stop : skipped order submit");
}


/**
 * Send target order
 */
bool Trader::execute_target(Trade trade) {

    bool result = false;

    if (trade.rowid < 1)
        return tx_fail("execute_target : empty trade.rowid");

    Account account = AccountModel::select_row(trade.account_id);
    if (account.rowid < 1 || account.rowid != trade.account_id)
        return tx_fail("execute_target : invalid trade.account_id");

    Stock stock = StockModel::select_one_symbol(trade.symbol_id);
    if (stock.rowid < 1 || stock.symbol_id != trade.symbol_id)
        return tx_fail("execute_target : invalid trade.symbol_id");

    // update trade

    trade.exec_close_date   = QDateTime::currentDateTime();
    trade.exec_target       = stock.last_price;

    result = TradeModel::save(trade);

    if (result != true)
        return tx_fail("execute_target : failed to save trade");

    // create order

    Order order;
    order.trade_id          = trade.rowid;
    order.account_number    = account.account;

    result &= OrderModel::save(order);

    if (result != true)
        return tx_fail("execute_target : failed to save order");

    // send live order to broker

    if (account.allow_ai == "R") {

        return tx_success("execute_target : sent order to Questrade");
    }

    // send virtual order to simulator

    else {

        return tx_success("execute_target : sent order to Simulator");
    }
    return tx_fail("execute_target : skipped order submit");
}


/**
 * Emit transaction fail notifications
 */
bool Trader::tx_fail(QString message) {

    qCritical() << qPrintable(message);
    return false;
}


/**
 * Emit transaction success notifications
 */
bool Trader::tx_success(QString message) {

    qInfo() << qPrintable(message);
    return true;
}
