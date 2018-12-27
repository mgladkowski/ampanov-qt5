#ifndef STRATEGY_H
#define STRATEGY_H

#include "core/database.h"

struct Strategy {

    int         rowid                   = 0;
    QString     status                  = "";
    QString     name                    = "";
    QString     direction               = "";
};
typedef QList<Strategy> Strategies;


class StrategyModel : public Database {

public:

    static Strategies   select_all_active();
    static Strategy     select_one_row(int rowid);

private:

    static Strategies   fetchAll(QSqlQuery query);
    static Strategy     fetch(QSqlQuery &query);

};

#endif // STRATEGY_H
