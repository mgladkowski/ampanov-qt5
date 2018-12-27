#ifndef QuestradeApi_H
#define QuestradeApi_H

#include <QObject>
#include <QNetworkAccessManager>
#include "models/account.h"
#include "models/market.h"
#include "models/stock.h"
#include "models/candle.h"
#include "models/option.h"
#include "models/order.h"
#include "models/trade.h"
#include "application/analyst.h"


struct HttpRequest {

    QString         api_server;
    QString         port = "443";
    QString         method = "GET";
    QString         path;
    QMap<QString, QVariant>  params;
    QString         json;
};


class QuestradeApi : public QObject {

    Q_OBJECT

public:

    explicit QuestradeApi(QObject *parent = nullptr);
    ~QuestradeApi();

    bool            test();

    bool            update_markets();
    bool            update_symbol(int symbol_id);
    bool            update_option_chain(int symbol_id);
    bool            update_option(int symbol_id);

    bool            update_candles_day(int symbol_id, QDateTime start, QDateTime end);
    bool            update_candles_hour(int symbol_id, QDateTime start, QDateTime end);
    bool            update_candles_thirty(int symbol_id, QDateTime start, QDateTime end);
    bool            update_candles_minute(int symbol_id, QDateTime start, QDateTime end);

    bool            update_account_balances(Account account);
    bool            update_account_executions(Account account);
    bool            update_account_positions(Account account);
    bool            update_account_orders(Account account);

    bool            history_account_orders(Account account);

    OptionChain     get_option_chain(int symbol_id);


private:

    QNetworkAccessManager   *network;
    QEventLoop              eventLoop;

    QString         auth_server = "https://login.questrade.com/oauth2/token?grant_type=refresh_token&refresh_token=";
    QString         lastResponse;
    QString         lastError;

    Account         account;

    bool            httpGet(QString url);
    bool            httpRequest(HttpRequest data);

    bool            current_access_token();
};

#endif // QuestradeApi_H
