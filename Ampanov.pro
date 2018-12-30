QT += core
QT += sql
QT += network
QT -= gui

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

TARGET = Ampanov
TEMPLATE = app

INCLUDEPATH += "C:\\Program Files\\ZeroMQ 4.0.4\\include"

LIBS += -L"C:\\Program Files\\MySQL\\MySQL Connector C 6.1\\lib" -llibmysql
LIBS += -L"C:\\Program Files\\ZeroMQ 4.0.4\\bin" -lzmq

SOURCES += \
    main.cpp \
    application/ampanov.cpp \
    application/service.cpp \
    application/control.cpp \
    application/helpers.cpp \
    models/engine.cpp \
    core/database.cpp \
    core/zeromq.cpp \
    broker/questrade.cpp \
    broker/simulator.cpp \
    models/account.cpp \
    models/candle.cpp \
    models/market.cpp \
    models/option.cpp \
    models/order.cpp \
    models/stock.cpp \
    models/strategy.cpp \
    models/trade.cpp \
    application/scanner.cpp \
    application/trader.cpp \
    models/candleset.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    application/ampanov.h \
    application/service.h \
    application/control.h \
    application/helpers.h \
    models/engine.h \
    core/database.h \
    core/enums.h \
    core/zeromq.h \
    core/zmq.hpp \
    broker/questrade.h \
    broker/simulator.h \
    models/account.h \
    models/candle.h \
    models/market.h \
    models/option.h \
    models/order.h \
    models/stock.h \
    models/strategy.h \
    models/trade.h \
    application/scanner.h \
    application/trader.h \
    models/candleset.h


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
