/********************************************************************************
** Form generated from reading UI file 'menu_connect.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENU_CONNECT_H
#define UI_MENU_CONNECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MenuConnect
{
public:
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QFrame *GlossyFrame;
    QGridLayout *gridLayout_2;
    QComboBox *portComboBox;
    QComboBox *baudComboBox;
    QPushButton *connectPushButton;
    QPushButton *disconnectPushButton;
    QListWidget *configList;
    QGraphicsView *graphicsView;

    void setupUi(QWidget *MenuConnect)
    {
        if (MenuConnect->objectName().isEmpty())
            MenuConnect->setObjectName(QStringLiteral("MenuConnect"));
        MenuConnect->resize(1016, 578);
        MenuConnect->setStyleSheet(QStringLiteral(""));
        gridLayout = new QGridLayout(MenuConnect);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        splitter = new QSplitter(MenuConnect);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        GlossyFrame = new QFrame(splitter);
        GlossyFrame->setObjectName(QStringLiteral("GlossyFrame"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GlossyFrame->sizePolicy().hasHeightForWidth());
        GlossyFrame->setSizePolicy(sizePolicy);
        GlossyFrame->setMaximumSize(QSize(210, 16777215));
        GlossyFrame->setFrameShape(QFrame::StyledPanel);
        GlossyFrame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(GlossyFrame);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(-1, -1, -1, 0);
        portComboBox = new QComboBox(GlossyFrame);
        portComboBox->setObjectName(QStringLiteral("portComboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(portComboBox->sizePolicy().hasHeightForWidth());
        portComboBox->setSizePolicy(sizePolicy1);
        portComboBox->setMinimumSize(QSize(0, 0));
        portComboBox->setMaximumSize(QSize(16777215, 16777215));
        portComboBox->setAutoFillBackground(false);
        portComboBox->setStyleSheet(QStringLiteral("font: 14pt \"MS Shell Dlg 2\";"));
        portComboBox->setEditable(true);
        portComboBox->setFrame(true);

        gridLayout_2->addWidget(portComboBox, 0, 0, 1, 1);

        baudComboBox = new QComboBox(GlossyFrame);
        baudComboBox->setObjectName(QStringLiteral("baudComboBox"));
        baudComboBox->setStyleSheet(QStringLiteral("font: 14pt \"MS Shell Dlg 2\";"));
        baudComboBox->setEditable(true);

        gridLayout_2->addWidget(baudComboBox, 1, 0, 1, 1);

        connectPushButton = new QPushButton(GlossyFrame);
        connectPushButton->setObjectName(QStringLiteral("connectPushButton"));
        sizePolicy1.setHeightForWidth(connectPushButton->sizePolicy().hasHeightForWidth());
        connectPushButton->setSizePolicy(sizePolicy1);
        connectPushButton->setStyleSheet(QStringLiteral("font: 14pt \"MS Shell Dlg 2\";"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/resources/connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        connectPushButton->setIcon(icon);
        connectPushButton->setIconSize(QSize(150, 50));
        connectPushButton->setCheckable(false);
        connectPushButton->setChecked(false);
        connectPushButton->setFlat(true);

        gridLayout_2->addWidget(connectPushButton, 2, 0, 1, 1);

        disconnectPushButton = new QPushButton(GlossyFrame);
        disconnectPushButton->setObjectName(QStringLiteral("disconnectPushButton"));
        sizePolicy1.setHeightForWidth(disconnectPushButton->sizePolicy().hasHeightForWidth());
        disconnectPushButton->setSizePolicy(sizePolicy1);
        disconnectPushButton->setStyleSheet(QStringLiteral("font: 14pt \"MS Shell Dlg 2\";"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/resources/disconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        disconnectPushButton->setIcon(icon1);
        disconnectPushButton->setIconSize(QSize(150, 50));
        disconnectPushButton->setCheckable(false);
        disconnectPushButton->setChecked(false);
        disconnectPushButton->setFlat(true);

        gridLayout_2->addWidget(disconnectPushButton, 3, 0, 1, 1);

        configList = new QListWidget(GlossyFrame);
        configList->setObjectName(QStringLiteral("configList"));
        configList->setAlternatingRowColors(true);

        gridLayout_2->addWidget(configList, 4, 0, 1, 1);

        splitter->addWidget(GlossyFrame);
        graphicsView = new QGraphicsView(splitter);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setStyleSheet(QLatin1String("QGraphicsView {\n"
" border: 1px solid white;\n"
" border-radius: 3px;\n"
" background: qradialgradient(\n"
"   cx: 0.5, cy: -1.8,\n"
"   fx: 0.5, fy: 0,\n"
"   radius: 2, \n"
"   stop: 0 #9aa9be,\n"
"   stop: 1 #293859);\n"
"  font: bold;\n"
"}\n"
""));
        splitter->addWidget(graphicsView);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);


        retranslateUi(MenuConnect);

        QMetaObject::connectSlotsByName(MenuConnect);
    } // setupUi

    void retranslateUi(QWidget *MenuConnect)
    {
        MenuConnect->setWindowTitle(QApplication::translate("MenuConnect", "Form", 0));
        portComboBox->setCurrentText(QApplication::translate("MenuConnect", "Select Comm Port...", 0));
        baudComboBox->setCurrentText(QApplication::translate("MenuConnect", "Select Baud Rate...", 0));
    } // retranslateUi

};

namespace Ui {
    class MenuConnect: public Ui_MenuConnect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENU_CONNECT_H
