#ifndef PANEL_MONITOR_H
#define PANEL_MONITOR_H

#include <QWidget>

namespace Ui {
class PanelMonitor;
}

class PanelMonitor : public QWidget
{
    Q_OBJECT
    
public:
    explicit PanelMonitor(QWidget *parent = 0);
    ~PanelMonitor();

public slots:
    void displayCommData(QByteArray data);
    
signals:
    void messageIn(QByteArray);
    void messageOut(QByteArray);

private slots:
    void on_sendButton_clicked();
    void on_clearButton_clicked();

private:
    Ui::PanelMonitor *ui;
};

#endif // PANEL_MONITOR_H
