#ifndef ORDER_H
#define ORDER_H

#include "core/database.h"
#include "core/zeromq.h"


struct Order {

    int         rowid                   = 0;
    int         trade_id                = 0;
    int         questrade_id            = 0;
    QString     exchange_order_id       = "";
    QString     account_number          = "";
    QString     symbol                  = "";
    int         symbol_id               = 0;
    QString     side                    = "";
    QString     source                  = "";
    QString     state                   = "";
    QString     client_reason           = "";
    QString     strategy_type           = "";
    QString     order_type              = "";
    QString     time_in_force           = "";
    QString     action                  = "";
    int         quantity                = 0;
    int         total_quantity          = 0;
    int         open_quantity           = 0;
    int         filled_quantity         = 0;
    int         canceled_quantity       = 0;
    float       limit_price             = 0;
    float       stop_price              = 0;
    float       avg_exec_price          = 0;
    float       last_exec_price         = 0;
    float       commission_charged      = 0;
    bool        is_allornone            = false;
    bool        is_anonymous            = false;
    QString     primary_route           = "";
    QString     secondary_route         = "";
    QString     order_route             = "";
    QString     created_by              = "";
    QDateTime   created_date;
    QString     modified_by             = "";
    QDateTime   modified_date;
    QString     questrade_created       = "";
    QString     questrade_updated       = "";

    QJsonObject toJsonObject() {
        QJsonObject data;
        return data;
    }

};
typedef QList<Order> Orders;


class OrderModel : public Database {

public:

    static Orders       select_active();
    static Order        select_one(int id);
    static Order        select_one_row(int rowid);

    static bool         insert(Order data);
    static bool         update(Order data);
    static bool         save(Order data);
    static bool         insert_actual(Order data);
    static bool         update_actual(Order data);
    static bool         save_actual(Order data);

private:

    static Orders       fetchAll(QSqlQuery query);
    static Order        fetch(QSqlQuery &query);
    static bool         zmqEmit(Order order);

};


#endif // ORDER_H
