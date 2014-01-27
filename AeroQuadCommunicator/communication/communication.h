#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QByteArray>
#include <QString>

class Communication : public QObject
{
    Q_OBJECT
public:
    Communication();
    bool isConnected;
    virtual QString checkError() = 0;

public slots:
    virtual void open(QString connectionSettings) = 0;
    virtual void write(const QByteArray &data) = 0;
    virtual QByteArray read() = 0;
    virtual void close() = 0;
    virtual bool getConnectionState() = 0;

signals:
    void openConnection(QString);
    void readConnection(QByteArray);
    void writeConnection(QByteArray);
    void closeConnection();
    void errorMessage(QString);
    void connectionState(bool);
    void dataAvailable(QByteArray);
};

#endif // COMMUNICATION_H
