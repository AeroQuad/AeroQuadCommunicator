#include "panel_status.h"
#include "ui_panel_status.h"
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>

const float rad2deg = 57.2957795;

PanelStatus::PanelStatus(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelStatus)
{
    ui->setupUi(this);

    QPixmap aiBackgroundImage(":/images/resources/artificialHorizonBackGround.png");
    aiBackground =  new QGraphicsPixmapItem(aiBackgroundImage);
    movePitchRoll(0,0);

    QPixmap aiDialImage(":/images/resources/artificialHorizonDial.png");
    aiDial = new QGraphicsPixmapItem(aiDialImage);
    aiDial->setPos(100.0, 550.0);

    QPixmap aiCompassBackgroundImage(":/images/resources/artificialHorizonCompassBackGround.png");
    aiCompassBackground = new QGraphicsPixmapItem(aiCompassBackgroundImage);
    aiCompassBackground->setPos(100.0, 550.0);

    QPixmap aiCompassImage(":/images/resources/artificialHorizonCompass.png");
    aiCompass = new QGraphicsPixmapItem(aiCompassImage);
    aiCompass->setPos(100.0, 550.0);
    aiCompass->setTransformOriginPoint(150.0, 150.0);

    aiScene = new QGraphicsScene;
    aiScene->addItem(aiBackground);
    //aiScene->addItem(aiDial);
    aiScene->addItem(aiCompassBackground);
    aiScene->addItem(aiCompass);
    ui->artificialHorizon->setScene(aiScene);

    altitudeText = aiScene->addText("Altitude:");
    altitudeText->setDefaultTextColor(Qt::white);
    altitudeText->setPos(50, 820);

    QImage image(":/images/resources/TxCalDial.png");
    QGraphicsPixmapItem *leftDial = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    leftScene = new QGraphicsScene;
    leftScene->addItem(leftDial);
    leftStick = new QGraphicsEllipseItem;
    leftStick->setBrush(QBrush(QColor(Qt::blue),Qt::SolidPattern));
    leftStick->setRect(75, 75, 50, 50);
    leftScene->addItem(leftStick);
    ui->leftStick->setScene(leftScene);
    ui->leftStick->fitInView(leftDial);

    QGraphicsPixmapItem *rightDial = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    rightScene = new QGraphicsScene;
    rightScene->addItem(rightDial);
    rightStick = new QGraphicsEllipseItem;
    rightStick->setBrush(QBrush(QColor(Qt::blue),Qt::SolidPattern));
    rightStick->setRect(75, 75, 50, 50);
    rightScene->addItem(rightStick);
    ui->rightStick->setScene(rightScene);
    ui->rightStick->fitInView(rightDial);

    connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
    connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));
}

PanelStatus::~PanelStatus()
{
    delete aiBackground;
    delete aiDial;
    delete aiCompassBackground;
    delete aiCompass;
    delete aiScene;
    delete ui;
}

void PanelStatus::initialize(QMap<QString, QString> config)
{
    configuration = config;
    emit getConnectionState();
    sendMessage("s");
}
void PanelStatus::parseMessage(QByteArray data)
{
    QString incomingMessage = data;
    QStringList status = incomingMessage.split(',');
    roll = status.at(1).toFloat() * rad2deg;
    pitch = status.at(2).toFloat() * rad2deg;
    movePitchRoll(roll, pitch);
    float heading = status.at(3).toFloat() * rad2deg;
    aiCompass->setRotation(-heading);
    QString alt = status.at(4);
    alt.chop(1);
    altitudeText->setPlainText("Altitude: " + alt);
}

float PanelStatus::scale(float value, float inputMin, float inputMax, float outputMin, float outputMax)
{
    return (((float)value - inputMin) / (inputMax-inputMin)) * (outputMax-outputMin) + outputMin;
}

void PanelStatus::movePitchRoll(float rollAngle, float pitchAngle)
{
    float pitchPosition = scale(-pitchAngle, -175.0, 175.0, 700.0, -700.0);
    float rollCenter = scale(-pitchAngle, -175.0, 175.0, 0.0, 1400.0);
    aiBackground->setPos(0, pitchPosition);
    aiBackground->setTransformOriginPoint(250.0, rollCenter);
    aiBackground->setRotation(-rollAngle);
}

//case 's': // Send all flight data
//  PrintValueComma(motorArmed);
//  PrintValueComma(kinematicsAngle[XAXIS]);
//  PrintValueComma(kinematicsAngle[YAXIS]);
//  PrintValueComma(getHeading());
//  #if defined AltitudeHoldBaro || defined AltitudeHoldRangeFinder
//    #if defined AltitudeHoldBaro
//      PrintValueComma(getBaroAltitude());
//    #elif defined AltitudeHoldRangeFinder
//      PrintValueComma(rangeFinderRange[ALTITUDE_RANGE_FINDER_INDEX] != INVALID_RANGE ? rangeFinderRange[ALTITUDE_RANGE_FINDER_INDEX] : 0.0);
//    #endif
//    PrintValueComma((int)altitudeHoldState);
//  #else
//    PrintValueComma(0);
//    PrintValueComma(0);
//  #endif

//  for (byte channel = 0; channel < 8; channel++) { // Configurator expects 8 values
//    PrintValueComma((channel < LASTCHANNEL) ? receiverCommand[channel] : 0);
//  }

//  for (byte motor = 0; motor < LASTMOTOR; motor++) {
//    PrintValueComma(motorCommand[motor]);
//  }
//  PrintDummyValues(8 - LASTMOTOR); // max of 8 motor outputs supported

//  #ifdef BattMonitor
//    PrintValueComma((float)batteryData[0].voltage/100.0); // voltage internally stored at 10mV:s
//  #else
//    PrintValueComma(0);
//  #endif
//  PrintValueComma(flightMode);
//  SERIAL_PRINTLN();
//  break;
