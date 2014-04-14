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
    //ui->artificialHorizon->setFocusPolicy(Qt::NoFocus);
    ui->artificialHorizon->setSceneRect(49, 560, 400, 280); //406, 282
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
    rollText->setPos(50, 580);
    pitchText = aiScene->addText("Pitch:");
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
    if (flightConfig == "Quad +")
        ui->flightConfigSelect->setCurrentIndex(0);
    if (flightConfig == "Quad X")
        ui->flightConfigSelect->setCurrentIndex(1);
    if (flightConfig == "Hex +")
        ui->flightConfigSelect->setCurrentIndex(2);
    if (flightConfig == "Hex X")
        ui->flightConfigSelect->setCurrentIndex(3);
    if (flightConfig == "Octo +")
        ui->flightConfigSelect->setCurrentIndex(4);
    if (flightConfig == "Octo X")
        ui->flightConfigSelect->setCurrentIndex(5);
    if (flightConfig == "Hex Y6")
        ui->flightConfigSelect->setCurrentIndex(6);
//    if (flightConfig == "Octo X8")
//        ui->flightConfigSelect->setCurrentIndex();
//    if (flightConfig == "Quad Y4")
//        ui->flightConfigSelect->setCurrentIndex();
//    if (flightConfig == "Tri")
//        ui->flightConfigSelect->setCurrentIndex();
//    if (flightConfig == "Rover")
//        ui->flightConfigSelect->setCurrentIndex();
    sendMessage("s");
}
void PanelStatus::parseMessage(QByteArray data)
{
    QString incomingMessage = data;
    QStringList status = incomingMessage.split(',');
    if (status.size() != 25)
        return; // Don't process if expected message size not received
    motorArmed = status.at(0).toInt();
    roll = status.at(1).toFloat() * Rad2Deg;
    pitch = status.at(2).toFloat() * Rad2Deg;
    heading = status.at(3).toFloat() * Rad2Deg;
    altitude = status.at(4).toFloat();
    altitudeHold = status.at(5).toInt();
    for (int index=6; index<14; index++)
        receiverChannel[index-6] = status.at(index).toFloat();
    for (int index=14; index<22; index++)
        motorPower[index-14] = status.at(index).toInt();
    batteryPower = status.at(22).toFloat();
    flightMode = status.at(23).toInt();

    aiCompass->setRotation(-heading);\
    movePitchRoll(roll, pitch);
    updateSticks();
    updateMotors();
    updateButtons();
    updateTextStatus();
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
    if (flightConfig == "Quad +")
    {
        ui->QuadPlus1Motor->setValue(motorPower.at(0));
        ui->QuadPlus1Value->setText(QString::number(motorPower.at(0)));
        ui->QuadPlus2Motor->setValue(motorPower.at(1));
        ui->QuadPlus2Value->setText(QString::number(motorPower.at(1)));
        ui->QuadPlus3Motor->setValue(motorPower.at(2));
        ui->QuadPlus3Value->setText(QString::number(motorPower.at(2)));
        ui->QuadPlus4Motor->setValue(motorPower.at(3));
        ui->QuadPlus4Value->setText(QString::number(motorPower.at(3)));
    }
    if (flightConfig == "Quad Y4")
    {

    }
    if (flightConfig == "Tri")
    {

    }
    if (flightConfig == "Hex +")
    {
        ui->HexPlus1Motor->setValue(motorPower.at(0));
        ui->HexPlus1Value->setText(QString::number(motorPower.at(0)));
        ui->HexPlus2Motor->setValue(motorPower.at(1));
        ui->HexPlus2Value->setText(QString::number(motorPower.at(1)));
        ui->HexPlus3Motor->setValue(motorPower.at(2));
        ui->HexPlus3Value->setText(QString::number(motorPower.at(2)));
        ui->HexPlus4Motor->setValue(motorPower.at(3));
        ui->HexPlus4Value->setText(QString::number(motorPower.at(3)));
        ui->HexPlus5Motor->setValue(motorPower.at(4));
        ui->HexPlus5Value->setText(QString::number(motorPower.at(4)));
        ui->HexPlus6Motor->setValue(motorPower.at(5));
        ui->HexPlus6Value->setText(QString::number(motorPower.at(5)));
    }
    if (flightConfig == "Hex X")
    {
        ui->HexX1Motor->setValue(motorPower.at(0));
        ui->HexX1Value->setText(QString::number(motorPower.at(0)));
        ui->HexX2Motor->setValue(motorPower.at(1));
        ui->HexX2Value->setText(QString::number(motorPower.at(1)));
        ui->HexX3Motor->setValue(motorPower.at(2));
        ui->HexX4Value->setText(QString::number(motorPower.at(2)));
        ui->HexX4Motor->setValue(motorPower.at(3));
        ui->HexX4Value->setText(QString::number(motorPower.at(3)));
        ui->HexX5Motor->setValue(motorPower.at(4));
        ui->HexX5Value->setText(QString::number(motorPower.at(4)));
        ui->HexX6Motor->setValue(motorPower.at(5));
        ui->HexX6Value->setText(QString::number(motorPower.at(5)));
    }
    if (flightConfig == "Hex Y6")
    {
        ui->Y61Motor->setValue(motorPower.at(0));
        ui->Y61Value->setText(QString::number(motorPower.at(0)));
        ui->Y62Motor->setValue(motorPower.at(1));
        ui->Y62Value->setText(QString::number(motorPower.at(1)));
        ui->Y63Motor->setValue(motorPower.at(2));
        ui->Y64Value->setText(QString::number(motorPower.at(2)));
        ui->Y64Motor->setValue(motorPower.at(3));
        ui->Y64Value->setText(QString::number(motorPower.at(3)));
        ui->Y65Motor->setValue(motorPower.at(4));
        ui->Y65Value->setText(QString::number(motorPower.at(4)));
        ui->Y66Motor->setValue(motorPower.at(5));
        ui->Y66Value->setText(QString::number(motorPower.at(5)));
    }
    if (flightConfig == "Octo X8")
    {

    }
    if (flightConfig == "Octo X")
    {
        ui->octoX1Motor->setValue(motorPower.at(0));
        ui->octoX1Value->setText(QString::number(motorPower.at(0)));
        ui->octoX2Motor->setValue(motorPower.at(1));
        ui->octoX2Value->setText(QString::number(motorPower.at(1)));
        ui->octoX3Motor->setValue(motorPower.at(2));
        ui->octoX4Value->setText(QString::number(motorPower.at(2)));
        ui->octoX4Motor->setValue(motorPower.at(3));
        ui->octoX4Value->setText(QString::number(motorPower.at(3)));
        ui->octoX5Motor->setValue(motorPower.at(4));
        ui->octoX5Value->setText(QString::number(motorPower.at(4)));
        ui->octoX6Motor->setValue(motorPower.at(5));
        ui->octoX6Value->setText(QString::number(motorPower.at(5)));
        ui->octoX7Motor->setValue(motorPower.at(6));
        ui->octoX7Value->setText(QString::number(motorPower.at(6)));
        ui->octoX8Motor->setValue(motorPower.at(7));
        ui->octoX8Value->setText(QString::number(motorPower.at(7)));
    }
    if (flightConfig == "Octo +")
    {
        ui->octoPlus1Motor->setValue(motorPower.at(0));
        ui->octoPlus1Value->setText(QString::number(motorPower.at(0)));
        ui->octoPlus2Motor->setValue(motorPower.at(1));
        ui->octoPlus2Value->setText(QString::number(motorPower.at(1)));
        ui->octoPlus3Motor->setValue(motorPower.at(2));
        ui->octoPlus4Value->setText(QString::number(motorPower.at(2)));
        ui->octoPlus4Motor->setValue(motorPower.at(3));
        ui->octoPlus4Value->setText(QString::number(motorPower.at(3)));
        ui->octoPlus5Motor->setValue(motorPower.at(4));
        ui->octoPlus5Value->setText(QString::number(motorPower.at(4)));
        ui->octoPlus6Motor->setValue(motorPower.at(5));
        ui->octoPlus6Value->setText(QString::number(motorPower.at(5)));
        ui->octoPlus7Motor->setValue(motorPower.at(6));
        ui->octoPlus7Value->setText(QString::number(motorPower.at(6)));
        ui->octoPlus8Motor->setValue(motorPower.at(7));
        ui->octoPlus8Value->setText(QString::number(motorPower.at(7)));
    }
    if (flightConfig == "Rover")
    {

    }
}

void PanelStatus::updateSticks()
{
    int throttlePos = scale(receiverChannel.at(ThrottleChannel), 1000.0, 2000.0, 55.0, -75.0);
    int yawPos = scale(receiverChannel.at(YawChannel), 1000.0, 2000.0, -75.0, 55.0);
    int pitchPos = scale(receiverChannel.at(PitchChannel), 1000.0, 2000.0, 55.0, -75.0);
    int rollPos = scale(receiverChannel.at(RollChannel), 1000.0, 2000.0, -75.0, 55.0);
    leftStick->setPos(yawPos, throttlePos);
    rightStick->setPos(rollPos, pitchPos);
}

void PanelStatus::updateButtons()
{
    if (altitudeHold)
        updateButton(ui->altHoldState, "Altitude Hold\nOn", "green");
    else
        updateButton(ui->altHoldState, "Altitude Hold\nOff", "grey");
    if (motorArmed)
        updateButton(ui->motorState, "Motors\nArmed", "green");
    else
        updateButton(ui->motorState, "Motors\nNot Armed", "grey");
    if (flightMode)
        updateButton(ui->attitudeState, "Stable Mode", "steelblue");
    else
        updateButton(ui->attitudeState, "Acrobatic Mode", "green");
}

void PanelStatus::updateTextStatus()
{
    rollText->setPlainText("Roll: " + QString::number(roll, 'f', 1));
    pitchText->setPlainText("Pitch: " + QString::number(pitch, 'f', 1));
    altitudeText->setPlainText("Altitude: " + QString::number(altitude, 'f', 1));
    batteryText->setPlainText("Battery: " + QString::number(batteryPower, 'f', 1));
}


