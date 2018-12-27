#include <QDebug>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThread>
#include <QUrl>
#include <QUrlQuery>
#include "questrade.h"
#include "application/helpers.h"


QuestradeApi::QuestradeApi(QObject *parent) : QObject(parent) {

    network = new QNetworkAccessManager(this);
    QObject::connect(network, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
}


QuestradeApi::~QuestradeApi() {

    network->disconnect(network, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    delete network;
}


/*
 * Test API connection
 */
bool QuestradeApi::test() {

    return true;
}


/**
 * Market
 */
bool QuestradeApi::update_markets() {

    HttpRequest request;
    request.path = "v1/markets";

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("markets").toArray();

        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();

            QString market = o.value("name").toString();
            Market m = MarketModel::select_one(market);

            m.name = market;
            m.extendedStartTime = Helpers::atom_to_datetime(o.value("extendedStartTime").toString());
            m.startTime = Helpers::atom_to_datetime(o.value("startTime").toString());
            m.endTime = Helpers::atom_to_datetime(o.value("endTime").toString());
            m.extendedEndTime = Helpers::atom_to_datetime(o.value("extendedEndTime").toString());
            m.defaultTradingVenue = o.value("defaultTradingVenue").toString();

            if (m.rowid > 0) {
                MarketModel::save(m);
            } else {
                MarketModel::insert(m);
            }
        }
        return true;
    }
    return false;
}


/**
 * Symbol
 */
bool QuestradeApi::update_symbol(int symbol_id) {

    Stock s = StockModel::select_one_symbol(symbol_id);
    if (s.rowid < 1) return false;

    HttpRequest request;
    request.path = "v1/symbols/";
    request.path.append(QString::number(symbol_id));

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("symbols").toArray();

        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();
            if (s.symbol_id != o.value("symbolId").toInt()) return false;
            s.symbol_id = o.value("symbolId").toInt();
            s.symbol = o.value("symbol").toString();
            s.description = o.value("description").toString();
            s.currency = o.value("currency").toString();
            s.exchange = o.value("listingExchange").toString();
            s.trade_unit = o.value("tradeUnit").toInt();
            s.is_tradable = o.value("isTradable").toBool();
            s.is_quotable = o.value("isQuotable").toBool();
            s.has_options = o.value("hasOptions").toBool();
            s.security_type = o.value("securityType").toString();
            s.prev_day_close_price = o.value("prevDayClosePrice").toDouble();
            s.high_price_52 = o.value("highPrice52").toDouble();
            s.low_price_52 = o.value("lowPrice52").toDouble();
            s.average_vol_90 = o.value("averageVol3Months").toDouble();
            s.average_vol_20 = o.value("averageVol20Days").toDouble();
            s.outstanding_shares = o.value("outstandingShares").toVariant().toUInt();
            s.industry_sector = o.value("industrySector").isString() ? o.value("industrySector").toString() : QString("");
            s.industry_group = o.value("industryGroup").isString() ? o.value("industryGroup").toString() : QString("");
            s.industry_subgroup = o.value("industrySubgroup").isString() ? o.value("industrySubgroup").toString() : QString("");

            bool result = StockModel::save(s);
            return result;
        }
    }
    return false;
}


/**
 * Option chain
 */
bool QuestradeApi::update_option_chain(int symbol_id) {

    Stock s = StockModel::select_one_symbol(symbol_id);

    if (s.rowid < 1) return false;
    if (s.has_options != true) return false;

    HttpRequest request;
    request.path = "v1/symbols/";
    request.path.append(QString::number(s.symbol_id));
    request.path.append("/options");

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if (doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("optionChain").toArray();

        foreach (QJsonValue value, array) {

            QJsonObject oroot = value.toObject();
            QJsonArray roots = oroot.value("chainPerRoot").toArray();

            QDateTime expiry = Helpers::atom_to_datetime(oroot.value("expiryDate").toString());

            foreach (QJsonValue value, roots) {

                QJsonObject ostrike = value.toObject();
                QJsonArray strikes = ostrike.value("chainPerStrikePrice").toArray();
                int multiplier = ostrike.value("multiplier").toInt();

                foreach (QJsonValue value, strikes) {

                    QJsonObject o = value.toObject();

                    float strike = o.value("strikePrice").toDouble();

                    OptionChain chain = OptionChainModel::select_one_strike(s.symbol_id, expiry, strike);

                    chain.underlying_id = s.symbol_id;
                    chain.underlying = s.symbol;
                    chain.expiry_date = expiry;
                    chain.strike_price = strike;
                    chain.call_symbol_id = o.value("callSymbolId").toInt();
                    chain.put_symbol_id = o.value("putSymbolId").toInt();
                    chain.multiplier = multiplier;

                    OptionChainModel::save(chain);
                    QThread::msleep(20);
                }
            }
        }
        return true;
    }
    return false;
}


/**
 * Options
 */
bool QuestradeApi::update_option(int symbol_id) {

    Stock s = StockModel::select_one_symbol(symbol_id);

    if (s.rowid < 1) return false;
    if (s.has_options != true) return false;

    OptionChains chain = OptionChainModel::select_best_range(s);
    OptionChain item;
    QString list = "";
    foreach (item, chain) {
        if (list.length() > 0) list.append(",");
        list.append(QString::number(item.call_symbol_id));
        list.append(",");
        list.append(QString::number(item.put_symbol_id));
    }

    HttpRequest request;
    request.path = "v1/markets/quotes/options";
    request.method = "POST";
    request.json = "{\"optionIds\":[";
    request.json.append(list);
    request.json.append("]}");

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("optionQuotes").toArray();

        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();

            float symbol_id = o.value("symbolId").toInt();

            Option option = OptionModel::select_one_symbol(symbol_id);

            option.underlying_id = s.symbol_id;
            option.underlying = s.symbol;
            option.symbol_id = symbol_id;
            option.symbol = o.value("symbol").toString();
            option.bid_price = o.value("bidPrice").toDouble();
            option.ask_price = o.value("askPrice").toDouble();
            option.last_trade_price_trhrs = o.value("lastTradePriceTrHrs").toDouble();
            option.last_trade_price = o.value("lastTradePrice").toDouble();
            option.volume = o.value("volume").toInt();
            option.open = o.value("openPrice").toDouble();
            option.high = o.value("highPrice").toDouble();
            option.low = o.value("lowPrice").toDouble();
            option.volatility = o.value("volatility").toDouble();
            option.delta = o.value("delta").toDouble();
            option.gamma = o.value("gamma").toDouble();
            option.theta = o.value("theta").toDouble();
            option.vega = o.value("vega").toDouble();
            option.rho = o.value("rho").toDouble();
            option.open_interest = o.value("openInterest").toInt();
            option.delay = o.value("delay").toInt();
            option.is_halted = o.value("isHalted").toBool();
            option.vwap = o.value("VWAP").toDouble();

            OptionModel::save(option);
            QThread::msleep(20);
        }
        return true;
    }
    return false;
}


/**
 * Candles - day
 */
bool QuestradeApi::update_candles_day(int symbol_id, QDateTime from, QDateTime to) {

    bool ret = false;
    HttpRequest request;

    request.path = "v1/markets/candles/";
    request.path.append(QString::number(symbol_id));

    QDateTime now = QDateTime::currentDateTime();
    int offset = now.offsetFromUtc();
    from.setOffsetFromUtc(offset);
    QString start = from.toString(Qt::ISODate);
    to.setOffsetFromUtc(offset);
    QString end = to.toString(Qt::ISODate);

    request.params["startTime"] = start;
    request.params["endTime"] = end;
    request.params["interval"] = "OneDay";

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("candles").toArray();

        ret = true;
        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();

            QDateTime start = Helpers::atom_to_datetime(o.value("start").toString());
            QDateTime end = Helpers::atom_to_datetime(o.value("end").toString());

            Candle c = CandleModel::select_one(symbol_id, start);

            c.symbol_id = symbol_id;
            c.start = start;
            c.end = end;
            c.open = o.value("open").toDouble();
            c.close = o.value("close").toDouble();
            c.high = o.value("high").toDouble();
            c.low = o.value("low").toDouble();
            c.volume = o.value("volume").toInt();
            c.vwap = o.value("VWAP").toDouble();

            if (c.rowid > 0) {
                ret = CandleModel::save(c);
            } else {
                ret = CandleModel::insert(c);
            }

            StockModel::update_last_price(c.symbol_id, c.close, c.volume, c.end);

            Analyst chart;
            if (chart.loadslice(c.symbol_id, "OneDay") == true) {
                chart.calculate_candle();
            }
        }
        return ret;
    }
    return false;
}


/**
 * Candles - hour
 */
bool QuestradeApi::update_candles_hour(int symbol_id, QDateTime from, QDateTime to) {

    bool ret = false;
    QString interval("OneHour");
    HttpRequest request;

    request.path = "v1/markets/candles/";
    request.path.append(QString::number(symbol_id));

    QDateTime now = QDateTime::currentDateTime();
    int offset = now.offsetFromUtc();
    from.setOffsetFromUtc(offset);
    QString start = from.toString(Qt::ISODate);
    to.setOffsetFromUtc(offset);
    QString end = to.toString(Qt::ISODate);

    request.params["startTime"] = start;
    request.params["endTime"] = end;
    request.params["interval"] = "OneHour";

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("candles").toArray();

        ret = true;
        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();

            QDateTime start = Helpers::atom_to_datetime(o.value("start").toString());
            QDateTime end = Helpers::atom_to_datetime(o.value("end").toString());

            Candle c = CandleModel::select_one(symbol_id, start, interval);

            c.symbol_id = symbol_id;
            c.start = start;
            c.end = end;
            c.open = o.value("open").toDouble();
            c.close = o.value("close").toDouble();
            c.high = o.value("high").toDouble();
            c.low = o.value("low").toDouble();
            c.volume = o.value("volume").toInt();
            c.vwap = o.value("VWAP").toDouble();

            if (c.rowid > 0) {
                ret = CandleModel::save(c, interval);
            } else {
                ret = CandleModel::insert(c, interval);
            }
            Analyst chart;
            if (chart.loadslice(c.symbol_id, "OneHour") == true) {
                chart.calculate_candle();
            }
        }
        return ret;
    }
    return false;
}


/**
 * Candles - 30 minute
 */
bool QuestradeApi::update_candles_thirty(int symbol_id, QDateTime from, QDateTime to) {

    bool ret = false;
    QString interval("HalfHour");
    HttpRequest request;

    request.path = "v1/markets/candles/";
    request.path.append(QString::number(symbol_id));

    QDateTime now = QDateTime::currentDateTime();
    int offset = now.offsetFromUtc();
    from.setOffsetFromUtc(offset);
    QString start = from.toString(Qt::ISODate);
    to.setOffsetFromUtc(offset);
    QString end = to.toString(Qt::ISODate);

    request.params["startTime"] = start;
    request.params["endTime"] = end;
    request.params["interval"] = "HalfHour";

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("candles").toArray();

        ret = true;
        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();

            QDateTime start = Helpers::atom_to_datetime(o.value("start").toString());
            QDateTime end = Helpers::atom_to_datetime(o.value("end").toString());

            Candle c = CandleModel::select_one(symbol_id, start, interval);

            c.symbol_id = symbol_id;
            c.start = start;
            c.end = end;
            c.open = o.value("open").toDouble();
            c.close = o.value("close").toDouble();
            c.high = o.value("high").toDouble();
            c.low = o.value("low").toDouble();
            c.volume = o.value("volume").toInt();
            c.vwap = o.value("VWAP").toDouble();

            if (c.rowid > 0) {
                ret = CandleModel::save(c, interval);
            } else {
                ret = CandleModel::insert(c, interval);
            }
        }
        return ret;
    }
    return false;
}


/**
 * Candles - minute
 */
bool QuestradeApi::update_candles_minute(int symbol_id, QDateTime from, QDateTime to) {

    bool result = false;
    QString interval("OneMinute");
    HttpRequest request;

    request.path = "v1/markets/candles/";
    request.path.append(QString::number(symbol_id));

    QDateTime now = QDateTime::currentDateTime();
    int offset = now.offsetFromUtc();
    from.setOffsetFromUtc(offset);
    QString start = from.toString(Qt::ISODate);
    to.setOffsetFromUtc(offset);
    QString end = to.toString(Qt::ISODate);

    request.params["startTime"] = start;
    request.params["endTime"] = end;
    request.params["interval"] = "OneMinute";

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("candles").toArray();

        result = true;

        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();

            QDateTime start = Helpers::atom_to_datetime(o.value("start").toString());
            QDateTime end = Helpers::atom_to_datetime(o.value("end").toString());

            Candle c = CandleModel::select_one(symbol_id, start, interval);

            c.symbol_id = symbol_id;
            c.start = start;
            c.end = end;
            c.open = o.value("open").toDouble();
            c.close = o.value("close").toDouble();
            c.high = o.value("high").toDouble();
            c.low = o.value("low").toDouble();
            c.volume = o.value("volume").toInt();
            c.vwap = o.value("VWAP").toDouble();

            if (c.rowid > 0) {
                result = CandleModel::save(c, interval);
            } else {
                result = CandleModel::insert(c, interval);
            }
        }
        return result;
    }
    return false;
}


/**
 * Account : Balances
 */
bool QuestradeApi::update_account_balances(Account account) {

    HttpRequest request;
    request.path = "v1/accounts/";
    request.path.append(account.account);
    request.path.append("/balances");

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("perCurrencyBalances").toArray();

        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();

            QString currency = o.value("currency").toString();

            if (currency == account.currency) {

                account.cash                = o.value("cash").toDouble();
                account.market_value        = o.value("marketValue").toDouble();
                account.total_equity        = o.value("totalEquity").toDouble();
                account.buying_power        = o.value("buyingPower").toDouble();
                account.maintenance_excess  = o.value("maintenanceExcess").toDouble();
                account.is_real_time        = o.value("isRealTime").toBool();

                bool result = AccountModel::save_balance(account);
                return result;
            }
        }
        return true;
    }
    return false;
}


/**
 * Account : Positions
 */
bool QuestradeApi::update_account_positions(Account account) {

    HttpRequest request;
    request.path = "v1/accounts/";
    request.path.append(account.account);
    request.path.append("/positions");

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("positions").toArray();

        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();

            int symbol_id = o.value("symbolId").toInt();

            Trade t = TradeModel::select_one_active(account.rowid, symbol_id);

            t.account_id            = account.rowid;
            t.symbol_id             = o.value("symbolId").toInt();
            t.symbol                = o.value("symbol").toString();
            t.position_open         = o.value("openQuantity").toInt();
            t.position_closed       = o.value("closedQuantity").toInt();
            t.position_value        = o.value("currentMarketValue").toDouble();
            t.position_price        = o.value("currentPrice").toDouble();
            t.position_average      = o.value("averageEntryPrice").toDouble();
            t.position_closed_pnl   = o.value("closedPnl").toDouble();
            t.position_open_pnl     = o.value("openPnl").toDouble();
            t.position_total_cost   = o.value("totalCost").toDouble();
            t.position_realtime     = o.value("isRealTime").toBool();
            t.position_reorg        = o.value("isUnderReorg").toBool();

            TradeModel::save(t);
        }
        return true;
    }
    return false;
}


/**
 * Account : Orders
 */
bool QuestradeApi::update_account_orders(Account account) {

    HttpRequest request;
    request.path = "v1/accounts/";
    request.path.append(account.account);
    request.path.append("/orders");

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("orders").toArray();

        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();

            int order_number            = o.value("id").toInt();

            Order order = OrderModel::select_one(order_number);

            order.questrade_id          = order_number;
            order.exchange_order_id     = o.value("exchangeOrderId").toString();
            order.account_number        = account.account;
            order.symbol                = o.value("symbol").toString();
            order.symbol_id             = o.value("symbolId").toInt();
            order.side                  = o.value("side").toString();
            order.source                = o.value("source").toString();
            order.state                 = o.value("state").toString();
            order.client_reason         = o.value("clientReasonStr").toString();
            order.strategy_type         = o.value("strategyType").toString();
            order.order_type            = o.value("orderType").toString();
            order.time_in_force         = o.value("timeInForce").toString();
            order.total_quantity        = o.value("totalQuantity").toInt();
            order.open_quantity         = o.value("openQuantity").toInt();
            order.filled_quantity       = o.value("filledQuantity").toInt();
            order.canceled_quantity     = o.value("canceledQuantity").toInt();
            order.limit_price           = o.value("limitPrice").toDouble();
            order.stop_price            = o.value("stopPrice").toDouble();
            order.avg_exec_price        = o.value("avgExecPrice").toDouble();
            order.last_exec_price       = o.value("lastExecPrice").toDouble();
            order.commission_charged    = o.value("commissionCharged").toDouble();
            order.is_allornone          = o.value("isAllOrNone").toBool();
            order.is_anonymous          = o.value("isAnonymous").toBool();
            order.primary_route         = o.value("primaryRoute").toString();
            order.secondary_route       = o.value("secondaryRoute").toString();
            order.order_route           = o.value("orderRoute").toString();
            order.questrade_created     = o.value("creationTime").toString();
            order.questrade_updated     = o.value("updateTime").toString();

            OrderModel::save_actual(order);
        }
        return true;
    }
    return false;
}


/**
 * Account : Orders history
 */
bool QuestradeApi::history_account_orders(Account account) {

    QDateTime now = QDateTime::currentDateTime();
    int offset = now.offsetFromUtc();
    now.setOffsetFromUtc(offset);
    QDateTime start = QDateTime(QDate(2017,5,1), QTime(0,0,0));
    start.setOffsetFromUtc(offset);
    QString startDate = start.toString(Qt::ISODate);
    QString endDate = now.toString(Qt::ISODate);

    HttpRequest request;
    request.path = "v1/accounts/";
    request.path.append(account.account);
    request.path.append("/orders?startTime=");
    request.path.append(startDate);
    request.path.append("&endTime=");
    request.path.append(endDate);

    if (httpRequest(request) != true) return false;

    QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());
    if(doc.isObject()) {

        QJsonObject obj = doc.object();
        QJsonArray array = obj.value("orders").toArray();

        foreach (QJsonValue value, array) {

            QJsonObject o = value.toObject();

            int order_number            = o.value("id").toInt();

            qDebug() << order_number;

            Order order = OrderModel::select_one(order_number);

            order.questrade_id          = order_number;
            order.exchange_order_id     = o.value("exchangeOrderId").toString();
            order.account_number        = account.account;
            order.symbol                = o.value("symbol").toString();
            order.symbol_id             = o.value("symbolId").toInt();
            order.side                  = o.value("side").toString();
            order.source                = o.value("source").toString();
            order.state                 = o.value("state").toString();
            order.client_reason         = o.value("clientReasonStr").toString();
            order.strategy_type         = o.value("strategyType").toString();
            order.order_type            = o.value("orderType").toString();
            order.time_in_force         = o.value("timeInForce").toString();
            order.total_quantity        = o.value("totalQuantity").toInt();
            order.open_quantity         = o.value("openQuantity").toInt();
            order.filled_quantity       = o.value("filledQuantity").toInt();
            order.canceled_quantity     = o.value("canceledQuantity").toInt();
            order.limit_price           = o.value("limitPrice").toDouble();
            order.stop_price            = o.value("stopPrice").toDouble();
            order.avg_exec_price        = o.value("avgExecPrice").toDouble();
            order.last_exec_price       = o.value("lastExecPrice").toDouble();
            order.commission_charged    = o.value("commissionCharged").toDouble();
            order.is_allornone          = o.value("isAllOrNone").toBool();
            order.is_anonymous          = o.value("isAnonymous").toBool();
            order.primary_route         = o.value("primaryRoute").toString();
            order.secondary_route       = o.value("secondaryRoute").toString();
            order.order_route           = o.value("orderRoute").toString();
            order.questrade_created     = o.value("creationTime").toString();
            order.questrade_updated     = o.value("updateTime").toString();

            OrderModel::save_actual(order);
        }
        return true;
    }
    return false;
}


/**
 * Establish a valid access token
 */
bool QuestradeApi::current_access_token() {

    account = AccountModel::select_data_account();

    if (account.requires_key != true) return true;

    // use current token if not expired

    QDateTime now(QDateTime::currentDateTime());

    if ((now.secsTo(account.access_expires) > 60) && (account.access_token.size() > 0)) return true;

    // access token expired, check if the refresh token is expired

    if ((now.secsTo(account.refresh_expires) > 0) && (account.refresh_token.size() > 0)) {

        // refresh token valid, consume it for a new access token

        QString url = auth_server + account.refresh_token;
        if (httpGet(url) != true) return false;

        QJsonDocument doc = QJsonDocument::fromJson(lastResponse.toUtf8());

        if(doc.isObject()) {

            QJsonObject obj = doc.object();

            account.api_server = obj.value("api_server").toString();
            account.access_token = obj.value("access_token").toString();
            account.access_type = obj.value("token_type").toString();
            int expires_in = obj.value("expires_in").toInt();
            account.access_expires = now.addSecs(expires_in);
            account.refresh_token = obj.value("refresh_token").toString();
            account.refresh_expires = now.addDays(7);

            return AccountModel::save(account);
        }
    }

    // refresh token expired, nothing else to try
    qDebug() << "Error with access token";
    return false;
}


/**
 * HTTP simple GET
 */
bool QuestradeApi::httpGet(QString url) {

    lastResponse = "";
    lastError = "";

    qInfo() << url;

    bool result = false;
    QUrl qurl(url);
    QNetworkRequest request(qurl);
    QNetworkReply *reply = network->get(request);
    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {

        lastResponse = reply->readAll();
        result = true;

    } else {

        lastError = reply->errorString();
        qDebug() << "httpGet error : " << lastError;
        result = false;
    }
    reply->deleteLater();
    return result;
}


/**
 * HTTP authorized GET
 */
bool QuestradeApi::httpRequest(HttpRequest data) {

    lastResponse = "";
    lastError = "";

    if (current_access_token() != true) return false;

    bool result = false;

    QString url;
    url.append(account.api_server);
    url.append(data.path);

    QString params;
    QMapIterator<QString, QVariant> p(data.params);

    while (p.hasNext()) {

        p.next();
        if (params.size() > 0) params += "&";
        params.append(p.key());
        params.append("=");
        params.append(p.value().toString());
    }
    if (params.size() > 0) {

        url.append("?");
        url.append(params);
    }

    QUrl qurl(url);
    QNetworkRequest request(qurl);
    request.setRawHeader("Authorization", QByteArray().append(account.access_type+" "+account.access_token));

    qInfo() << qPrintable(url);

    QNetworkReply *reply = nullptr;

    if (data.method == "GET") {

        reply = network->get(request);

    } else if (data.method == "POST") {

        request.setRawHeader("Content-Type", "application/json");
        reply = network->post(request, data.json.toUtf8() );
    }

    eventLoop.exec();

    if (reply->error() == QNetworkReply::NoError) {

        lastResponse = reply->readAll();
        result = true;

    } else {

        lastError = reply->errorString();
        qCritical() << "httpRequest error :" << qPrintable(lastError);
        result = false;
    }
    reply->deleteLater();
    return result;
}
