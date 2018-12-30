#ifndef CHART_H
#define CHART_H

#include "core/database.h"

class Chart {
public:

    int         rowid;
    int         symbol_id;
    QString     symbol_interval;
    int         candle_id;
    QString     event;
};
typedef QList<Chart> Charts;

class ChartModel : public Database {

public:

    static Chart        select_one( int symbol_id, QString interval, int candle_id );
    static bool         insert(Chart data);
    static bool         save(Chart data);
    static bool         truncate();

private:

    static Charts       fetchAll(QSqlQuery &query);
    static Chart        fetch(QSqlQuery &query);

};

#endif // CHART_H
