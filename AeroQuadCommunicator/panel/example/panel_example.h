#ifndef PANEL_EXAMPLE_H
#define PANEL_EXAMPLE_H

#include <QWidget>

/// *****************************************************
/// Add the include line below to your custom panel class
#include <QMap>
/// *****************************************************

namespace Ui {
class PanelExample;
}

class PanelExample : public QWidget
{
    Q_OBJECT

public:
    explicit PanelExample(QWidget *parent = 0);
    ~PanelExample();

private:
    Ui::PanelExample *ui;

/// ********************************************************
/// Add the following lines below to your custom panel class
public:
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
/// ********************************************************

private slots:
    void on_exampleCommand_clicked();
};

#endif // PANEL_EXAMPLE_H
