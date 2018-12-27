#ifndef ACCOUNTMODEL_H
#define ACCOUNTMODEL_H

#include "core/database.h"


struct Account {

    int         rowid               = 0;
    QString     user                = "";
    QString     status              = "";
    QString     type                = "";
    QString     allow_ai            = "";
    QString     name                = "";
    QString     account             = "";
    QString     currency            = "";
    bool        is_data             = false;
    bool        requires_key        = false;
    QString     refresh_token       = "";
    QDateTime   refresh_expires;
    QString     access_token        = "";
    QString     access_type         = "";
    QDateTime   access_expires;
    QString     api_server          = "";
    float       risk_unit           = 0;
    float       risk_cap            = 0;
    bool        allow_stock         = false;
    bool        allow_option        = false;
    QString     prefer_trade        = "";
    QString     created_by          = "";
    QDateTime   created_date;
    QString     modified_by         = "";
    QDateTime   modified_date;

    float       cash                = 0;
    float       market_value        = 0;
    float       total_equity        = 0;
    float       buying_power        = 0;
    float       maintenance_excess  = 0;
    bool        is_real_time        = false;
    QDateTime   updated_date;

    QJsonObject toJsonObject() {
        QJsonObject data;
        data["cash"] = QString::number( cash, 'f', 2 );
        data["market_value"] = QString::number( market_value, 'f', 2 );
        data["total_equity"] = QString::number( total_equity, 'f', 2 );
        data["buying_power"] = QString::number( buying_power, 'f', 2 );
        data["maintenance_excess"] = QString::number( maintenance_excess, 'f', 2 );
        return data;
    }

};
typedef QList<Account> Accounts;


struct AccountPosition {

    int         rowid;
    QString     account;
    QString     symbol;
    int         symbolId;
    float       openQuantity;
    float       closedQuantity;
    float       currentMarketValue;
    float       currentPrice;
    float       averageEntryPrice;
    float       closedPnL;
    float       openPnL;
    float       totalCost;
    bool        isRealTime;
    bool        isUnderReorg;
};
typedef QList<AccountPosition> AccountPositions;


struct AccountBalance {

    QString     currency;
    float       cash;
    float       marketValue;
    float       totalEquity;
    float       buyingPower;
    float       maintenanceExcess;
    bool        isRealTime;
};


class AccountModel : public Database {

public:

    static Account     select_data_account();
    static Account     select_row(int rowid);
    static bool        save(Account data);
    static bool        save_auth(Account data);
    static bool        save_balance(Account data);

private:

    static Accounts    fetchAll(QSqlQuery query);
    static Account     fetch(QSqlQuery &query);
};

#endif // ACCOUNTMODEL_H
