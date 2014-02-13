/********************************************************************************
** Form generated from reading UI file 'panel_route.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PANEL_ROUTE_H
#define UI_PANEL_ROUTE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "marble/MarbleWidget.h"

QT_BEGIN_NAMESPACE

class Ui_PanelRoute
{
public:
    QGridLayout *gridLayout_2;
    QSplitter *splitter;
    Marble::MarbleWidget *map;
    QFrame *frame;
    QGridLayout *gridLayout;
    QToolButton *save;
    QToolButton *upload;
    QToolButton *load;
    QTableWidget *routeList;
    QToolButton *removeLast;
    QToolButton *clear;
    QToolButton *editWaypoints;
    QToolButton *autopilot;
    QLabel *autopilotState;
    QToolButton *throttleCruise;
    QToolButton *throttleMax;
    QToolButton *left;
    QToolButton *center;
    QToolButton *right;
    QSpacerItem *horizontalSpacer_2;
    QToolButton *throttleOff;
    QSpacerItem *horizontalSpacer_3;
    QSlider *throttle;
    QSlider *rudder;

    void setupUi(QWidget *PanelRoute)
    {
        if (PanelRoute->objectName().isEmpty())
            PanelRoute->setObjectName(QStringLiteral("PanelRoute"));
        PanelRoute->resize(981, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(PanelRoute->sizePolicy().hasHeightForWidth());
        PanelRoute->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(PanelRoute);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        splitter = new QSplitter(PanelRoute);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setFrameShape(QFrame::NoFrame);
        splitter->setOrientation(Qt::Vertical);
        splitter->setOpaqueResize(true);
        map = new Marble::MarbleWidget(splitter);
        map->setObjectName(QStringLiteral("map"));
        map->setProperty("projection", QVariant(0));
        map->setProperty("longitude", QVariant(0));
        map->setProperty("showCompass", QVariant(true));
        map->setProperty("showAtmosphere", QVariant(false));
        splitter->addWidget(map);
        frame = new QFrame(splitter);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setMinimumSize(QSize(0, 0));
        frame->setMaximumSize(QSize(16777215, 170));
        frame->setStyleSheet(QLatin1String(" QSlider::groove:horizontal {\n"
"     border: 1px solid #999999;\n"
"     height: 8px; /* the groove expands to the size of the slider by default. by giving it a height, it has a fixed size */\n"
"     background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #B1B1B1, stop:1 #c4c4c4);\n"
"     margin: 2px 0;\n"
" }\n"
"\n"
" QSlider::handle:horizontal {\n"
"     background:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 95, 143, 255), stop:1 rgba(0, 150, 225, 255));\n"
"     border: 1px solid #5c5c5c;\n"
"     width: 50px;\n"
"     margin: -15px 0;\n"
"     border-radius: 3px;\n"
" }"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Sunken);
        frame->setLineWidth(10);
        gridLayout = new QGridLayout(frame);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        save = new QToolButton(frame);
        save->setObjectName(QStringLiteral("save"));
        save->setMinimumSize(QSize(90, 0));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/resources/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        save->setIcon(icon);
        save->setIconSize(QSize(50, 50));
        save->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(save, 3, 8, 2, 1);

        upload = new QToolButton(frame);
        upload->setObjectName(QStringLiteral("upload"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(upload->sizePolicy().hasHeightForWidth());
        upload->setSizePolicy(sizePolicy2);
        upload->setMinimumSize(QSize(90, 0));
        upload->setLayoutDirection(Qt::RightToLeft);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/resources/UploadRoute.png"), QSize(), QIcon::Normal, QIcon::Off);
        upload->setIcon(icon1);
        upload->setIconSize(QSize(45, 50));
        upload->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        upload->setAutoRaise(false);

        gridLayout->addWidget(upload, 3, 7, 2, 1);

        load = new QToolButton(frame);
        load->setObjectName(QStringLiteral("load"));
        load->setMinimumSize(QSize(90, 0));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/resources/load.png"), QSize(), QIcon::Normal, QIcon::Off);
        load->setIcon(icon2);
        load->setIconSize(QSize(50, 50));
        load->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(load, 1, 8, 2, 1);

        routeList = new QTableWidget(frame);
        if (routeList->columnCount() < 6)
            routeList->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        routeList->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        routeList->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        __qtablewidgetitem2->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        routeList->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        routeList->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        __qtablewidgetitem4->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        routeList->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        __qtablewidgetitem5->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        routeList->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        routeList->setObjectName(QStringLiteral("routeList"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(routeList->sizePolicy().hasHeightForWidth());
        routeList->setSizePolicy(sizePolicy3);
        routeList->horizontalHeader()->setStretchLastSection(true);
        routeList->verticalHeader()->setVisible(false);

        gridLayout->addWidget(routeList, 1, 10, 4, 1);

        removeLast = new QToolButton(frame);
        removeLast->setObjectName(QStringLiteral("removeLast"));
        sizePolicy2.setHeightForWidth(removeLast->sizePolicy().hasHeightForWidth());
        removeLast->setSizePolicy(sizePolicy2);
        removeLast->setMinimumSize(QSize(90, 0));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/resources/RemoveLast.png"), QSize(), QIcon::Normal, QIcon::Off);
        removeLast->setIcon(icon3);
        removeLast->setIconSize(QSize(45, 50));
        removeLast->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(removeLast, 1, 11, 2, 1);

        clear = new QToolButton(frame);
        clear->setObjectName(QStringLiteral("clear"));
        sizePolicy2.setHeightForWidth(clear->sizePolicy().hasHeightForWidth());
        clear->setSizePolicy(sizePolicy2);
        clear->setMinimumSize(QSize(90, 0));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/resources/ClearAll.png"), QSize(), QIcon::Normal, QIcon::Off);
        clear->setIcon(icon4);
        clear->setIconSize(QSize(45, 50));
        clear->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(clear, 3, 11, 2, 1);

        editWaypoints = new QToolButton(frame);
        editWaypoints->setObjectName(QStringLiteral("editWaypoints"));
        sizePolicy2.setHeightForWidth(editWaypoints->sizePolicy().hasHeightForWidth());
        editWaypoints->setSizePolicy(sizePolicy2);
        editWaypoints->setMinimumSize(QSize(90, 0));
        editWaypoints->setLayoutDirection(Qt::RightToLeft);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/resources/EditWaypoints.png"), QSize(), QIcon::Normal, QIcon::Off);
        editWaypoints->setIcon(icon5);
        editWaypoints->setIconSize(QSize(45, 50));
        editWaypoints->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        editWaypoints->setAutoRaise(false);

        gridLayout->addWidget(editWaypoints, 1, 7, 2, 1);

        autopilot = new QToolButton(frame);
        autopilot->setObjectName(QStringLiteral("autopilot"));
        autopilot->setMinimumSize(QSize(90, 0));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/resources/autopilot.png"), QSize(), QIcon::Normal, QIcon::Off);
        autopilot->setIcon(icon6);
        autopilot->setIconSize(QSize(45, 50));
        autopilot->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(autopilot, 1, 12, 2, 1);

        autopilotState = new QLabel(frame);
        autopilotState->setObjectName(QStringLiteral("autopilotState"));
        sizePolicy2.setHeightForWidth(autopilotState->sizePolicy().hasHeightForWidth());
        autopilotState->setSizePolicy(sizePolicy2);
        autopilotState->setMinimumSize(QSize(90, 72));
        autopilotState->setFrameShape(QFrame::Box);
        autopilotState->setFrameShadow(QFrame::Raised);
        autopilotState->setLineWidth(1);
        autopilotState->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(autopilotState, 3, 12, 2, 1);

        throttleCruise = new QToolButton(frame);
        throttleCruise->setObjectName(QStringLiteral("throttleCruise"));
        throttleCruise->setMinimumSize(QSize(75, 75));

        gridLayout->addWidget(throttleCruise, 3, 4, 1, 1);

        throttleMax = new QToolButton(frame);
        throttleMax->setObjectName(QStringLiteral("throttleMax"));
        throttleMax->setMinimumSize(QSize(75, 75));

        gridLayout->addWidget(throttleMax, 3, 5, 1, 1);

        left = new QToolButton(frame);
        left->setObjectName(QStringLiteral("left"));
        left->setMinimumSize(QSize(75, 75));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/resources/leftArrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        left->setIcon(icon7);
        left->setIconSize(QSize(30, 30));
        left->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(left, 3, 14, 1, 1);

        center = new QToolButton(frame);
        center->setObjectName(QStringLiteral("center"));
        center->setMinimumSize(QSize(75, 75));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/resources/centerArrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        center->setIcon(icon8);
        center->setIconSize(QSize(30, 30));
        center->setToolButtonStyle(Qt::ToolButtonIconOnly);

        gridLayout->addWidget(center, 3, 15, 1, 1);

        right = new QToolButton(frame);
        right->setObjectName(QStringLiteral("right"));
        right->setMinimumSize(QSize(75, 75));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/resources/rightArrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        right->setIcon(icon9);
        right->setIconSize(QSize(30, 30));
        right->setToolButtonStyle(Qt::ToolButtonIconOnly);
        right->setArrowType(Qt::NoArrow);

        gridLayout->addWidget(right, 3, 16, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 3, 13, 1, 1);

        throttleOff = new QToolButton(frame);
        throttleOff->setObjectName(QStringLiteral("throttleOff"));
        throttleOff->setMinimumSize(QSize(75, 75));

        gridLayout->addWidget(throttleOff, 3, 3, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 3, 6, 1, 1);

        throttle = new QSlider(frame);
        throttle->setObjectName(QStringLiteral("throttle"));
        throttle->setMinimumSize(QSize(0, 75));
        throttle->setMinimum(1000);
        throttle->setMaximum(2000);
        throttle->setValue(1500);
        throttle->setOrientation(Qt::Horizontal);
        throttle->setTickPosition(QSlider::TicksAbove);
        throttle->setTickInterval(100);

        gridLayout->addWidget(throttle, 1, 3, 1, 3);

        rudder = new QSlider(frame);
        rudder->setObjectName(QStringLiteral("rudder"));
        rudder->setMinimumSize(QSize(0, 75));
        rudder->setStyleSheet(QStringLiteral(""));
        rudder->setMinimum(1000);
        rudder->setMaximum(2000);
        rudder->setValue(1500);
        rudder->setOrientation(Qt::Horizontal);
        rudder->setTickPosition(QSlider::TicksBelow);
        rudder->setTickInterval(100);

        gridLayout->addWidget(rudder, 1, 14, 1, 3);

        splitter->addWidget(frame);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);


        retranslateUi(PanelRoute);

        QMetaObject::connectSlotsByName(PanelRoute);
    } // setupUi

    void retranslateUi(QWidget *PanelRoute)
    {
        PanelRoute->setWindowTitle(QApplication::translate("PanelRoute", "Form", 0));
#ifndef QT_NO_TOOLTIP
        map->setToolTip(QApplication::translate("PanelRoute", "The Earth", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        map->setWhatsThis(QApplication::translate("PanelRoute", "The Marble widget displays a virtual globe.", 0));
#endif // QT_NO_WHATSTHIS
        map->setProperty("mapThemeId", QVariant(QString()));
        save->setText(QApplication::translate("PanelRoute", "Save", 0));
        upload->setText(QApplication::translate("PanelRoute", "Upload Route", 0));
        load->setText(QApplication::translate("PanelRoute", "Load", 0));
        QTableWidgetItem *___qtablewidgetitem = routeList->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("PanelRoute", "ID", 0));
        QTableWidgetItem *___qtablewidgetitem1 = routeList->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("PanelRoute", "Latitude", 0));
        QTableWidgetItem *___qtablewidgetitem2 = routeList->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("PanelRoute", "Longitude", 0));
        QTableWidgetItem *___qtablewidgetitem3 = routeList->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("PanelRoute", "Altitude", 0));
        QTableWidgetItem *___qtablewidgetitem4 = routeList->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("PanelRoute", "Speed", 0));
        QTableWidgetItem *___qtablewidgetitem5 = routeList->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("PanelRoute", "Type", 0));
        removeLast->setText(QApplication::translate("PanelRoute", "Remove Last", 0));
        clear->setText(QApplication::translate("PanelRoute", "Clear Route", 0));
        editWaypoints->setText(QApplication::translate("PanelRoute", "Edit Waypoints", 0));
        autopilot->setText(QApplication::translate("PanelRoute", "Autopilot", 0));
        autopilotState->setText(QApplication::translate("PanelRoute", "Autopilot\n"
"Disabled", 0));
        throttleCruise->setText(QApplication::translate("PanelRoute", "Cruise", 0));
        throttleMax->setText(QApplication::translate("PanelRoute", "Max\n"
"Throttle", 0));
        left->setText(QApplication::translate("PanelRoute", "+5 Left", 0));
        left->setShortcut(QApplication::translate("PanelRoute", "3, 0, Return", 0));
        center->setText(QApplication::translate("PanelRoute", "Center", 0));
        right->setText(QApplication::translate("PanelRoute", "+5 Right", 0));
        throttleOff->setText(QApplication::translate("PanelRoute", "Throttle\n"
"Off", 0));
    } // retranslateUi

};

namespace Ui {
    class PanelRoute: public Ui_PanelRoute {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PANEL_ROUTE_H
