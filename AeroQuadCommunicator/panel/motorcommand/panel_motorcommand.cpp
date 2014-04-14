#include "panel_motorcommand.h"
#include "ui_panel_motorcommand.h"
#include <QDebug>

PanelMotorCommand::PanelMotorCommand(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelMotorCommand)
{
    ui->setupUi(this);
    connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
    connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));
}

PanelMotorCommand::~PanelMotorCommand()
{
    delete ui;
}

void PanelMotorCommand::initialize(QMap<QString, QString> config)
{
    configuration = config;
    emit getConnectionState();
    if (!connectState)
    {
        configuration["Flight Config"] = "Octo X";
        configuration["Motors"] = "8";
    }
    motorCount = configuration["Motors"].toInt();
    for (int index=0; index<motorCount; index++)
        motorValue.append(1000);
    commandMotors();
    ui->flightConfigDesc->setText("Flight Configuration: " + configuration["Flight Config"]);

    int configIndex;
    if (configuration["Flight Config"] == "Quad +")
        configIndex = 0;
    if (configuration["Flight Config"] == "Quad X")
        configIndex = 1;
    if (configuration["Flight Config"] == "Hex +")
        configIndex = 2;
    if (configuration["Flight Config"] == "Hex X")
        configIndex = 3;
    if (configuration["Flight Config"] == "Octo +")
        configIndex = 4;
    if (configuration["Flight Config"] == "Octo X")
        configIndex = 5;
    if (configuration["Flight Config"] == "Hex Y6")
        configIndex = 6;
    ui->flightConfigSelect->setCurrentIndex(configIndex);
}

void PanelMotorCommand::parseMessage(QByteArray data)
{
    QString incomingMessage = data;
    // Add code below if we need to process any messages from flight board
}

void PanelMotorCommand::commandMotors()
{
    QString command = "5123.45;";
    for (int index=0; index<motorValue.size(); index++)
        command += QString::number(motorValue.at(index)) + ";";
    sendMessage(command);
}

void PanelMotorCommand::on_stopMotors_clicked()
{
    if (configuration["Flight Config"] == "Quad +")
    {
        ui->QuadPlus1Motor->setValue(1000);
        ui->QuadPlus1Label->setText("1000");
        ui->QuadPlus2Motor->setValue(1000);
        ui->QuadPlus2Label->setText("1000");
        ui->QuadPlus3Motor->setValue(1000);
        ui->QuadPlus3Label->setText("1000");
        ui->QuadPlus4Motor->setValue(1000);
        ui->QuadPlus4Label->setText("1000");
    }
    if (configuration["Flight Config"] == "Quad X")
    {
        ui->QuadX1Motor->setValue(1000);
        ui->QuadX1Value->setText("1000");
        ui->QuadX2Motor->setValue(1000);
        ui->QuadX2Value->setText("1000");
        ui->QuadX3Motor->setValue(1000);
        ui->QuadX3Value->setText("1000");
        ui->QuadX4Motor->setValue(1000);
        ui->QuadX4Value->setText("1000");
    }
    if (configuration["Flight Config"] == "Hex +")
    {
        ui->HexPlus1Motor->setValue(1000);
        ui->HexPlus1Value->setText("1000");
        ui->HexPlus2Motor->setValue(1000);
        ui->HexPlus2Value->setText("1000");
        ui->HexPlus3Motor->setValue(1000);
        ui->HexPlus3Value->setText("1000");
        ui->HexPlus4Motor->setValue(1000);
        ui->HexPlus4Value->setText("1000");
        ui->HexPlus5Motor->setValue(1000);
        ui->HexPlus5Value->setText("1000");
        ui->HexPlus6Motor->setValue(1000);
        ui->HexPlus6Value->setText("1000");
    }
    if (configuration["Flight Config"] == "Hex X")
    {
        ui->HexX1Motor->setValue(1000);
        ui->HexX1Value->setText("1000");
        ui->HexX2Motor->setValue(1000);
        ui->HexX2Value->setText("1000");
        ui->HexX3Motor->setValue(1000);
        ui->HexX3Value->setText("1000");
        ui->HexX4Motor->setValue(1000);
        ui->HexX4Value->setText("1000");
        ui->HexX5Motor->setValue(1000);
        ui->HexX5Value->setText("1000");
        ui->HexX6Motor->setValue(1000);
        ui->HexX6Value->setText("1000");
    }
    if (configuration["Flight Config"] == "Octo +")
    {
        ui->octoPlus1Motor->setValue(1000);
        ui->octoPlus1Value->setText("1000");
        ui->octoPlus2Motor->setValue(1000);
        ui->octoPlus2Value->setText("1000");
        ui->octoPlus3Motor->setValue(1000);
        ui->octoPlus3Value->setText("1000");
        ui->octoPlus4Motor->setValue(1000);
        ui->octoPlus4Value->setText("1000");
        ui->octoPlus5Motor->setValue(1000);
        ui->octoPlus5Value->setText("1000");
        ui->octoPlus6Motor->setValue(1000);
        ui->octoPlus6Value->setText("1000");
        ui->octoPlus7Motor->setValue(1000);
        ui->octoPlus7Value->setText("1000");
        ui->octoPlus8Motor->setValue(1000);
        ui->octoPlus8Value->setText("1000");
    }
    if (configuration["Flight Config"] == "Octo X")
    {
        ui->octoX1Motor->setValue(1000);
        ui->octoX1Value->setText("1000");
        ui->octoX2Motor->setValue(1000);
        ui->octoX2Value->setText("1000");
        ui->octoX3Motor->setValue(1000);
        ui->octoX3Value->setText("1000");
        ui->octoX4Motor->setValue(1000);
        ui->octoX4Value->setText("1000");
        ui->octoX5Motor->setValue(1000);
        ui->octoX5Value->setText("1000");
        ui->octoX6Motor->setValue(1000);
        ui->octoX6Value->setText("1000");
        ui->octoX7Motor->setValue(1000);
        ui->octoX7Value->setText("1000");
        ui->octoX8Motor->setValue(1000);
        ui->octoX8Value->setText("1000");
    }
    if (configuration["Flight Config"] == "Hex Y6")
    {
        ui->Y61Motor->setValue(1000);
        ui->Y61Value->setText("1000");
        ui->Y62Motor->setValue(1000);
        ui->Y62Value->setText("1000");
        ui->Y63Motor->setValue(1000);
        ui->Y63Value->setText("1000");
        ui->Y64Motor->setValue(1000);
        ui->Y64Value->setText("1000");
        ui->Y65Motor->setValue(1000);
        ui->Y65Value->setText("1000");
        ui->Y66Motor->setValue(1000);
        ui->Y66Value->setText("1000");
    }
    commandMotors();
}

void PanelMotorCommand::on_QuadPlus1Motor_valueChanged(int value)
{
    motorValue[0] = value;
    ui->QuadPlus1Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_QuadPlus2Motor_valueChanged(int value)
{
    motorValue[1] = value;
    ui->QuadPlus2Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_QuadPlus3Motor_valueChanged(int value)
{
    motorValue[2] = value;
    ui->QuadPlus3Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_QuadPlus4Motor_valueChanged(int value)
{
    motorValue[3] = value;
    ui->QuadPlus4Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_QuadX1Motor_valueChanged(int value)
{
    motorValue[0] = value;
    ui->QuadX1Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_QuadX2Motor_valueChanged(int value)
{
    motorValue[1] = value;
    ui->QuadX2Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_QuadX3Motor_valueChanged(int value)
{
    motorValue[2] = value;
    ui->QuadX3Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_QuadX4Motor_valueChanged(int value)
{
    motorValue[3] = value;
    ui->QuadX4Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexPlus1Motor_valueChanged(int value)
{
    motorValue[0] = value;
    ui->HexPlus1Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexPlus2Motor_valueChanged(int value)
{
    motorValue[1] = value;
    ui->HexPlus2Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexPlus3Motor_valueChanged(int value)
{
    motorValue[2] = value;
    ui->HexPlus3Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexPlus4Motor_valueChanged(int value)
{
    motorValue[3] = value;
    ui->HexPlus4Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexPlus5Motor_valueChanged(int value)
{
    motorValue[4] = value;
    ui->HexPlus5Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexPlus6Motor_valueChanged(int value)
{
    motorValue[5] = value;
    ui->HexPlus6Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexX1Motor_valueChanged(int value)
{
    motorValue[0] = value;
    ui->HexX1Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexX2Motor_valueChanged(int value)
{
    motorValue[1] = value;
    ui->HexX2Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexX3Motor_valueChanged(int value)
{
    motorValue[2] = value;
    ui->HexX3Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexX4Motor_valueChanged(int value)
{
    motorValue[3] = value;
    ui->HexX4Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexX5Motor_valueChanged(int value)
{
    motorValue[4] = value;
    ui->HexX5Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_HexX6Motor_valueChanged(int value)
{
    motorValue[5] = value;
    ui->HexX6Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoPlus1Motor_valueChanged(int value)
{
    motorValue[0] = value;
    ui->octoPlus1Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoPlus2Motor_valueChanged(int value)
{
    motorValue[1] = value;
    ui->octoPlus2Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoPlus3Motor_valueChanged(int value)
{
    motorValue[2] = value;
    ui->octoPlus3Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoPlus4Motor_valueChanged(int value)
{
    motorValue[3] = value;
    ui->octoPlus4Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoPlus5Motor_valueChanged(int value)
{
    motorValue[4] = value;
    ui->octoPlus5Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoPlus6Motor_valueChanged(int value)
{
    motorValue[5] = value;
    ui->octoPlus6Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoPlus7Motor_valueChanged(int value)
{
    motorValue[6] = value;
    ui->octoPlus7Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoPlus8Motor_valueChanged(int value)
{
    motorValue[7] = value;
    ui->octoPlus8Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoX1Motor_valueChanged(int value)
{
    motorValue[0] = value;
    ui->octoX1Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoX2Motor_valueChanged(int value)
{
    motorValue[1] = value;
    ui->octoX2Value->setText(QString::number(value));
    commandMotors();
}
void PanelMotorCommand::on_octoX3Motor_valueChanged(int value)
{
    motorValue[2] = value;
    ui->octoX3Value->setText(QString::number(value));
    commandMotors();
}
void PanelMotorCommand::on_octoX4Motor_valueChanged(int value)
{
    motorValue[3] = value;
    ui->octoX4Value->setText(QString::number(value));
    commandMotors();
}
void PanelMotorCommand::on_octoX5Motor_valueChanged(int value)
{
    motorValue[4] = value;
    ui->octoX5Value->setText(QString::number(value));
    commandMotors();
}
void PanelMotorCommand::on_octoX6Motor_valueChanged(int value)
{
    motorValue[5] = value;
    ui->octoX6Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoX7Motor_valueChanged(int value)
{
    motorValue[6] = value;
    ui->octoX7Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_octoX8Motor_valueChanged(int value)
{
    motorValue[7] = value;
    ui->octoX8Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_Y61Motor_valueChanged(int value)
{
    motorValue[0] = value;
    ui->Y61Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_Y62Motor_valueChanged(int value)
{
    motorValue[1] = value;
    ui->Y62Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_Y63Motor_valueChanged(int value)
{
    motorValue[2] = value;
    ui->Y63Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_Y64Motor_valueChanged(int value)
{
    motorValue[3] = value;
    ui->Y64Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_Y65Motor_valueChanged(int value)
{
    motorValue[4] = value;
    ui->Y65Value->setText(QString::number(value));
    commandMotors();
}

void PanelMotorCommand::on_Y66Motor_valueChanged(int value)
{
    motorValue[5] = value;
    ui->Y66Value->setText(QString::number(value));
    commandMotors();
}
