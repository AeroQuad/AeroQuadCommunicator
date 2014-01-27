// This simulates position reported in radians based on start point, bearing and speed
// Equations were taken from: http://www.movable-type.co.uk/scripts/latlong.html

#include "positionsimulator.h"

using namespace Marble;

PositionSimulator::PositionSimulator(QObject *parent) :
    QObject(parent)
{
    R = 6378100 ; // meters
    alt = 12192; // meters or 40,000 feet
    currentPosition.setLatitude(33.9425, GeoDataCoordinates::Degree);   // coordianates of LAX
    currentPosition.setLongitude(118.4081, GeoDataCoordinates::Degree);
    currentPosition.setAltitude(alt);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePosition()));
}

void PositionSimulator::initialize(GeoDataCoordinates startPositionDeg, double headingDeg, double speedKnots, int intervalMsec)
{
    currentPosition.setLatitude(startPositionDeg.latitude());
    currentPosition.setLongitude(startPositionDeg.longitude());
    brng = headingDeg * DEG2RAD; // convert to radians
    // Calculate distance based on units of speed and update interval
    metersPerSec = speedKnots * KNOT2MPS; // convert to meters/sec
    d = metersPerSec * (long double)(intervalMsec/1000.0); // calclate meters travelled during interval
    timer->start(intervalMsec);
}

GeoDataCoordinates PositionSimulator::updatePosition()
{
    long double lat1 = currentPosition.latitude(GeoDataCoordinates::Radian);
    long double lon1 = currentPosition.longitude(GeoDataCoordinates::Radian);
    long double lat2 = asin(sin(lat1)*cos(d/R) + cos(lat1)*sin(d/R)*cos(brng));
    long double lon2 = lon1 + atan2(sin(brng)*sin(d/R)*cos(lat1), cos(d/R)-sin(lat1)*sin(lat2));

    currentPosition.setLatitude(lat2, GeoDataCoordinates::Radian);
    currentPosition.setLongitude(lon2, GeoDataCoordinates::Radian);
    emit position(currentPosition);
    return currentPosition;
}

void PositionSimulator::updateHeading(double heading)
{
    timer->stop();
    initialize(currentPosition, heading, metersPerSec/KNOT2MPS, timer->interval());
}

void PositionSimulator::updateSpeed(double speedKnots)
{
    timer->stop();
    initialize(currentPosition, brng/DEG2RAD, speedKnots, timer->interval());
}

PositionSimulator::~PositionSimulator()
{
    timer->stop();
    delete timer;
}
