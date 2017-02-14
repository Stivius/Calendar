#-------------------------------------------------
#
# Project created by QtCreator 2016-07-21T13:25:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql printsupport xlsx

TARGET = Calendar
TEMPLATE = app

#include(qtxlsx/qtxlsx.pri)

RC_ICONS = favicon.ico

DESTDIR = $${OUT_PWD}/build/bin

#QMAKE_LFLAGS_RELEASE += -static -static-libgcc
QMAKE_LFLAGS += "-Wl,-rpath,'\$$ORIGIN/../lib'"

QT_LIBS.path = $${OUT_PWD}/build/lib

linux-g++ {
    QT_LIBS.files += $$PWD/libqt/*.so.*
}

QT_PLUGINS.path = $${OUT_PWD}/build

linux-g++ {
    QT_PLUGINS.files += $$PWD/libqt/plugins
}

QT_CONF.path = $${OUT_PWD}/build/bin
QT_CONF.files += $$PWD/libqt/qt.conf

INSTALLS += QT_LIBS \
            QT_PLUGINS \
            QT_CONF


TRANSLATIONS += translations/Calendar_en.ts \
    translations/Calendar_ua.ts

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
    controller/importcontroller.cpp \
    model/translationmodel.cpp

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
    controller/importcontroller.h \
    model/translationmodel.h

FORMS    += \
    ui/eventsmainwindow.ui \
    ui/eventview.ui \
    ui/exportview.ui \
    ui/settingsview.ui \
    ui/importview.ui

DISTFILES +=

RESOURCES += \
    resources.qrc
