#include "panel_route.h"
#include "ui_panel_route.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QStringList>
#include <QDebug>
#include <QThread>
#include <marble/MarbleWidget.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/MarbleDirs.h>

// State names for parseIncomingMessage()
#define ONBOARD_QUERY_ROUTE         0
#define ONBOARD_REQUEST_WAYPOINT    1
#define POSITION_PARSE_DATA1        2
#define POSITION_PARSE_DATA2        3
#define POSITION_PARSE_SAT          4
#define UPLOAD_WAYPOINT_COUNT       5
#define UPLOAD_WAYPOINT             6
#define UPDATE_AUTOPILOT_STATE      7
#define UPDATE_HOME_POSITION        8
#define ERROR_REPORT                9
#define DEBUG_MSG                   10

#define OFF                 0
#define AUTO_NAVIGATION     1
#define POSITION_HOLD       2
#define RETURN_TO_HOME      3
#define SET_HOME_POSITION   4

PanelRoute::PanelRoute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelRoute)
{
    ui->setupUi(this);
    connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
    connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));

    readXML("panel_route.xml");
    QString execdir = QCoreApplication::applicationDirPath();
    QDir folder = QDir(execdir.append(QDir::separator()).append("data"));
    MarbleDirs::setMarbleDataPath(folder.absolutePath());
    QString dataPath = MarbleDirs::marbleDataPath();
    qDebug() << dataPath;
    //QMessageBox::critical(this, "Debug", dataPath, QMessageBox::Ok);

    ui->map->setMapThemeId(mapConfig);
    ui->map->setProjection(Marble::Spherical);
    //ui->map->setProjection(Marble::Mercator);

    float startLat = settings.value("startLat").toFloat();
    float startLon = settings.value("startLon").toFloat();
    float startZoom = settings.value("startZoom").toInt();
    if (startZoom == 0) startZoom = 1100;
    ui->map->centerOn(startLon, startLat, true);
    ui->map->setZoom(startZoom);
    ui->map->update();

    display = new GeoDataDocument;
    vehicle = new Vehicle("AQ", "position.png", display);
    vehicle->updateHeading(heading);
    trailEnable = false;
    vehicle->enableTrail(trailEnable);
    //ui->trail->setChecked(trailEnable);
    centerOnVehicle = true;

    route = new Route(ui->map->model(), display);
    route->loadRoute();
    route->initializeRoute(); // display vehicle first, so it's on top of route graphics
    vehicle->updatePosition(route->getWaypoint(0));
    ui->map->model()->treeModel()->addDocument(display);
    updateRouteTable();

    waypointEditor = false;
    waypointCursor.load(":/images/resources/waypointCursor.png");
    waypointCursor = waypointCursor.scaledToWidth(50);
    connect(ui->map, SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)), this, SLOT(createWaypoint(qreal,qreal,GeoDataCoordinates::Unit)));

    autoPilotState = false;
    updateIndicatorStatus(ui->autopilotState, autoPilotState, "Autopilot\nDisabled");
    positionHoldState = false;
    updateIndicatorStatus(ui->positionHoldState, positionHoldState, "Position Hold\nDisabled");
    rtmState = false;
    updateIndicatorStatus(ui->rtmState, rtmState, "Return\nTo Home\nDisabled");
    positionState = 0;

    timer = new QTimer(this);
    timer->stop();
    connect(timer, SIGNAL(timeout()), this, SLOT(requestPosition()));

    nextParseState = ONBOARD_QUERY_ROUTE;
}

PanelRoute::~PanelRoute()
{
    settings.setValue("startLat", ui->map->centerLatitude());
    settings.setValue("startLon", ui->map->centerLongitude());
    settings.setValue("startZoom", ui->map->zoom());
    settings.sync();
    timer->stop();
    delete timer;
    delete ui;
}

void PanelRoute::initialize(QMap<QString, QString> config)
{
    configuration = config;
    emit getConnectionState();
    readXML("panel_route.xml");
    ui->map->setMapThemeId(mapConfig);

    retryMessage = 0;

    // Check GPS State
    sendMessage("^2;");
    positionState = 3;
    nextParseState = POSITION_PARSE_SAT;
    timer->start(POSITION_UPDATE_RATE);
}

void PanelRoute::readXML(QString filename)
{
    // Open XML configuration file
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "AeroQuad Communicator", "Unable to find configuration file:\n" + QCoreApplication::applicationDirPath() + "/" +  filename, QMessageBox::Ok);
        file->close();
        return;
    }

    // Read XML file and populate data storage
    QXmlStreamReader xml(file);
    while (!xml.atEnd() && !xml.hasError())
    {
        xml.readNext();
        if (xml.isStartElement() && (xml.name() == "MapConfig"))
        {
            if(xml.name() == "MapConfig")
                mapConfig = xml.readElementText();
            if(xml.name() == "PositionImageWidth")
                imageWidth = xml.readElementText().toInt();
        }
    }
    if (xml.hasError())
        QMessageBox::warning(NULL,"Error",xml.errorString(),QMessageBox::Ok);
    file->close();
}

// *********************************************************************************************
// parseMessage() is setup as a state machine to parse either position or route messages
// It traverses from state to state based on the nextParseState variable.
// *********************************************************************************************
void PanelRoute::parseMessage(QByteArray data)
{
    QString incomingMessage = data;
    QString errorMessage;
    QThread wait;
//    qDebug() << "Message Type: " << nextParseState;
//    qDebug() << "New incoming message: " << incomingMessage << " Message size: " << data.size();

    switch(nextParseState)
    {
    // *********************************************************************
    // Checks to see if there is a route stored on the flight control board.
    // *********************************************************************
    case ONBOARD_QUERY_ROUTE:
        waypointCount = incomingMessage.toInt();
        if (waypointCount)
        {
            route->clearWaypoints();
            updateRouteTable();
            waypointIndex = 0;
            sendMessage("o0;");
            nextParseState = ONBOARD_REQUEST_WAYPOINT;
        }
        else // no route onboard flight control board, request position data
        {
            emit panelStatus("Route not detected on flight control board, loading last loaded route from file.");
            startPositionRequest();
        }
        break;
    // *********************************************************************
    // Loads route from board and verifies if waypoint message has correct
    // number of parameters.
    // *********************************************************************
    case ONBOARD_REQUEST_WAYPOINT:
        {
            QStringList waypoint = incomingMessage.split(',');
            // Verify waypoint message by checking number of parameters
            if (waypoint.size() == 4)
            {
                qreal lat = waypoint[1].toDouble()/1.0E7;
                qreal lon = waypoint[2].toDouble()/1.0E7;
                qreal alt = waypoint[3].toDouble();
                route->addWaypoint(lon, lat, alt, 0.0, GeoDataCoordinates::Degree);
                retryMessage = 0;

                waypointIndex++;
                if (waypointIndex < waypointCount) // Setup command for next waypoint request
                {
                    sendMessage("o" + QString::number(waypointIndex) + ";");
                    nextParseState = ONBOARD_REQUEST_WAYPOINT;
                }
                else // finished route retrieveal from flight control board, update route table
                {
                    updateRouteTable();
                    emit panelStatus("Loaded route from flight control board successfully!");
                    startPositionRequest();
                }
            }
            else // Incorrect number of parameters received, retry waypoint request
            {
                wait.msleep(200);
                retryMessage++;
                if (retryMessage<MAX_MSG_RETRY)
                {
                    sendMessage( "o" + QString::number(waypointIndex) + ";");
                    nextParseState = ONBOARD_REQUEST_WAYPOINT;
                }
                else
                {
                    errorMessage = "Unable to retrieve stored route from control board.";
                    nextParseState = ERROR_REPORT;
                }
            }
        }
        break;
    // *********************************************************************
    // Parse lat, lon and heading from periodic run of requestPosition()
    // *********************************************************************
    case POSITION_PARSE_DATA1:
        {
            QStringList positionList = incomingMessage.split(',');
            if ((positionList.size() == 3) && (gpsState > 1))
            {
                float lat = positionList.at(0).toFloat() / 1.0E7;
                float lon = positionList.at(1).toFloat() / 1.0E7;
                if ((lat == 0.0) && (lon == 0.0))
                    return;
                heading = positionList.at(2).toFloat() * 57.2957795;
                vehicle->updatePosition(GeoDataCoordinates(lon, lat, 1000, GeoDataCoordinates::Degree));
                vehicle->updateHeading(heading);
                refreshMap(GeoDataCoordinates(lon, lat, 1000, GeoDataCoordinates::Degree));
            }
        }
        break;
    // *********************************************************************
    // Parse altitude, course and speed from periodic run of requestPosition()
    // *********************************************************************
    case POSITION_PARSE_DATA2:
        {
            QStringList positionList = incomingMessage.split(',');
            positionList[2].chop(1);
            if (positionList.size() == 3)
            {
                //qDebug() << "Alt/Course/Speed: " << positionList[0] << positionList[1] << positionList [2];
            }
        }
        break;
    // *********************************************************************
    // Parse # sats, accuracy, GPS state from periodic run of requestPosition()
    // *********************************************************************
    case POSITION_PARSE_SAT:
        {
            QStringList positionList = incomingMessage.split(',');
            positionList[2].chop(1);
            if (positionList.size() == 3)
            {
                //qDebug() << "Sats/Accuracy/State: " << positionList[0] << positionList[1] << positionList [2];
                ui->sats->setText(positionList[0]);
                gpsState = positionList[2].toInt();
                if (gpsState == 0) ui->gpsState->setText("Detecting GPS");
                if (gpsState == 1) ui->gpsState->setText("Waiting GPS Fix");
                if (gpsState == 2) ui->gpsState->setText("2D GPS Fix");
                if (gpsState == 3) ui->gpsState->setText("3D GPS Fix");
                if (gpsState == 4) ui->gpsState->setText("3D Diff GPS Fix");
            };
        }
        break;
    // *********************************************************************
    // This state gets executed when on_upload_clicked() is selected
    // and starts the route upload process to the flight control board.
    // *********************************************************************
    case UPLOAD_WAYPOINT_COUNT:
        // Check if waypoint count uploaded correctly
        if (incomingMessage.toInt() == route->getRouteSize())
        {
            retryMessage = 0;
            waypointIndex = 0;
            uploadWaypoint(waypointIndex);
            sendMessage("o0;");
            nextParseState = UPLOAD_WAYPOINT;
        }
        else
        {
            retryMessage++;
            if (retryMessage < MAX_MSG_RETRY)
            {
                sendMessage("o-1;");
                nextParseState = UPLOAD_WAYPOINT_COUNT;
            }
            else
            {
                errorMessage = "Error uploading route, please try again";
                nextParseState = ERROR_REPORT;
            }
        }
        break;
    // *********************************************************************
    // This state verifies the current waypoint is successfully uploaded.
    // After verification, the next waypoint is uploaded until route finished.
    // *********************************************************************
    case UPLOAD_WAYPOINT:
        {
            // Verify current waypoint loaded correctly
            bool verifyWaypoint = false;
            double waypointTolerance;
            GeoDataCoordinates waypoint;
            QStringList waypointQuery = incomingMessage.split(',');
            if (waypointQuery.size() == 4) // check if correct number of parameters received
            {
                // There will be a little rounding error so calculate the difference for verification
                waypoint = route->getWaypoint(waypointIndex);
                waypointTolerance = fabs(waypoint.latitude(GeoDataCoordinates::Degree) - waypointQuery[1].toDouble()/1.0E7);
                qDebug() << "check lat index[" + QString::number(waypointIndex) +"]" << waypointTolerance;
                verifyWaypoint = waypointTolerance < WAYPOINT_ROUNDING_ERROR;
                waypointTolerance = fabs(waypoint.longitude(GeoDataCoordinates::Degree) - waypointQuery[2].toDouble()/1.0E7);
                qDebug() << "check lon index[" + QString::number(waypointIndex) +"]" << waypointTolerance;
                verifyWaypoint &= waypointTolerance < WAYPOINT_ROUNDING_ERROR;
                waypointTolerance = fabs(waypoint.altitude() - waypointQuery[3].toDouble()/1.0E7);
                qDebug() << "check alt index[" + QString::number(waypointIndex) +"]" << waypointTolerance;
                verifyWaypoint &= waypointTolerance < WAYPOINT_ROUNDING_ERROR;
            }
            char label = 'A';
            emit panelStatus("Uploading waypoint " + QString(char(label+waypointIndex)) + "...");


            // If current waypoint matches waypoint on flight control board, load next waypoint
            if (verifyWaypoint)
            {
                waypointIndex++;
                if (waypointIndex < route->getRouteSize())
                {
                    retryMessage = 0;
                    uploadWaypoint(waypointIndex);
                    sendMessage("o" + QString::number(waypointIndex) + ";");
                    nextParseState = UPLOAD_WAYPOINT;
                }
                else // Finished uploading route, request position data for map
                {
                    sendMessage("W"); // store route in EEPROM
                    QMessageBox::critical(this, "Route Upload", "Route uploaded successfully!", QMessageBox::Ok);
                    panelStatus("Waypoint upload successful.");
                    startPositionRequest();
                }
            }
            // Current waypoint values does not match value on flight control board, resend current waypoint
            else
            {
                retryMessage++;
                if (retryMessage<MAX_MSG_RETRY)
                {
                    waypoint = route->getWaypoint(waypointIndex);
                    QString command = "O" + QString::number(waypointIndex) + ";";
                    int coordinate = waypoint.latitude(GeoDataCoordinates::Degree)*1E7;
                    command += QString::number(coordinate) + ";";
                    coordinate = waypoint.longitude(GeoDataCoordinates::Degree)*1E7;
                    command += QString::number(coordinate) + ";";
                    command += QString::number(waypoint.altitude()) + ";";
                    sendMessage(command);
                    // Read back current waypoint from board
                    sendMessage("o" + QString::number(waypointIndex) + ";");
                }
                else
                {
                    errorMessage = "Unable to upload route.\nPlease try again.";
                    panelStatus("Waypoint upload failed.");
                    nextParseState = ERROR_REPORT;
                }
            }
        }
        break;
    case DEBUG_MSG:
        {
            data.chop(1);
            qDebug() << "\nmessage size: " << data.size();
            qDebug() << data.toHex();
//            QByteArray floatData = data.mid(4, 4);
//            QByteArray longData = parseData.mid(4, 4);
//            QDataStream convertFloat(data.mid(0, 4));
//            convertFloat.setByteOrder(QDataStream::LittleEndian);
//            float floatNumber;
//            convertFloat >> floatNumber;
//            floatNumber = *reinterpret_cast<const float*>(floatData.data());
//            qDebug() << "raw values: " << floatData << "converted float: " << floatNumber << "\n";
            startPositionRequest();
        }
        break;
    case UPDATE_AUTOPILOT_STATE:
        {
            QStringList state = incomingMessage.split(',');
            if (state.size() == 1)
            {
                autoPilotStateChanged = false;
                int type = incomingMessage.toInt();
                //qDebug() << "Autopilot state response: " << type;
                QString state;

                autoPilotState = false;
                positionHoldState = false;
                rtmState = false;

                if (type == AUTO_NAVIGATION)
                    autoPilotState = true;
                if (autoPilotState)
                    state = "Autopilot\nEnabled";
                else
                    state = "Autopilot\nDisabled";
                updateIndicatorStatus(ui->autopilotState, autoPilotState, state);

                if (type == POSITION_HOLD)
                    positionHoldState = true;
                if (positionHoldState)
                    state = "Position Hold\nEnabled";
                else
                    state = "Position Hold\nDisabled";
                updateIndicatorStatus(ui->positionHoldState, positionHoldState, state);

                if (type == RETURN_TO_HOME)
                    rtmState = true;
                if (rtmState)
                    state = "Return\nTo Home\nEnabled";
                else
                    state = "Return\nTo Home\nDisabled";
                updateIndicatorStatus(ui->rtmState, rtmState, state);
            }

            if (autoPilotStateChanged)
                sendMessage("<");
            else
                startPositionRequest();
        }
    }
    // *********************************************************************
    // This state displays a dialog box indicating an error and begins
    // requesting position reports for the map.
    // *********************************************************************
    if (nextParseState == ERROR_REPORT)
    {
         QMessageBox::critical(this, "Communicator", errorMessage, QMessageBox::Ok);
         startPositionRequest();
    }
}

void PanelRoute::startPositionRequest()
{
    positionState = 0;
    nextParseState = POSITION_PARSE_DATA1;
    timer->start(POSITION_UPDATE_RATE);
}

void PanelRoute::requestPosition()
{
    // This is designed so that every other position request is lat/lon/heading
    //qDebug() << "\n\nPosition request: " << positionState;
    if (positionState == 0)
    {
        sendMessage("^0;");
        nextParseState = POSITION_PARSE_DATA1;
    }
    if (positionState == 1)
    {
        sendMessage("^1;");
        nextParseState = POSITION_PARSE_DATA2;
    }
    if (positionState == 2)
    {
        sendMessage("^0;");
        nextParseState = POSITION_PARSE_DATA1;
    }
    if (positionState == 3)
    {
        sendMessage("^2;");
        nextParseState = POSITION_PARSE_SAT;
    }

    if (positionState < 3)
        positionState++;
    else
        positionState = 0;
}

void PanelRoute::stopPositionRequest()
{
    timer->stop();
}

void PanelRoute::createWaypoint(qreal lon, qreal lat, GeoDataCoordinates::Unit unit)
{
    if (waypointEditor)
    {
        // figure out how to add speed
        qreal alt;
        if (int routeSize = route->getRouteSize())
        {
            GeoDataCoordinates waypoint;
            waypoint = route->getWaypoint(routeSize-1);
            alt = waypoint.altitude();
        }
        else
            alt = 0.0;

        route->addWaypoint(lon, lat, alt, 0.0, unit);
        updateRouteTable();
    }
}

void PanelRoute::uploadWaypoint(int waypointIndex)
{
    GeoDataCoordinates waypoint = route->getWaypoint(waypointIndex);
    QString command = "O" + QString::number(waypointIndex) + ";";
    int coordinate = waypoint.latitude(GeoDataCoordinates::Degree)*1E7;
    command += QString::number(coordinate) + ";";
    coordinate = waypoint.longitude(GeoDataCoordinates::Degree)*1E7;
    command += QString::number(coordinate) + ";";
    command += QString::number(waypoint.altitude()) + ";";
    sendMessage(command);
    qDebug() << command;
}

void PanelRoute::updateRouteTable()
{
    QVector<Route::waypointData> routeValues(route->getRoute());
    char label = 'A';
    ui->routeList->setRowCount(routeValues.size());
    for (int index=0; index<routeValues.size(); index++)
    {
        QTableWidgetItem *ID = new QTableWidgetItem(QString(char(label+index)));
        ui->routeList->setItem(index, 0, ID);
        QTableWidgetItem *lat = new QTableWidgetItem(QString(QString::number(routeValues[index].coord.latitude(GeoDataCoordinates::Degree), 'f', 7)));
        ui->routeList->setItem(index, 1, lat);
        QTableWidgetItem *lon = new QTableWidgetItem(QString(QString::number(routeValues[index].coord.longitude(GeoDataCoordinates::Degree), 'f', 7)));
        ui->routeList->setItem(index, 2, lon);
        QTableWidgetItem *alt = new QTableWidgetItem(QString(QString::number(routeValues[index].coord.altitude(), 'f', 1)));
        ui->routeList->setItem(index, 3, alt);
        QTableWidgetItem *speed = new QTableWidgetItem(QString(QString::number(routeValues[index].speed, 'f', 1)));
        ui->routeList->setItem(index, 4, speed);
        QTableWidgetItem *type = new QTableWidgetItem(QString(routeValues[index].type));
        ui->routeList->setItem(index, 5, type);
    }
    ui->routeList->resizeColumnsToContents();
    ui->routeList->scrollToBottom();
    ui->map->update();
}

void PanelRoute::refreshMap(Marble::GeoDataCoordinates position)
{
    if (centerOnVehicle)
        ui->map->centerOn(position, true);
    ui->map->update();
}

// **********************************************************************
// Panel button slots
// **********************************************************************

void PanelRoute::on_upload_clicked() // Starts uploading route to flight control board
{
    if (route->getRouteSize())
    {
        retryMessage = 0;
        stopPositionRequest();

        // Load waypoint count first
        sendMessage("O-1;" + QString::number(route->getRouteSize()) +";");
        sendMessage("o-1;");
        waypointIndex = -1;
        nextParseState = UPLOAD_WAYPOINT_COUNT;
    }
}

void PanelRoute::on_removeLast_clicked()
{
    route->removeLastWaypoint();
    updateRouteTable();
}

void PanelRoute::on_clear_clicked()
{
    QMessageBox::StandardButton response;
    response = QMessageBox::question(this, "Clear Route", "Are you sure you want to clear the current route?");
    if (response == QMessageBox::Yes)
    {
        route->clearWaypoints();
        updateRouteTable();
    }
}

void PanelRoute::on_editWaypoints_clicked()
{
    if (waypointEditor)
    {
        waypointEditor = false;
        QApplication::restoreOverrideCursor();
        emit panelStatus("Waypoint edit mode is off.");
    }
    else
    {
        waypointEditor = true;
        QApplication::setOverrideCursor(QCursor(waypointCursor));
        emit panelStatus("Waypoint edit mode is on.");
    }
}

void PanelRoute::on_load_clicked()
{
    QString routePath = QFileDialog::getOpenFileName(0, "Load Route", ".", "Route (*.xml)");
    if (!routePath.isNull())
    {
        route->loadRoute(routePath);
        route->updatePaths();
        updateRouteTable();
        settings.setValue("lastRouteFile", routePath);
        ui->map->centerOn(route->getWaypoint(0), true);
    }
}

void PanelRoute::on_save_clicked()
{
    QString routePath = QFileDialog::getSaveFileName(0, "Save Route", ".", "Route (*.xml)");
    if (!routePath.isNull())
    {
        route->saveRoute(routePath);
        settings.setValue("lastRouteFile", routePath);
    }
}

void PanelRoute::updateIndicatorStatus(QLabel *indicator, bool state, QString status)
{
    if (state)
        indicator->setStyleSheet("background-color: rgb(116, 232, 0);");
    else
        indicator->setStyleSheet("background-color:rgba(255, 0, 0, 100);");
    indicator->setText(status);
}

void PanelRoute::on_autopilot_clicked()
{
   stopPositionRequest();
   if (autoPilotState)
    {
        autoPilotState = false;
        sendMessage(">" + QString::number(OFF) + ";");
    }
    else
    {
        autoPilotState = true;
        sendMessage(">" + QString::number(AUTO_NAVIGATION) + ";");
    }
    sendMessage("<");
    autoPilotStateChanged = true;
    nextParseState = UPDATE_AUTOPILOT_STATE;
}

void PanelRoute::on_positionHold_clicked()
{
    stopPositionRequest();
    if (positionHoldState)
    {
        positionHoldState = false;
        sendMessage(">" + QString::number(OFF) + ";");
    }
    else
    {
        positionHoldState = true;
        sendMessage(">" + QString::number(POSITION_HOLD) + ";");
    }
    sendMessage("<");
    autoPilotStateChanged = true;
    nextParseState = UPDATE_AUTOPILOT_STATE;
}

void PanelRoute::on_returnToHome_clicked()
{
    stopPositionRequest();
    if (rtmState)
    {
        rtmState = false;
        sendMessage(">" + QString::number(OFF) + ";");
    }
    else
    {
        rtmState = true;
        sendMessage(">" + QString::number(RETURN_TO_HOME) + ";");
    }
    sendMessage("<");
    autoPilotStateChanged = true;
    nextParseState = UPDATE_AUTOPILOT_STATE;
}

void PanelRoute::on_setHome_clicked()
{
    QMessageBox::StandardButton response;
    response = QMessageBox::question(this, "Set Home Posiiton", "Are you sure you want to update the Return To Home position?");
    if (response == QMessageBox::Yes)
    {
        stopPositionRequest();
        sendMessage(">" + QString::number(SET_HOME_POSITION) + ";");
        sendMessage("^3;");
        nextParseState = UPDATE_HOME_POSITION;
    }
}

void PanelRoute::on_centerVehicle_clicked()
{
    if (centerOnVehicle)
    {
        centerOnVehicle = false;
        emit panelStatus("Center on vehicle is off.");
    }
    else
    {
        centerOnVehicle = true;
        emit panelStatus(("Center on vehicle is on."));
    }
}

void PanelRoute::on_recallRoute_clicked()
{
    stopPositionRequest();
    sendMessage("o-1;");
    nextParseState = ONBOARD_QUERY_ROUTE;
}
