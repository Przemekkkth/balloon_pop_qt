#include <QApplication>
#include "view.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    QCursor cursor(Qt::BlankCursor);
    QApplication::setOverrideCursor(cursor);
    View v;
    v.setWindowTitle("Balloons Popping Qt/C++");
    v.show();
    a.exec();
}
