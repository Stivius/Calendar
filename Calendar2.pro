#-------------------------------------------------
#
# Project created by QtCreator 2016-07-21T13:25:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql printsupport

TARGET = Calendar2
TEMPLATE = app

QMAKE_LFLAGS_RELEASE += -static -static-libgcc
include(C:/Users/maxim/Desktop/qtxlsx/src/xlsx/qtxlsx.pri)


SOURCES += main.cpp\
        mainwindow.cpp \
    event.cpp \
    settings.cpp \
    export.cpp \
    model.cpp \
    database.cpp \
    filter.cpp \
    import.cpp

HEADERS  += mainwindow.h \
    event.h \
    settings.h \
    export.h \
    model.h \
    database.h \
    filter.h \
    import.h

FORMS    += mainwindow.ui \
    event.ui \
    settings.ui \
    export.ui \
    import.ui
