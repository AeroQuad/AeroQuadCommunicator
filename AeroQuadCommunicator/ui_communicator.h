/********************************************************************************
** Form generated from reading UI file 'communicator.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMMUNICATOR_H
#define UI_COMMUNICATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Communicator
{
public:
    QAction *actionConnect;
    QAction *actionStatus;
    QAction *actionHome;
    QAction *actionCalibrate;
    QAction *actionSetup;
    QAction *actionTerminal;
    QAction *actionPlots;
    QAction *actionNavigation;
    QAction *actionFirmware;
    QAction *actionRoute;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Communicator)
    {
        if (Communicator->objectName().isEmpty())
            Communicator->setObjectName(QStringLiteral("Communicator"));
        Communicator->resize(750, 500);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Communicator->sizePolicy().hasHeightForWidth());
        Communicator->setSizePolicy(sizePolicy);
        Communicator->setMinimumSize(QSize(500, 500));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/resources/AeroQuadIcon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        Communicator->setWindowIcon(icon);
        actionConnect = new QAction(Communicator);
        actionConnect->setObjectName(QStringLiteral("actionConnect"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/resources/AQConnect.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon1);
        actionStatus = new QAction(Communicator);
        actionStatus->setObjectName(QStringLiteral("actionStatus"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/resources/AQState.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionStatus->setIcon(icon2);
        actionHome = new QAction(Communicator);
        actionHome->setObjectName(QStringLiteral("actionHome"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/resources/AQMainMenu.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionHome->setIcon(icon3);
        actionHome->setVisible(false);
        actionCalibrate = new QAction(Communicator);
        actionCalibrate->setObjectName(QStringLiteral("actionCalibrate"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/resources/AQCal.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionCalibrate->setIcon(icon4);
        actionSetup = new QAction(Communicator);
        actionSetup->setObjectName(QStringLiteral("actionSetup"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/resources/AQConfig.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionSetup->setIcon(icon5);
        actionTerminal = new QAction(Communicator);
        actionTerminal->setObjectName(QStringLiteral("actionTerminal"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/resources/AQTerminal.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionTerminal->setIcon(icon6);
        actionPlots = new QAction(Communicator);
        actionPlots->setObjectName(QStringLiteral("actionPlots"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/resources/AQPlot.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlots->setIcon(icon7);
        actionNavigation = new QAction(Communicator);
        actionNavigation->setObjectName(QStringLiteral("actionNavigation"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/resources/AQGlobe.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionNavigation->setIcon(icon8);
        actionFirmware = new QAction(Communicator);
        actionFirmware->setObjectName(QStringLiteral("actionFirmware"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/resources/AQupload.svg"), QSize(), QIcon::Normal, QIcon::Off);
        icon9.addFile(QStringLiteral(":/images/resources/AQupload.svg"), QSize(), QIcon::Normal, QIcon::On);
        actionFirmware->setIcon(icon9);
        actionRoute = new QAction(Communicator);
        actionRoute->setObjectName(QStringLiteral("actionRoute"));
        actionRoute->setIcon(icon8);
        centralWidget = new QWidget(Communicator);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        Communicator->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Communicator);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 750, 21));
        Communicator->setMenuBar(menuBar);
        mainToolBar = new QToolBar(Communicator);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setIconSize(QSize(40, 40));
        mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        Communicator->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(Communicator);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setAutoFillBackground(false);
        statusBar->setStyleSheet(QStringLiteral("background-color: rgb(220, 220, 220);"));
        Communicator->setStatusBar(statusBar);

        mainToolBar->addAction(actionConnect);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionRoute);
        mainToolBar->addAction(actionSetup);
        mainToolBar->addAction(actionPlots);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionFirmware);
        mainToolBar->addAction(actionTerminal);

        retranslateUi(Communicator);

        QMetaObject::connectSlotsByName(Communicator);
    } // setupUi

    void retranslateUi(QMainWindow *Communicator)
    {
        Communicator->setWindowTitle(QApplication::translate("Communicator", "AeroQuad Communicator", 0));
        actionConnect->setText(QApplication::translate("Communicator", "Connect", 0));
        actionStatus->setText(QApplication::translate("Communicator", "Status", 0));
        actionHome->setText(QApplication::translate("Communicator", "Home", 0));
        actionCalibrate->setText(QApplication::translate("Communicator", "Calibrate", 0));
        actionSetup->setText(QApplication::translate("Communicator", "Settings", 0));
        actionTerminal->setText(QApplication::translate("Communicator", "Terminal", 0));
        actionPlots->setText(QApplication::translate("Communicator", "Plots", 0));
        actionNavigation->setText(QApplication::translate("Communicator", "Navigation", 0));
        actionFirmware->setText(QApplication::translate("Communicator", "Firmware", 0));
#ifndef QT_NO_TOOLTIP
        actionFirmware->setToolTip(QApplication::translate("Communicator", "Upload Firmware", 0));
#endif // QT_NO_TOOLTIP
        actionRoute->setText(QApplication::translate("Communicator", "Route", 0));
#ifndef QT_NO_TOOLTIP
        actionRoute->setToolTip(QApplication::translate("Communicator", "Upload navigation routes", 0));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class Communicator: public Ui_Communicator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMMUNICATOR_H
