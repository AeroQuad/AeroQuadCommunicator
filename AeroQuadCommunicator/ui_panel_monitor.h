/********************************************************************************
** Form generated from reading UI file 'panel_monitor.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PANEL_MONITOR_H
#define UI_PANEL_MONITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PanelMonitor
{
public:
    QGridLayout *gridLayout;
    QPushButton *clearButton;
    QLineEdit *commandLine;
    QPushButton *sendButton;
    QPlainTextEdit *console;

    void setupUi(QWidget *PanelMonitor)
    {
        if (PanelMonitor->objectName().isEmpty())
            PanelMonitor->setObjectName(QStringLiteral("PanelMonitor"));
        PanelMonitor->resize(400, 300);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/resources/AeroQuadIcon.ico"), QSize(), QIcon::Normal, QIcon::Off);
        PanelMonitor->setWindowIcon(icon);
        gridLayout = new QGridLayout(PanelMonitor);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        clearButton = new QPushButton(PanelMonitor);
        clearButton->setObjectName(QStringLiteral("clearButton"));

        gridLayout->addWidget(clearButton, 1, 2, 1, 1);

        commandLine = new QLineEdit(PanelMonitor);
        commandLine->setObjectName(QStringLiteral("commandLine"));

        gridLayout->addWidget(commandLine, 1, 0, 1, 1);

        sendButton = new QPushButton(PanelMonitor);
        sendButton->setObjectName(QStringLiteral("sendButton"));
        sendButton->setDefault(true);

        gridLayout->addWidget(sendButton, 1, 1, 1, 1);

        console = new QPlainTextEdit(PanelMonitor);
        console->setObjectName(QStringLiteral("console"));

        gridLayout->addWidget(console, 0, 0, 1, 3);


        retranslateUi(PanelMonitor);

        QMetaObject::connectSlotsByName(PanelMonitor);
    } // setupUi

    void retranslateUi(QWidget *PanelMonitor)
    {
        PanelMonitor->setWindowTitle(QApplication::translate("PanelMonitor", "Form", 0));
        clearButton->setText(QApplication::translate("PanelMonitor", "Clear", 0));
        sendButton->setText(QApplication::translate("PanelMonitor", "Send", 0));
    } // retranslateUi

};

namespace Ui {
    class PanelMonitor: public Ui_PanelMonitor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PANEL_MONITOR_H
