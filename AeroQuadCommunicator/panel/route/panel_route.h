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
#define ROLL          2
#define PITCH         1
#define THROTTLE      0
#define RUDDER        3
#define GEAR          4
#define AUX1          5
#define AUX2          6
#define AUX3          7

#define POSITION_UPDATE_RATE 500
#define MAX_ROUTE_RETRY 5
#define WAYPOINT_ROUNDING_ERROR 0.100000

#define POSITION 0
#define ROUTE 1
#define CHECK_ONBOARD_ROUTE 2
#define LOAD_ONBOARD_ROUTE 3

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
    void parseIncomingMessage(QByteArray);
    void requestPosition();
    void on_throttle_valueChanged(int value);
    void on_rudder_valueChanged(int value);
    //void on_rudder_sliderMoved(int position);
    void on_load_clicked();
    void on_save_clicked();
    void on_upload_clicked();

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
    //QVector<QString> routeVerify;
    //PositionSimulator *positionSim;
    bool trailEnable;
    bool centerOnVehicle;
    bool waypointEditor;
    bool autoPilotState;
    int control[CHANNEL_COUNT];
    int incomingMessageType;
    int waypointIndex;
    int waypointRetry;
    int waypointCount;

    void initialize(QString filename);
    void createVehicle();
    void updateRouteTable();
    void refreshMap(GeoDataCoordinates position);
    void sendCommand(int channel, int commandValue);
};

#endif // PANEL_ROUTE_H
