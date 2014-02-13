/*! \mainpage Developer's Documentation
 *
 * \section requirements_sec Requirements
 * Communicator Requirements:
 * 1. The Communicator shall display flight board configuration.
 * 2. The Communicator shall modify and store user configurable options.
 * 3. The Communicator shall plot telemetry values as streamed from serial commununication.
 * 4. The Communicator shall upload firmware to the AQ board.
 * 5. The Communicator shall provide a Serial Monitor for troubleshooting.
 * 6. The Communicator shall command motors individually for checkout.
 * 7. The Communicator shall provide a means to calibrate onboard sensors, transmitter stick positions and ESC's.
 * 8. The Communicator shall provide an autopilot configuration tool.
 *
 * \section design_sec Design
 * * Show design diagram
 * *
 *
 * \section addPanel_sec How to Add New Panels
 * 1. Use Qt Designer to create a new widget
 * 2. Update communicator.cpp to use new widget
 *  1. Add header file
 *  2. Update Communicator::initToolBar()
 *  3. Update Communicator::loadPanel()
 *
 */

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
