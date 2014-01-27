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

namespace Ui {
class PanelConfig;
}

class PanelConfig : public QWidget
{
    Q_OBJECT

public:
    explicit PanelConfig(QWidget *parent = 0);
    ~PanelConfig();

public slots:
    void selectConfig(int index);
    void selectDescription(int row, int col);
    void updateConfig(QByteArray);

signals:
    void messageIn(QByteArray);
    void messageOut(QByteArray);
    void panelStatus(QString);

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
    struct configuration
    {
        QString configName;
        QString requirement;
        QString command;
        QString telemetry;
        QString updateEEPROM;
        QVector<parameter> parameters;
    };
    QVector<configuration> panelConfig;
    QVector<configuration> defaultPanelConfig;
    int currentPanel;

    void initialize(QString filename);
    void setupConfigList();
    void setupTableHeader();
    void updateParameterValues();
};

#endif // PANEL_CONFIG_H
