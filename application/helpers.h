#ifndef HELPERS_H
#define HELPERS_H

#include <QString>
#include <QDateTime>
#include <QTimeZone>

class Helpers {

public:

    static QString         atom_date();
    static QString         iso_date();
    static QString         iso_datetime();
    static QDateTime       atom_to_datetime(QString value);
    static bool            isWeekday();

};

#endif // HELPERS_H
