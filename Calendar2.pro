#-------------------------------------------------
#
# Project created by QtCreator 2016-07-21T13:25:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql network

TARGET = Calendar2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    event.cpp \
    settings.cpp \
    export.cpp \
    model.cpp \
    database.cpp \
    filter.cpp

HEADERS  += mainwindow.h \
    event.h \
    settings.h \
    export.h \
    model.h \
    database.h \
    filter.h

FORMS    += mainwindow.ui \
    event.ui \
    settings.ui \
    export.ui
