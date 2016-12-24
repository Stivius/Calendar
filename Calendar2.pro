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
    model/eventsproxymodel.cpp \
    model/eventssqlmodel.cpp \
    model/settingssqlmodel.cpp \
    view/event.cpp \
    view/eventsmainwindow.cpp \
    view/export.cpp \
    view/import.cpp \
    view/settings.cpp

HEADERS  += \
    model/eventsproxymodel.h \
    model/eventssqlmodel.h \
    model/settingssqlmodel.h \
    view/event.h \
    view/eventsmainwindow.h \
    view/export.h \
    view/import.h \
    view/settings.h

FORMS    += \
    ui/event.ui \
    ui/eventsmainwindow.ui \
    ui/export.ui \
    ui/import.ui \
    ui/settings.ui
