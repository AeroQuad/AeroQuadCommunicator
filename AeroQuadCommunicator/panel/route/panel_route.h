#ifndef PANEL_ROUTE_H
#define PANEL_ROUTE_H

#include <QMainWindow>
#include <QWidget>
#include <QSettings>
#include <QTimer>
#include <QPixmap>
#include <QLabel>
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

#define MAX_MSG_RETRY 30
#define POSITION_UPDATE_RATE 250
#define WAYPOINT_ROUNDING_ERROR 0.100000

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
    void parseIncomingMessage(QByteArray);
    void requestPosition();
    void on_load_clicked();
    void on_save_clicked();
    void on_upload_clicked();
    void on_positionHold_clicked();
    void on_returnToHome_clicked();
    void on_setHome_clicked();

private:
    Ui::PanelRoute *ui;
    QSettings settings;
    QString mapConfig;
    Vehicle *vehicle;
    Route *route;
    float heading;
    GeoDataDocument *display;
    QTimer *timer;
    bool trailEnable;
    bool centerOnVehicle;
    bool waypointEditor;
    bool autoPilotState;
    bool autoPilotStateChanged;
    bool positionHoldState;
    bool rtmState;
    int control[CHANNEL_COUNT];
    int waypointIndex;
    int waypointCount;
    int nextParseState;
    int retryMessage;
    float imageWidth;
    int positionState;
    QPixmap waypointCursor;

    void initialize(QString filename);
    void createVehicle();
    void updateRouteTable();
    void refreshMap(GeoDataCoordinates position);
    void sendMessage(QString message);
    void uploadWaypoint(int waypointIndex);
    void updateIndicatorStatus(QLabel *indicator, bool state, QString status);
    void startPositionRequest();
    void stopPositionRequest();
};

#endif // PANEL_ROUTE_H
