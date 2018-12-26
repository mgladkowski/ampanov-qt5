#include "database.h"


QSqlDatabase Database::database() {

    QSqlDatabase dbo;

    if(QSqlDatabase::contains("ampanov")) {

        dbo = QSqlDatabase::database("ampanov");

    } else {

        dbo = QSqlDatabase::addDatabase("QMYSQL", "ampanov");
        dbo.setHostName("localhost");
        dbo.setDatabaseName("ampanov");
        dbo.setUserName("root");
        dbo.setPassword("root");
    }

    if (dbo.isValid() != true) {

        qCritical() << "Database QMYSQL driver error";

    } else if (dbo.isOpen() != true) {

        dbo.open();

        qDebug() << "Database opened";

        if (dbo.isOpenError() == true) {

            qCritical() << "Database connection failed : " << dbo.lastError();
        }
    }
    return dbo;
}

bool Database::test() {

    bool result = false;

    QString sql = "SELECT rowid FROM engine LIMIT 1;";
    QSqlQuery q(Database::database());
    q.prepare(sql);
    if (q.exec()) {
        if (q.next()) {
            QSqlRecord record = q.record();
            QMap<QString, QVariant> map;
            for(int i=0; i < record.count(); ++i) {
                map.insert(record.fieldName(i), q.value(i));
            }
            int   rowid      = map["rowid"].toInt();
            result = (rowid > 0);
        }
    }
    q.finish();
    return result;
}
