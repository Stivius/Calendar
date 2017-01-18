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

RC_ICONS = favicon.ico

SOURCES += main.cpp\
    model/eventsproxymodel.cpp \
    model/eventssqlmodel.cpp \
    model/settingssqlmodel.cpp \
    view/eventsmainwindow.cpp \
    view/eventview.cpp \
    controller/eventcontroller.cpp \
    controller/exportcontroller.cpp \
    view/exportview.cpp \
    controller/settingscontroller.cpp \
    view/settingsview.cpp \
    view/importview.cpp \
    controller/importcontroller.cpp

HEADERS  += \
    model/eventsproxymodel.h \
    model/eventssqlmodel.h \
    model/settingssqlmodel.h \
    view/eventsmainwindow.h \
    view/eventview.h \
    controller/eventcontroller.h \
    controller/exportcontroller.h \
    view/exportview.h \
    controller/settingscontroller.h \
    view/settingsview.h \
    view/importview.h \
    controller/importcontroller.h

FORMS    += \
    ui/eventsmainwindow.ui \
    ui/eventview.ui \
    ui/exportview.ui \
    ui/settingsview.ui \
    ui/importview.ui
