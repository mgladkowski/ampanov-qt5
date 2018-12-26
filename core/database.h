#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QDebug>

class Database {

public:

    static QSqlDatabase     database();
    static bool             test();
};

#endif // DATABASE_H
