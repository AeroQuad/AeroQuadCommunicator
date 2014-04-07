#ifndef PANEL_STATUS_H
#define PANEL_STATUS_H

#include <QWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

namespace Ui {
class PanelStatus;
}

class PanelStatus : public QWidget
{
    Q_OBJECT

public:
    explicit PanelStatus(QWidget *parent = 0);
    ~PanelStatus();

private slots:
    void on_simRoll_valueChanged(int value);
    void on_simPitch_valueChanged(int value);
    void on_simHeading_valueChanged(int value);

private:
    Ui::PanelStatus *ui;
    QGraphicsPixmapItem *aiBackground;
    QGraphicsPixmapItem *aiCompassBackground;
    QGraphicsPixmapItem *aiDial;
    QGraphicsPixmapItem *aiCompass;
    QGraphicsScene *aiScene;
    QGraphicsScene *leftScene;
    QGraphicsScene *rightScene;
    QGraphicsEllipseItem *leftStick;
    QGraphicsEllipseItem *rightStick;
    float roll;
    float pitch;
    float posThrottle;
    float posYaw;
    float posPitch;
    float posRoll;

    float scale(float value, float inputMin, float inputMax, float outputMin, float outputMax);
    void movePitchRoll(float rollAngle, float pitchAngle);
};

#endif // PANEL_STATUS_H
