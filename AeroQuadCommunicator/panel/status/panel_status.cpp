#include "panel_status.h"
#include "ui_panel_status.h"
#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>

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

void PanelStatus::on_simRoll_valueChanged(int value)
{
    roll = (float)value;
    movePitchRoll(roll, pitch);
}

void PanelStatus::on_simPitch_valueChanged(int value)
{
    pitch = (float)value;
    movePitchRoll(roll, pitch);
    qDebug() << value;
}

void PanelStatus::on_simHeading_valueChanged(int value)
{
    aiCompass->setRotation(-value);
}

float PanelStatus::scale(float value, float inputMin, float inputMax, float outputMin, float outputMax)
{
    return (((float)value - inputMin) / (inputMax-inputMin)) * (outputMax-outputMin) + outputMin;
}

void PanelStatus::movePitchRoll(float rollAngle, float pitchAngle)
{
//    float pitchPosition = scale(-pitchAngle, -135.0, 135.0, 540.0, -540.0);
//    float rollCenter = scale(-pitchAngle, -135.0, 135.0, 0.0, 1080.0);
    float pitchPosition = scale(-pitchAngle, -175.0, 175.0, 700.0, -700.0);
    float rollCenter = scale(-pitchAngle, -175.0, 175.0, 0.0, 1400.0);
    aiBackground->setPos(0, pitchPosition);
    aiBackground->setTransformOriginPoint(250.0, rollCenter);
    aiBackground->setRotation(-rollAngle);
}
