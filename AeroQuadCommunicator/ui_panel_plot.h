/********************************************************************************
** Form generated from reading UI file 'panel_plot.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PANEL_PLOT_H
#define UI_PANEL_PLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QWidget>
#include "panel/plot/qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_PanelPlot
{
public:
    QGridLayout *gridLayout;
    QSplitter *plotSplitter;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QSplitter *typeSplitter;
    QListWidget *plotTypes;
    QListWidget *plotNames;
    QCustomPlot *plotWindow;

    void setupUi(QWidget *PanelPlot)
    {
        if (PanelPlot->objectName().isEmpty())
            PanelPlot->setObjectName(QStringLiteral("PanelPlot"));
        PanelPlot->resize(530, 450);
        gridLayout = new QGridLayout(PanelPlot);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        plotSplitter = new QSplitter(PanelPlot);
        plotSplitter->setObjectName(QStringLiteral("plotSplitter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plotSplitter->sizePolicy().hasHeightForWidth());
        plotSplitter->setSizePolicy(sizePolicy);
        plotSplitter->setOrientation(Qt::Horizontal);
        frame = new QFrame(plotSplitter);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        typeSplitter = new QSplitter(frame);
        typeSplitter->setObjectName(QStringLiteral("typeSplitter"));
        typeSplitter->setOrientation(Qt::Vertical);
        plotTypes = new QListWidget(typeSplitter);
        plotTypes->setObjectName(QStringLiteral("plotTypes"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(plotTypes->sizePolicy().hasHeightForWidth());
        plotTypes->setSizePolicy(sizePolicy2);
        typeSplitter->addWidget(plotTypes);
        plotNames = new QListWidget(typeSplitter);
        plotNames->setObjectName(QStringLiteral("plotNames"));
        sizePolicy2.setHeightForWidth(plotNames->sizePolicy().hasHeightForWidth());
        plotNames->setSizePolicy(sizePolicy2);
        typeSplitter->addWidget(plotNames);

        gridLayout_2->addWidget(typeSplitter, 0, 0, 1, 1);

        plotSplitter->addWidget(frame);
        plotWindow = new QCustomPlot(plotSplitter);
        plotWindow->setObjectName(QStringLiteral("plotWindow"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(plotWindow->sizePolicy().hasHeightForWidth());
        plotWindow->setSizePolicy(sizePolicy3);
        plotSplitter->addWidget(plotWindow);

        gridLayout->addWidget(plotSplitter, 0, 0, 1, 1);


        retranslateUi(PanelPlot);

        QMetaObject::connectSlotsByName(PanelPlot);
    } // setupUi

    void retranslateUi(QWidget *PanelPlot)
    {
        PanelPlot->setWindowTitle(QApplication::translate("PanelPlot", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class PanelPlot: public Ui_PanelPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PANEL_PLOT_H
