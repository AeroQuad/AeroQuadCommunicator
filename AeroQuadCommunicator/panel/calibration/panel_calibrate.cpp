#include "panel_calibrate.h"
#include "ui_panel_calibrate.h"
#include <QDebug>
#include <QLabel>
#include <QMessageBox>
#include <QGraphicsPixmapItem>
#include <QThread>

PanelCalibrate::PanelCalibrate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelCalibrate)
{
    ui->setupUi(this);
    connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
    connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));

    ui->userConfirm->hide();
    ui->calPanel->setCurrentIndex(0);
    ui->displayInstructions->setText("Select a calibration to perform.");
    ui->next->setEnabled(false);
    ui->cancel->setEnabled(false);
    ui->calProgress->setValue(0);

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

    countdownTimer = new QTimer();
    countdownTimer->setInterval(1000);
    connect(countdownTimer, SIGNAL(timeout()), this, SLOT(countdownCheck()));
}

PanelCalibrate::~PanelCalibrate()
{
    delete ui;
}

void PanelCalibrate::initialize(QMap<QString, QString> config)
{
    configuration = config;
    emit getConnectionState();
}

void PanelCalibrate::parseMessage(QByteArray data)
{
    QString incomingMessage = data;
    switch (nextMessage)
    {
    case ACCEL_WAIT:
        // Wait until user hits NEXT button to start calibration
        break;
    case ACCEL_RIGHTSIDEUP:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_RIGHTSIDEUP;
        else
            ui->accelInstructions->setText("Place the AeroQuad upside down on a flat surface.");
        break;
    case ACCEL_UPSIDEDOWN:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_UPSIDEDOWN;
        else
            ui->accelInstructions->setText("Point the left side down.");
        break;
    case ACCEL_LEFT:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_LEFT;
        else
            ui->accelInstructions->setText("Point the right side down.");
        break;
    case ACCEL_RIGHT:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_RIGHT;
        else
            ui->accelInstructions->setText("Point the nose up.");
        break;
    case ACCEL_UP:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_UP;
        else
            ui->accelInstructions->setText("Point the nose down.");
        break;
    case ACCEL_DOWN:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_DOWN;
        else
            ui->accelInstructions->setText("Return the AeroQuad to a flat surface.");
        break;
    case ACCEL_FINISH:
    {
        ui->calPanel->setCurrentIndex(0);
        ui->displayInstructions->setText("The accelerometer calibration is finished!");
        nextMessage = ACCEL_WAIT;
        float xAccelScaleFactor = calculateAccelScaleFactor(accelX.at(5), accelX.at(4));
        float yAccelScaleFactor = calculateAccelScaleFactor(accelY.at(3), accelY.at(2));
        float zAccelScaleFactor = calculateAccelScaleFactor(accelZ.at(0), accelZ.at(1));
        sendMessage("J");
        sendMessage("K" + QString::number(xAccelScaleFactor) + ";0;" + QString::number(yAccelScaleFactor) + ";0;" + QString::number(zAccelScaleFactor) + ";0;");
        sendMessage("W");
        ui->userConfirm->hide();
        break;
    }
    case MAG_ACQUIRE:
    {
        QStringList parseData = incomingMessage.split(',');
        minMagX = qMin(minMagX, parseData.at(0).toFloat());
        maxMagX = qMax(maxMagX, parseData.at(0).toFloat());
        minMagY = qMin(minMagY, parseData.at(1).toFloat());
        maxMagY = qMax(maxMagY, parseData.at(1).toFloat());
        minMagZ = qMin(minMagZ, parseData.at(2).toFloat());
        maxMagZ = qMax(maxMagZ, parseData.at(2).toFloat());
        ui->xAxisMinus->setValue(abs(minMagX));
        ui->xAxisPlus->setValue(maxMagX);
        ui->yAxisMinus->setValue(abs(minMagY));
        ui->yAxisPlus->setValue(maxMagY);
        ui->zAxisMinus->setValue(abs(minMagZ));
        ui->zAxisPlus->setValue(maxMagZ);
        ui->xAxisValue->setText(parseData.at(0));
        ui->yAxisValue->setText(parseData.at(1));
        ui->zAxisValue->setText(parseData.at(2).trimmed());
        break;
    }
    case MAG_FINISH:
    {
        QString magBiasX = QString::number((minMagX + maxMagX) / 2.0);
        QString magBiasY = QString::number((minMagY + maxMagY) / 2.0);
        QString magBiasZ = QString::number((minMagZ + maxMagZ) / 2.0);
        sendMessage("M" + magBiasX +";" + magBiasY + ";" + magBiasZ + ";");
        sendMessage("W");
        //qDebug() << "M" + magBiasX +";" + magBiasY + ";" + magBiasZ + ";";
        ui->calPanel->setCurrentIndex(0);
        ui->displayInstructions->setText("The magnetometer calibration is finished!");
        ui->userConfirm->hide();
        break;
    }
    case XMIT_WAIT:
        ui->xmitInstructions->setText("Verify each channel is centered.  Move each transmitter stick to each of it's four corners, then flip all switches to measure the max and min value of each channel.");
        nextMessage = XMIT_ACQUIRE;
        break;
    case XMIT_ACQUIRE:
    {
        QStringList parseData = incomingMessage.split(',');
        for (int index=0; index<XMIT_CHANNEL_COUNT; index++)
        {
            minXmit[index] = qMin(minXmit.at(index), parseData.at(index).toFloat());
            maxXmit[index] = qMax(maxXmit.at(index), parseData.at(index).toFloat());
        }
        ui->throttleLabel->setText("Throttle: " + parseData.at(THROTTLE));
        ui->pitchLabel->setText("Pitch: " + parseData.at(PITCH));
        ui->rollLabel->setText("Roll: " + parseData.at(ROLL));
        ui->yawLabel->setText("Yaw: " + parseData.at(YAW));

        int throttlePos = map(parseData.at(THROTTLE).toFloat(), 1000.0, 2000.0, 55.0, -75.0);
        int yawPos = map(parseData.at(YAW).toFloat(), 1000.0, 2000.0, -75.0, 55.0);
        int pitchPos = map(parseData.at(PITCH).toFloat(), 1000.0, 2000.0, 55.0, -75.0);
        int rollPos = map(parseData.at(ROLL).toFloat(), 1000.0, 2000.0, -75.0, 55.0);
        leftStick->setPos(yawPos, throttlePos);
        rightStick->setPos(rollPos, pitchPos);
        ui->ch5Value->setValue(parseData.at(AUX1).toInt());
        ui->ch6Value->setValue(parseData.at(AUX2).toInt());
        ui->ch7Value->setValue(parseData.at(AUX3).toInt());
        ui->ch8Value->setValue(parseData.at(AUX4).toInt());
        break;
    }
    case XMIT_FINISH:
    {
        QVector<float> scaleFactor;
        QVector<float> offset;
        for (int index=0; index<XMIT_CHANNEL_COUNT; index++)
        {
            scaleFactor.append(1000.0/(maxXmit.at(index)-minXmit.at(index)));
            offset.append(1000.0-(minXmit.at(index)*(1000/(maxXmit.at(index)-minXmit.at(index)))));
            sendMessage("G" + QString::number(index) + ";" + QString::number(scaleFactor.at(index)) + ";");
            sendMessage("H" + QString::number(index) + ";" + QString::number(offset.at(index)) + ";");
        }
        sendMessage("W");
        ui->userConfirm->hide();
        ui->xmitInstructions->setText("The transmitter calibration is finished!");
        sendMessage("t");
        nextMessage = XMIT_ACQUIRE;
        break;
    }
    }
}

void PanelCalibrate::on_cancel_clicked()
{
    ui->userConfirm->hide();
    ui->calPanel->setCurrentIndex(0);
    ui->displayInstructions->setText("Calibration has been cancelled.");
    sendMessage("X");
    nextMessage = WAIT;
}

void PanelCalibrate::on_next_clicked()
{
    switch(calibrationType)
    {
    case CALTYPE_ACCEL:
        nextMessage++;
        ui->next->setEnabled(false);
        sendMessage("l");
        break;
    case CALTYPE_ESC:
        switch(nextMessage)
        {
        case ESC_START:
            ui->escInstructions->setText("When the counter reaches zero, apply battery power to the motors and ESC's.\n\n\nPress NEXT to begin the countdown.");
            ui->escAdjust->show();
            sendMessage("2123.45;");
            break;
        case ESC_COUNTDOWN:
            ui->escAdjust->hide();
            ui->escInstructions->setText("");
            ui->countdown->setText("  Get Ready!  ");
            countdownTimer->start();
            ui->countdown->show();
            ui->userConfirm->hide();
            break;
        case ESC_POWER_ON:
            ui->escInstructions->setFont(QFont("MS Shell Dlg 2", 35));
            ui->escInstructions->setAlignment(Qt::AlignHCenter);
            ui->escInstructions->setText("Connect battery power to the AeroQuad!");
            ui->countdown->hide();
            ui->userConfirm->hide();
            if (ui->powerOnDelayValue->value() == 0)
                sendMessage("1123.45;");
            break;
        case ESC_THROTTLE_COMMANDS:
            if (ui->powerOnDelayValue->value() > 0)
            {
                QThread::sleep(ui->powerOnDelayValue->value());
                sendMessage("1123.45;");
            }
            ui->escInstructions->setFont(QFont("MS Shell Dlg 2", 24));
            ui->escInstructions->setText("High throttle command being sent.\n\nListen for two beeps from the ESC's.");
            ui->escInstructions->repaint();
            QThread::sleep(2);
            ui->escInstructions->setText("Low throttle command being sent.\n\nListen for 4 beeps from the ESC's");
            ui->escInstructions->repaint();
            ui->calDisplay->repaint();
            sendMessage("2123.45;");
            QThread::sleep(4);
            ui->escInstructions->setText("Verify each motor is spinning at the same rate.\n\nIf not, adjust the Power On Delay value and perform the calibration again.");
            sendMessage("3123.45;1050;");
            ui->motorPowerAdjust->show();
            countdownTimer->stop();
            ui->next->hide();
            ui->cancel->hide();
            ui->done->show();
            ui->userConfirm->show();
            break;
        }
        nextMessage++;
        break;
    }
}

void PanelCalibrate::on_done_clicked()
{
    switch(calibrationType)
    {
    case CALTYPE_MAG:
        nextMessage = MAG_FINISH;
        break;
    case CALTYPE_XMIT:
        nextMessage = XMIT_FINISH;
        break;
    case CALTYPE_ESC:
        sendMessage("4123.45;");
        nextMessage = ESC_FINISH;
        ui->motorPowerAdjust->hide();
        ui->userConfirm->hide();
        ui->escInstructions->setFont(QFont("MS Shell Dlg 2", 35));
        ui->escInstructions->setAlignment(Qt::AlignHCenter);
        ui->escInstructions->setText("The ESC calibration is finished!\n\nDisconnect battery power before continuing.");
        break;
    }
}

// ***************************************************************
// ********************** EEPROM Calibration *********************
// ***************************************************************

void PanelCalibrate::on_initEEPROM_clicked()
{
    ui->calPanel->setCurrentIndex(0);
    ui->displayInstructions->setText("");
    ui->userConfirm->hide();
    int response = QMessageBox::critical(this, "AeroQuad Communicator", "Are you sure you wish to initialize the EEPROM?  You will need to perform all board calibrations after this operation.", QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes)
    {
        sendMessage("I");
        sendMessage("W");
        ui->displayInstructions->setText("EEPROM has been initialized.");
    }
    else
        ui->displayInstructions->setText("EEPROM has not been modified.");
}

// ***************************************************************
// ********************** Accel Calibration **********************
// ***************************************************************

void PanelCalibrate::on_accelCal_clicked()
{
    ui->calPanel->setCurrentIndex(1);
    ui->userConfirm->show();
    accelX.clear();
    accelY.clear();
    accelZ.clear();
    ui->accelInstructions->setText("Place the AeroQuad on a flat surface.");
    calibrationType = CALTYPE_ACCEL;
    sendMessage("l");
    nextMessage = ACCEL_WAIT;
    ui->next->setEnabled(true);
    ui->cancel->setEnabled(true);
    ui->calProgress->show();
    ui->done->hide();
    ui->next->show();
}

float PanelCalibrate::calculateAccelScaleFactor(float input1, float input2)
{
    float m = (input2 - input1) / 19.613;
    float accelBias = input2 - (m * 9.0865);
    float biased = input2 - accelBias;
    return 9.8065 / biased;
}

bool PanelCalibrate::storeAccelData(QString incomingMessage)
{
    QStringList parseData = incomingMessage.split(',');
    workingAccelX.append(parseData.at(0).toFloat());
    workingAccelY.append(parseData.at(1).toFloat());
    workingAccelZ.append(parseData.at(2).toFloat());
    int count = workingAccelX.size();
    ui->calProgress->setValue(count*2);
    if (count > 50)
    {
        float sumX = 0;
        float sumY = 0;
        float sumZ = 0;
        for (int index=0; index<count; index++)
        {
            sumX += workingAccelX.at(index);
            sumY += workingAccelY.at(index);
            sumZ += workingAccelZ.at(index);
        }
        accelX.append(sumX/(float)workingAccelX.size());
        accelY.append(sumY/(float)workingAccelY.size());
        accelZ.append(sumZ/(float)workingAccelZ.size());
        workingAccelX.clear();
        workingAccelY.clear();
        workingAccelZ.clear();
        ui->next->setEnabled(true);
        sendMessage("X");
        return false;
    }
    return true;
}

// ***************************************************************
// *********************** Mag Calibration ***********************
// ***************************************************************

void PanelCalibrate::on_magCal_clicked()
{
    ui->calPanel->setCurrentIndex(2);
    ui->userConfirm->show();
    ui->magInstructions->setText("Rotate the AeroQuad to determine the max/min value for each axis of the magnetometer.");
    calibrationType = CALTYPE_MAG;
    sendMessage("j");
    nextMessage = MAG_ACQUIRE;
    ui->next->setEnabled(true);
    ui->cancel->setEnabled(true);
    ui->calProgress->hide();
    ui->done->show();
    ui->next->hide();
    minMagX = 1000000;
    maxMagX = -1000000;
    minMagY = 1000000;
    maxMagY = -1000000;
    minMagZ = 1000000;
    maxMagZ = -1000000;
    ui->xAxisMinus->setValue(0);
    ui->xAxisPlus->setValue(0);
    ui->yAxisMinus->setValue(0);
    ui->yAxisPlus->setValue(0);
    ui->zAxisMinus->setValue(0);
    ui->zAxisPlus->setValue(0);
    ui->xAxisValue->setText("0");
    ui->yAxisValue->setText("0");
    ui->zAxisValue->setText("0");
}

// ***************************************************************
// ******************* Transmitter Calibration *******************
// ***************************************************************

void PanelCalibrate::on_xmitCal_clicked()
{
    ui->calPanel->setCurrentIndex(3);
    ui->userConfirm->show();
    ui->next->setEnabled(true);
    ui->cancel->setEnabled(true);
    ui->calProgress->hide();
    ui->done->show();
    ui->next->hide();
    ui->xmitInstructions->setText("Removing previous calibration factors...");
    for (int index=0; index<XMIT_CHANNEL_COUNT; index++)
    {
         sendMessage("G" + QString::number(index) + ";1;");
         sendMessage("H" + QString::number(index) + ";0;");
         minXmit.append(2000);
         maxXmit.append(1000);
    }
    sendMessage("t");
    calibrationType = CALTYPE_XMIT;
    nextMessage = XMIT_WAIT;
}

float PanelCalibrate::map(float x, float in_min, float in_max, float out_min, float out_max)
{
    float value = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    if (out_max > out_min)
    {
        if (value > out_max) value = out_max;
        if (value < out_min) value = out_min;
    }
    else
    {
        if (value < out_max) value = out_max;
        if (value > out_min) value = out_min;
    }
    return value;
}

// ***************************************************************
// *********************** ESC Calibration ***********************
// ***************************************************************

void PanelCalibrate::on_escCal_clicked()
{
    ui->calPanel->setCurrentIndex(4);
    ui->userConfirm->show();
    ui->next->setEnabled(true);
    ui->cancel->setEnabled(true);
    ui->calProgress->hide();
    ui->done->hide();
    ui->next->show();
    ui->countdown->hide();
    ui->escAdjust->hide();
    ui->motorPowerAdjust->hide();
    ui->escInstructions->setFont(QFont("MS Shell Dlg 2", 24));
    ui->escInstructions->setText("Warning!  Before starting the ESC Calibration, remove the propellers for safety considerations.\n\nIf this calibration is performed incorrectly with the propellers on, it could result in property damage and personal injury, as this next step in the calibration sends a high throttle command to the ESCs.\n\nDisconnect battery power to the AeroQuad NOW before the start of this calibration.\n\nLeave USB power On to the AeroQuad.");
    ui->escInstructions->setAlignment(Qt::AlignLeft);
    calibrationType = CALTYPE_ESC;
    nextMessage = ESC_START;
    countdown = 5;
}

void PanelCalibrate::countdownCheck()
{
    ui->countdown->setText(QString::number(countdown));
    if (countdown == 0)
        on_next_clicked();
    if (countdown == -1)
    {
        // delay extra 3 seconds for ESC power up
        on_next_clicked();
        countdownTimer->stop();
    }
    countdown--;
}

void PanelCalibrate::on_motorPower_valueChanged(int value)
{
    ui->motorPowerValue->setText(QString::number(value));
    if (nextMessage >= ESC_THROTTLE_COMMANDS)
        sendMessage("3123.45;" + QString::number(value) +";");
}

void PanelCalibrate::on_decrement_clicked()
{
    int value = ui->motorPower->value() - 1;
    if (value < 1000) value = 1000;
    ui->motorPower->setValue(value);
    on_motorPower_valueChanged(value);
}

void PanelCalibrate::on_increment_clicked()
{
    int value = ui->motorPower->value() + 1;
    if (value > 2000) value = 2000;
    ui->motorPower->setValue(value);
    on_motorPower_valueChanged(value);
}
