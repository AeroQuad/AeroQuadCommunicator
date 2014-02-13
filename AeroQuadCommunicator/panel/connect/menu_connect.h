#ifndef MENU_CONNECT_H
#define MENU_CONNECT_H

#include <QWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QSettings>
#include <QThread>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "communication\communication.h"

namespace Ui {
class MenuConnect;
}

/**
 * @brief Defines a panel that establishes communication to the AQ board.
 */
class MenuConnect : public QWidget
{
    Q_OBJECT
    
public:
    /**
     * @brief Constructor that initializes communication to AQ board.
     * @param commIn - Class used for communication to AQ board.
     */
    explicit MenuConnect(Communication *commIn);
    ~MenuConnect();
    QStringList getCommPorts();
    QStringList getBaudRates();
    QSettings settings;
    QString storedPort;

    // Should read these in from file
    QByteArray connectionTest;
    QByteArray connectionStop;
    QByteArray configuration;
    double compatibleVersion;

public slots:
    void autoConnect();
    void connectionResponse(QByteArray);

signals:
    void messageIn(QByteArray);
    void messageOut(QByteArray);
    void setConnection(QString);
    void open(QString);
    void close(QString);
    void panelStatus(QString);

private slots:
    void on_connectPushButton_clicked();
    void on_disconnectPushButton_clicked();
    void on_portComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::MenuConnect *ui;
    bool connectState;
    Communication *comm;
    QByteArray messageSent;
    int retry;
    QMap<QString, QString> flightConfigs;
    QPixmap configPix;
    QString flightConfig;
    QGraphicsScene *scene;
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);

    void setConnectionState(bool state);
};

#endif // MENU_CONNECT_H
