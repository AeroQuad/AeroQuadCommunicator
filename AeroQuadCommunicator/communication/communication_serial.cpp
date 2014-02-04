#include "communication_serial.h"
#include <QDebug>

CommunicationSerial::CommunicationSerial()
{
    serial = new QSerialPort;
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    isConnected = false;

    connect(this,   SIGNAL(openConnection(QString)),        this, SLOT(open(QString)));
    connect(this,   SIGNAL(writeConnection(QByteArray)),    this, SLOT(write(QByteArray)));
    connect(serial, SIGNAL(readyRead()),                    this, SLOT(read()));
    connect(this,   SIGNAL(closeConnection()),              this, SLOT(close()));
}

CommunicationSerial::~CommunicationSerial()
{
    delete serial;
}

void CommunicationSerial::open(QString connectionSettings)
{
    QStringList settings;
    settings = connectionSettings.split(";");

    serial->close();
    serial->setPortName(settings.at(0));
    serial->setBaudRate(settings.at(1).toInt());
    if (serial->open(QIODevice::ReadWrite))
    {
        isConnected = true;
    }
    else
    {
        emit errorMessage(serial->errorString());
        serial->close();
        isConnected = false;
    }
    emit connectionState(isConnected);
}

void CommunicationSerial::write(const QByteArray &data)
{
    if (serial->write(data) < 0)
    {
        emit errorMessage(serial->errorString());
        serial->close();
        isConnected = false;
        emit connectionState(isConnected);
    }
    else
        qDebug() << data;
}

QByteArray CommunicationSerial::read()
{
    QByteArray data;
    if (isConnected)
    {
        if (serial->canReadLine())
        {
            data = serial->readAll();
        }
        else
        {
            data = serial->readAll();
            data.append(serial->readAll());
        }
        emit readConnection(data);
        emit dataAvailable(data);
        //qDebug() << data;
        return data;
    }
}

void CommunicationSerial::close()
{
    serial->close();
    isConnected = false;
    emit connectionState(isConnected);
}

QString CommunicationSerial::checkError()
{
    return serial->errorString();
}

QList<QSerialPortInfo> getAvailablePorts()
{
    return QSerialPortInfo::availablePorts();
}

bool CommunicationSerial::getConnectionState()
{
    emit connectionState(isConnected);
    return isConnected;
}
