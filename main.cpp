#include <QtGui/QApplication>
#include <QDebug>
#include "dspdfviewer.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    if ( app.arguments().size() < 2 )
    {
      /// FIXME Error message goes here
      return 1;
    }
    
    
    DSPDFViewer foo(app.arguments().at(1));
    
    return app.exec();
}
