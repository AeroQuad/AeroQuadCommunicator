#ifndef PANEL_CONFIG_H
#define PANEL_CONFIG_H

#include <QWidget>
#include <QFile>
#include <QXmlStreamReader>
#include <QVector>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QDebug>
#include <QTableWidgetItem>
#include <QMap>

namespace Ui {
class PanelConfig;
}

class PanelConfig : public QWidget
{
    Q_OBJECT

public:
    explicit PanelConfig(QWidget *parent = 0);
    ~PanelConfig();
    bool connectState;
    QMap<QString, QString> configuration;
    void sendMessage(QString message) {emit messageOut(message.toUtf8());}

public slots:
    void selectConfig(int index);
    void selectDescription(int row, int col);
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
    void on_defaultButton_clicked();
    void on_uploadButton_clicked();

private:
    Ui::PanelConfig *ui;
    struct parameter
    {
        QString name;
        QString value;
        QString description;
    };
    struct cfg
    {
        QString configName;
        QString requirement;
        QString command;
        QString telemetry;
        QString updateEEPROM;
        QVector<parameter> parameters;
    };
    QVector<cfg> panelConfig;
    QVector<cfg> defaultPanelConfig;
    int currentPanel;

    void readXML(QString filename);
    void setupConfigList();
    void setupTableHeader();
    void updateParameterValues();
};

#endif // PANEL_CONFIG_H
