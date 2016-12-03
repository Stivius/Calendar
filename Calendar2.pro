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
include(qtxlsx/src/xlsx/qtxlsx.pri)


SOURCES += main.cpp\
    event.cpp \
    settings.cpp \
    export.cpp \
    filter.cpp \
    import.cpp \
    eventssqlmodel.cpp \
    eventsproxymodel.cpp \
    settingssqlmodel.cpp \
    eventsmainwindow.cpp

HEADERS  += \
    event.h \
    settings.h \
    export.h \
    filter.h \
    import.h \
    eventssqlmodel.h \
    eventsproxymodel.h \
    settingssqlmodel.h \
    eventsmainwindow.h

FORMS    += \
    event.ui \
    settings.ui \
    export.ui \
    import.ui \
    eventsmainwindow.ui
