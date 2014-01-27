#include <QApplication>
#include "communicator.h"
#include "utilities\patchedWebViewStyle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("AeroQuad");
    QCoreApplication::setOrganizationDomain("AeroQuad.com");
    QCoreApplication::setApplicationName("Communicator");
    Communicator w;
    // Patch Checkbox & Radio Button Bugs
    QApplication::setStyle( new PatchedWebViewStyle() );
    w.show();
    
    return a.exec();
}
