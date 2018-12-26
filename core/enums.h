#ifndef ENUMS_H
#define ENUMS_H

#include <QString>
#include <QTime>

class TaskState {

public:

    const static int Never      = 0;
    const static int Started    = 1;
    const static int Fail       = 3;
    const static int Success    = 7;

    static QString text(int state) {

        switch (state) {
            case 0: return "Never";
            case 1: return "Start";
            case 3: return "Fail";
            case 7: return "Ok";
            default: return "";
        }
    }
};

struct Task {

    int         state = 0;
    QTime       time;
    QString     data = "";
};

#endif // ENUMS_H
