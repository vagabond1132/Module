#include "appupdate.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Appupdate w;
    w.show();

    return a.exec();
}
