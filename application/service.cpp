#include "service.h"


Service::Service(QObject *parent) : QObject(parent) {

    ampanov = new Ampanov();
}


void Service::start() {

    const QStringList args = QCoreApplication::arguments();
    QString symbol = "";

    // -D : run Ampanov::dev() only
    // -C [-A][SYMBOL] : recalculate indicators
    // -H [-A][SYMBOL] : replay pattern analysis on entire history

    if (args.size() >= 2) {

        if (QRegExp("-C").indexIn(args.at(1)) != -1) {

            if (args.size() >= 3) symbol = args.at(2);
            ampanov->recalc(symbol);
            emit finished();
            return;

        } else if (QRegExp("-H").indexIn(args.at(1)) != -1) {

            if (args.size() >= 3) symbol = args.at(2);
            ampanov->replay(symbol, 1);
            emit finished();
            return;

        } else if (QRegExp("-D").indexIn(args.at(1)) != -1) {

            ampanov->dev();
            emit finished();
            return;
        }
    }

    if (ampanov->start() == true) {

        qInfo() << APP_START;

    } else {

        qCritical() << APP_FAIL;
        emit finished();
    }
}


void Service::stop() {

    ampanov->stop();
    delete ampanov;
    ampanov = nullptr;

    qInfo() << APP_STOP;
    emit finished();
}


void Service::pause() {

    ampanov->pause();

    qInfo() << APP_PAUSE;
}


void Service::resume() {

    ampanov->resume();

    qInfo() << APP_RESUME;
}
