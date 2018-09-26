TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += --pedantic -std=c++2a

DESTDIR = ../bin

SOURCES += \
        main.cpp

HEADERS += \
    Chapter2.hpp \
    Chapter6.hpp \
    Facilities.hpp \
    Chapter3.hpp \
    Chapter1.hpp \
    Chapter3_temp.hpp
