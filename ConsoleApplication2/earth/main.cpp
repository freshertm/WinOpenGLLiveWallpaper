#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGLFormat format;
    format.setSampleBuffers(true);
    format.setSamples(4);
    Widget w(format);
    w.show();
    
    return a.exec();
}
