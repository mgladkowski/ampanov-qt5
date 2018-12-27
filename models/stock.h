#ifndef STOCKMODEL_H
#define STOCKMODEL_H

#include "core/database.h"
#include "core/zeromq.h"

struct Stock {

    int         rowid                   = 0;
    QString     status                  = "";
    QString     symbol                  = "";
    int         symbol_id               = 0;
    QString     description             = "";
    QString     currency                = "";
    QString     exchange                = "";
    int         trade_unit              = 0;
    bool        is_index                = false;
    bool        is_tradable             = false;
    bool        is_quotable             = false;
    bool        has_options             = false;
    QString     security_type           = "";
    float       prev_day_close_price    = 0;
    float       high_price_52           = 0;
    float       low_price_52            = 0;
    uint        average_vol_90          = 0;
    uint        average_vol_20          = 0;
    uint        outstanding_shares      = 0;
    QString     industry_sector         = "";
    QString     industry_group          = "";
    QString     industry_subgroup       = "";
    QDateTime   last_update;
    QString     last_update_by          = "";
    float       last_price              = 0;
    int         last_price_volume       = 0;
    QDateTime   last_price_time;

    QJsonObject toJsonObject() {
        QJsonObject data;
        data["last_price"] = QString::number( last_price, 'f', 2 );
        data["last_price_volume"] = QString::number(last_price_volume);
        data["last_price_time"] = last_price_time.toString();
        data["change"] = QString::number( last_price - prev_day_close_price, 'f', 2 );
        return data;
    }
};
typedef QList<Stock> Stocks;


class StockModel : public Database {

public:

    static Stocks      select_all_active();
    static Stock       select_one_symbol(int symbol_id);
    static Stock       select_one_row(int rowid);

    static bool        save(Stock data);
    static bool        update_last_price(int symbol_id, float last_price, int last_volume, QDateTime last_time);

private:

    static Stocks      fetchAll(QSqlQuery query);
    static Stock       fetch(QSqlQuery &query);
    static bool        zmqEmit(Stock stock);

};

#endif // STOCKMODEL_H
