#ifndef VEHICLE_H
#define VEHICLE_H

#include <marble/GeoDataCoordinates.h>
#include <marble/GeoDataStyle.h>
#include <marble/GeoDataIconStyle.h>
#include <marble/GeoDataPlacemark.h>
#include <marble/GeoDataDocument.h>
#include <QObject>
#include <QDir>
#include <QImage>
#include <QTransform>
#include <QDebug>

class Vehicle : public QObject
{
    Q_OBJECT
public:
    explicit Vehicle(QString vehicleName, QString filename, Marble::GeoDataDocument *geoDocument);
    
signals:
    
public slots:
    void updatePosition(Marble::GeoDataCoordinates position);
    void updateHeading(double heading);
    void enableTrail(bool state, int length = 10000);
    
private:
    Marble::GeoDataPlacemark *vehicle;
    Marble::GeoDataStyle *styleVehicle;
    Marble::GeoDataPlacemark *trail;
    Marble::GeoDataLineString *trailData;
    int trailLength;
    Marble::GeoDataDocument *display;
    QImage imageVehicle;
    bool trailState;
};

#endif // VEHICLE_H
