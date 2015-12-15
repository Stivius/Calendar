#-------------------------------------------------
#
# Project created by QtCreator 2015-07-04T13:18:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql printsupport

TARGET = Calendar
TEMPLATE = app

include(QtXlsx/src/xlsx/qtxlsx.pri)

SOURCES += main.cpp \
    maininterface.cpp \
    newevent.cpp \
    eventsmodel.cpp

HEADERS  += \
    maininterface.h \
    newevent.h \
    eventsmodel.h
