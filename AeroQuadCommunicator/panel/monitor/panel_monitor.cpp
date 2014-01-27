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
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(displayCommData(QByteArray)));
    connect(ui->commandLine, SIGNAL(returnPressed()), this, SLOT(on_sendButton_clicked()));
}

PanelMonitor::~PanelMonitor()
{
    delete ui;
}

void PanelMonitor::displayCommData(QByteArray data)
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
