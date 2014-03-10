#include "panel_calibrate.h"
#include "ui_panel_calibrate.h"
#include <QDebug>
#include <QLabel>
#include <QMessageBox>

PanelCalibrate::PanelCalibrate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelCalibrate)
{
    ui->setupUi(this);
    connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
    connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));

    ui->userConfirm->hide();
    calDisplay = new QLabel;
    calDisplay->setStyleSheet("font:36pt; color:white;");
    calLayout = new QVBoxLayout();
    calLayout->addWidget(calDisplay, 0, Qt::AlignCenter);
    ui->calScreen->setLayout(calLayout);
    calDisplay->setText("Select a calibration to perform.");
    ui->next->setEnabled(false);
    ui->cancel->setEnabled(false);
    ui->calProgress->setValue(0);
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
            calDisplay->setText("<Insert Picture Here>\n\nPlace the AeroQuad upside down\non a flat surface.");
        break;
    case ACCEL_UPSIDEDOWN:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_UPSIDEDOWN;
        else
            calDisplay->setText("<Insert Picture Here>\n\nPoint the left side down.");
        break;
    case ACCEL_LEFT:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_LEFT;
        else
            calDisplay->setText("<Insert Picture Here>\n\nPoint the right side down.");
        break;
    case ACCEL_RIGHT:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_RIGHT;
        else
            calDisplay->setText("<Insert Picture Here>\n\nPoint the nose up.");
        break;
    case ACCEL_UP:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_UP;
        else
            calDisplay->setText("<Insert Picture Here>\n\nPoint the nose down.");
        break;
    case ACCEL_DOWN:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_DOWN;
        else
            calDisplay->setText("<Insert Picture Here>\n\nPlace the AeroQuad on a flat surface.");
        break;
    case ACCEL_FINISH:
        if (storeAccelData(incomingMessage))
            nextMessage = ACCEL_FINISH;
        else
        {
            calDisplay->setText("The accelerometer calibration is finished!");
            nextMessage = ACCEL_WAIT;
            qDebug() << "Go to accel calculation";
            ui->userConfirm->hide();
        }
        break;
    }
}

void PanelCalibrate::on_accelCal_clicked()
{
    ui->userConfirm->show();
    calDisplay->setText("<Insert Picture Here>\n\nPlace the AeroQuad on a flat surface.");
    calibrationType = CALTYPE_ACCEL;
    sendMessage("l");
    nextMessage = ACCEL_WAIT;
    ui->next->setEnabled(true);
    ui->cancel->setEnabled(true);
}

void PanelCalibrate::on_cancel_clicked()
{
    ui->userConfirm->hide();
    calDisplay->setText("Calibration has been cancelled.");
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
    case CALTYPE_MAG:
        break;
    case CALTYPE_XMIT:
        break;
    }
}

bool PanelCalibrate::storeAccelData(QString incomingMessage)
{
    QStringList parseData = incomingMessage.split(',');
    workingAccelX.append(parseData.at(0).toFloat());
    workingAccelY.append(parseData.at(1).toFloat());
    workingAccelZ.append(parseData.at(2).toFloat());
    int count = workingAccelX.size();
    ui->calProgress->setValue(count);
    if (count > 100)
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

void PanelCalibrate::on_initEEPROM_clicked()
{
    int response = QMessageBox::critical(this, "AeroQuad Communicator", "Are you sure you wish to initialize the EEPROM?  You will need to perform all board calibrations after this operation.", QMessageBox::Yes | QMessageBox::No);
    if (response == QMessageBox::Yes)
    {
        sendMessage("I");
        sendMessage("W");
        calDisplay->setText("EEPROM has been initialized.");
    }
    else
        calDisplay->setText("EEPROM has not been modified.");
}
