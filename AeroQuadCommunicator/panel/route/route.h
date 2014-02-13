#ifndef ROUTE_H
#define ROUTE_H

#include <marble/GeoDataCoordinates.h>
#include <marble/GeoDataDocument.h>
#include <marble/MarbleModel.h>
#include <marble/GeoDataStyle.h>
#include <marble/GeoDataLineStyle.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/GeoPainter.h>
#include <marble/RoutingManager.h>
#include <marble/RouteRequest.h>
#include <marble/MarbleWidget.h>
#include <marble/GeoPainter.h>
#include <marble/GeoDataLineString.h>
#include <QObject>
#include <QList>

class Route : public QObject
{
    Q_OBJECT

public:
    struct waypointData
    {
        Marble::GeoDataCoordinates coord;
        float speed;
        QString type;
    };
    Route(Marble::MarbleModel *dataModel, Marble::GeoDataDocument *geoDocument);
    void loadRoute();
    void loadRoute(QString routePath);
    void saveRoute(QString routePath);
    void initializeRoute();
    Marble::GeoDataCoordinates getWaypoint(int index);
    int getRouteSize();
    void addWaypoint(qreal lon, qreal lat, qreal alt, qreal speed, Marble::GeoDataCoordinates::Unit units);
    void removeLastWaypoint();
    void clearWaypoints();
    QVector<waypointData> getRoute();

public slots:
    void updatePaths();

private:  
    Marble::MarbleModel *mapModel;
    Marble::GeoDataDocument *display;
    Marble::RoutingManager *routeManager;
    Marble::RouteRequest *routeRequest;
    Marble::GeoPainter *viewRoute;
    Marble::GeoDataLineString *path;
    QVector<waypointData> route;

};

#endif // ROUTE_H
