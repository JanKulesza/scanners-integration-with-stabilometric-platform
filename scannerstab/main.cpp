#include "scannerstab.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    scannerstab w;
    w.show();
    return a.exec();
}
