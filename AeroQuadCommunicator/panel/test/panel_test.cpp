#include "panel_test.h"
#include "ui_panel_test.h"

PanelTest::PanelTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelTest)
{
    ui->setupUi(this);
}

PanelTest::~PanelTest()
{
    delete ui;
}

void PanelTest::initPanel(QStandardItemModel dataModel)
{
//    ui->tableView->setModel(dataModel);
//    ui->tableView->show();
}
