#include "clienttcpwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientTcpWindow w;
    w.show();
    
    return a.exec();
}
