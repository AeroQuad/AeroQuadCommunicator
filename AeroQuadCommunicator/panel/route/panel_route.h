#ifndef PANEL_ROUTE_H
#define PANEL_ROUTE_H

#include <QMainWindow>
#include <QWidget>
#include <QSettings>
#include <QTimer>
#include <QPixmap>
#include <QLabel>
#include <QMap>
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
    bool connectState;
    QMap<QString, QString> configuration;
    void sendMessage(QString message) {emit messageOut(message.toUtf8());}

public slots:
    void initialize(QMap<QString, QString> config);
    void updateConnectionState(bool state) {connectState = state;}
    void parseMessage(QByteArray);

signals:
    void initializePanel(QMap<QString, QString>);
    void messageIn(QByteArray);
    void messageOut(QByteArray);
    void panelStatus(QString);
    void connectionState(bool);
    void getConnectionState();

private slots:
    void on_removeLast_clicked();
    void on_clear_clicked();
    void on_editWaypoints_clicked(); 
    void createWaypoint(qreal lon, qreal lat, GeoDataCoordinates::Unit unit);
    void on_autopilot_clicked();
    void requestPosition();
    void on_load_clicked();
    void on_save_clicked();
    void on_upload_clicked();
    void on_positionHold_clicked();
    void on_returnToHome_clicked();
    void on_setHome_clicked();
    void on_centerVehicle_clicked();
    void on_recallRoute_clicked();

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
    int gpsState;
    QPixmap waypointCursor;

    void readXML(QString filename);
    void createVehicle();
    void updateRouteTable();
    void refreshMap(GeoDataCoordinates position);
    void uploadWaypoint(int waypointIndex);
    void updateIndicatorStatus(QLabel *indicator, bool state, QString status);
    void startPositionRequest();
    void stopPositionRequest();
};

#endif // PANEL_ROUTE_H
