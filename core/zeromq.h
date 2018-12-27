#ifndef ZEROMQ_H
#define ZEROMQ_H

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QTime>
#include "zmq.hpp"

class ZeroMq {

public:

    static bool push( QByteArray data );
    static bool push( QJsonObject data );
    static bool test();
};

#endif // ZEROMQ_H
