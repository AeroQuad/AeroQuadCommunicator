#include "panel_firmware.h"
#include "ui_panel_firmware.h"

PanelFirmware::PanelFirmware(Communication *commIn) :
    ui(new Ui::PanelFirmware)
{
    ui->setupUi(this);
    //ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    //ui->webView->page()->setForwardUnsupportedContent(true);
    comm = commIn;
    scroll = false;
    upload = new QProcess(this);
    connect(upload, SIGNAL(readyReadStandardOutput()), this, SLOT(printConsole()));
    connect(upload, SIGNAL(readyReadStandardError()), this, SLOT(printErrors()));
    connect(ui->webView->page(), SIGNAL(loadStarted()), this, SLOT(startFirmwareUpload()));
    connect(ui->webView->page(), SIGNAL(loadProgress(int)), this, SLOT(scrollToBottom(int)));
    connect(ui->webView->page(), SIGNAL(loadFinished(bool)), this, SLOT(downloadFirmware(bool)));
    connect(&networkManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(firmwareDownloaded(QNetworkReply*)));

    QFont consoleFont = ui->console->font();
    consoleFont.setPointSize(12);
    ui->console->setFont(consoleFont);

    ui->console->append("To return the board it's default factory configuration, select the Bootloader button.  To upload existing firmware, select the Firmware button.\n\nTo create and upload new firmware, complete the selections below.  You will need to be connected to the internet for this feature.");
}

PanelFirmware::~PanelFirmware()
{
    delete ui;
}

void PanelFirmware::waitForConsole(QString message)
{
    bool status = true;
    int delay = 250;
    int timeout = 0;
    QString progress = " . ";
    while (status)
    {
        status = !upload->waitForFinished(delay);
        if (timeout > 60000)
        {
            emit panelStatus("Error uploading to board...");
            status = false;
        }
        else
        {
            progress += ". ";
            emit panelStatus(message + progress);
            timeout += delay;
            if (progress.size() > 100)
                progress = " ";
        }
    }
}

void PanelFirmware::on_bootloader_clicked()
{
    QMessageBox message;
    message.setText("Disconnect the USB cable from the board, connect BOOT0 to 3.3V, then re-connect the USB cable to place the board into programming mode.");
    message.setInformativeText("In addition to loading the bootloader, the default flight software will be loaded.\n\nPlease note, re-loading the bootloader is only need if you cannot communicate with your board using USB.  If you simply want update the flight software on your board, please select the Firmware button instead.\n\nDuring the upload process, the application may appear to be unresponsive.  Please be patient while the bootloader and firmware are loaded.");
    message.setIconPixmap(QPixmap(":/images/resources/AQ32v2.jpg"));
    message.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    message.setDefaultButton(QMessageBox::Ok);
    int response = message.exec();
    if (response==QMessageBox::Ok)
    {
        comm->close();
        ui->console->clear();

        upload->start("dfu-util.exe -l --reset --device 0483:df11 --alt 0 --dfuse-address 0x08000000 --download prod_image_256k.bin");
        waitForConsole("Uploading bootloader");

        upload->start("dfu-util.exe -l --reset --device 0483:df11 --alt 0 --dfuse-address 0x08010000 --download AeroQuad32Production.bin");
        waitForConsole("Uploading flight software");

        emit panelStatus("Bootloader and flight software upload complete.");
        QMessageBox complete;
        complete.setModal(true);
        complete.setText("To complete the upload process, disconnect the USB cable, then disconnect BOOT0 from 3.3V.\n\nYou will now be able to communicate with your board using USB only.");
        complete.exec();
    }
}

void PanelFirmware::printConsole()
{
    QString data = QString(upload->readAllStandardOutput());
    if (data == ".")
        return;
    else
        ui->console->append(data);
}

void PanelFirmware::printErrors()
{
    QString data = QString(upload->readAllStandardError());
    if (data.contains("No valid DFU") || data.contains("Warning:File") || data.contains("can't detach"))
        return;
    else
     ui->console->append(data);
}

void PanelFirmware::downloadFirmware(bool complete)
{
    QString fileRef;
    QWebElementCollection elements = ui->webView->page()->mainFrame()->findAllElements("a");

    foreach (QWebElement element, elements) {
        QStringList fileCheck = element.toPlainText().split(".");
        if (fileCheck[0] == "AeroQuad32")
        {
            QStringList fileRefCheck = element.toOuterXml().split('"');
            fileRef = fileRefCheck[1];
        }
    }
    if (fileRef == "")
        emit panelStatus("Select desired flight configuration options.");
    else
    {
        emit panelStatus("Firmware compiled, please wait as it is being uploaded to the flight control board...");

        QUrl firmwareUrl("http://aeroquad.hautio.net"+fileRef);
        QNetworkRequest request(firmwareUrl);
        networkManager.get(request);
    }
}


void PanelFirmware::startFirmwareUpload()
{
    if (!scroll)
        emit panelStatus("Please wait, firmware compilation may take a few minutes...");
}

void PanelFirmware::firmwareDownloaded(QNetworkReply *reply)
{
    QSettings settings;
    QString path = settings.value("firmwarePath", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/AeroQuad32Firmware.bin").toString();
    QString fileName = QFileDialog::getSaveFileName(this, "Select location to save firmware.", path, "Firmware (*.bin)");
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(reply->readAll());
        file.flush();
        emit panelStatus("Firmware downloaded...");
        settings.setValue("firmwarePath", fileName);
        settings.sync();
        comm->close();
        programmingMode("Com3");
        upload->start("dfu-util.exe -l --reset --device 0483:df11 --alt 0 --dfuse-address 0x08010000 --download " + fileName);
        waitForConsole("Uploading flight software");
        QMessageBox complete;
        complete.setModal(true);
        complete.setText("Firmware upload complete.  Press the reset button or disconnect the USB cable to restart the board.");
        complete.exec();
    }
    else
        emit panelStatus("Firmware download not successful.");
    file.close();
    reply->deleteLater();
}

void PanelFirmware::scrollToBottom(int row)
{
    if (ui->webView->page()->findText("getting source"))
        scroll = true;
    if (scroll)
        ui->webView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, ui->webView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
}

void PanelFirmware::on_firmware_clicked()
{
    ui->console->clear();
    QSettings settings;
    QString path = settings.value("firmwarePath", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/AeroQuad32Firmware.bin").toString();
    QString fileName = QFileDialog::getOpenFileName(this, "Select location to load firmware.", path, "Firmware (*.bin)");
    if (fileName != "")
    {
        emit panelStatus("Uploading firmware...");
        settings.setValue("firmwarePath", fileName);
        settings.sync();
        comm->close();
        programmingMode("Com3");
        upload->start("dfu-util.exe -l --reset --device 0483:df11 --alt 0 --dfuse-address 0x08010000 --download \"" + fileName + "\"");
        waitForConsole("Uploading flight software");
        displayCompleteMessage();
    }
    else
        emit panelStatus("Firmware upload not successful.");

}

void PanelFirmware::programmingMode(QString port)
{
    upload->execute("cmd /c mode " + port + ":DTR=off");
    upload->execute("cmd /c mode " + port + ":DTR=on");
    upload->execute("cmd /c mode " + port + ":DTR=off");
    upload->execute("cmd /c echo 1EAF >" + port + ":");
    QThread::msleep(3000);
}


void PanelFirmware::displayCompleteMessage()
{
    QMessageBox complete;
    complete.setModal(true);
    complete.setText("To complete the upload process, disconnect the USB cable, then disconnect BOOT0 from 3.3V.\n\nYou will now be able to communicate with your board using USB only.");
    complete.exec();
}
