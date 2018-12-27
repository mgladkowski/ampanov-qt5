#ifndef TRADEMODEL_H
#define TRADEMODEL_H

#include "core/database.h"


struct Trade {

    int         rowid               = 0;
    int         account_id          = 0;
    int         symbol_id           = 0;
    QString     symbol              = "";
    int         opt_symbol_id       = 0;
    QString     opt_symbol          = "";
    QString     type                = "";
    QString     status              = "E";
    QString     direction           = "";
    float       risk_unit           = 0;
    QDateTime   plan_date;
    float       plan_entry          = 0;
    float       plan_stop           = 0;
    float       plan_target         = 0;
    float       plan_risk           = 0;
    float       plan_reward         = 0;
    float       plan_ratio          = 0;
    int         plan_shares         = 0;
    int         plan_options        = 0;
    int         exec_quantity       = 0;
    QDateTime   exec_open_date;
    QDateTime   exec_close_date;
    float       exec_entry          = 0;
    int         exec_entry_id       = 0;
    float       exec_stop           = 0;
    int         exec_stop_id        = 0;
    float       exec_target         = 0;
    int         exec_target_id      = 0;
    float       position_price      = 0;
    float       position_average    = 0;
    int         position_open       = 0;
    float       position_open_pnl   = 0;
    int         position_closed     = 0;
    float       position_closed_pnl = 0;
    float       position_total_cost = 0;
    float       position_value      = 0;
    bool        position_realtime   = false;
    bool        position_reorg      = false;
    QDateTime   position_updated;

};
typedef QList<Trade> Trades;


class TradeModel : public Database {
public:

    static Trade           select_one_active(int account_row, int symbol_id);
    static Trades          select_open();
    static Trades          select_open(int symbol_id);
    static Trade           select_one_row(int rowid);

    static bool            insert(Trade data);
    static bool            update(Trade data);
    static bool            save(Trade data);

private:

    static Trades          fetchAll(QSqlQuery query);
    static Trade           fetch(QSqlQuery &query);

};

#endif // TRADEMODEL_H
