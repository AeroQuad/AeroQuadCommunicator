#ifndef PANEL_PLOT_H
#define PANEL_PLOT_H

#include <QWidget>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QListWidgetItem>
#include <QMap>

namespace Ui {
class PanelPlot;
}

class PanelPlot : public QWidget
{
    Q_OBJECT
    
public:
    explicit PanelPlot(QWidget *parent = 0);
    ~PanelPlot();
    bool connectState;
    QMap<QString, QString> configuration;
    void sendMessage(QString message) {emit messageOut(message.toUtf8());}


public slots:
    void initializePlot(int index);
    void setupPlotNames(int index);
    void selectPlot(QListWidgetItem* item);

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

private:
    Ui::PanelPlot *ui;
    struct plot
    {
        QString name;
        QString requirement;
        QString telemetry;
        QVector<QString> plotNames;
    };
    QVector<plot> plots;

    int plotSize;
    int currentPlot;
    double scale;
    double offset;
    int timeAxisLength;
    QVector<QString> colorNames;

    void readXML(QString filename);
    void setupPlotList();
};

#endif // PANEL_PLOT_H
