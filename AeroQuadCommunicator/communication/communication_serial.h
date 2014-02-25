#ifndef COMMUNICATION_SERIAL_H
#define COMMUNICATION_SERIAL_H

#include "communication.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QThread>
#include <QStringList>

class CommunicationSerial : public Communication
{
    Q_OBJECT

public:
    CommunicationSerial();
    ~CommunicationSerial();
    QString checkError();

public slots:
    void open(QString connectionSettings);
    void write(const QByteArray &data);
    QByteArray read();
    void close();
    bool getConnectState();

signals:
    void openConnection(QString);
    void readData(QByteArray);
    void writeData(QByteArray);
    void closeConnection();
    void errorMessage(QString);
    void connectionState(bool);
    void getConnectionState();
    //void dataAvailable(QByteArray);

private:
    QSerialPort *serial;
    QByteArray dataBuffer;
    QList<QSerialPortInfo> getAvailablePorts();
};

#endif // COMMUNICATION_SERIAL_H
