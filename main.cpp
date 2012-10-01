#include <QtGui/QApplication>
#include "math_practice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setWindowIcon(QIcon(":/pi.svg"));
    math_practice w;
    w.show();

    return a.exec();
}
