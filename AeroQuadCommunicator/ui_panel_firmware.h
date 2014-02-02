/********************************************************************************
** Form generated from reading UI file 'panel_firmware.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PANEL_FIRMWARE_H
#define UI_PANEL_FIRMWARE_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PanelFirmware
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *bootloader;
    QSplitter *splitter;
    QTextBrowser *console;
    QWebView *webView;
    QPushButton *firmware;

    void setupUi(QWidget *PanelFirmware)
    {
        if (PanelFirmware->objectName().isEmpty())
            PanelFirmware->setObjectName(QStringLiteral("PanelFirmware"));
        PanelFirmware->resize(1070, 836);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PanelFirmware->sizePolicy().hasHeightForWidth());
        PanelFirmware->setSizePolicy(sizePolicy);
        PanelFirmware->setLayoutDirection(Qt::LeftToRight);
        gridLayout = new QGridLayout(PanelFirmware);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 3, 5, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 3, 4, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 3, 2, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 3, 3, 1, 1);

        bootloader = new QPushButton(PanelFirmware);
        bootloader->setObjectName(QStringLiteral("bootloader"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(bootloader->sizePolicy().hasHeightForWidth());
        bootloader->setSizePolicy(sizePolicy1);
        bootloader->setMaximumSize(QSize(150, 50));
        bootloader->setLayoutDirection(Qt::RightToLeft);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/resources/bootloader.png"), QSize(), QIcon::Normal, QIcon::Off);
        bootloader->setIcon(icon);
        bootloader->setIconSize(QSize(150, 50));
        bootloader->setFlat(true);

        gridLayout->addWidget(bootloader, 3, 1, 1, 1);

        splitter = new QSplitter(PanelFirmware);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        console = new QTextBrowser(splitter);
        console->setObjectName(QStringLiteral("console"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(console->sizePolicy().hasHeightForWidth());
        console->setSizePolicy(sizePolicy2);
        console->setMinimumSize(QSize(0, 75));
        console->setMaximumSize(QSize(16777215, 16777215));
        console->setLayoutDirection(Qt::LeftToRight);
        splitter->addWidget(console);
        webView = new QWebView(splitter);
        webView->setObjectName(QStringLiteral("webView"));
        webView->setUrl(QUrl(QStringLiteral("http://aeroquad.hautio.net/")));
        splitter->addWidget(webView);

        gridLayout->addWidget(splitter, 1, 0, 1, 8);

        firmware = new QPushButton(PanelFirmware);
        firmware->setObjectName(QStringLiteral("firmware"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/resources/firmware.png"), QSize(), QIcon::Normal, QIcon::Off);
        firmware->setIcon(icon1);
        firmware->setIconSize(QSize(150, 50));
        firmware->setFlat(true);

        gridLayout->addWidget(firmware, 3, 6, 1, 1);


        retranslateUi(PanelFirmware);

        QMetaObject::connectSlotsByName(PanelFirmware);
    } // setupUi

    void retranslateUi(QWidget *PanelFirmware)
    {
        PanelFirmware->setWindowTitle(QApplication::translate("PanelFirmware", "Upload Firmware", 0));
        bootloader->setText(QString());
        firmware->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PanelFirmware: public Ui_PanelFirmware {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PANEL_FIRMWARE_H
