#include "route.h"
#include <QDebug>

using namespace Marble;

Route::Route(MarbleModel *dataModel, GeoDataDocument *geoDocument)
{
    mapModel = dataModel;
    display = geoDocument;
}

void Route::loadRoute()
{
    // Change this so we load route externally
    route.append(GeoDataCoordinates(-121.6152403200,36.4526629061, 1000.0, Marble::GeoDataCoordinates::Degree));
    route.append(GeoDataCoordinates(-118.2301233342,36.4068540534, 1000.0, Marble::GeoDataCoordinates::Degree));
    route.append(GeoDataCoordinates(-118.2008367506,33.9709742993, 1000.0, Marble::GeoDataCoordinates::Degree));
    route.append(GeoDataCoordinates(-117.9707431962,31.6205917851, 10000.0, Marble::GeoDataCoordinates::Degree));
}

void Route::displayRoute()
{
    // Define waypoints on map
    routeManager = mapModel->routingManager();
    routeRequest = routeManager->routeRequest();
    foreach(GeoDataCoordinates waypoint, route)
    {
        routeRequest->append(waypoint);
    }
    connect(routeRequest, SIGNAL(positionChanged(int,GeoDataCoordinates)), this, SLOT(updatePaths()));

    // Define path between each waypoint
    path = new GeoDataLineString(Marble::Tessellate);
    foreach(GeoDataCoordinates waypoint, route)
    {
        path->append(waypoint);
    }
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
    return route[index];
}

int Route::getRouteSize()
{
    return route.size();
}

void Route::updatePaths()
{
    path->clear();
    for (int index=0; index<routeRequest->size(); index++)
    {
        path->append(routeRequest->at(index));
    }
}

void Route::addWaypoint(qreal lon, qreal lat, Marble::GeoDataCoordinates::Unit unit)
{
    routeRequest->append(GeoDataCoordinates(lon, lat, 0, unit));
    updatePaths();
}

void Route::removeLastWaypoint()
{
    routeRequest->remove(routeRequest->size()-1);
    updatePaths();
}

void Route::clearWaypoints()
{
    routeRequest->clear();
    updatePaths();
}

QList<Marble::GeoDataCoordinates> Route::getRoute()
{
    QList<GeoDataCoordinates> routeList;
    for (int index=0; index<routeRequest->size(); index++)
    {
        routeList.append(routeRequest->at(index));
    }
    return routeList;
}
