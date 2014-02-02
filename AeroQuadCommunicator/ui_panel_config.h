/********************************************************************************
** Form generated from reading UI file 'panel_config.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PANEL_CONFIG_H
#define UI_PANEL_CONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PanelConfig
{
public:
    QGridLayout *gridLayout_2;
    QSplitter *splitter;
    QListWidget *configList;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QTableWidget *configTable;
    QFrame *frame_2;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_4;
    QToolButton *uploadButton;
    QTextBrowser *configDescription;
    QToolButton *defaultButton;

    void setupUi(QWidget *PanelConfig)
    {
        if (PanelConfig->objectName().isEmpty())
            PanelConfig->setObjectName(QStringLiteral("PanelConfig"));
        PanelConfig->resize(700, 500);
        gridLayout_2 = new QGridLayout(PanelConfig);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        splitter = new QSplitter(PanelConfig);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        configList = new QListWidget(splitter);
        configList->setObjectName(QStringLiteral("configList"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(configList->sizePolicy().hasHeightForWidth());
        configList->setSizePolicy(sizePolicy);
        configList->setMinimumSize(QSize(0, 0));
        configList->setMaximumSize(QSize(16777215, 16777215));
        splitter->addWidget(configList);
        frame = new QFrame(splitter);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(0);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        configTable = new QTableWidget(frame);
        if (configTable->columnCount() < 2)
            configTable->setColumnCount(2);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setFont(font);
        configTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setFont(font);
        configTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        configTable->setObjectName(QStringLiteral("configTable"));

        gridLayout_3->addWidget(configTable, 0, 0, 1, 1);

        frame_2 = new QFrame(frame);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::NoFrame);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(frame_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setVerticalSpacing(0);
        gridLayout_4->setContentsMargins(-1, -1, -1, 0);
        uploadButton = new QToolButton(frame_2);
        uploadButton->setObjectName(QStringLiteral("uploadButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(uploadButton->sizePolicy().hasHeightForWidth());
        uploadButton->setSizePolicy(sizePolicy1);
        uploadButton->setMaximumSize(QSize(16777215, 100));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/resources/AQupload.svg"), QSize(), QIcon::Normal, QIcon::Off);
        uploadButton->setIcon(icon);
        uploadButton->setIconSize(QSize(50, 50));
        uploadButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_4->addWidget(uploadButton, 0, 2, 1, 1);

        configDescription = new QTextBrowser(frame_2);
        configDescription->setObjectName(QStringLiteral("configDescription"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(configDescription->sizePolicy().hasHeightForWidth());
        configDescription->setSizePolicy(sizePolicy2);
        configDescription->setMaximumSize(QSize(16777215, 100));
        configDescription->setFrameShape(QFrame::StyledPanel);

        gridLayout_4->addWidget(configDescription, 0, 1, 1, 1);

        defaultButton = new QToolButton(frame_2);
        defaultButton->setObjectName(QStringLiteral("defaultButton"));
        defaultButton->setMaximumSize(QSize(16777215, 100));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/resources/AQConfig.svg"), QSize(), QIcon::Normal, QIcon::Off);
        defaultButton->setIcon(icon1);
        defaultButton->setIconSize(QSize(50, 50));
        defaultButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout_4->addWidget(defaultButton, 0, 0, 1, 1);


        gridLayout->addLayout(gridLayout_4, 0, 0, 1, 1);


        gridLayout_3->addWidget(frame_2, 1, 0, 1, 1);

        splitter->addWidget(frame);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);


        retranslateUi(PanelConfig);

        QMetaObject::connectSlotsByName(PanelConfig);
    } // setupUi

    void retranslateUi(QWidget *PanelConfig)
    {
        PanelConfig->setWindowTitle(QApplication::translate("PanelConfig", "Form", 0));
        QTableWidgetItem *___qtablewidgetitem = configTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("PanelConfig", "Parameter", 0));
        QTableWidgetItem *___qtablewidgetitem1 = configTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("PanelConfig", "Value", 0));
        uploadButton->setText(QApplication::translate("PanelConfig", "Upload", 0));
        defaultButton->setText(QApplication::translate("PanelConfig", "Restore\n"
"Defaults", 0));
    } // retranslateUi

};

namespace Ui {
    class PanelConfig: public Ui_PanelConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PANEL_CONFIG_H
