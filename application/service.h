#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>
#include <QTimer>
#include "ampanov.h"

class Service : public QObject {

    Q_OBJECT

private:

    Ampanov* ampanov;

    const char * APP_DESCRIPTION  = "Ark building chaos management robot";
    const char * APP_FAIL         = "Ampanov service failed to start";
    const char * APP_START        = "Ampanov service started";
    const char * APP_STOP         = "Ampanov service stopped";
    const char * APP_PAUSE        = "Ampanov service paused";
    const char * APP_RESUME       = "Ampanov service resumed";

public:

    explicit Service(QObject *parent = nullptr);

    void start();
    void stop();
    void pause();
    void resume();

signals:

    void finished();
};

#endif // SERVICE_H
