#ifndef PANEL_FIRMWARE_H
#define PANEL_FIRMWARE_H

#include <QWidget>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QWebFrame>
#include <QWebElementCollection>
#include <QtWebKitWidgets/QWebView>
#include <QMessageBox>
#include <QProcess>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>
#include <QThread>
#include "communication\communication.h"

namespace Ui {
class PanelFirmware;
}

class PanelFirmware : public QWidget
{
    Q_OBJECT

public:
    explicit PanelFirmware(Communication *commIn);
    ~PanelFirmware();

signals:
    void messageIn(QByteArray);
    void messageOut(QByteArray);
    void panelStatus(QString);

private slots:
    void on_bootloader_clicked();
    void printConsole();
    void printErrors();
    void downloadFirmware(bool);
    void firmwareDownloaded(QNetworkReply *reply);
    void scrollToBottom(int);
    void startFirmwareUpload();

    void on_firmware_clicked();

private:
    Ui::PanelFirmware *ui;
    QProcess *upload;
    void waitForConsole(QString message);
    bool scroll;
    QNetworkAccessManager networkManager;
    Communication *comm;

    void programmingMode(QString port);
    void displayCompleteMessage();
};

#endif // PANEL_FIRMWARE_H
