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

PanelRoute::PanelRoute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelRoute)
{
    ui->setupUi(this);
    initialize("panel_route.xml");
    ui->map->setProjection(Marble::Mercator);
    //ui->map->setProjection(Marble::Spherical);
    qDebug() << mapConfig;
    ui->map->setMapThemeId(mapConfig);

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
    //ui->center->setChecked(centerOnVehicle);

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

    ui->autopilotState->setStyleSheet("background-color: rgba(255, 0, 0, 100);");
    ui->autopilotState->setText("Autopilot\nNot Enabled");
    autoPilotState = false;

    timer = new QTimer(this);
    timer->stop();
    connect(timer, SIGNAL(timeout()), this, SLOT(requestPosition()));

    nextParseState = ONBOARD_QUERY_ROUTE;
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseIncomingMessage(QByteArray)));
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

void PanelRoute::initialize(QString filename)
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
// parseIncomingMessage() is setup as a state machine to parse either position or route messages
// It traverses from state to state based on the nextParseState variable.
// *********************************************************************************************
void PanelRoute::parseIncomingMessage(QByteArray data)
{
    QString incomingMessage = data;
    QString errorMessage;
    QThread wait;
    qDebug() << "Message Type: " << nextParseState;
    qDebug() << "New incoming message: " << incomingMessage << " Message size: " << data.size();

    if (incomingMessage == "initialize")
    {
        retryMessage = 0;

        // Setup default transmitter command states
        for (int index=0; index<CHANNEL_COUNT; index++)
            control[index] = 1500;
        sendTransmitterCommand(AUX1, 1000);

        // Request number of stored waypoints
        sendMessage("o-1;");
        nextParseState = ONBOARD_QUERY_ROUTE;
        return;
    }

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
            timer->start(POSITION_UPDATE_RATE);
            emit panelStatus("Route not detected on flight control board, loading last loaded route from file.");
            nextParseState = POSITION_PARSE_DATA;
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
                    timer->start(POSITION_UPDATE_RATE); // This starts periodic run of requestPosition()
                    emit panelStatus("Loaded route from flight control board successfully!");
                    nextParseState = POSITION_PARSE_DATA;
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
    // Parse position data from periodic run of requestPosition()
    // *********************************************************************
    case POSITION_PARSE_DATA:
        {
            QStringList positionList = incomingMessage.split(',');
            if (positionList.size() == 4)
            {
                float lat = positionList.at(0).toFloat() / 1.0E7;
                float lon = positionList.at(1).toFloat() / 1.0E7;
                if ((lat == 0.0) && (lon == 0.0))
                    return;
                heading = positionList.at(2).toFloat() * 57.2957795;
                // TODO: Look at how to implement speed
                // float speed = positionList.at(3).toFloat();
                vehicle->updatePosition(GeoDataCoordinates(lon, lat, 1000, GeoDataCoordinates::Degree));
                vehicle->updateHeading(heading);
                refreshMap(GeoDataCoordinates(lon, lat, 1000, GeoDataCoordinates::Degree));
                nextParseState = POSITION_PARSE_DATA;
            }
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
            errorMessage = "Error uploading route, please try again";
            nextParseState = ERROR_REPORT;
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
                    timer->start(POSITION_UPDATE_RATE);
                    nextParseState = POSITION_PARSE_DATA;
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
////            //QByteArray longData = parseData.mid(4, 4);
////            QDataStream convertFloat(data.mid(0, 4));
////            convertFloat.setByteOrder(QDataStream::LittleEndian);
//            float floatNumber;
////            convertFloat >> floatNumber;
//            floatNumber = *reinterpret_cast<const float*>(floatData.data());
//            qDebug() << "raw values: " << floatData << "converted float: " << floatNumber << "\n";
            timer->start(POSITION_UPDATE_RATE);
            nextParseState = POSITION_PARSE_DATA;
        }
        break;
    }
    // *********************************************************************
    // This state displays a dialog box indicating an error and begins
    // requesting position reports for the map.
    // *********************************************************************
    if (nextParseState == ERROR_REPORT)
    {
         QMessageBox::critical(this, "Communicator", errorMessage, QMessageBox::Ok);
         timer->start(POSITION_UPDATE_RATE);
         nextParseState = POSITION_PARSE_DATA;
    }
}

void PanelRoute::requestPosition()
{
    QString command = "*";
    emit messageOut(command.toUtf8());
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
    emit messageOut(command.toUtf8());
    qDebug() << command;
}

void PanelRoute::sendMessage(QString message)
{
    emit messageOut(message.toUtf8());
    qDebug() << message;
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

void PanelRoute::sendTransmitterCommand(int channel, int commandValue)
{
    QString command = ">";
    control[channel] = commandValue;
    for (int index=0; index<CHANNEL_COUNT; index++)
    {
        command += QString::number(control[index]) +";";
    }
    command += "\n";
    emit messageOut(command.toUtf8());
    qDebug() << command;
}

// **********************************************************************
// Panel button slots
// **********************************************************************

void PanelRoute::on_upload_clicked() // Starts uploading route to flight control board
{
    if (route->getRouteSize())
    {
        retryMessage = 0;
        timer->stop();

        // Load waypoint count first
        QString command = "O-1;" + QString::number(route->getRouteSize()) +";";
        emit messageOut(command.toUtf8());
        command = "o-1;";
        emit messageOut(command.toUtf8());
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
    }
    else
    {
        waypointEditor = true;
        QApplication::setOverrideCursor(QCursor(waypointCursor));
        //QApplication::setOverrideCursor(Qt::CrossCursor);
        //this->setCursor(QCursor(waypointCursor));
    }
}

void PanelRoute::on_autopilot_clicked()
{
    if (autoPilotState)
    {
        autoPilotState = false;
        ui->autopilotState->setStyleSheet("background-color:rgba(255, 0, 0, 100);");
        ui->autopilotState->setText("Autopilot\nNot Enabled");
        sendTransmitterCommand(AUX1, 1000);
    }
    else
    {
        autoPilotState = true;
        ui->autopilotState->setStyleSheet("background-color: rgb(116, 232, 0);");
        ui->autopilotState->setText("Autopilot\nEnabled");
        sendTransmitterCommand(AUX1, 2000);
    }
}

void PanelRoute::on_left_clicked()
{
    ui->rudder->setValue(ui->rudder->value() - 100);
    sendTransmitterCommand(ROLL, ui->rudder->value());
}

void PanelRoute::on_center_clicked()
{
    ui->rudder->setValue(1500);
    sendTransmitterCommand(ROLL, ui->rudder->value());

    //DEBUG ONLY
//    timer->stop();
//    sendMessage("=");
//    nextParseState = DEBUG_MSG;
}

void PanelRoute::on_right_clicked()
{
    ui->rudder->setValue(ui->rudder->value() + 100);
    sendTransmitterCommand(ROLL, ui->rudder->value());
}

void PanelRoute::on_throttleOff_clicked()
{
    ui->throttle->setValue(1500);
    sendTransmitterCommand(PITCH, 1500);
}

void PanelRoute::on_throttleCruise_clicked()
{
    ui->throttle->setValue(1650);
    sendTransmitterCommand(PITCH, 1650);
}

void PanelRoute::on_throttleMax_clicked()
{
    ui->throttle->setValue(2000);
    sendTransmitterCommand(PITCH, 2000);
}

void PanelRoute::on_throttle_valueChanged(int value)
{
    sendTransmitterCommand(PITCH, value);
}

void PanelRoute::on_rudder_valueChanged(int value)
{
    sendTransmitterCommand(ROLL, value);
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
