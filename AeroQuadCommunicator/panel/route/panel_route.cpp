#include "panel_route.h"
#include "ui_panel_route.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QStringList>
#include <QDebug>
#include <marble/MarbleWidget.h>
#include <marble/GeoDataTreeModel.h>
#include <marble/MarbleDirs.h>

PanelRoute::PanelRoute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelRoute)
{
    ui->setupUi(this);
//    QString testMessage = ("******\n");
//    testMessage += "Marble Data Path: " + Marble::MarbleDirs::marbleDataPath();
//    testMessage += "\nMarble System Path: " + Marble::MarbleDirs::systemPath();
//    testMessage += "\nMarble PlugIn Path: " + Marble::MarbleDirs::pluginSystemPath();
//    testMessage += "\nMarble Local Path: " + Marble::MarbleDirs::localPath();
//    testMessage += "\nMarble Plugin Local Path: " + Marble::MarbleDirs::pluginLocalPath();
//    testMessage += "\nMarble Plugin Path: " + Marble::MarbleDirs::marblePluginPath();
//    testMessage += "\n******\n";
//    QMessageBox::critical(this, "Stupid Message", testMessage, QMessageBox::Ok);

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
    vehicle = new Vehicle("AQ", "fighter.png", display);
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
    connect(ui->map, SIGNAL(mouseClickGeoPosition(qreal,qreal,GeoDataCoordinates::Unit)), this, SLOT(createWaypoint(qreal,qreal,GeoDataCoordinates::Unit)));

    ui->autopilotState->setStyleSheet("background-color: rgba(255, 0, 0, 100);");
    ui->autopilotState->setText("Autopilot\nNot Enabled");
    autoPilotState = false;

    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parsePositionData(QByteArray)));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(requestPosition()));
    timer->start(500);

    for (int index=0; index<CHANNEL_COUNT; index++)
    {
        control[index] = 1500;
    }
    sendCommand(AUX1, 1000);
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
            {
                mapConfig = xml.readElementText();
            }
        }
    }
    if (xml.hasError())
    {
        QMessageBox::warning(NULL,"Error",xml.errorString(),QMessageBox::Ok);
    }
    file->close();
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
        QTableWidgetItem *lat = new QTableWidgetItem(QString(QString::number(routeValues[index].coord.latitude(GeoDataCoordinates::Degree), 'f', 6)));
        ui->routeList->setItem(index, 1, lat);
        QTableWidgetItem *lon = new QTableWidgetItem(QString(QString::number(routeValues[index].coord.longitude(GeoDataCoordinates::Degree), 'f', 6)));
        ui->routeList->setItem(index, 2, lon);
        QTableWidgetItem *alt = new QTableWidgetItem(QString(QString::number(routeValues[index].coord.altitude(), 'f', 1)));
        ui->routeList->setItem(index, 3, alt);
        QTableWidgetItem *speed = new QTableWidgetItem(QString(QString::number(routeValues[index].speed, 'f', 1)));
        ui->routeList->setItem(index, 4, speed);
        QTableWidgetItem *type = new QTableWidgetItem(QString(routeValues[index].type));
        ui->routeList->setItem(index, 5, type);
    }
    ui->routeList->resizeColumnToContents(0);
    ui->routeList->scrollToBottom();
    ui->map->update();
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
        QApplication::setOverrideCursor(Qt::CrossCursor);
    }
}


void PanelRoute::createWaypoint(qreal lon, qreal lat, GeoDataCoordinates::Unit unit)
{
    if (waypointEditor)
    {
        route->addWaypoint(lon, lat, unit);
        updateRouteTable();
    }
}

void PanelRoute::on_autopilot_clicked()
{
    if (autoPilotState)
    {
        autoPilotState = false;
        ui->autopilotState->setStyleSheet("background-color:rgba(255, 0, 0, 100);");
        ui->autopilotState->setText("Autopilot\nNot Enabled");
        sendCommand(AUX1, 1000);
    }
    else
    {
        autoPilotState = true;
        ui->autopilotState->setStyleSheet("background-color: rgb(116, 232, 0);");
        ui->autopilotState->setText("Autopilot\nEnabled");
        sendCommand(AUX1, 2000);
    }
}

void PanelRoute::on_left_clicked()
{
    ui->rudder->setValue(ui->rudder->value() - 100);
    sendCommand(ROLL, ui->rudder->value());
}

void PanelRoute::on_center_clicked()
{
    ui->rudder->setValue(1500);
    sendCommand(ROLL, ui->rudder->value());
}

void PanelRoute::on_right_clicked()
{
    ui->rudder->setValue(ui->rudder->value() + 100);
    sendCommand(ROLL, ui->rudder->value());
}

void PanelRoute::on_throttleOff_clicked()
{
    ui->throttle->setValue(1500);
    sendCommand(THROTTLE, 1500);
}

void PanelRoute::on_throttleCruise_clicked()
{
    ui->throttle->setValue(1650);
    sendCommand(THROTTLE, 1650);
}

void PanelRoute::on_throttleMax_clicked()
{
    ui->throttle->setValue(2000);
    sendCommand(THROTTLE, 2000);
}

void PanelRoute::parsePositionData(QByteArray data)
{
    QString message = data;
    positionMessage.append(data);
    if (message.contains("\r\n"))
    {
        QString positionString(positionMessage);
        QStringList positionList = positionString.split(',');
        float lat = positionList.at(0).toFloat() / 1.0E7;
        float lon = positionList.at(1).toFloat() / 1.0E7;
        if ((lat == 0.0) && (lon == 0.0))
        {
            positionMessage.clear();
            return;
        }
        heading = positionList.at(2).toFloat() * 57.2957795;
        float speed = positionList.at(3).toFloat();
        vehicle->updatePosition(GeoDataCoordinates(lon, lat, 1000, GeoDataCoordinates::Degree));
        vehicle->updateHeading(heading);
        refreshMap(GeoDataCoordinates(lon, lat, 1000, GeoDataCoordinates::Degree));
        positionMessage.clear();
    }
}

void PanelRoute::requestPosition()
{
    QString command = "*";
    emit messageOut(command.toUtf8());
}

void PanelRoute::refreshMap(Marble::GeoDataCoordinates position)
{
    if (centerOnVehicle)
        ui->map->centerOn(position, true);
    ui->map->update();
}

void PanelRoute::sendCommand(int channel, int commandValue)
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

void PanelRoute::on_throttle_valueChanged(int value)
{
    sendCommand(THROTTLE, value);
}

void PanelRoute::on_rudder_valueChanged(int value)
{
    sendCommand(ROLL, value);
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
