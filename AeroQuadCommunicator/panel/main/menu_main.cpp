#include "menu_main.h"
#include "ui_menu_main.h"

MenuMain::MenuMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuMain)
{
    ui->setupUi(this);
}

MenuMain::~MenuMain()
{
    delete ui;
}
