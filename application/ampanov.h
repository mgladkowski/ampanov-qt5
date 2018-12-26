#ifndef AMPANOV_H
#define AMPANOV_H

#include <QObject>
#include <QCoreApplication>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include "control.h"
#include "helpers.h"

class Ampanov : public QObject {

    Q_OBJECT


private:

    QTimer*         loopTimer;
    const int       loopInterval = 1000;
    bool            isRunning;
    bool            isPaused;

    bool            isLive;
    bool            isReplay;
    QString         replaySymbol;
    int             replayStrategy;

    QTime           time_start;
    QTime           time_open_ext;
    QTime           time_open;
    QTime           time_close;
    QTime           time_close_ext;
    QTime           time_stop;


public:

    explicit Ampanov(QObject *parent = nullptr);
    ~Ampanov();

    void            dev();
    void            recalc( QString symbol );
    void            replay( QString symbol, int strategy_id );

    bool            isHealthy();
    bool            start();
    bool            stop();
    bool            pause();
    bool            resume();

    void            tx_done();
    void            tx_fail();


private slots:

    void            think();

};

#endif // AMPANOV_H
