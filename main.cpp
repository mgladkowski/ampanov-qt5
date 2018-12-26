#include <QCoreApplication>
#include "application/service.h"

int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);

    Service* service = new Service();
    service->start();

    QObject::connect(service, SIGNAL(finished()), qApp, SLOT(quit()), Qt::QueuedConnection);

    return a.exec();
}
