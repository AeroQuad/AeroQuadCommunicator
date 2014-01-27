#ifndef POSITIONSIMULATOR_H
#define POSITIONSIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <marble/GeoDataCoordinates.h>

#define DEG2RAD 0.0174532925
#define KNOT2MPS 0.514444444

class PositionSimulator : public QObject
{
    Q_OBJECT
public:
    explicit PositionSimulator(QObject *parent = 0);
    ~PositionSimulator();
    void initialize(Marble::GeoDataCoordinates startPositionDeg, double headingDeg, double speedKnots, int intervalMsec);
    
signals:
    void position(Marble::GeoDataCoordinates);
    
public slots:
    Marble::GeoDataCoordinates updatePosition();
    void updateHeading(double headingDeg);
    void updateSpeed(double speedKnots);

private:
    Marble::GeoDataCoordinates currentPosition;
    QTimer *timer;
    long double brng; // bearing (radians)
    long double d;    // distance travelled (meters)
    long double R;    // radius of Earth (meters)
    long double alt;  // altitude (meters)
    long double metersPerSec;
};

#endif // POSITIONSIMULATOR_H
