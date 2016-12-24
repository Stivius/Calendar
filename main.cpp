#include "view/eventsmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EventsMainWindow w;
    w.showMaximized();

    return a.exec();
}
