#include "route.h"
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

using namespace Marble;

Route::Route(MarbleModel *dataModel, GeoDataDocument *geoDocument)
{
    mapModel = dataModel;
    display = geoDocument;
    path = new GeoDataLineString(Marble::Tessellate);
    routeManager = mapModel->routingManager();
    routeRequest = routeManager->routeRequest();
    connect(routeRequest, SIGNAL(positionChanged(int,GeoDataCoordinates)), this, SLOT(updatePaths()));
}

void Route::loadRoute()
{
    QSettings settings;
    QString routePath = settings.value("lastRouteFile", "InitialRoute.xml").toString();
    QFile checkFile(routePath);
    if (!checkFile.exists())
        routePath = "InitialRoute.xml";
    loadRoute(routePath);
}

void Route::loadRoute(QString routePath)
{
    QFile routeFile(routePath);
    if (routeFile.open(QIODevice::ReadOnly))
    {
        QXmlStreamReader xml(routeFile.readAll());
        routeFile.close();
        route.clear();
        while(!xml.atEnd())
        {
            xml.readNext();
            if (xml.isStartElement())
            {
                if (xml.name() == "waypoint")
                {
                    waypointData waypoint;
                    foreach(const QXmlStreamAttribute &attribute, xml.attributes())
                    {
                        if (attribute.name().toString() == "lat")
                            waypoint.coord.setLatitude(attribute.value().toFloat(),GeoDataCoordinates::Degree);
                        if (attribute.name().toString() == "lon")
                            waypoint.coord.setLongitude(attribute.value().toFloat(),GeoDataCoordinates::Degree);
                        if (attribute.name().toString() == "alt")
                            waypoint.coord.setAltitude(attribute.value().toFloat());
                        if (attribute.name().toString() == "speed")
                            waypoint.speed = attribute.value().toFloat();
                        if (attribute.name().toString() == "type")
                            waypoint.type = attribute.value().toString();
                    }
                    route.append(waypoint);
                }
            }
        }
        // Define waypoints on map
        routeRequest->clear();
        path->clear();
        foreach(waypointData waypoint, route)
        {
            routeRequest->append(waypoint.coord);
            path->append(waypoint.coord);
        }
    }
}

void Route::saveRoute(QString routePath)
{
    QFile xmlFile(routePath);
    if (xmlFile.open(QIODevice::WriteOnly))
    {
        QXmlStreamWriter xml(&xmlFile);
        xml.setAutoFormatting(true);
        xml.writeStartElement("route");
        foreach(waypointData waypoint, route)
        {
            xml.writeStartElement("waypoint");
            xml.writeAttribute(("lat"), QString(QString::number(waypoint.coord.latitude(GeoDataCoordinates::Degree), 'f', 6)));
            xml.writeAttribute("lon", QString(QString::number(waypoint.coord.longitude(GeoDataCoordinates::Degree), 'f', 6)));
            xml.writeAttribute("alt", QString(QString::number(waypoint.coord.altitude(), 'f', 1)));
            xml.writeAttribute("speed", QString(QString::number(waypoint.speed, 'f', 1)));
            xml.writeAttribute("type", waypoint.type);
            xml.writeEndElement();
        }
        xml.writeEndElement();
    }
}

void Route::initializeRoute()
{
    // Define path between each waypoint
    GeoDataPlacemark *placemarkRoute = new GeoDataPlacemark;
    placemarkRoute->setGeometry(path);
    GeoDataStyle *styleRoute = new GeoDataStyle;
    GeoDataLineStyle lineStyle(QColor("white"));
    lineStyle.setWidth(8);
    styleRoute->setLineStyle(lineStyle);
    placemarkRoute->setStyle(styleRoute);

    // Display complete route on map
    display->append(placemarkRoute);
}

GeoDataCoordinates Route::getWaypoint(int index)
{
    return route[index].coord;
}

int Route::getRouteSize()
{
    return route.size();
}

void Route::updatePaths()
{
    path->clear();
    for (int index=0; index<route.size(); index++)
    {
        path->append(route[index].coord);
    }
}

void Route::addWaypoint(qreal lon, qreal lat, qreal alt, qreal speed, GeoDataCoordinates::Unit units)
{
    GeoDataCoordinates newLocation(lon, lat, alt, units);
    routeRequest->append(newLocation);
    waypointData newWaypoint;
    newWaypoint.coord = newLocation;
    newWaypoint.speed = speed;
    newWaypoint.type = "Waypoint";
    route.append(newWaypoint);
    updatePaths();
}

void Route::removeLastWaypoint()
{
    if (route.size())
    {
        route.removeLast();
        routeRequest->remove(routeRequest->size()-1);
        updatePaths();
    }
}

void Route::clearWaypoints()
{
    route.clear();
    routeRequest->clear();
    updatePaths();
}

QVector<Route::waypointData> Route::getRoute()
{
    return route;
}
