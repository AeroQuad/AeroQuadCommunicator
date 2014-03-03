#include "panel_plot.h"
#include "ui_panel_plot.h"
#include <QDebug>
PanelPlot::PanelPlot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelPlot)
{
    connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
    connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));

    timeAxisLength = 15; // seconds
    ui->setupUi(this);
    ui->plotWindow->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->plotWindow->xAxis->setDateTimeFormat("hh:mm:ss");
    ui->plotWindow->xAxis->setAutoTickStep(false);
    ui->plotWindow->xAxis->setTickStep(2);
    ui->plotWindow->axisRect()->setupFullAxesBox();
    ui->plotWindow->autoAddPlottableToLegend();
    ui->plotWindow->legend->setVisible(true);
    ui->plotWindow->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);
    ui->plotWindow->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    //Should we read this from file?
    colorNames.append("blue");
    colorNames.append("red");
    colorNames.append("green");
    colorNames.append("orange");
    colorNames.append("purple");
    colorNames.append("brown");
    colorNames.append("light blue");
    colorNames.append("light red");
    colorNames.append("lime");
    colorNames.append("violet");

    //setupPlotNames(0);
    connect(ui->plotWindow->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotWindow->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plotWindow->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotWindow->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plotTypes, SIGNAL(currentRowChanged(int)), this, SLOT(initializePlot(int)));
    connect(ui->plotTypes, SIGNAL(currentRowChanged(int)), this, SLOT(setupPlotNames(int)));
    connect(ui->plotNames, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPlot(QListWidgetItem*)));
}

PanelPlot::~PanelPlot()
{
    delete ui;
}

void PanelPlot::initialize(QMap<QString, QString> config)
{
    configuration = config;
    emit getConnectionState();
    readXML("panel_plot.xml");
    setupPlotList();
    initializePlot(0);
    setupPlotNames(0);
}

void PanelPlot::readXML(QString filename)
{
    // Open XML configuration file
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "AeroQuad Communicator", "Unable to find configuration file:\n" + QCoreApplication::applicationDirPath() + "/" +  filename, QMessageBox::Ok);
        file->close();
        return;
    }

    // Read XML file and populate data storage
    plots.clear();
    QXmlStreamReader xml(file);
    while (!xml.atEnd() && !xml.hasError())
    {
        xml.readNext();
        if (xml.isStartElement() && (xml.name() == "Plot"))
        {
            plot plotItem;
            plotItem.name = xml.attributes().value("Name").toString();
            xml.readNextStartElement();
            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "Plot"))
            {
                if(xml.tokenType() == QXmlStreamReader::StartElement)
                {
                    if(xml.name() == "Requirement")
                        plotItem.requirement = xml.readElementText();
                    if(xml.name() == "Telemetry")
                        plotItem.telemetry = xml.readElementText();
                    if(xml.name() == "PlotName")
                        plotItem.plotNames.append(xml.readElementText());
                }
                xml.readNextStartElement();
            }
            plots.append(plotItem);
        }
    }
    if (xml.hasError())
    {
        QMessageBox::warning(NULL,"Error",xml.errorString(),QMessageBox::Ok);
    }
    file->close();
}


void PanelPlot::setupPlotList()
{
    for (int index=0; index < plots.size(); index++)
        ui->plotTypes->addItem(plots[index].name);
    ui->frame->setMaximumWidth(ui->plotTypes->sizeHintForColumn(0)+25);
    ui->plotTypes->setCurrentRow(0);
}

void PanelPlot::setupPlotNames(int index)
{
    ui->plotNames->clear();
    for (int nameIndex=0; nameIndex<plots[currentPlot].plotNames.size(); nameIndex++)
    {
        ui->plotNames->addItem(plots[currentPlot].plotNames[nameIndex]);
        ui->plotNames->item(nameIndex)->setCheckState(Qt::Checked);
    }
    if (ui->plotNames->sizeHintForColumn(0) > ui->frame->maximumWidth())
        ui->frame->setMaximumWidth(ui->plotNames->sizeHintForColumn(0)+25);
    ui->plotNames->setCurrentRow(0);
}

void PanelPlot::initializePlot(int index)
{
    QVector<QString> plotName;
    QVector<QPen> pen;
    currentPlot = index;

    ui->plotWindow->clearGraphs();
    plotSize = plots[index].plotNames.size();
    for (int plotIndex=0; plotIndex<plotSize; plotIndex++)
    {
         plotName.append(plots[index].plotNames[plotIndex]);
         QPen plotPen;
         plotPen.setColor(QColor(colorNames[plotIndex]));
         plotPen.setWidth(2);
         pen.append(plotPen);
    }
    for (int plotIndex=0; plotIndex<plotSize; plotIndex++)
    {
        ui->plotWindow->addGraph();
        ui->plotWindow->graph(plotIndex)->setName(plotName[plotIndex]);
        ui->plotWindow->graph(plotIndex)->setPen(pen[plotIndex]);
    }
    sendMessage(plots[index].telemetry.toUtf8());
}

void PanelPlot::parseMessage(QByteArray data)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    QString messages = data;
    messages.chop(1); // remove LF
    QList<QString> message = messages.split(',');
    if (message.last() == "")
        message.removeLast();
    if (message.size() == plotSize)
    {
        for (int index=0; index < plotSize; index++)
        {
            if (ui->plotNames->item(index)->checkState() == Qt::Checked)
            {
                double dataValue = message[index].toDouble();
                ui->plotWindow->graph(index)->addData(key, dataValue);
                ui->plotWindow->graph(index)->removeDataBefore(key-timeAxisLength);
            }
        }
        // make key axis range scroll with the data (at a constant range size of 10)
        ui->plotWindow->xAxis->setRange(key+0.25, timeAxisLength, Qt::AlignRight);
        ui->plotWindow->replot();
        ui->plotWindow->rescaleAxes(true);
    }
    else if (message.size() > plotSize)
        emit panelStatus("Plot count (" + QString::number(message.size()) +") does not match plot count in XML configuration file (" + QString::number(plotSize) +")");
}

void PanelPlot::selectPlot(QListWidgetItem* item)
{
    int row = 0;
    while(item->text() != ui->plotNames->item(row)->text())
        row++;
    if (ui->plotNames->item(row)->checkState() == Qt::Checked)
        ui->plotNames->item(row)->setCheckState(Qt::Unchecked);
    else
        ui->plotNames->item(row)->setCheckState((Qt::Checked));;
    initializePlot(currentPlot);
}
