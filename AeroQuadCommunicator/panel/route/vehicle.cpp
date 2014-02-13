#include "vehicle.h"

using namespace Marble;

Vehicle::Vehicle(QString vehicleName, QString filename, Marble::GeoDataDocument *geoDocument)
{
    vehicle = new GeoDataPlacemark(vehicleName);
    styleVehicle = new GeoDataStyle;
    GeoDataIconStyle iconVehicle;

    imageVehicle.load(QDir::currentPath() + "/" + filename);
    iconVehicle.setIcon(imageVehicle);
    vehicle->setStyle(styleVehicle);
    styleVehicle->setIconStyle(iconVehicle);
    display = geoDocument;
    display->append(vehicle);

    trail = new GeoDataPlacemark;
    trailData = new GeoDataLineString(Marble::Tessellate);
    trail->setGeometry(trailData);
    GeoDataStyle *styleTrail = new GeoDataStyle;
    GeoDataLineStyle lineStyle(QColor("blue"));
    lineStyle.setWidth(5);
    styleTrail->setLineStyle(lineStyle);
    trail->setStyle(styleTrail);
    enableTrail(true);
    display->append(trail);
}

void Vehicle::updatePosition(Marble::GeoDataCoordinates position)
{
    vehicle->setCoordinate(position);
    if (trailState)
    {
        trailData->append(position);
        if (trailData->size() > trailLength)
            trailData->remove(0);
    }
}

void Vehicle::updateHeading(double heading)
{
    QTransform rotateVehicle;
    rotateVehicle.rotate(heading);
    QImage rotatedVehicle(imageVehicle.transformed(rotateVehicle));
    GeoDataIconStyle iconVehicle;
    iconVehicle.setIcon(rotatedVehicle);
    styleVehicle->setIconStyle(iconVehicle);
}

void Vehicle::enableTrail(bool state, int length)
{
    if (state)
    {
        trailState = true;
        trailLength = length;
    }
    else
    {
        trailState = false;
        trailData->clear();
    }
}
