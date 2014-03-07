#ifndef MENU_CONNECT_H
#define MENU_CONNECT_H

#include <QWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QSettings>
#include <QThread>
#include <QGraphicsView>
#include <QGraphicsScene>
//#include "communication\communication.h"
#include <QMap>

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
     */
    explicit MenuConnect(QWidget *parent = 0);
    ~MenuConnect();
    /// ********************************************************
    bool connectState;
    QMap<QString, QString> configuration;
    void sendMessage(QString message) {emit messageOut(message.toUtf8());}
    /// ********************************************************
    QStringList getCommPorts();
    QStringList getBaudRates();
    QSettings settings;
    QString storedPort;

    // Should read these in from file
    QByteArray connectionTest;
    QByteArray connectionStop;
    QByteArray configRequest;
    double compatibleVersion;

public slots:
    void autoConnect();
    void initialize(QMap<QString, QString> config);
    void updateConnectionState(bool state);
    void parseMessage(QByteArray);

signals:
    void setConnection(QString);
    void openConnection(QString);
    void closeConnection();
    void initializePanel(QMap<QString, QString>);
    void messageIn(QByteArray);
    void messageOut(QByteArray);
    void panelStatus(QString);
    void connectionState(bool);
    void getConnectionState();
    void writeConfig(QMap<QString, QString>);

private slots:
    void on_connectPushButton_clicked();
    void on_disconnectPushButton_clicked();
    void on_portComboBox_currentIndexChanged(const QString &arg1);

    void on_refresh_clicked();

private:
    Ui::MenuConnect *ui;
    QByteArray messageType;
    int retry;
    QPixmap configPix;
    QMap<QString, QString> flightConfigs;
    QString flightConfig;
    QGraphicsScene *scene;
    void showEvent(QShowEvent *);
    void resizeEvent(QResizeEvent *);
    void setConnectionState(bool state);
};

#endif // MENU_CONNECT_H
