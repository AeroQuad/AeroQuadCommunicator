#ifndef PANEL_MONITOR_H
#define PANEL_MONITOR_H

#include <QWidget>
#include <QMap>

namespace Ui {
class PanelMonitor;
}

class PanelMonitor : public QWidget
{
    Q_OBJECT
    
public:
    explicit PanelMonitor(QWidget *parent = 0);
    ~PanelMonitor();
    bool connectState;
    QMap<QString, QString> configuration;
    void sendMessage(QString message) {emit messageOut(message.toUtf8());}

public slots:
    void initialize(QMap<QString, QString> config);
    void updateConnectionState(bool state);
    void parseMessage(QByteArray);

signals:
    void initializePanel(QMap<QString, QString>);
    void messageIn(QByteArray);
    void messageOut(QByteArray);
    void panelStatus(QString);
    void connectionState(bool);
    void getConnectionState();

private slots:
    void on_sendButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::PanelMonitor *ui;
};

#endif // PANEL_MONITOR_H
