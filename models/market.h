#ifndef MARKETMODEL_H
#define MARKETMODEL_H

#include "core/database.h"

struct Market {

    int         rowid   = 0;
    QString     name    = "";
    QDateTime   extendedStartTime;
    QDateTime   startTime;
    QDateTime   endTime;
    QDateTime   extendedEndTime;
    QString     defaultTradingVenue = "";
};
typedef QList<Market> Markets;


class MarketModel : public Database {

public:

    static Market          select_one(QString name);
    static bool            insert(Market data);
    static bool            save(Market data);

private:

    static Markets         fetchAll(QSqlQuery &query);
    static Market          fetch(QSqlQuery &query);
};

#endif // MARKETMODEL_H
