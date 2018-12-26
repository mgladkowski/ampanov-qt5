#include "helpers.h"


QString Helpers::atom_date() {

    QDateTime now = QDateTime::currentDateTime();
    now.setOffsetFromUtc(now.offsetFromUtc());
    return now.toString(Qt::ISODate);
}


QString Helpers::iso_date() {

    QDate now(QDate::currentDate());
    return now.toString("yyyy-MM-dd");
}


QString Helpers::iso_datetime() {

    QDateTime now(QDateTime::currentDateTime());
    return now.toString("yyyy-MM-dd hh:mm:ss");
}


QDateTime Helpers::atom_to_datetime(QString value) {

    QString parsed;
    parsed = value.left(19);

    int timeZoneOffset;
    timeZoneOffset = value.right(6).left(3).toInt() * 3600;
    QDateTime result;
    result = QDateTime::fromString(parsed,"yyyy-MM-ddTHH:mm:ss");
    result.setTimeSpec(Qt::OffsetFromUTC);
    result.setUtcOffset(timeZoneOffset);
    return result;
}


bool Helpers::isWeekday() {

    QDateTime now = QDateTime::currentDateTime();

    if (now.date().dayOfWeek() >= 1 && now.date().dayOfWeek() <= 5) return true;

    return false;
}
