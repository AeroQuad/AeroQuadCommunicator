#include "panel_status.h"
#include "ui_panel_status.h"
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>

const float Rad2Deg = 57.2957795;
const int RollChannel = 0;
const int PitchChannel = 1;
const int YawChannel = 2;
const int ThrottleChannel = 3;
const int ModeChannel = 4;
const int Aux1Channel = 5;
const int Aux2Channel = 6;
const int Aux3Channel = 7;
const int Aux4Channel = 8;
const int Aux5Channel = 9;

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
    QString fontFamily = altitudeText->font().family();
    altitudeText->setFont(QFont(fontFamily, 12));
    altitudeText->setPos(50, 815);
    batteryText = aiScene->addText("Battery:");
    batteryText->setDefaultTextColor(Qt::white);
    batteryText->setFont(QFont(fontFamily, 12));
    batteryText->setPos(50, 795);
    rollText = aiScene->addText("Roll:");
    rollText->setDefaultTextColor(Qt::white);
    rollText->setFont(QFont(fontFamily, 12));
    rollText->setPos(50, 580);    pitchText = aiScene->addText("Pitch:");
    pitchText->setDefaultTextColor(Qt::white);
    pitchText->setFont(QFont(fontFamily, 12));
    pitchText->setPos(50, 560);

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

    for (int index=0; index<8; index++)
    {
        motorPower.append(0);
        receiverChannel.append(0);
    }

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
    motorCount = configuration["Motors"].toInt();
    flightConfig = configuration["Flight Config"];
    sendMessage("s");
}
void PanelStatus::parseMessage(QByteArray data)
{
    QString incomingMessage = data;
    QStringList status = incomingMessage.split(',');
    if (status.size() != 25)
        return;
    motorArmed = status.at(0).toInt();
    if (motorArmed)
        updateButton(ui->motorState, "Motors\nArmed", "green");
    else
        updateButton(ui->motorState, "Motors\nNot Armed", "grey");
    roll = status.at(1).toFloat() * Rad2Deg;
    rollText->setPlainText("Roll: " + QString::number(roll, 'f', 1));
    pitch = status.at(2).toFloat() * Rad2Deg;
    pitchText->setPlainText("Pitch: " + QString::number(pitch, 'f', 1));
    movePitchRoll(roll, pitch);
    float heading = status.at(3).toFloat() * Rad2Deg;
    aiCompass->setRotation(-heading);\
    QString alt = status.at(4);
    alt.chop(1);
    altitudeText->setPlainText("Altitude: " + alt);
    altitudeHold = status.at(5).toInt();
    if (altitudeHold)
        updateButton(ui->altHoldState, "Altitude Hold\nOn", "green");
    else
        updateButton(ui->altHoldState, "Altitude Hold\nOff", "grey");
    for (int index=6; index<14; index++)
        receiverChannel[index-6] = status.at(index).toFloat();
    updateSticks();
    for (int index=14; index<22; index++)
        motorPower[index-14] = status.at(index).toInt();
    updateMotors();
    batteryPower = status.at(22).toFloat();
    batteryText->setPlainText("Battery: " + QString::number(batteryPower, 'f', 1));
    flightMode = status.at(23).toInt();
    if (flightMode)
        updateButton(ui->attitudeState, "Stable Mode", "steelblue");
    else
        updateButton(ui->attitudeState, "Acrobatic Mode", "green");
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

void PanelStatus::updateButton(QLabel *button, QString text, QString color)
{
    button->setStyleSheet("color:white;border-width:1px;border-color:black;border-radius:5px;background-color:" + color);
    button->setText(text);
}

void PanelStatus::updateMotors()
{
    if (flightConfig == "Quad X")
    {
        ui->QuadX1Motor->setValue(motorPower.at(0));
        ui->QuadX1Value->setText(QString::number(motorPower.at(0)));
        ui->QuadX2Motor->setValue(motorPower.at(1));
        ui->QuadX2Value->setText(QString::number(motorPower.at(1)));
        ui->QuadX3Motor->setValue(motorPower.at(2));
        ui->QuadX3Value->setText(QString::number(motorPower.at(2)));
        ui->QuadX4Motor->setValue(motorPower.at(3));
        ui->QuadX4Value->setText(QString::number(motorPower.at(3)));
    }
}

void PanelStatus::updateSticks()
{
    qDebug() << "throttle" << receiverChannel.at(ThrottleChannel);
    int throttlePos = scale(receiverChannel.at(ThrottleChannel), 1000.0, 2000.0, 55.0, -75.0);
    int yawPos = scale(receiverChannel.at(YawChannel), 1000.0, 2000.0, -75.0, 55.0);
    int pitchPos = scale(receiverChannel.at(PitchChannel), 1000.0, 2000.0, 55.0, -75.0);
    int rollPos = scale(receiverChannel.at(RollChannel), 1000.0, 2000.0, -75.0, 55.0);
    leftStick->setPos(yawPos, throttlePos);
    rightStick->setPos(rollPos, pitchPos);
}

//SERIAL_PRINT("Flight Config: ");
//#if defined(quadPlusConfig)
//  SERIAL_PRINTLN("Quad +");
//#elif defined(quadXConfig)
//  SERIAL_PRINTLN("Quad X");
//#elif defined (quadY4Config)
//  SERIAL_PRINTLN("Quad Y4");
//#elif defined (triConfig)
//  SERIAL_PRINTLN("Tri");
//#elif defined(hexPlusConfig)
//  SERIAL_PRINTLN("Hex +");
//#elif defined(hexXConfig)
//  SERIAL_PRINTLN("Hex X");
//#elif defined(hexY6Config)
//  SERIAL_PRINTLN("Hex Y6");
//#elif defined(octoX8Config)
//  SERIAL_PRINTLN("Octo X8");
//#elif defined(octoXConfig)
//  SERIAL_PRINTLN("Octo X");
//#elif defined(octoPlusConfig)
//  SERIAL_PRINTLN("Octo +");
//#elif defined(roverConfig)
//  SERIAL_PRINTLN("Rover");
