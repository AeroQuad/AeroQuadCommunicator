#ifndef PANEL_MOTORCOMMAND_H
#define PANEL_MOTORCOMMAND_H

#include <QWidget>
#include <QMap>

namespace Ui {
class PanelMotorCommand;
}

class PanelMotorCommand : public QWidget
{
    Q_OBJECT

public:
    explicit PanelMotorCommand(QWidget *parent = 0);
    ~PanelMotorCommand();
    bool connectState;
    QMap<QString, QString> configuration;
    void sendMessage(QString message) {emit messageOut(message.toUtf8());}

public slots:
    void initialize(QMap<QString, QString> config);
    void updateConnectionState(bool state) {connectState = state;}
    void parseMessage(QByteArray);

signals:
    void initializePanel(QMap<QString, QString>);
    void messageIn(QByteArray);
    void messageOut(QByteArray);
    void panelStatus(QString);
    void connectionState(bool);
    void getConnectionState();

private slots:
    void on_stopMotors_clicked();
    void on_QuadPlus1Motor_valueChanged(int value);
    void on_QuadPlus2Motor_valueChanged(int value);
    void on_QuadPlus3Motor_valueChanged(int value);
    void on_QuadPlus4Motor_valueChanged(int value);
    void on_QuadX1Motor_valueChanged(int value);
    void on_QuadX2Motor_valueChanged(int value);
    void on_QuadX3Motor_valueChanged(int value);
    void on_QuadX4Motor_valueChanged(int value);
    void on_HexPlus1Motor_valueChanged(int value);
    void on_HexPlus2Motor_valueChanged(int value);
    void on_HexPlus3Motor_valueChanged(int value);
    void on_HexPlus4Motor_valueChanged(int value);
    void on_HexPlus5Motor_valueChanged(int value);
    void on_HexPlus6Motor_valueChanged(int value);
    void on_HexX1Motor_valueChanged(int value);
    void on_HexX2Motor_valueChanged(int value);
    void on_HexX3Motor_valueChanged(int value);
    void on_HexX4Motor_valueChanged(int value);
    void on_HexX5Motor_valueChanged(int value);
    void on_HexX6Motor_valueChanged(int value);
    void on_octoPlus1Motor_valueChanged(int value);
    void on_octoPlus2Motor_valueChanged(int value);
    void on_octoPlus3Motor_valueChanged(int value);
    void on_octoPlus4Motor_valueChanged(int value);
    void on_octoPlus5Motor_valueChanged(int value);
    void on_octoPlus6Motor_valueChanged(int value);
    void on_octoPlus7Motor_valueChanged(int value);
    void on_octoPlus8Motor_valueChanged(int value);
    void on_octoX1Motor_valueChanged(int value);
    void on_octoX2Motor_valueChanged(int value);
    void on_octoX3Motor_valueChanged(int value);
    void on_octoX4Motor_valueChanged(int value);
    void on_octoX5Motor_valueChanged(int value);
    void on_octoX6Motor_valueChanged(int value);
    void on_octoX7Motor_valueChanged(int value);
    void on_octoX8Motor_valueChanged(int value);
    void on_Y61Motor_valueChanged(int value);
    void on_Y62Motor_valueChanged(int value);
    void on_Y63Motor_valueChanged(int value);
    void on_Y64Motor_valueChanged(int value);
    void on_Y65Motor_valueChanged(int value);
    void on_Y66Motor_valueChanged(int value);

private:
    Ui::PanelMotorCommand *ui;
    void commandMotors();
    int motorCount;
    QList<int> motorValue;
};

#endif // PANEL_MOTORCOMMAND_H
