#include "panel_example.h"
#include "ui_panel_example.h"
#include <QDebug>

PanelExample::PanelExample(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelExample)
{
    ui->setupUi(this);
    /// **********************************************************************
    /// Add the following lines below to your custom panel class
    connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
    connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));
    /// **********************************************************************
}

PanelExample::~PanelExample()
{
    delete ui;
}

/// **********************************************************************
/// Add the following lines below to your custom panel class
/// Make sure to change PanelExample to the class name of your custom panel
void PanelExample::initialize(QMap<QString, QString> config)
{
    configuration = config;
    emit getConnectionState();

    // Add any additional initialization steps to perform here.
    // Remove these lines when copying to another custom panel
    int rowCount = configuration.size();
    ui->config->setRowCount(rowCount);
    ui->config->setColumnCount(2);
    ui->config->setHorizontalHeaderItem(0, new QTableWidgetItem("Config Type"));
    ui->config->setHorizontalHeaderItem(1, new QTableWidgetItem("Value"));
    QStringList type = configuration.keys();
    QStringList value = configuration.values();
    for (int row=0; row<rowCount; row++)
    {
        ui->config->setItem(row, 0, new QTableWidgetItem(type.at(row)));
        ui->config->setItem(row, 1, new QTableWidgetItem(value.at(row)));
    }
}

void PanelExample::parseMessage(QByteArray data)
{
    QString incomingMessage = data;

    // Replace the following line as required in your custom panel
    ui->exampleResponse->setText(incomingMessage);

    // Example of how to show message on status panel of GUI
    emit panelStatus("Flight software version " + incomingMessage);

    // If you need an example of how to parse multiple telemetry requests,
    // look at panel_route.h and panel_route.cpp

    // If you need an example of how to parse telemetry continuously,
    // Look at panel_plot.h and panel_plot.cpp
}
/// **********************************************************************


void PanelExample::on_exampleCommand_clicked()
{
    // Example of how to send message to the Communication class
    sendMessage("!");
}
