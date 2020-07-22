TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        thumbnail.cpp

LIBS += -lavformat -lavcodec -lswscale -lavutil

HEADERS += \
    thumbnail.h
