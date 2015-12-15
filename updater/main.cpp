#include <QCoreApplication>
#include "manageupdate.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ManageUpdate upd;
    return a.exec();
}
