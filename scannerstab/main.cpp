#include <QApplication>
#include "scannerstab.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    scannerstab scanner;
    scanner.show();
    return app.exec();
}
