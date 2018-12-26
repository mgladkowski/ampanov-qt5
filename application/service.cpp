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

    for (int i = 1; i < args.size(); ++i) {

        if (QRegExp("-C").indexIn(args.at(i)) != -1) {

            if (i+1 <= args.size()) symbol = args.at(i+1);
            ampanov->recalc(symbol);
            return;

        } else if (QRegExp("-H").indexIn(args.at(i)) != -1) {

            if (i+1 <= args.size()) symbol = args.at(i+1);
            ampanov->replay(symbol, 1);
            return;

        } else if (QRegExp("-D").indexIn(args.at(i)) != -1) {

            ampanov->dev();
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
