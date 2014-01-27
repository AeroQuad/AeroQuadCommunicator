#ifndef PANEL_PLOT_H
#define PANEL_PLOT_H

#include <QWidget>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QListWidgetItem>

namespace Ui {
class PanelPlot;
}

class PanelPlot : public QWidget
{
    Q_OBJECT
    
public:
    explicit PanelPlot(QWidget *parent = 0);
    ~PanelPlot();


public slots:
    void displayCommData(QByteArray data);
    void initializePlot(int index);
    void setupPlotNames(int index);
    void selectPlot(QListWidgetItem* item);

signals:
    void messageIn(QByteArray);
    void messageOut(QByteArray);
    void panelStatus(QString);

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

    void initialize(QString filename);
    void setupPlotList();
};

#endif // PANEL_PLOT_H
