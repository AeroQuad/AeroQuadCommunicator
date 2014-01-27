#ifndef ROUTE_H
#define ROUTE_H

#include <marble/GeoDataCoordinates.h>
#include <marble/GeoDataDocument.h>
#include <marble/MarbleModel.h>
#include <marble/GeoDataStyle.h>
#include <marble/GeoDataLineStyle.h>
#include <marble/GeoDataLineString.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/GeoPainter.h>
#include <marble/RoutingManager.h>
#include <marble/RouteRequest.h>
#include <QObject>
#include <QList>

class Route : public QObject
{
    Q_OBJECT

public:
    Route(Marble::MarbleModel *dataModel, Marble::GeoDataDocument *geoDocument);
    void loadRoute();
    void displayRoute();
    Marble::GeoDataCoordinates getWaypoint(int index);
    int getRouteSize();
    void addWaypoint(qreal lon, qreal lat, Marble::GeoDataCoordinates::Unit unit);
    void removeLastWaypoint();
    void clearWaypoints();
    QList<Marble::GeoDataCoordinates> getRoute();

public slots:
    void updatePaths();

private:
    Marble::MarbleModel *mapModel;
    Marble::GeoDataDocument *display;
    Marble::RoutingManager *routeManager;
    Marble::RouteRequest *routeRequest;
    Marble::GeoPainter *viewRoute;
    Marble::GeoDataLineString *path;
    QVector<Marble::GeoDataCoordinates> route;
};

#endif // ROUTE_H
