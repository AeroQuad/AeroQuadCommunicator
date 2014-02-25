#include "communication_serial.h"
#include <QDebug>

CommunicationSerial::CommunicationSerial()
{

    serial = new QSerialPort;
    isConnected = false;
    dataBuffer.clear();

    connect(this,   SIGNAL(openConnection(QString)),        this, SLOT(open(QString)));
    connect(this,   SIGNAL(writeConnection(QByteArray)),    this, SLOT(write(QByteArray)));
    connect(serial, SIGNAL(readyRead()),                    this, SLOT(read()));
    connect(this,   SIGNAL(closeConnection()),              this, SLOT(close()));
}

CommunicationSerial::~CommunicationSerial()
{
    serial->close();
    delete serial;
}

void CommunicationSerial::open(QString connectionSettings)
{
    QStringList settings;
    settings = connectionSettings.split(";");

    serial->close();
    serial->setPortName(settings.at(0));
    if (serial->open(QIODevice::ReadWrite))
    {
        isConnected = true;
        serial->setBaudRate(settings.at(1).toInt());
        serial->setDataBits(QSerialPort::Data8);
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->setTextModeEnabled(true);
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
    if (isConnected)
    {
        bool overflow = dataBuffer.size()>64;
        if (serial->canReadLine())
        {
            while (serial->canReadLine())
                dataBuffer.append(serial->readLine());
            emit readConnection(dataBuffer);
            emit dataAvailable(dataBuffer);
            QByteArray data = dataBuffer;
            dataBuffer.clear();
            return data;
        }
        if (overflow)
        {
            QByteArray data = dataBuffer;
            dataBuffer.clear();
            return data;
        }
        else
            dataBuffer.append(serial->readAll());
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
