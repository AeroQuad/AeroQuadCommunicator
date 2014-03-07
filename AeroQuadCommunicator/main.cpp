/*! \mainpage Developer's Documentation
 *
 * \section installation_sec Installation
 * 1. Download the source code from GitHub: https://github.com/AeroQuad/AeroQuadCommunicator.git
 * 2. Install Qt (Source originally compiled with v5.2): http://qt-project.org/downloads
 *  * Select the Qt version which includes MinGW
 *  * The instructions below will assume you have a basic knowledge of Qt
 *  * Check out this tutorial if you are new to Qt: http://qt-project.org/doc/qtcreator-3.0/creator-writing-program.html
 * 3. Open the AeroQuadCommunicator.pro project file.
 * 4. Qt will ask you to configure your project.
 *  * Deselect the Debug checkbox
 *  * Select Browse for the Release folder, create a new folder called "Release", then choose the Select Folder button
 * @image html CreateReleaseFolder.png
 *  * Select Configure Project
 * 5. Copy the following files into the new "Release" folder you just created
 *  * These files are found under ./AeroQuadCommunicator/AeroQuadCommunicator
 *   * panel_config.xml
 *   * panel_plot.xml
 *   * panel_route.xml
 *   * position.png
 *   * InitialRoute.xml
 *   * dfu-util.exe
 *   * AeroQuad32Production.bin
 *   * prod_image_256k.bin
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
 * \section addPanel_sec How to Add New Panels
 * 1. Use Qt Designer to create a new widget
 *  1. In the Projects pane, right click on the AeroQuadCommunicator project and select "Add new.."
 *  2. In the Add New dialog box, select Qt->Qt Designer Form Class and press "Choose.."
 *  3. Under templates/forms select widget and press "Next"
 *  4. Enter a name for the class you wish to create.  The naming convention is to call it PanelName (replace Name with the unique name you want to call your class).
 *  5. The name of the .cpp, .h and .ui file will be auto generated as panelname.x.  For our naming convention, place an underscore between panel and the name (panel_name.cpp).
 *  6. For the path, create a new subfolder under the panel folder and select "Next"
 *  7. The new files will now appear under Headers, Sources and Forms in the Projects pane.
 * 2. Update the newly created .h and .cpp files with required class properties and methods.  You can also look at panel_example.h and panel_example.cpp for an example in code.
 *  1. Insert the following code into the new .h file:
 * \code
 * #include <QMap>
 * public:
 *     bool connectState;
 *     QMap<QString, QString> configuration;
 *     void sendMessage(QString message) {emit messageOut(message.toUtf8());}
 *
 * public slots:
 *     void initialize(QMap<QString, QString> config);
 *     void updateConnectionState(bool state) {connectState = state;}
 *     void parseMessage(QByteArray);
 *
 * signals:
 *     void initializePanel(QMap<QString, QString>);
 *     void messageIn(QByteArray);
 *     void messageOut(QByteArray);
 *     void panelStatus(QString);
 *     void connectionState(bool);
 *     void getConnectionState();
 * \endcode
 *  2. Insert the following code into the new .cpp's constructor:
 * \code
 * connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
 * connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
 * connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));
 * \endcode
 *  3. Insert the following code at the bottom of the panel's .cpp file:
 * \code
 * void ReplaceWithPanelClassName::initialize(QMap<QString, QString> config)
 * {
 *     configuration = config;
 *     emit getConnectionState();
 * }
 *
 * void ReplaceWithPanelClassName::parseMessage(QByteArray data)
 * {
 *     QString incomingMessage = data;
 *
 *     // Add code below to do something with incomingMessage.
 *     ui->exampleResponse->setText(incomingMessage); // this is an example line
 * }
 * \endcode
 *
 * 3. Update communicator.cpp to use new widget
 *  1. Include your custom panel's header file at the top of communicator.cpp
 *  2. Update Communicator::initToolBar(), this adds a new menu icon in the tool bar
 *   1. To add a graphical icon, you will need to add the graphics to the resources file: communicator.qrc
 *   2. Edit communicator.ui by adding a new Action in the Action Editor
 *    1. Define the Object Name, use this to add a new connect() statement in Communicator::initToolBar()
 *    2. When defining signalMapper->setMapping(), choose a string to refer to your new panel
 *  3. Update Communicator::loadPanel()
 *   1. Use the string you defined from step 3B to call your new panel:
 * \code
 * else if (panelName == "Test")
 *   panel = new PanelExample;
 * \endcode
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
