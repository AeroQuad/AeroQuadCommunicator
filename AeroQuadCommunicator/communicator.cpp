#include "communicator.h"
#include "ui_communicator.h"
#include <QSignalMapper>
#include "panel/connect/menu_connect.h"
#include "panel/monitor/panel_monitor.h"
#include "panel/plot/panel_plot.h"
#include "panel/config/panel_config.h"
#include "panel/firmware/panel_firmware.h"
#include "panel/route/panel_route.h"

/// Add custom panel headers here
#include "panel/example/panel_example.h"

Communicator::Communicator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Communicator)
{
    ui->setupUi(this);
    QMainWindow::addToolBar(Qt::LeftToolBarArea, ui->mainToolBar);

    // Initialize comms, update in the future to choose custom comm type
    comm = new CommunicationSerial;
    connect(comm, SIGNAL(errorMessage(QString)), this, SLOT(getConnectionError(QString)));
    connect(this, SIGNAL(messageOut(QByteArray)), comm, SIGNAL(writeData(QByteArray)));

    // Initialize connection menu
    panel = new MenuConnect;
    //panel = new PanelExample;
    connectionStop = "X";
    setCentralWidget(panel);
    initToolBar();
    connect(this, SIGNAL(autoConnect()), panel, SLOT(autoConnect()));
    connect(panel, SIGNAL(panelStatus(QString)), this, SLOT(updateStatusBar(QString)));
    connect(comm, SIGNAL(readData(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
    connect(panel, SIGNAL(messageOut(QByteArray)), comm, SIGNAL(writeData(QByteArray)));
    connect(panel, SIGNAL(getConnectionState()), comm, SIGNAL(getConnectionState()));
    connect(comm, SIGNAL(connectionState(bool)), panel, SIGNAL(connectionState(bool)));
    connect(panel, SIGNAL(openConnection(QString)), comm, SIGNAL(openConnection(QString)));
    connect(panel, SIGNAL(closeConnection()), comm, SIGNAL(closeConnection()));
    emit autoConnect();

    ui->statusBar->showMessage("Ready to connect to the AeroQuad...", 10000);
    restoreGeometry(settings.value("windowGeometry").toByteArray());
}

// *********************************************************
// To add new menu item:
// 1) Create QT Designer Form Class
// 2) Update initToolBar()
// 3) Update loadPanel()
// *********************************************************

void Communicator::initToolBar()
{
    QSignalMapper *signalMapper = new QSignalMapper(this);

    connect(ui->actionConnect,      SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionStatus,       SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionCalibrate,    SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionSetup,        SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionRoute,        SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionTerminal,     SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionPlots,        SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionFirmware,     SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionTest,         SIGNAL(triggered()), signalMapper, SLOT(map()));

    signalMapper->setMapping(ui->actionTest,        "Test");
    signalMapper->setMapping(ui->actionConnect,     "Connect");
    signalMapper->setMapping(ui->actionStatus,      "Status");
    signalMapper->setMapping(ui->actionRoute,       "Route");
    signalMapper->setMapping(ui->actionCalibrate,   "Calibrate");
    signalMapper->setMapping(ui->actionSetup,       "Setup");
    signalMapper->setMapping(ui->actionTerminal,    "Terminal");
    signalMapper->setMapping(ui->actionPlots,       "Plots");
    signalMapper->setMapping(ui->actionFirmware,    "Firmware");
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(loadPanel(QString))) ;
}

void Communicator::loadPanel(QString panelName)
{
    emit messageOut(connectionStop);
    // Clear out previous panel
    QWidget* clearWidget = QMainWindow::centralWidget();
    clearWidget->setParent(0);
    delete panel;

    // Load new panel
    if (panelName == "Test")
    {
        panel = new PanelExample;
        connect(panel, SIGNAL(panelStatus(QString)), this, SLOT(updateStatusBar(QString)));
        connect(comm, SIGNAL(readData(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
        connect(panel, SIGNAL(messageOut(QByteArray)), comm, SIGNAL(writeData(QByteArray)));
        connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), panel, SIGNAL(initializePanel(QMap<QString,QString>)));
        emit initializePanel(config);
    }
    else if (panelName == "Connect")
    {
        panel = new MenuConnect;
        connect(panel, SIGNAL(panelStatus(QString)), this, SLOT(updateStatusBar(QString)));
        connect(comm, SIGNAL(readData(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
        connect(panel, SIGNAL(messageOut(QByteArray)), comm, SIGNAL(writeData(QByteArray)));
    }
    else if (panelName == "Terminal")
    {
        panel = new PanelMonitor;
        connect(comm, SIGNAL(readData(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
        connect(panel, SIGNAL(messageOut(QByteArray)), comm, SIGNAL(writeData(QByteArray)));
    }
    else if (panelName == "Plots")
    {
        panel = new PanelPlot;
        connect(panel, SIGNAL(panelStatus(QString)), this, SLOT(updateStatusBar(QString)));
        connect(comm, SIGNAL(readData(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
        connect(panel, SIGNAL(messageOut(QByteArray)), comm, SIGNAL(writeData(QByteArray)));
        connect(this, SIGNAL(panelMessage(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
        emit panelMessage("initialize");
    }
    else if (panelName == "Setup")
    {
        panel = new PanelConfig;
        connect(comm, SIGNAL(readData(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
        connect(panel, SIGNAL(messageOut(QByteArray)), comm, SIGNAL(writeData(QByteArray)));
        connect(panel, SIGNAL(panelStatus(QString)), this, SLOT(updateStatusBar(QString)));
        connect(this, SIGNAL(panelMessage(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
        emit panelMessage("initialize");
    }
    else if (panelName == "Firmware")
    {
        panel = new PanelFirmware(comm);
        connect(comm, SIGNAL(readData(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
        connect(panel, SIGNAL(messageOut(QByteArray)), comm, SIGNAL(writeData(QByteArray)));
        connect(panel, SIGNAL(panelStatus(QString)), this, SLOT(updateStatusBar(QString)));
    }
    else if (panelName == "Route")
    {
        panel = new PanelRoute;
        connect(comm, SIGNAL(readData(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
        connect(panel, SIGNAL(messageOut(QByteArray)), comm, SIGNAL(writeData(QByteArray)));
        connect(panel, SIGNAL(panelStatus(QString)), this, SLOT(updateStatusBar(QString)));
        connect(this, SIGNAL(panelMessage(QByteArray)), panel, SIGNAL(messageIn(QByteArray)));
        emit panelMessage("initialize");
    }

    setCentralWidget(panel);
}

Communicator::~Communicator()
{
    settings.setValue("windowGeometry", saveGeometry());
    settings.sync();
    comm->close();
    delete ui;
    delete panel;
}

void Communicator::updateStatusBar(QString message)
{
    ui->statusBar->showMessage(message);
}

void Communicator::getConnectionError(QString error)
{
    updateStatusBar(error);
}
