#ifndef PANEL_CALIBRATE_H
#define PANEL_CALIBRATE_H

#include <QWidget>
#include <QMap>
#include <QWizard>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QGraphicsScene>
#include <QTimer>

#define WAIT -1

#define CALTYPE_ACCEL 0
#define CALTYPE_MAG 1
#define CALTYPE_XMIT 2
#define CALTYPE_ESC 3

#define ACCEL_WAIT -1
#define ACCEL_RIGHTSIDEUP 0
#define ACCEL_UPSIDEDOWN 1
#define ACCEL_LEFT 2
#define ACCEL_RIGHT 3
#define ACCEL_UP 4
#define ACCEL_DOWN 5
#define ACCEL_FINISH 6

#define MAG_WAIT 7
#define MAG_ACQUIRE 8
#define MAG_FINISH 9

#define XMIT_WAIT 10
#define XMIT_ACQUIRE 11
#define XMIT_FINISH 12

#define ESC_START 13
#define ESC_COUNTDOWN 14
#define ESC_POWER_ON 15
#define ESC_THROTTLE_COMMANDS 16

#define ESC_HIGH_THROTTLE 16
#define ESC_LOW_THROTTLE 17
#define ESC_MOTOR_TEST 18
#define ESC_FINISH 19

#define XMIT_CHANNEL_COUNT 8
#define ROLL 0
#define PITCH 1
#define YAW 2
#define THROTTLE 3
#define AUX1 4
#define AUX2 5
#define AUX3 6
#define AUX4 7

namespace Ui {
class PanelCalibrate;
}

class PanelCalibrate : public QWidget
{
    Q_OBJECT

public:
    explicit PanelCalibrate(QWidget *parent = 0);
    ~PanelCalibrate();
    bool connectState;
    QMap<QString, QString> configuration;
    void sendMessage(QString message) {emit messageOut(message.toUtf8());}

public slots:
    void initialize(QMap<QString, QString> config);
    void updateConnectionState(bool state) {connectState = state;}
    void parseMessage(QByteArray);
    void countdownCheck();

signals:
    void initializePanel(QMap<QString, QString>);
    void messageIn(QByteArray);
    void messageOut(QByteArray);
    void panelStatus(QString);
    void connectionState(bool);
    void getConnectionState();

private slots:
    void on_accelCal_clicked();
    void on_cancel_clicked();
    void on_next_clicked();
    void on_initEEPROM_clicked();
    void on_magCal_clicked();
    void on_xmitCal_clicked();
    void on_escCal_clicked();
    void on_done_clicked();
    void on_motorPower_valueChanged(int value);
    void on_decrement_clicked();
    void on_increment_clicked();

private:
    Ui::PanelCalibrate *ui;
    int nextMessage;
    int calibrationType;
    QVector<float> accelX;
    QVector<float> accelY;
    QVector<float> accelZ;
    QVector<float> workingAccelX;
    QVector<float> workingAccelY;
    QVector<float> workingAccelZ;
    float minMagX;
    float maxMagX;
    float minMagY;
    float maxMagY;
    float minMagZ;
    float maxMagZ;
    bool storeAccelData(QString incomingData);
    float calculateAccelScaleFactor(float input1, float input2);
    QGraphicsScene *leftScene;
    QGraphicsScene *rightScene;
    QGraphicsEllipseItem *leftStick;
    QGraphicsEllipseItem *rightStick;
    float posThrottle;
    float posYaw;
    float posPitch;
    float posRoll;
    QVector<float> minXmit;
    QVector<float> maxXmit;
    QTimer *countdownTimer;
    int countdown;
    float map(float x,  float in_min,  float in_max,  float out_min,  float out_max);
};

#endif // PANEL_CALIBRATE_H
