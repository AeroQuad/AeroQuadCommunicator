#include "panel_config.h"
#include "ui_panel_config.h"

PanelConfig::PanelConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelConfig)
{
    ui->setupUi(this);

    connect(ui->configList, SIGNAL(currentRowChanged(int)), this, SLOT(selectConfig(int)));
    connect(ui->configTable, SIGNAL(cellClicked(int,int)), this, SLOT(selectDescription(int,int)));
    connect(this, SIGNAL(initializePanel(QMap<QString,QString>)), this, SLOT(initialize(QMap<QString,QString>)));
    connect(this, SIGNAL(messageIn(QByteArray)), this, SLOT(parseMessage(QByteArray)));
    connect(this, SIGNAL(connectionState(bool)), this, SLOT(updateConnectionState(bool)));
}

PanelConfig::~PanelConfig()
{
    delete ui;
}

void PanelConfig::initialize(QMap<QString, QString> config)
{
    configuration = config;
    emit getConnectionState();
    readXML("panel_config.xml");
    setupConfigList();
    setupTableHeader();
    selectConfig(0);
}

void PanelConfig::readXML(QString filename)
{
    // Open XML configuration file
    QFile* file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "AeroQuad Communicator", "Unable to find configuration file:\n" + QCoreApplication::applicationDirPath() + "/panel_config.xml", QMessageBox::Ok);
        file->close();
        return;
    }

    // Read XML file and populate data storage
    panelConfig.clear();
    QXmlStreamReader xml(file);
    while (!xml.atEnd() && !xml.hasError())
    {
        xml.readNext();
        if (xml.isStartElement() && (xml.name() == "ConfigType"))
        {
            cfg configItem;
            configItem.configName = xml.attributes().value("Name").toString();
            xml.readNextStartElement();
            while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "ConfigType"))
            {
                if(xml.tokenType() == QXmlStreamReader::StartElement)
                {
                    if(xml.name() == "Requirement")
                        configItem.requirement = xml.readElementText();
                    if(xml.name() == "Command")
                        configItem.command = xml.readElementText();
                    if(xml.name() == "Telemetry")
                        configItem.telemetry = xml.readElementText();
                    if(xml.name() == "UpdateEEPROM")
                        configItem.updateEEPROM = xml.readElementText();
                    if(xml.name() == "Parameter")
                    {
                        QXmlStreamAttributes parameterAttributes = xml.attributes();
                        parameter parameterItem;
                        parameterItem.name = parameterAttributes.value("Name").toString();
                        parameterItem.value = xml.readElementText();
                        parameterItem.description = parameterAttributes.value("Description").toString();
                        configItem.parameters.append(parameterItem);
                    }
                }
                xml.readNextStartElement();
            }
            panelConfig.append(configItem);
        }
    }
    if (xml.hasError())
    {
        QMessageBox::warning(NULL,"Error",xml.errorString(),QMessageBox::Ok);
    }
    file->close();
    defaultPanelConfig = panelConfig;
}

void PanelConfig::setupConfigList()
{
    for (int index=0; index < panelConfig.size(); index++)
        ui->configList->addItem(panelConfig[index].configName);
    ui->configList->setMaximumWidth(ui->configList->sizeHintForColumn(0)+25);
    ui->configList->setCurrentRow(0);
}

void PanelConfig::setupTableHeader()
{
    ui->configTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->configTable->setAlternatingRowColors(true);
    QFont font = ui->configTable->horizontalHeader()->font();
    font.setBold(true);
    ui->configTable->horizontalHeader()->setFont(font);
    QTableWidgetItem *header0 = new QTableWidgetItem("Parameter");
    ui->configTable->setHorizontalHeaderItem(0, header0);
    ui->configTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    QTableWidgetItem *header1 = new QTableWidgetItem("Value");
    ui->configTable->setHorizontalHeaderItem(1, header1);
    ui->configTable->verticalHeader()->setVisible(false);
}

void PanelConfig::selectConfig(int index)
{
    ui->configTable->clear();
    setupTableHeader();
    ui->configTable->setRowCount(panelConfig[index].parameters.size());
    currentPanel = index;
    updateParameterValues();
    ui->configTable->selectRow(0);
    selectDescription(0, 0);
    QByteArray message(panelConfig[index].telemetry.toUtf8());
    emit messageOut(message);
}

void PanelConfig::selectDescription(int row, int col)
{
    ui->configDescription->clear();
    ui->configDescription->append(panelConfig[currentPanel].parameters[row].description);
}

void PanelConfig::parseMessage(QByteArray incoming)
{
    QString message = incoming;
    message.chop(2); // remove CR/LF
    QStringList parameters = message.split(",");
    if (parameters[parameters.size()-1] == "")
        parameters.removeLast();
    for (int index=0; index<panelConfig[currentPanel].parameters.size(); index++)
        if (index < parameters.size()) // Protect if incoming message is incorrect size
            panelConfig[currentPanel].parameters[index].value = parameters[index];
    updateParameterValues();
}

void PanelConfig::updateParameterValues()
{
    for (int paramIndex=0; paramIndex<panelConfig[currentPanel].parameters.size(); paramIndex++)
    {
        QTableWidgetItem *name = new QTableWidgetItem(panelConfig[currentPanel].parameters[paramIndex].name);
        name->setFlags(name->flags() ^ Qt::ItemIsEditable);
        ui->configTable->setItem(paramIndex, 0, name);
        QDoubleSpinBox *parameterInput = new QDoubleSpinBox;
        parameterInput->setMaximum(9999);
        parameterInput->setMinimum(-9999);
        parameterInput->setDecimals(2);
        parameterInput->setValue(panelConfig[currentPanel].parameters[paramIndex].value.toDouble());
        parameterInput->setAlignment(Qt::AlignRight);
        ui->configTable->setCellWidget(paramIndex, 1, parameterInput);
    }
}

void PanelConfig::on_defaultButton_clicked()
{
    QMessageBox::StandardButton response = QMessageBox::question(this, "AeroQuad Communicator", "Are you sure you want to revert all values for " + panelConfig[currentPanel].configName + " to their default values?");
    if (response == QMessageBox::Yes)
        for (int index=0; index<defaultPanelConfig[currentPanel].parameters.size(); index++)
        {
            panelConfig[currentPanel].parameters = defaultPanelConfig[currentPanel].parameters;
            updateParameterValues();
        }
}

void PanelConfig::on_uploadButton_clicked()
{
    QString command = panelConfig[currentPanel].command;
    QDoubleSpinBox *cellValue;
    for (int index=0; index<ui->configTable->rowCount(); index++)
    {
        cellValue = qobject_cast<QDoubleSpinBox*>(ui->configTable->cellWidget(index, 1));
        command += QString::number(cellValue->value()) + ";"; // convert this way so extra zeros aren't included
    }
    emit messageOut(command.toUtf8());
    emit messageOut(panelConfig[currentPanel].telemetry.toUtf8());
    emit messageOut(panelConfig[currentPanel].updateEEPROM.toUtf8());
}
