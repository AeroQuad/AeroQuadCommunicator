#include "panel_monitor.h"
#include "ui_panel_monitor.h"

PanelMonitor::PanelMonitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelMonitor)
{
    ui->setupUi(this);
    ui->console->setReadOnly(true);
    ui->console->setMaximumBlockCount(1024);
    ui->console->setWordWrapMode(QTextOption::NoWrap);
    connect(ui->commandLine, SIGNAL(returnPressed()), this, SLOT(on_sendButton_clicked()));
    connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
    connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));
}

PanelMonitor::~PanelMonitor()
{
    delete ui;
}

void PanelMonitor::initialize(QMap<QString, QString> config)
{
    configuration = config;
    emit getConnectionState();
}

void PanelMonitor::parseMessage(QByteArray data)
{
    ui->console->insertPlainText(data);
    ui->console->ensureCursorVisible();
}

void PanelMonitor::on_sendButton_clicked()
{
    QByteArray message;
    message.append(ui->commandLine->text());
    emit messageOut(message);
    ui->commandLine->clear();
    ui->commandLine->setFocus();
}

void PanelMonitor::on_clearButton_clicked()
{
    ui->console->clear();
}

void PanelMonitor::updateConnectionState(bool state)
{
    connectState = state;
    ui->sendButton->setEnabled(connectState);
    ui->clearButton->setEnabled(connectState);
}
