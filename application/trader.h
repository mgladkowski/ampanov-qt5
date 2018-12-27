#ifndef TRADER_H
#define TRADER_H

#include "models/account.h"
#include "models/option.h"
#include "models/order.h"
#include "models/stock.h"
#include "models/trade.h"
#include "core/zeromq.h"

class Trader {

public:

    Trader();

    bool    is_live = false;

    bool    process_open();
    bool    process_setup();
    bool    process_entry();

private:

    bool    execute_entry(Trade trade);
    bool    execute_stop(Trade trade);
    bool    execute_target(Trade trade);

    bool    tx_fail(QString message);
    bool    tx_success(QString message);
};

#endif // TRADER_H
