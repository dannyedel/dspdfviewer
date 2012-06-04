#include <QtGui/QApplication>
#include "dspdfviewer.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    dspdfviewer foo;
    foo.show();
    return app.exec();
}
