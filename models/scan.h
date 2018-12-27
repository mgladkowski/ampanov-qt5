#ifndef SCAN_H
#define SCAN_H

#include "core/database.h"

class Scan {
public:

    int         rowid               = 0;
    int         symbol_id           = 0;
    QString     symbol_interval     = "";
    QString     status              = "A";
    QDateTime   created_date;
    QString     created_by          = "";
    int         candle_row_id       = 0;
    int         candle_pattern_id   = 0;
    int         candle_eval_code    = 0;
    QString     candle_eval_text    = "";
    int         chart_pattern_id    = 0;
    int         chart_eval_code     = 0;
    QString     chart_eval_text     = "";
};
typedef QList<Scan> Scans;


class ScanModel : public Database {
public:

    static Scan         select_active(int symbol_id, QString interval, int candle_id);

    static bool         insert( Scan data, bool is_live );
    static bool         update( Scan data, bool is_live );
    static bool         save( Scan data, bool is_live );

private:

    static Scans        fetchAll( QSqlQuery query );
    static Scan         fetch( QSqlQuery &query );

};

#endif // SCAN_H
