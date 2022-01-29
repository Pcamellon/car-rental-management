#include <QtGui/QApplication>
#include "inicial.h"
#include "ui_inicial.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Inicial w;
    w.show();

    return a.exec();
}
