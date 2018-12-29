#include <QCoreApplication>
#include "application/service.h"

int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);

    Service* service = new Service();

    QObject::connect(service, SIGNAL(finished()), qApp, SLOT(quit()), Qt::QueuedConnection);

    service->start();

    return a.exec();
}
