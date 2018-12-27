#ifndef OPTIONMODEL_H
#define OPTIONMODEL_H

#include "core/database.h"
#include "core/zeromq.h"
#include "models/stock.h"

struct OptionChain {

    int         rowid           = 0;
    int         underlying_id   = 0;
    QString     underlying      = "";
    QDateTime   expiry_date;
    float       strike_price    = 0;
    int         call_symbol_id  = 0;
    int         put_symbol_id   = 0;
    int         multiplier      = 0;
    QDateTime   last_updated;
};
typedef QList<OptionChain> OptionChains;


struct Option {

    int         rowid           = 0;
    int         underlying_id   = 0;
    QString     underlying      = "";
    int         symbol_id       = 0;
    QString     symbol          = "";
    float       bid_price       = 0;
    float       ask_price       = 0;
    float       last_trade_price_trhrs  = 0;
    float       last_trade_price        = 0;
    int         volume          = 0;
    float       open            = 0;
    float       high            = 0;
    float       low             = 0;
    float       volatility      = 0;
    float       delta           = 0;
    float       gamma           = 0;
    float       theta           = 0;
    float       vega            = 0;
    float       rho             = 0;
    float       vwap            = 0;
    int         open_interest   = 0;
    int         delay           = 0;
    bool        is_halted       = false;
    QDateTime   last_updated;
};
typedef QList<Option> Options;


class OptionModel {
public:

    static Option       select_one_symbol(int symbol_id);
    static Option       select_one_atm(Stock stock, int horizon);
    static Option       select_one_row(int rowid);
    static bool         insert(Option data);
    static bool         update(Option data);
    static bool         save(Option data);

private:

    static Options      fetchAll(QSqlQuery query);
    static Option       fetch(QSqlQuery &query);
};


class OptionChainModel {
public:

    static OptionChains select_best_range(Stock stock);
    static OptionChain  select_one_strike(int symbol_id, QDateTime expiry, float strike);
    static OptionChain  select_one_row(int rowid);
    static bool         insert(OptionChain data);
    static bool         update(OptionChain data);
    static bool         save(OptionChain data);

private:

    static OptionChains fetchAll(QSqlQuery query);
    static OptionChain  fetch(QSqlQuery &query);
};


#endif // OPTIONMODEL_H
