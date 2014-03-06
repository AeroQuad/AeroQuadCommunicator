/*! \mainpage Developer's Documentation
 *
 * \section installation_sec Installation
 * 1. Download the source code from GitHub: https://github.com/Carancho/AeroQuadCommunicator.git
 * 2. Install Qt (Source originally compiled with v5.2): http://qt-project.org/downloads
 *  * Select the Qt version which includes MinGW
 * 3. Open the AeroQuadCommunicator.pro project file.
 * 4. Qt will ask you to configure your project.
 *  * Deselect the Debug checkbox
 *  * Select Browse for the Release folder and create a new folder called "Release"
 *  * Select Configure Project
 * 5. Copy the following files into the new "Release" folder you just created
 *  * These files are found under ./AeroQuadCommunicator/AeroQuadCommunicator
 *   * panel_config.xml
 *   * panel_plot.xml
 *   * panel_route.xml
 *   * position.png
 *   * InitialRoute.xml
 *  * Extract the data.zip file found under ./AeroQuadCommunicator/marble into ./AeroQuadCommunicator/Release/data
 *  * Create another folder in ./AeroQuadCommunicator/Release/Release and place libmarblewidget.dll inside
 *   * libmarblewidget.dll is found inside ./AeroQuadCommunicator/marble
 * 6. Click the Run button to build and run the Communicator
 *
 * \section requirements_sec Software Requirements
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
 * \subsection architecture High Level Architecture Diagram
 * This describes at a high level how each of the main components of the Communicator interact with each other.
 * @image html HighLevelArchitecture.png
 *
 * \subsection panelClass Panel Class Diagram
 * These are the main slots and signals of a Panel class.
 * @image html PanelArchitecture.png
 *
 * \subsection communicationClass Communication Class Diagram
 * These are the main slots and signals of the Communication class
 * @image html CommunicationArchitecture.png
 *
 * \subsection communicationSequence Communication Sequence Diagram
 * Insert block diagram here.
 *
 * \section addPanel_sec How to Add New Panels
 * 1. Use Qt Designer to create a new widget
 *  1. In the Projects pane, right click on the AeroQuadCommunicator project and select "Add new.."
 *  2. In the Add New dialog box, select Qt->Qt Designer Form Class and press "Choose.."
 *  3. Under templates/forms select widget and press "Next"
 *  4. Enter a name for the class you wish to create.  The naming convention is to call it PanelName (replace Name with the unique name you want to call your class).
 *  5. The name of the .cpp, .h and .ui file will be auto generated as panelname.x.  For our naming convention, place an underscore between panel and the name (panel_name.cpp).
 *  6. For the path, create a new subfolder under the panel folder and select "Next"
 *  7. The new files will now appear under Headers, Sources and Forms in the Projects pane.
 * 2. Update the newly created .h and .cpp files with required class properties and methods
 *  1. Add the following to the header file:
 *   1. Addition 1
 *   2. Addition 2
 *  2. Add the following to the source file:
 *   1. Addition 1
 *   2. Addition 2
 * 3. Update communicator.cpp to use new widget
 *  1. Add header file
 *  2. Update Communicator::initToolBar()
 *  3. Update Communicator::loadPanel()
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
