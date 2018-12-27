#include "zeromq.h"


bool ZeroMq::push(QByteArray data) {

    bool result = false;

    std::string text = data.constData();
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUSH);

    socket.connect("tcp://localhost:7777");

    zmq::message_t request( text.size() );
    memcpy ( static_cast<void*>(request.data()), text.c_str(), text.size() );

    result = socket.send(request);
    QThread::msleep(20);
    return result;
}


bool ZeroMq::push( QJsonObject data ) {

    QJsonDocument doc(data);
    QByteArray bytes = doc.toJson( QJsonDocument::JsonFormat::Compact );
    bool result = ZeroMq::push(bytes);
    return result;
}


bool ZeroMq::test() {

    QJsonObject data;
    data["type"] = "test";
    data["time"] = QTime::currentTime().toString();
    data["data"] = "";

    bool result = ZeroMq::push(data);
    return result;
}
