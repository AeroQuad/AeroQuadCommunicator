#ifndef PANEL_ROUTE_H
#define PANEL_ROUTE_H

#include <QMainWindow>
#include <QWidget>
#include <QSettings>
#include <QTimer>
#include "panel/route/vehicle.h"
#include "panel/route/route.h"
#include <marble/GeoDataDocument.h>
#include <marble/GeoDataCoordinates.h>

#define CHANNEL_COUNT 9
#define ROLL          1
#define PITCH         0
#define THROTTLE      2
#define RUDDER        3
#define GEAR          4
#define AUX1          5
#define AUX2          6
#define AUX3          7

using namespace Marble;

namespace Ui {
class PanelRoute;
}

class PanelRoute : public QWidget
{
    Q_OBJECT

public:
    explicit PanelRoute(QWidget *parent = 0);
    ~PanelRoute();

signals:
    void messageIn(QByteArray);
    void messageOut(QByteArray);
    void panelStatus(QString);

private slots:
    void on_removeLast_clicked();
    void on_clear_clicked();
    void on_editWaypoints_clicked(); 
    void createWaypoint(qreal lon, qreal lat, GeoDataCoordinates::Unit unit);
    void on_autopilot_clicked();
    void on_left_clicked();
    void on_center_clicked();
    void on_right_clicked();
    void on_throttleOff_clicked();
    void on_throttleCruise_clicked();
    void on_throttleMax_clicked();
    void parsePositionData(QByteArray);
    void requestPosition();
    void on_throttle_valueChanged(int value);
    void on_rudder_valueChanged(int value);
    //void on_rudder_sliderMoved(int position);

    void on_load_clicked();

    void on_save_clicked();

private:
    Ui::PanelRoute *ui;
    QSettings settings;
    QString mapConfig;
    Vehicle *vehicle;
    Route *route;
    float heading;
    GeoDataDocument *display;
    QTimer *timer;
    QByteArray positionMessage;
    //PositionSimulator *positionSim;
    bool trailEnable;
    bool centerOnVehicle;
    bool waypointEditor;
    bool autoPilotState;
    int control[CHANNEL_COUNT];

    void initialize(QString filename);
    void createVehicle();
    void updateRouteTable();
    void refreshMap(GeoDataCoordinates position);
    void sendCommand(int channel, int commandValue);
};

#endif // PANEL_ROUTE_H
