#ifndef BRAIN_H
#define BRAIN_H

#include <QObject>

class Brain : public QObject {

    Q_OBJECT


public:

    explicit Brain(QObject *parent = nullptr);
    ~Brain();

    void process();


private:


signals:


public slots:


};

#endif // BRAIN_H
