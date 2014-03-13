#include "menu_connect.h"
#include "ui_menu_connect.h"
#include <QDebug>

MenuConnect::MenuConnect(QWidget *parent) :
    QWidget (parent),
    ui(new Ui::MenuConnect)
{
    ui->setupUi(this);

    connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
    connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));

    ui->portComboBox->addItems(getCommPorts());
    ui->portComboBox->insertSeparator(2);
    ui->baudComboBox->addItems(getBaudRates());
    storedPort = settings.value("SerialCom/port", "Select com port...").toString();
    int indexPort = ui->portComboBox->findText(storedPort);
    indexPort = (indexPort < 0) ? 0 : indexPort;
    ui->portComboBox->setCurrentIndex(indexPort);

    QString storedBaud = settings.value("SerialCom/baud", "Select baud rate...").toString();
    storedBaud = (storedBaud == "") ? "115200" : storedBaud;
    int indexBaud = ui->baudComboBox->findText(storedBaud);
    if (indexBaud >= 0)
        ui->baudComboBox->setCurrentIndex(indexBaud);
    else
    {
        ui->baudComboBox->insertItem(1, storedBaud);
        ui->baudComboBox->setCurrentIndex(1);
    }
    ui->baudComboBox->insertSeparator(1);
    scene = new QGraphicsScene();

    // Should read these in from file
    connectionTest = "!";
    connectionStop = "X";
    configRequest = "#";
    compatibleVersion = 3.2;
    flightConfigs["Quad +"] =   ":/flightConfigs/resources/Quad+.png";
    flightConfigs["Quad X"] =   ":/flightConfigs/resources/QuadX.png";
    flightConfigs["Quad Y4"] =  ":/flightConfigs/resources/Tri.png";
    flightConfigs["Tri"] =      ":/flightConfigs/resources/Tri.png";
    flightConfigs["Hex +"] =    ":/flightConfigs/resources/Hexa+.png";
    flightConfigs["Hex X"] =    ":/flightConfigs/resources/HexaX.png";
    flightConfigs["Hex Y6"] =   ":/flightConfigs/resources/Y6.png";
    flightConfigs["Octo X8"] =  ":/flightConfigs/resources/QuadX.png";
    flightConfigs["Octo X"] =   ":/flightConfigs/resources/OctoX.png";
    flightConfigs["Octo +"] =   ":/flightConfigs/resources/Octo+.png";
    flightConfigs["Rover"] =    ":/flightConfigs/resources/QuadX.png";
}

void MenuConnect::initialize(QMap<QString, QString> config)
{
    configuration = config;
    emit getConnectionState();
    on_refresh_clicked();
}

void MenuConnect::showEvent(QShowEvent *)
{
    // Load default graphics on screen
    int width = ui->graphicsView->width()-25;
    scene = new QGraphicsScene();
    QPixmap logo(":/images/resources/AeroQuadLogo3.png");
    QPixmap scaledLogo = logo.scaled(width, width, Qt::KeepAspectRatio);
    scene->addPixmap(scaledLogo);
    ui->graphicsView->setScene(scene);
}

void MenuConnect::resizeEvent(QResizeEvent *event)
{
    int width = ui->graphicsView->width()-25;
    int height = ui->graphicsView->height()-25;
    int size = (width > height) ? height : width;
    scene = new QGraphicsScene();
    if (configPix.isNull())
        configPix = QPixmap(":/images/resources/AeroQuadLogo3.png");
    QPixmap scaledLogo = configPix.scaled(size, size, Qt::KeepAspectRatio);
    scene->addPixmap(scaledLogo);
    ui->graphicsView->setScene(scene);
}

MenuConnect::~MenuConnect()
{
    delete ui;
}

QStringList MenuConnect::getCommPorts()
{
    QStringList ports;
    ports << "Select com port..." << "Refresh List"; //<< "Autoconnect";
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        // Keep this parsing of port info for future use
        QStringList list;
        list << info.portName()
             << info.description()
             << info.manufacturer()
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString())
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : QString());

        ports << info.portName();
    }
    return ports;
}

QStringList MenuConnect::getBaudRates()
{
    QStringList baudRates;
    baudRates << "Select baud rate..."
              << "1200"
              << "9600"
              << "19200"
              << "38400"
              << "57600"
              << "115200";
    return baudRates;
}

void MenuConnect::on_portComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Refresh List")
    {
        ui->portComboBox->clear();
        ui->portComboBox->addItems(getCommPorts());
        ui->portComboBox->insertSeparator(2);
        ui->portComboBox->setCurrentIndex(0);
    }
}

void MenuConnect::autoConnect()
{
    emit panelStatus("Attempting autoconnect...");
    if (storedPort == ui->portComboBox->currentText())
        on_connectPushButton_clicked();
}

void MenuConnect::on_connectPushButton_clicked()
{
    settings.setValue("SerialCom/port", ui->portComboBox->currentText());
    settings.setValue("SerialCom/baud", ui->baudComboBox->currentText());
    QString connectString = ui->portComboBox->currentText() + ";" + ui->baudComboBox->currentText();

    emit openConnection(connectString);
    sendMessage(connectionTest);
    messageType = connectionTest;
    setConnectionState(false);
    // Now wait for connectionResponse() signal
}

void MenuConnect::setConnectionState(bool state)
{
    if (state)
    {
        ui->baudComboBox->setEnabled(false);
        ui->portComboBox->setEnabled(false);
        ui->connectPushButton->setEnabled(false);
        ui->disconnectPushButton->setEnabled(true);

    }
    else
    {
        ui->baudComboBox->setEnabled(true);
        ui->portComboBox->setEnabled(true);
        ui->connectPushButton->setEnabled(true);
        ui->disconnectPushButton->setEnabled(false);
        emit panelStatus("Not connected to AeroQuad Flight Control Board");
    }
}

void MenuConnect::updateConnectionState(bool state)
{
    connectState = state;
    setConnectionState(connectState);
}

void MenuConnect::parseMessage(QByteArray dataIn)
{
    if (messageType == connectionTest)
    {
        QString version = dataIn;
        if (version.toDouble() >= compatibleVersion)
        {
            setConnectionState(true);
            emit panelStatus("Connected to AeroQuad Flight Software v" + version);
            sendMessage(configRequest);
            messageType = configRequest;
            retry = 0;
        }
        else
            on_disconnectPushButton_clicked();
    }
    if (messageType == configRequest)
    {
        ui->configList->clear();
        QString configData = dataIn;
        QStringList configItems = configData.split("\n");
        int count = configItems[0].toInt() + 1;
        configItems.removeFirst();
        qDebug() << count << configItems.size();
        if ((count == configItems.size()))
        {
            ui->configList->addItems(configItems);
            for (int index=0; index<configItems.size()-1; index++)
            {
                QStringList configItem = configItems[index].split(":");
                configuration[configItem[0]] = configItem[1].trimmed();
            }
            configPix = QPixmap(configuration["Flight Config"]);
            QResizeEvent *event;
            resizeEvent(event);
            emit writeConfig(configuration);
        }
    }
}

void MenuConnect::on_disconnectPushButton_clicked()
{
    emit closeConnection();
    emit getConnectionState();
    ui->configList->clear();
}

void MenuConnect::on_refresh_clicked()
{
    sendMessage(configRequest);
    messageType = configRequest;
}
