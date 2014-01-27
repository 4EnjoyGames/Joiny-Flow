TEMPLATE = app
CONFIG += console
CONFIG -= qt

INCLUDEPATH += ../Classes
SOURCES += main.cpp \
    FGenerator.cpp \
    TwoShuffle.cpp \
    Joiny.cpp

HEADERS += \
    FGenerator.h \
    FlowPoint.h \
    Joiny.h

